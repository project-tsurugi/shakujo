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
#include "FixRelationInfo.h"

#include <map>
#include <set>

#include <cassert>
#include <shakujo/common/core/type/Relation.h>

#include "ComparisonTerm.h"

#include "shakujo/common/util/utility.h"

#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"

namespace shakujo::analyzer::optimize {

using common::util::is_valid;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

namespace {

class Engine : public model::util::NodeWalker {
public:
    explicit Engine(Context& context) noexcept : context_(context) {
        (void) context_;
    }

    template<class T>
    std::shared_ptr<binding::RelationBinding> extract(T* node) {
        auto* provider = dynamic_pointer_cast<model::key::RelationKey::Provider>(node);
        return context_.bindings().get(provider->relation_key());
    }

    template<class T>
    auto extract_from_key(T* key) {
        return context_.bindings().get(key);
    }

    void exit(model::expression::relation::ScanExpression* node) override {
        auto relation = extract(node);
        auto&& profile = relation->output();
        auto&& strategy = relation->scan_strategy();
        profile.order().clear();
        profile.unique_keys().clear();

        assert(profile.columns().size() == strategy.table().columns().size());  // NOLINT
        std::map<std::string, std::shared_ptr<binding::VariableBinding>> columns {};
        for (std::size_t i = 0, n = profile.columns().size(); i < n; ++i) {
            columns.emplace(strategy.table().columns()[i].name(), profile.columns()[i]);
        }
        if (auto&& index = strategy.table().primary_index(); is_valid(index)) {
            std::set<std::shared_ptr<binding::VariableBinding>> key {};
            for (auto&& column : index.columns()) {
                auto it = columns.find(column.name());
                assert(it != columns.end());  // NOLINT
                key.emplace(it->second);
            }
            profile.unique_keys().emplace(std::move(key));
        }
        if (auto&& index = strategy.index(); is_valid(index)) {
            // FIXME: strict unique index?
            // FIXME: test is sorted index?
            std::vector<binding::RelationBinding::Order> order {};
            order.reserve(index.columns().size());
            for (auto&& column : index.columns()) {
                auto it = columns.find(column.name());
                assert(it != columns.end());  // NOLINT
                order.emplace_back(it->second, column.direction());
            }
            profile.order() = std::move(order);
        }
        rebuild_type(node, *relation);
    }

    void exit(model::expression::relation::SelectionExpression* node) override {
        auto parent = extract(node->operand());
        auto relation = extract(node);
        relation->process() = parent->output();
        relation->output() = parent->output();
        rebuild_type(node, *relation);
    }

    void exit(model::expression::relation::LimitExpression* node) override {
        auto parent = extract(node->operand());
        auto relation = extract(node);
        relation->process() = parent->output();
        relation->output() = parent->output();
        rebuild_type(node, *relation);
    }

    void exit(model::expression::relation::DistinctExpression* node) override {
        auto parent = extract(node->operand());
        if (parent->output().distinct()) {
            node->replace_with(node->release_operand());
            return;
        }

        auto relation = extract(node);
        relation->process() = parent->output();
        relation->output() = parent->output();
        assert(!relation->output().unique_keys().empty());  // NOLINT

        std::set<std::shared_ptr<binding::VariableBinding>> key {};
        for (auto&& column : relation->output().columns()) {
            // FIXME: use only subset
            key.emplace(column);
        }
        relation->output().unique_keys().emplace(std::move(key));
        rebuild_type(node, *relation);
    }

    void exit(model::expression::relation::JoinExpression* node) override {
        auto left = extract(node->left());
        auto right = extract(node->right());
        std::vector<std::shared_ptr<binding::VariableBinding>> input {};
        input.reserve(left->output().columns().size() + right->output().columns().size());
        for (auto&& v : left->output().columns()) {
            input.emplace_back(v);
        }
        for (auto&& v : right->output().columns()) {
            input.emplace_back(v);
        }

        auto relation = extract(node);
        relation->process() = input;

        auto&& output = relation->output();
        auto&& strategy = relation->join_strategy();

        // FIXME: ordering?
        output.order().clear();

        output.unique_keys().clear();
        if (left->output().distinct() && right->output().distinct()) {
            std::map<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>> iomap {};
            for (auto&& column : strategy.columns()) {
                if (is_valid(column.left_source())) {
                    iomap.emplace(column.left_source(), column.output());
                }
                if (is_valid(column.right_source())) {
                    iomap.emplace(column.right_source(), column.output());
                }
            }
            for (auto&& left_key : left->output().unique_keys()) {
                bool left_diff = false;
                std::set<std::shared_ptr<binding::VariableBinding>> left_output_key {};
                for (auto&& var : left_key) {
                    if (auto it = iomap.find(var); it != iomap.end()) {
                        left_output_key.emplace(it->second);
                    } else {
                        left_diff = true;
                        break;
                    }
                }
                if (left_diff) {
                    continue;
                }
                for (auto&& right_key : right->output().unique_keys()) {
                    bool right_diff = false;
                    std::set<std::shared_ptr<binding::VariableBinding>> output_key { left_output_key };
                    for (auto&& var : right_key) {
                        if (auto it = iomap.find(var); it != iomap.end()) {
                            output_key.emplace(it->second);
                        } else {
                            right_diff = true;
                            break;
                        }
                    }
                    if (right_diff) {
                        continue;
                    }
                    output.unique_keys().emplace(std::move(output_key));
                }
            }
        }
        rebuild_type(node, *relation);
    }

    void exit(model::expression::relation::OrderExpression* node) override {
        auto parent = extract(node->operand());
        auto relation = extract(node);
        relation->process() = parent->output();
        relation->output() = parent->output();
        auto&& output = relation->output();
        output.order().clear();
        for (auto* element : node->elements()) {
            auto expr = extract_from_key(element->key()->expression_key());
            auto var = extract_from_key(element->variable_key());
            // just a column reference
            if (parent->output().index_of(*var).has_value()) {
                output.order().emplace_back(
                    std::move(var),
                    element->direction() == model::expression::relation::OrderExpression::Direction::ASCENDANT
                        ? common::core::Direction::ASCENDANT
                        : common::core::Direction::DESCENDANT);
                continue;
            }
            // skip constant values
            if (expr->constant()) {
                continue;
            }
            // no more sort info
            break;
        }
        if (auto&& input = parent->output(); input.order().size() >= output.order().size()) {
            bool diff = false;
            for (std::size_t i = 0, n = output.order().size(); i < n; ++i) {
                auto&& in = input.order()[i];
                auto&& out = output.order()[i];
                if (in.column() != out.column() || in.direction() == out.direction()) {
                    diff = true;
                    break;
                }
            }
            if (!diff) {
                node->replace_with(node->release_operand());
                return;
            }
        }
        rebuild_type(node, *relation);
    }

    void exit(model::expression::relation::ProjectionExpression* node) override {
        auto parent = extract(node->operand());
        auto relation = extract(node);
        relation->process() = parent->output();

        std::vector<std::shared_ptr<binding::VariableBinding>> columns {};
        columns.reserve(node->columns().size());
        std::map<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>> iomap;
        for (auto* column : node->columns()) {
            auto out = extract_from_key(column->variable_key());
            columns.emplace_back(out);
            if (auto in = collect_variable(column->value()); is_valid(in)) {
                iomap.emplace(in, out);
            }
        }
        binding::RelationBinding::Profile output { std::move(columns) };
        if (auto&& source = relation->process().order(); !source.empty()) {
            output.order().clear();
            std::vector<binding::RelationBinding::Order> order {};
            for (auto&& element : source) {
                if (auto it = iomap.find(element.column()); it != iomap.end()) {
                    output.order().emplace_back(it->second, element.direction());
                } else {
                    break;
                }
            }
        }
        for (auto&& source : relation->process().unique_keys()) {
            std::set<std::shared_ptr<binding::VariableBinding>> key {};
            for (auto&& var : source) {
                if (auto it = iomap.find(var); it != iomap.end()) {
                    key.emplace(it->second);
                } else {
                    break;
                }
            }
            if (source.size() == key.size()) {
                output.unique_keys().emplace(std::move(key));
            }
        }
        relation->output() = std::move(output);

        std::vector<common::core::type::Relation::Column> type_columns {};
        for (auto* column : node->columns()) {
            std::string_view name {};
            if (column->alias()) {
                name = column->alias()->token();
            }
            auto var = extract_from_key(column->variable_key());
            type_columns.emplace_back(name, make_clone(var->type()));
        }
        node->expression_key(context_.bindings().create_key(std::make_shared<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Relation>(std::move(type_columns)))));
    }

    void exit(model::expression::relation::AggregationExpression* node) override {
        auto parent = extract(node->operand());
        auto relation = extract(node);
        relation->process() = parent->output();

        std::vector<std::shared_ptr<binding::VariableBinding>> columns {};
        columns.reserve(node->columns().size());
        for (auto* column : node->columns()) {
            auto out = extract_from_key(column->variable_key());
            columns.emplace_back(out);
        }
        binding::RelationBinding::Profile output { std::move(columns) };
        relation->output() = std::move(output);
        rebuild_type(node, *relation);
    }

    void exit(model::statement::dml::EmitStatement* node) override {
        auto parent = extract(node->source());
        auto relation = extract(node);
        relation->process() = parent->output();
    }

    void exit(model::statement::dml::InsertValuesStatement* node) override {
        // nothing to do
        (void) node;
    }

    void exit(model::statement::dml::InsertRelationStatement* node) override {
        auto parent = extract(node->source());
        auto relation = extract(node);
        relation->process() = parent->output();
    }

    void exit(model::statement::dml::UpdateStatement* node) override {
        auto parent = extract(node->source());
        auto relation = extract(node);
        relation->process() = parent->output();
    }

    void exit(model::statement::dml::DeleteStatement* node) override {
        auto parent = extract(node->source());
        auto relation = extract(node);
        relation->process() = parent->output();
    }

    void rebuild_type(model::expression::Expression* node, binding::RelationBinding const& relation) {
        std::vector<common::core::type::Relation::Column> columns {};
        for (auto&& var : relation.output().columns()) {
            columns.emplace_back(make_clone(var->type()));
        }
        node->expression_key(context_.bindings().create_key(std::make_shared<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Relation>(std::move(columns)))));
    }

    std::shared_ptr<binding::VariableBinding> collect_variable(model::expression::Expression* node, bool ignore_cast = false) {
        auto* current = node;
        if (ignore_cast) {
            while (auto* cast = dynamic_pointer_cast_if<model::expression::ImplicitCast>(current)) {
                current = cast->operand();
            }
        }
        if (auto* ref = dynamic_pointer_cast_if<model::expression::VariableReference>(current)) {
            return extract_from_key(ref->variable_key());
        }
        return {};
    }

private:
    Context& context_;
};
}  // namespace

void FixRelationInfo::operator()(model::program::Program* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void FixRelationInfo::operator()(model::statement::Statement* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void FixRelationInfo::operator()(model::expression::Expression *node) {
    Engine engine { context_ };
    engine.walk(node);
}
}  // namespace shakujo::analyzer::optimize
