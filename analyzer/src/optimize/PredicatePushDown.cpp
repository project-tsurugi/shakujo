/*
 * Copyright 2018 shakujo project.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "PredicatePushDown.h"

#include <map>

#include <cassert>

#include "ComparisonTerm.h"
#include "SimplifyPredicate.h"
#include "VariableRewriter.h"

#include "shakujo/common/core/value/Error.h"
#include "shakujo/common/util/utility.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/expression/ExpressionVisitor.h"
#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"

namespace shakujo::analyzer::optimize {

using common::util::is_defined;
using common::util::is_valid;
using common::util::make_clone;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;
using common::util::equals;

namespace {
class Term {
public:
    model::expression::Expression* node {};
    ComparisonTerm comparison {};
    bool alternative { false };
    bool sticky { false };
    explicit operator bool() const {
        return is_defined(node);
    }
};

struct Predicates {
    VariableRewriter rewriter {};
    std::vector<std::shared_ptr<Term>> terms {};
    bool through_heavy { false };
};

class TermCollector
    : public model::expression::ExpressionVisitor<void> {

public:
    using ExpressionVisitor::visit;

    void visitDefault(model::expression::Expression* node) override {
        auto term = std::make_shared<Term>();
        term->node = node;
        terms.emplace_back(std::move(term));
    }

    void visit(model::expression::ImplicitCast* node) override {
        dispatch(node->operand());
    }

    void visit(model::expression::BinaryOperator* node) override {
        if (node->operator_kind() == model::expression::BinaryOperator::Kind::CONDITIONAL_AND) {
            dispatch(node->left());
            dispatch(node->right());
            return;
        }
        visitDefault(node);
    }

    std::vector<std::shared_ptr<Term>> terms {};
};

class Engine
    : public model::expression::ExpressionVisitor<void, Predicates&&> {
public:
    explicit Engine(Context& context) : context_(context) {}

    using ExpressionVisitor::visit;

    void visit(model::expression::relation::SelectionExpression* node, Predicates&& preds) override {
        // simplifies current predicates
        SimplifyPredicate simplifier { context_ };
        if (auto predicate = simplifier(node->condition()); predicate == true) {
            // invariant: each relation.process.column[i] == relation.output.column[i]
            auto operand = node->replace_with(node->release_operand());
            dispatch(operand, std::move(preds));
            return;
        }

        // collect CNF terms in condition
        auto terms = collect(preds, node->condition());
        for (auto&& term : terms) {
            preds.terms.emplace_back(std::move(term));
        }

        // push-down predicate terms
        auto relation = relation_of(node);
        VariableRewriter rewriter {};
        rewriter.add_rule(relation->output().columns(), relation->process().columns());
        preds.rewriter.merge(rewriter);
        dispatch(node->operand(), std::move(preds));

        // simplifies processed predicates
        if (auto predicate = simplifier(node->condition()); predicate == true) {
            node->replace_with(node->release_operand());
            return;
        }
    }

    void visit(model::expression::relation::ScanExpression* node, Predicates&& preds) override {
        flush_predicates(node, std::move(preds), true);
    }

    void visit(model::expression::relation::ProjectionExpression* node, Predicates&& prev) override {
        auto relation = relation_of(node);
        VariableRewriter rewriter {};
        for (auto* column : node->columns()) {
            if (auto src = extract_variable_deep(column->value())) {
                auto dst = variable_of(column);
                rewriter.add_rule(dst, src);
            }
        }
        Predicates next { prev };
        next.rewriter.merge(rewriter);
        next.through_heavy = true;
        dispatch(node->operand(), std::move(next));

        flush_predicates(node, std::move(prev));
    }

    void visit(model::expression::relation::AggregationExpression* node, Predicates&& prev) override {
        // cannot propagate predicates
        flush_predicates(node, std::move(prev));
        dispatch(node->operand(), {});
    }

    void visit(model::expression::relation::JoinExpression* node, Predicates&& prev) override {
        SimplifyPredicate simplifier { context_ };
        if (is_defined(node->condition())) {
            if (auto predicate = simplifier(node->condition()); predicate == true) {
                node->condition({});
            }
        }
        auto relation = relation_of(node);
        auto&& join = relation->join_strategy();

        std::vector<std::shared_ptr<Term>> terms;
        if (is_defined(node->condition())) {
            Predicates preds { prev };
            VariableRewriter rewriter {};
            for (auto&& column : join.columns()) {
                if (column.left_source()) {
                    rewriter.add_rule(column.output(), column.left_source());
                } else if (column.right_source()) {
                    rewriter.add_rule(column.output(), column.right_source());
                }
            }
            preds.rewriter.merge(rewriter);
            terms = collect(preds, node->condition(), true);
        }
        if (!join.right_outer()) {
            Predicates next { prev };
            next.through_heavy = true;
            VariableRewriter rewriter {};
            for (auto&& column : join.columns()) {
                if (column.left_source()) {
                    rewriter.add_rule(column.output(), column.left_source());
                }
            }
            auto opposite = relation_of(dynamic_pointer_cast<model::key::RelationKey::Provider>(node->right()));
            rewriter.deny(opposite->output().columns());
            next.rewriter.merge(rewriter);
            for (auto&& term : terms) {
                if (term) {
                    next.terms.emplace_back(term);
                }
            }
            dispatch(node->left(), std::move(next));
        }

        if (!join.left_outer()) {
            Predicates next { prev };
            next.through_heavy = true;
            VariableRewriter rewriter {};
            for (auto&& column : join.columns()) {
                if (column.right_source()) {
                    rewriter.add_rule(column.output(), column.right_source());
                }
            }
            auto opposite = relation_of(dynamic_pointer_cast<model::key::RelationKey::Provider>(node->left()));
            rewriter.deny(opposite->output().columns());
            next.rewriter.merge(rewriter);
            for (auto&& term : terms) {
                if (term) {
                    next.terms.emplace_back(term);
                }
            }
            dispatch(node->right(), std::move(next));
        }

        if (is_defined(node->condition()) && (!join.right_outer() || !join.left_outer())) {
            if (auto predicate = simplifier(node->condition()); predicate == true) {
                node->condition({});
            }
        }
        flush_predicates(node, std::move(prev));
    }

    void visit(model::expression::relation::OrderExpression* node, Predicates&& preds) override {
        // through
        preds.through_heavy = true;
        auto relation = relation_of(node);
        VariableRewriter rewriter {};
        rewriter.add_rule(relation->output().columns(), relation->process().columns());
        preds.rewriter.merge(rewriter);
        dispatch(node->operand(), std::move(preds));
    }

    void visit(model::expression::relation::DistinctExpression* node, Predicates&& preds) override {
        // through
        preds.through_heavy = true;
        auto relation = relation_of(node);
        VariableRewriter rewriter {};
        rewriter.add_rule(relation->output().columns(), relation->process().columns());
        preds.rewriter.merge(rewriter);
        dispatch(node->operand(), std::move(preds));
    }

    void visit(model::expression::relation::LimitExpression* node, Predicates&& prev) override {
        // never propagate predicates
        flush_predicates(node, std::move(prev));
        dispatch(node->operand(), {});
    }

    std::vector<std::shared_ptr<Term>> collect(Predicates& preds, model::expression::Expression* node, bool sticky = false) {
        TermCollector collector;
        collector.dispatch(node);
        for (auto&& term : collector.terms) {
            term->comparison = ComparisonTerm::resolve(context_.bindings(), term->node);
            term->sticky = sticky;
        }
        std::vector<std::shared_ptr<Term>> terms {};
        terms.reserve(collector.terms.size() + preds.terms.size());
        for (auto&& term : collector.terms) {
            terms.emplace_back(term);
        }
        for (auto&& term : preds.terms) {
            terms.emplace_back(term);
        }
        auto constants = build_constants(preds, terms);
        if (!constants.empty()) {
            for (auto&& term : terms) {
                if (!term) {
                    continue;
                }
                auto&& comparison = term->comparison;
                if (!comparison
                        || comparison.op() != ComparisonTerm::Operator::EQ
                        || !comparison.left().is_variable()
                        || !comparison.right().is_variable()) {
                    continue;
                }
                {
                    auto rewrite = preds.rewriter.apply(comparison.left().variable());
                    if (!is_valid(rewrite)) {
                        continue;
                    }
                    if (auto it = constants.find(rewrite); it != constants.end()) {
                        comparison = ComparisonTerm(
                            comparison.source(),
                            comparison.op(),
                            comparison.right(),
                            ComparisonTerm::Factor(make_clone(it->second)));
                        term->alternative = true;
                        continue;
                    }

                }
                {
                    auto rewrite = preds.rewriter.apply(comparison.right().variable());
                    if (!is_valid(rewrite)) {
                        continue;
                    }
                    if (auto it = constants.find(rewrite); it != constants.end()) {
                        comparison.right() = ComparisonTerm::Factor(make_clone(it->second));
                        term->alternative = true;
                        continue;
                    }
                }
            }
        }
        return std::move(collector.terms);
    }

    std::map<std::shared_ptr<binding::VariableBinding>, std::unique_ptr<common::core::Value>> build_constants(
            Predicates& preds,
            std::vector<std::shared_ptr<Term>>& terms) {
        std::map<std::shared_ptr<binding::VariableBinding>, std::unique_ptr<common::core::Value>> constants {};
        for (auto&& term : terms) {
            if (!term) {
                continue;
            }
            auto&& comparison = term->comparison;
            if (!comparison
                    || comparison.op() != ComparisonTerm::Operator::EQ
                    || !comparison.left().is_variable()) {
                continue;
            }
            auto left = preds.rewriter.apply(comparison.left().variable());
            if (!is_valid(left)) {
                continue;
            }
            if (comparison.right().is_constant()) {
                // FIXME: propagate A = B AND A > 10 => B > 10
                if (auto it = constants.find(left); it != constants.end()) {
                    auto&& existing = it->second;
                    if (!equals(existing, comparison.right().constant())) {
                        existing = std::make_unique<common::core::value::Error>();
                    }
                } else {
                    constants.emplace(left, make_clone(comparison.right().constant()));
                }
            }
        }
        // FIXME: propagate transitivity
        return constants;
    }

    void flush_predicates(model::expression::Expression* node, Predicates&& preds, bool force = false) {
        if (preds.terms.empty()) {
            return;
        }
        if (!force && !preds.through_heavy) {
            return;
        }
        {
            auto relation = relation_of(dynamic_pointer_cast<model::key::RelationKey::Provider>(node));
            VariableRewriter rewriter {};
            rewriter.add_rule(relation->output().columns(), relation->output().columns());
            preds.rewriter.merge(rewriter);
        }
        std::unique_ptr<model::expression::Expression> predicate {};
        for (auto&& term : preds.terms) {
            if (!*term) {
                continue;
            }
            if (!force && term->sticky) {
                continue;
            }
            if (rewrite_variables(preds, *term)) {
                auto replaced = replace_constant(*term, true);
                predicate = merge_condition(std::move(predicate), std::move(replaced));
            }
        }
        if (!is_defined(predicate)) {
            return;
        }

        common::util::ManagedPtr<model::expression::Expression> manager { std::move(predicate) };
        SimplifyPredicate simplifier { context_ };
        if (simplifier(manager.get()) == true) {
            return;
        }

        auto selection = dynamic_pointer_cast<model::expression::relation::SelectionExpression>(node->replace([&](auto origin) {
            return f.SelectionExpression(std::move(origin), manager.release());
        }));

        bless(selection, type_of(selection->operand()));

        auto parent = relation_of(dynamic_pointer_cast<model::key::RelationKey::Provider>(selection->operand()));
        auto relation = std::make_shared<binding::RelationBinding>(parent->output(), parent->output());
        selection->relation_key(context_.bindings().create_key(std::move(relation)));
    }

    bool rewrite_variables(Predicates& preds, Term& term) {
        assert(term);  // NOLINT
        if (term.alternative) {
            auto&& comparison = term.comparison;
            // FIXME: simple implementation only
            assert(comparison);  // NOLINT
            assert(comparison.op() == ComparisonTerm::Operator::EQ);  // NOLINT
            assert(comparison.left().is_variable());  // NOLINT
            assert(comparison.right().is_constant());  // NOLINT

            auto rewrite = preds.rewriter.apply(comparison.left().variable());
            if (!is_valid(rewrite)) {
                return false;
            }
            auto left = f.VariableReference(f.Name(name_of(*rewrite)));
            bless(left.get(), rewrite->type());
            left->variable_key(context_.bindings().create_key(rewrite));

            auto right = f.Literal(make_clone(rewrite->type()), make_clone(comparison.right().constant()));
            bless(right.get(), right->type(), right->value(), true);

            auto compare = f.BinaryOperator(model::expression::BinaryOperator::Kind::EQUAL, std::move(left), std::move(right));
            compare->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
                std::make_unique<common::core::type::Bool>(common::core::Type::Nullity::NEVER_NULL)));

            auto original_type = make_clone(type_of(term.node));
            term.node = term.node->replace_with(std::move(compare));
            encast(original_type.get(), term.node);
            return true;
        }
        auto variables = VariableRewriter::collect(term.node);
        for (auto* ref : variables) {
            auto var = variable_of(ref);
            if (auto rewrite = preds.rewriter.apply(var); !is_valid(rewrite)) {
                return false;
            }
        }
        for (auto* ref : variables) {
            auto var = variable_of(ref);
            auto rewrite = preds.rewriter.apply(var);
            assert(is_valid(rewrite));  // NOLINT
            ref->variable_key(context_.bindings().create_key(rewrite));
            if (!equals(var->type(), rewrite->type())) {
                encast(rewrite->type(), ref);
            }
        }
        return true;
    }

    static std::string name_of(binding::VariableBinding const& variable) {
        if (!variable.name().empty()) {
            return variable.name().segments()[variable.name().segments().size() - 1];
        }
        return variable.id().to_unique_name();
    }

    common::core::Type const* type_of(model::key::ExpressionKey::Provider* node) {
        return context_.bindings().get(node->expression_key())->type();
    }

    std::shared_ptr<binding::RelationBinding> relation_of(model::key::RelationKey::Provider *node) {
        return context_.bindings().get(node->relation_key());
    }

    std::shared_ptr<binding::VariableBinding> variable_of(model::key::VariableKey::Provider *node) {
        return context_.bindings().get(node->variable_key());
    }

    void bless(
            model::expression::Expression* node,
            common::core::Type const* type,
            common::core::Value const* value = nullptr,
            bool constant = false) {
        auto expr = std::make_shared<binding::ExpressionBinding>(make_clone(type), make_clone(value), constant);
        node->expression_key(context_.bindings().create_key(std::move(expr)));
    }

    std::shared_ptr<binding::VariableBinding> extract_variable_deep(model::expression::Expression* node) {
        if (auto* ref = dynamic_pointer_cast_if<model::expression::VariableReference>(node)) {
            return variable_of(ref);
        }
        if (auto* cast = dynamic_pointer_cast_if<model::expression::ImplicitCast>(node)) {
            return extract_variable_deep(cast->operand());
        }
        return {};
    }

    std::unique_ptr<model::expression::Expression> merge_condition(
            std::unique_ptr<model::expression::Expression> target,
            std::unique_ptr<model::expression::Expression> term) {
        if (!is_defined(target)) {
            return decast(std::move(term));
        }
        auto result = f.BinaryOperator(
            model::expression::BinaryOperator::Kind::CONDITIONAL_AND,
            decast(std::move(target)),
            decast(std::move(term)));

        auto left = type_of(result->left());
        auto right = type_of(result->right());
        assert(left->kind() == common::core::type::Bool::tag);  // NOLINT
        assert(right->kind() == common::core::type::Bool::tag);  // NOLINT
        common::core::type::Bool t { left->nullity() | right->nullity() };
        encast(&t, result->left());
        encast(&t, result->right());
        bless(result.get(), &t);

        return result;
    }

    std::unique_ptr<model::expression::Expression> decast(std::unique_ptr<model::expression::Expression> node) {
        if (auto cast = dynamic_pointer_cast_if<model::expression::ImplicitCast>(node)) {
            if (type_of(cast->operand())->kind() == common::core::type::Bool::tag) {
                return cast->release_operand();
            }
            return cast;
        }
        return node;
    }

    void encast(common::core::Type const* type, model::expression::Expression* node) {
        if (auto actual = type_of(node); !equals(type, actual)) {
            auto expr = context_.bindings().get(node->expression_key());
            auto replaced = node->replace([&](auto origin) {
                return f.ImplicitCast(make_clone(type), std::move(origin));
            });
            bless(replaced, type, expr->value(), expr->constant());
        }
    }

    std::unique_ptr<model::expression::Expression> replace_constant(Term& term, bool value) {
        assert(term);  // NOLINT
        std::unique_ptr<model::expression::Expression> detached;
        auto replacement = constant(value);
        term.node->replace([&](auto origin) {
            detached = std::move(origin);
            return std::move(replacement);
        });
        term.node = nullptr;
        return detached;
    }

    std::unique_ptr<model::expression::Literal> constant(bool value) {
        auto node = f.Literal(
            std::make_unique<common::core::type::Bool>(common::core::Type::Nullity::NEVER_NULL),
            std::make_unique<common::core::value::Bool>(value));
        bless(node.get(), node->type(), node->value(), true);
        return node;
    }

private:
    Context& context_;
    model::IRFactory f;
};

class Walker : public model::util::NodeWalker {
public:
    explicit Walker(PredicatePushDown& entity) noexcept : entity_(entity) {}

    using NodeWalker::enter;

    bool enter(model::statement::dml::EmitStatement* node) override {
        entity_(node->source());
        return false;
    }

    bool enter(model::statement::dml::InsertRelationStatement* node) override {
        entity_(node->source());
        return false;
    }

    bool enter(model::statement::dml::UpdateStatement* node) override {
        entity_(node->source());
        return false;
    }

    bool enter(model::statement::dml::DeleteStatement* node) override {
        entity_(node->source());
        return false;
    }

private:
    PredicatePushDown& entity_;
};
}  // namespace

void PredicatePushDown::operator()(model::program::Program* node) {
    Walker adapter { *this };
    adapter.walk(node);
}

void PredicatePushDown::operator()(model::statement::Statement* node) {
    Walker adapter { *this };
    adapter.walk(node);
}

void PredicatePushDown::operator()(model::expression::Expression *node) {
    Engine engine { context_ };
    engine.dispatch(node, {});
}
}  // namespace shakujo::analyzer::optimize
