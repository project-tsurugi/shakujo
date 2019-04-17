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
#include "ProjectionPushDown.h"

#include <map>

#include <cassert>

#include "VariableRewriter.h"
#include "SimplifyPredicate.h"

#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/util/utility.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/expression/ExpressionVisitor.h"
#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/FunctionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"

namespace shakujo::analyzer::optimize {

using common::util::is_defined;
using common::util::is_valid;
using common::util::make_clone;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

namespace {
struct Requirements {
    Requirements() noexcept = default;

    Requirements(binding::RelationBinding const& upstream)  // NOLINT
        : universe_(upstream.output().columns().begin(), upstream.output().columns().end())
    {}

    bool requires(std::shared_ptr<binding::VariableBinding> const& variable) {
        return saw_.find(variable) != saw_.end();
    }

    void add(std::shared_ptr<binding::VariableBinding> const& var) {
        if (universe_.find(var) != universe_.end()) {
            auto pair = saw_.emplace(var);
            if (pair.second) {
                variables_.emplace_back(var);
            }
        }
    }

    void add(std::vector<std::shared_ptr<binding::VariableBinding>> const& vars) {
        for (auto&& var : vars) {
            add(var);
        }
    }

    bool through_heavy() const {
        return through_heavy_;
    }

    void through_heavy(bool value) {
        through_heavy_ = value;
    }

    std::vector<std::shared_ptr<binding::VariableBinding>> const& variables() const {
        return variables_;
    }

private:
    std::set<std::shared_ptr<binding::VariableBinding>> universe_ {};
    std::set<std::shared_ptr<binding::VariableBinding>> saw_ {};
    std::vector<std::shared_ptr<binding::VariableBinding>> variables_ {};
    bool through_heavy_ {};
};

class Engine
    : public model::expression::ExpressionVisitor<void, Requirements&&> {
public:
    explicit Engine(Context& context) : context_(context) {}

private:
    template<class T>
    auto binding_of(T *key) {
        return context_.binding_of(key);
    }

    template<class T>
    std::shared_ptr<binding::RelationBinding> relation_of(T* node) {
        auto* provider = dynamic_pointer_cast<model::key::RelationKey::Provider>(node);
        return binding_of(provider->relation_key());
    }

    common::core::Type const* type_of(model::key::ExpressionKey::Provider* node) {
        return context_.bindings().get(node->expression_key())->type();
    }

    template<class T>
    auto to_key(std::shared_ptr<T> binding) {
        return context_.bindings().create_key(std::move(binding));
    }

    template<class T, class... Args>
    auto create_key(Args&&... args) {
        return to_key(std::make_shared<T>(std::forward<Args>(args)...));
    }

    auto create_null_value() {
        auto value = f.Literal(
            std::make_unique<common::core::type::Null>(),
            std::make_unique<common::core::value::Null>());
        value->expression_key(create_key<binding::ExpressionBinding>(
            make_clone(value->type()),
            make_clone(value->value()),
            true));
        return value;
    }

    auto create_null_variable() {
        auto id = context_.bindings().next_variable_id();
        common::core::Name name { id.to_unique_name() };
        return std::make_shared<binding::VariableBinding>(
            std::move(id),
            std::move(name),
            std::make_unique<common::core::type::Null>(),
            std::make_unique<common::core::value::Null>());
    }

    static std::string name_of(binding::VariableBinding const& variable) {
        if (!variable.name().empty()) {
            return variable.name().segments()[variable.name().segments().size() - 1];
        }
        return variable.id().to_unique_name();
    }

    void collect(Requirements& requirements, model::expression::Expression* node) {
        auto refs = VariableRewriter::collect(node);
        for (auto&& ref : refs) {
            auto var = binding_of(ref->variable_key());
            requirements.add(var);
        }
    }

    void flush(model::expression::Expression* node, Requirements&& requirements) {
        if (!requirements.through_heavy()) {
            return;
        }
        auto relation = relation_of(node);
        if (requirements.variables().size() >= relation->output().columns().size()) {
            return;
        }
        auto type = dynamic_pointer_cast<common::core::type::Relation>(type_of(node));
        assert(type->columns().size() == relation->output().columns().size());  // NOLINT

        std::vector<std::unique_ptr<model::expression::relation::ProjectionExpression::Column>> columns {};
        columns.reserve(requirements.variables().size());
        for (auto&& var : requirements.variables()) {
            auto ref = f.VariableReference(f.Name(name_of(*var)));
            ref->expression_key(create_key<binding::ExpressionBinding>(
                make_clone(var->type()),
                make_clone(var->value()),
                is_valid(var->value())));
            ref->variable_key(to_key(var));

            auto index = relation->output().index_of(*var);
            assert(index.has_value());  // NOLINT
            auto&& name = type->at(index.value()).name();
            auto column = f.ProjectionExpressionColumn(std::move(ref), f.Name(name));
            column->variable_key(to_key<binding::VariableBinding>(var));

            columns.emplace_back(std::move(column));
        }

        auto* projection = node->replace([&](auto origin) {
            return f.ProjectionExpression(std::move(origin), std::move(columns));
        });

        normalize(dynamic_pointer_cast<model::expression::relation::ProjectionExpression>(projection));
    }

    void normalize(model::expression::relation::ProjectionExpression* node) {
        if (node->columns().empty()) {
            auto column = f.ProjectionExpressionColumn(create_null_value());
            column->variable_key(to_key(create_null_variable()));
            node->columns().push_back(std::move(column));
        }
        {
            std::vector<common::core::type::Relation::Column> columns {};
            columns.reserve(node->columns().size());
            for (auto* column : node->columns()) {
                std::string_view name {};
                if (column->alias()) {
                    name = column->alias()->token();
                }
                auto var = binding_of(column->variable_key());
                columns.emplace_back(name, make_clone(var->type()));
            }
            node->expression_key(create_key<binding::ExpressionBinding>(
                std::make_unique<common::core::type::Relation>(std::move(columns))));
        }
        {
            std::vector<std::shared_ptr<binding::VariableBinding>> columns {};
            columns.reserve(node->columns().size());
            for (auto* column : node->columns()) {
                auto out = binding_of(column->variable_key());
                columns.emplace_back(out);
            }
            auto parent = relation_of(node->operand());
            node->relation_key(create_key<binding::RelationBinding>(
                parent->output(),
                binding::RelationBinding::Profile { std::move(columns) }));
        }
    }

public:
    using ExpressionVisitor::visit;

    void visitDefault(model::expression::Expression*, Requirements&&) override {} 

    void visit(model::expression::relation::ScanExpression* node, Requirements&& prev) override {
        flush(node, std::move(prev));
    }

    void visit(model::expression::relation::SelectionExpression* node, Requirements&& prev) override {
        // just forward
        collect(prev, node->condition());
        dispatch(node->operand(), std::move(prev));
    }

    void visit(model::expression::relation::ProjectionExpression* node, Requirements&& prev) override {
        auto&& columns = node->columns();
        for (std::size_t i = 0; i < columns.size();) {
            auto* column = columns[i];
            auto&& var = binding_of(column->variable_key());
            if (!prev.requires(var)) {
                columns.remove(i);
            } else {
                ++i;
            }
        }
        Requirements next { *relation_of(node->operand()) };
        for (auto* column : columns) {
            collect(next, column->value());
        }
        dispatch(node->operand(), std::move(next));
        normalize(node);
    }

    void visit(model::expression::relation::AggregationExpression* node, Requirements&& prev) override {
        auto&& columns = node->columns();
        for (std::size_t i = 0; i < columns.size();) {
            auto* column = columns[i];
            auto&& var = binding_of(column->variable_key());
            if (!prev.requires(var)) {
                columns.remove(i);
            } else {
                ++i;
            }
        }
        if (columns.empty()) {
            auto column = f.AggregationExpressionColumn(
                {},
                model::expression::FunctionCall::Quantifier::ABSENT,
                create_null_value());
            // NOTE: empty function binding represents group key or constant, should be pick any one in group
            column->function_key(create_key<binding::FunctionBinding>());
            column->variable_key(to_key(create_null_variable()));
            columns.push_back(std::move(column));
        }

        Requirements next { *relation_of(node->operand()) };
        for (auto* key : node->keys()) {
            collect(next, key);
        }
        for (auto* column : columns) {
            if (is_defined(column->operand())) {
                collect(next, column->operand());
            }
        }
        dispatch(node->operand(), std::move(next));
    }

    void visit(model::expression::relation::DistinctExpression* node, Requirements&& prev) override {
        flush(node, std::move(prev));

        // DISTINCT requires all columns
        auto relation = relation_of(node->operand());
        Requirements requirements { *relation };
        requirements.add(relation->output().columns());
        dispatch(node->operand(), std::move(requirements));
    }

    void visit(model::expression::relation::OrderExpression* node, Requirements&& prev) override {
        for (auto* element : node->elements()) {
            collect(prev, element->key());
        }
        prev.through_heavy(true);
        dispatch(node->operand(), std::move(prev));
    }

    void visit(model::expression::relation::LimitExpression* node, Requirements&& prev) override {
        dispatch(node->operand(), std::move(prev));
    }

    void visit(model::expression::relation::JoinExpression* node, Requirements&& prev) override {
        auto relation = relation_of(node);
        auto&& strategy = relation->join_strategy();
        {
            auto&& columns = strategy.columns();
            for (auto it = columns.begin(); it != columns.end();) {
                bool erased = false;
                if (!prev.requires(it->output())) {
                    it = columns.erase(it);
                    erased = true;
                }
                if (!erased) {
                    ++it;
                }
            }
            if (columns.empty()) {
                columns.emplace_back(
                    std::vector<common::core::Name> {},
                    create_null_variable(),
                    std::shared_ptr<binding::VariableBinding> {}, false,
                    std::shared_ptr<binding::VariableBinding> {}, false);
            }
        }
        {
            Requirements next { *relation_of(node->left()) };
            for (auto&& column : strategy.columns()) {
                if (is_defined(column.left_source())) {
                    next.add(column.left_source());
                }
            }
            for (auto&& pair : strategy.equalities()) {
                next.add(std::get<0>(pair));
            }
            for (auto&& column : strategy.seek_columns()) {
                if (!column.is_resolved()) {
                    next.add(column.variable());
                }
            }
            if (is_defined(node->condition())) {
                collect(next, node->condition());
            }
            dispatch(node->left(), std::move(next));
        }
        {
            Requirements next { *relation_of(node->right()) };
            for (auto&& column : strategy.columns()) {
                if (is_defined(column.right_source())) {
                    next.add(column.right_source());
                }
            }
            for (auto&& pair : strategy.equalities()) {
                next.add(std::get<1>(pair));
            }
            // NOTE: seek_columns only contains variable from left operand
            if (is_defined(node->condition())) {
                collect(next, node->condition());
            }
            dispatch(node->right(), std::move(next));
        }
    }

private:
    Context& context_;
    model::IRFactory f {};
};

class Walker : public model::util::NodeWalker {
public:
    explicit Walker(Context& context) noexcept : context_(context) {}

    using NodeWalker::enter;
    using NodeWalker::enterDefault;

    bool enterDefault(model::expression::Expression* node) override {
        if (auto* provider = dynamic_pointer_cast_if<model::key::RelationKey::Provider>(node)) {
            auto relation = context_.bindings().get(provider->relation_key());
            Requirements requirements { *relation };
            requirements.add(relation->output().columns());
            Engine engine { context_ };
            engine.dispatch(node, std::move(requirements));
            return false;
        }
        return true;
    }

    bool enter(model::statement::dml::UpdateStatement* node) override {
        // update statement requires all columns in source
        for (auto&& set : node->columns()) {
            walk(set->value());
        }
        return false;
    }

    bool enter(model::statement::dml::DeleteStatement*) override {
        // delete statement requires all columns in source, temporary
        return false;
    }

private:
    Context& context_;
};
}  // namespace

void ProjectionPushDown::operator()(model::program::Program* node) {
    Walker adapter { context_ };
    adapter.walk(node);
}

void ProjectionPushDown::operator()(model::statement::Statement* node) {
    Walker adapter { context_ };
    adapter.walk(node);
}

void ProjectionPushDown::operator()(model::expression::Expression *node) {
    Walker adapter { context_ };
    adapter.walk(node);
}
}  // namespace shakujo::analyzer::optimize
