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

#include <functional>
#include <map>

#include <cassert>

#include "ComparisonTerm.h"
#include "SimplifyPredicate.h"
#include "VariableRewriter.h"
#include "../analyze/typing.h"

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
    Term(
            model::expression::Expression* node,
            ComparisonTerm comparison,
            std::function<std::unique_ptr<model::expression::Expression>(Term&, VariableRewriter&)> rewriter,
            std::function<void(Term&)> disposer)
        : node_(node)
        , comparison_(std::move(comparison))
        , rewriter_(std::move(rewriter))
        , disposer_(std::move(disposer))
    {}

    model::expression::Expression* node_;
    ComparisonTerm comparison_;
    bool sticky_ { false };

    void rewriter(std::function<std::unique_ptr<model::expression::Expression>(Term&, VariableRewriter&)> rewriter) {
        rewriter_ = std::move(rewriter);
    }

    std::unique_ptr<model::expression::Expression> rewrite(VariableRewriter& rewriter) {
        return rewriter_(*this, rewriter);
    }

    void disposer(std::function<void(Term&)> disposer) {
        disposer_ = std::move(disposer);
    }

    void dispose() {
        disposer_(*this);
        enabled_ = false;
        node_ = {};
        comparison_ = {};
        rewriter_ = [](auto&, auto&) -> std::unique_ptr<model::expression::Expression> { std::abort(); };
        disposer_ = [](auto&) {};
    }

    explicit operator bool() const {
        return enabled_;
    }

private:
    std::function<std::unique_ptr<model::expression::Expression>(Term&, VariableRewriter&)> rewriter_;
    std::function<void(Term&)> disposer_;
    bool enabled_ { true };
};

struct Predicates {
    VariableRewriter rewriter {};
    std::vector<std::shared_ptr<Term>> terms {};
    bool through_heavy { false };
};

class TermCollector
    : public model::expression::ExpressionVisitor<void, std::vector<std::shared_ptr<Term>>&> {

public:
    using ExpressionVisitor::visit;

    void visitDefault(model::expression::Expression* node, std::vector<std::shared_ptr<Term>>& terms) override {
        auto comparison = ComparisonTerm::resolve(context_.bindings(), node);
        auto term = std::make_shared<Term>(node, std::move(comparison), rewriter_, disposer_);
        terms.emplace_back(std::move(term));
    }

    void visit(model::expression::ImplicitCast* node, std::vector<std::shared_ptr<Term>>& terms) override {
        dispatch(node->operand(), terms);
    }

    void visit(model::expression::BinaryOperator* node, std::vector<std::shared_ptr<Term>>& terms) override {
        if (node->operator_kind() == model::expression::BinaryOperator::Kind::CONDITIONAL_AND) {
            dispatch(node->left(), terms);
            dispatch(node->right(), terms);
            return;
        }
        visitDefault(node, terms);
    }

    explicit TermCollector(
            Context& context,
            std::function<std::unique_ptr<model::expression::Expression>(Term&, VariableRewriter&)> rewriter,
            std::function<void(Term&)> disposer)
        : context_(context)
        , rewriter_(std::move(rewriter))
        , disposer_(std::move(disposer))
    {}

    std::vector<std::shared_ptr<Term>> collect(model::expression::Expression* node) {
        std::vector<std::shared_ptr<Term>> results {};
        dispatch(node, results);
        return results;
    }

private:
    Context& context_;
    std::function<std::unique_ptr<model::expression::Expression>(Term&, VariableRewriter&)> rewriter_;
    std::function<void(Term&)> disposer_;
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
        if (is_defined(node->condition()) || !join.equalities().empty()) {
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
            terms = collect(preds, node->condition(), &join.equalities(), true);
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
                next.terms.emplace_back(term);
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
                next.terms.emplace_back(term);
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

    std::vector<std::shared_ptr<Term>> collect(
            Predicates& preds,
            model::expression::Expression* node,
            std::set<std::pair<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>>>* equalities = {},
            bool sticky = false) {
        std::vector<std::shared_ptr<Term>> results {};
        if (is_defined(node)) {
            results = TermCollector {
                context_,
                [this](Term& term, VariableRewriter& rewriter) { return this->rewrite_default(term, rewriter); },
                [](auto&) {}
            }.collect(node);
            for (auto&& term : results) {
                term->sticky_ = sticky;
            }
        }
        if (is_defined(equalities)) {
            for (auto&& pair : *equalities) {
                // NOTE: clang cannot not capture structured binding variables in lambda
                auto& left = std::get<0>(pair);
                auto& right = std::get<1>(pair);
                auto term = std::make_shared<Term>(
                    nullptr,
                    ComparisonTerm {
                        nullptr,
                        ComparisonTerm::Operator::EQ,
                        ComparisonTerm::Factor { left },
                        ComparisonTerm::Factor { right },
                    },
                    [this](Term& term, VariableRewriter& rewriter) {
                        return this->rewrite_comparison(term, rewriter);
                    },
                    [equalities, left, right](Term&) {
                        auto it = equalities->find(std::make_pair(left, right));
                        assert(it != equalities->end());  // NOLINT
                        equalities->erase(it);
                    }
                );
                term->sticky_ = sticky;
                results.emplace_back(std::move(term));
            }
        }
        if (results.empty()) {
            return {};
        }
        std::vector<std::shared_ptr<Term>> terms;
        terms.reserve(results.size() + preds.terms.size());
        for (auto&& term : results) {
            terms.emplace_back(term);
        }
        for (auto&& term : preds.terms) {
            terms.emplace_back(term);
        }
        propagate_constants(preds.rewriter, std::move(terms));
        return results;
    }

    void propagate_constants(VariableRewriter& rewriter, std::vector<std::shared_ptr<Term>> terms) {
        auto constants = build_constants(rewriter, terms);
        if (constants.empty()) {
            return;
        }
        for (auto&& term : terms) {
            if (!*term) {
                continue;
            }
            auto&& comparison = term->comparison_;
            if (!comparison
                    || comparison.op() != ComparisonTerm::Operator::EQ
                    || !comparison.left().is_variable()
                    || !comparison.right().is_variable()) {
                continue;
            }
            {
                auto rewrite = rewriter.apply(comparison.left().variable());
                if (!is_valid(rewrite)) {
                    continue;
                }
                if (auto it = constants.find(rewrite); it != constants.end()) {
                    comparison = ComparisonTerm(
                        comparison.source(),
                        comparison.op(),
                        comparison.right(),
                        ComparisonTerm::Factor(make_clone(it->second)));
                    term->rewriter([this](Term& term, VariableRewriter& rewriter) {
                        return this->rewrite_comparison(term, rewriter);
                    });
                    continue;
                }
            }
            {
                auto rewrite = rewriter.apply(comparison.right().variable());
                if (!is_valid(rewrite)) {
                    continue;
                }
                if (auto it = constants.find(rewrite); it != constants.end()) {
                    comparison.right() = ComparisonTerm::Factor(make_clone(it->second));
                    term->rewriter([this](Term& term, VariableRewriter& rewriter) {
                        return this->rewrite_comparison(term, rewriter);
                    });
                    continue;
                }
            }
        }
    }

    std::map<std::shared_ptr<binding::VariableBinding>, std::unique_ptr<common::core::Value>> build_constants(
        VariableRewriter& rewriter,
            std::vector<std::shared_ptr<Term>>& terms) {
        std::map<std::shared_ptr<binding::VariableBinding>, std::unique_ptr<common::core::Value>> constants {};
        for (auto&& term : terms) {
            if (!term) {
                continue;
            }
            auto&& comparison = term->comparison_;
            if (!comparison
                    || comparison.op() != ComparisonTerm::Operator::EQ
                    || !comparison.left().is_variable()) {
                continue;
            }
            auto left = rewriter.apply(comparison.left().variable());
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
            if (!force && term->sticky_) {
                continue;
            }
            if (auto detached = term->rewrite(preds.rewriter); is_defined(detached)) {
                predicate = merge_condition(std::move(predicate), std::move(detached));
                term->dispose();
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
        node->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            make_clone(type), make_clone(value), constant));
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

    std::unique_ptr<model::expression::Expression> rewrite_default(Term& term, VariableRewriter& rewriter) {
        assert(is_defined(term.node_));  // NOLINT
        auto variables = VariableRewriter::collect(term.node_);
        for (auto* ref : variables) {
            auto var = variable_of(ref);
            if (auto rewrite = rewriter.apply(var); !is_valid(rewrite)) {
                return {};
            }
        }
        for (auto* ref : variables) {
            auto var = variable_of(ref);
            auto rewrite = rewriter.apply(var);
            assert(is_valid(rewrite));  // NOLINT
            ref->variable_key(context_.bindings().create_key(rewrite));
            if (!equals(var->type(), rewrite->type())) {
                encast(rewrite->type(), ref);
            }
        }
        return detach_node(term, create_true_node());
    }

    std::unique_ptr<model::expression::Expression> rewrite_comparison(Term& term, VariableRewriter& rewriter) {
        auto&& comparison = term.comparison_;
        assert(comparison);  // NOLINT
        assert(comparison.op() == ComparisonTerm::Operator::EQ);  // NOLINT
        assert(comparison.left().is_variable());  // NOLINT
        if (auto rewrite = rewriter.apply(comparison.left().variable()); !is_valid(rewrite)) {
            return {};
        }
        if (auto& factor = comparison.right(); factor.is_variable()) {
            if (auto rewrite = rewriter.apply(factor.variable()); !is_valid(rewrite)) {
                return {};
            }
        }
        comparison.left().variable(rewriter.apply(comparison.left().variable()));
        if (auto& factor = comparison.right(); factor.is_variable()) {
            factor.variable(rewriter.apply(factor.variable()));
        }
        auto compare = create_comparison_node(comparison);
        if (!is_defined(term.node_)) {
            return compare;
        }
        common::util::ManagedPtr holder { std::move(compare) };
        encast(type_of(term.node_), holder.get());
        detach_node(term, create_true_node());
        return holder.release();
    }

    std::unique_ptr<model::expression::Expression> create_true_node() {
        auto node = f.Literal(
            std::make_unique<common::core::type::Bool>(common::core::Type::Nullity::NEVER_NULL),
            std::make_unique<common::core::value::Bool>(true));
        node->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            make_clone(node->type()),
            make_clone(node->value()),
            true));
        return node;
    }

    std::unique_ptr<model::expression::Expression> create_comparison_node(ComparisonTerm const& term) {
        auto type = term.left().variable()->type();
        auto left = f.VariableReference(f.Name(name_of(*term.left().variable())));
        bless(left.get(), type);
        left->variable_key(context_.bindings().create_key(term.left().variable()));

        std::unique_ptr<model::expression::Expression> right {};
        if (term.right().is_variable()) {
            auto factor = f.VariableReference(f.Name(name_of(*term.right().variable())));
            bless(right.get(), type);
            factor->variable_key(context_.bindings().create_key(term.right().variable()));
            right = std::move(factor);
        } else if (term.right().is_constant()) {
            auto factor = f.Literal(make_clone(type), make_clone(term.right().constant()));
            bless(factor.get(), factor->type(), factor->value(), true);
            right = std::move(factor);
        } else {
            std::abort();
        }

        auto promoted = analyze::typing::binary_promotion(type_of(left.get()), type_of(right.get()));
        auto op = convert(term.op());
        auto compare = f.BinaryOperator(op,
            std::move(left),
            std::move(right));
        compare->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Bool>(
                term.op() == ComparisonTerm::Operator::EQ || term.op() == ComparisonTerm::Operator::NE
                    ? common::core::Type::Nullity::NEVER_NULL : promoted->nullity()
                )));
        encast(promoted.get(), compare->left());
        encast(promoted.get(), compare->right());

        return compare;
    }

    std::unique_ptr<model::expression::Expression> detach_node(
            Term& term,
            std::unique_ptr<model::expression::Expression> replacement) {
        assert(is_defined(term.node_));  // NOLINT
        std::unique_ptr<model::expression::Expression> detached;
        term.node_->replace([&](auto origin) {
            detached = std::move(origin);
            return std::move(replacement);
        });
        return detached;
    }

    static model::expression::BinaryOperator::Kind convert(ComparisonTerm::Operator value) {
        using IN = ComparisonTerm::Operator;
        using OUT = model::expression::BinaryOperator::Kind;
        switch (value) {
            case IN::EQ: return OUT::EQUAL;
            case IN::NE: return OUT::NOT_EQUAL;
            case IN::LT: return OUT::LESS_THAN;
            case IN::GT: return OUT::GREATER_THAN;
            case IN::LE: return OUT::LESS_THAN_OR_EQUAL;
            case IN::GE: return OUT::GREATER_THAN_OR_EQUAL;
        }
        std::abort();
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
