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

#include "ComparisonTerm.h"

#include "shakujo/common/core/type/Relation.h"
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

private:
    template<class T>
    std::shared_ptr<binding::RelationBinding> relation_of(T* node) {
        auto* provider = dynamic_pointer_cast<model::key::RelationKey::Provider>(node);
        auto binding = context_.bindings().find(provider->relation_key());
        return binding;
    }

    template<class T = common::core::Type>
    T const* type_of(model::expression::Expression* node) {
        return dynamic_pointer_cast<T>(context_.bindings().get(node->expression_key())->type());
    }

    template<class T>
    auto binding_of(T* key) {
        return context_.bindings().get(key);
    }

    void rebuild_type(
        model::expression::Expression* node,
        common::core::type::Relation const* parent_type,
        std::map<std::shared_ptr<binding::VariableBinding>, std::size_t> const& column_mapping) {
        auto relation = relation_of(node);
        std::vector<common::core::type::Relation::Column> columns {};
        for (auto&& var : relation->output().columns()) {
            if (auto it = column_mapping.find(var); it != column_mapping.end()) {
                auto&& inherited = parent_type->at(it->second);
                columns.emplace_back(inherited.qualifiers(), inherited.name(), make_clone(var->type()));
            }
            columns.emplace_back(make_clone(var->type()));
        }
        node->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Relation>(std::move(columns))));
    }

    void inherit_type(model::expression::Expression* node, common::core::type::Relation const* parent_type) {
        node->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(make_clone(parent_type)));
    }

    std::shared_ptr<binding::VariableBinding> collect_variable(model::expression::Expression* node, bool ignore_cast = false) {
        auto* current = node;
        if (ignore_cast) {
            while (auto* cast = dynamic_pointer_cast_if<model::expression::ImplicitCast>(current)) {
                current = cast->operand();
            }
        }
        if (auto* ref = dynamic_pointer_cast_if<model::expression::VariableReference>(current)) {
            return binding_of(ref->variable_key());
        }
        return {};
    }

    void extract_constants(model::expression::Expression* condition, binding::RelationBinding::Profile& profile) {
        auto terms = ComparisonTerm::collect(context_.bindings(), condition);
        auto&& constants = profile.constants();
        for (auto&& term : terms) {
            if (term.op() != ComparisonTerm::Operator::EQ
                    || !term.left().is_variable()
                    || !term.right().is_constant()) {
                continue;
            }
            constants.emplace(term.left().variable());
        }
    }

public:
    void exit(model::expression::relation::ScanExpression* node) override {
        auto relation = relation_of(node);
        auto&& profile = relation->output();
        auto&& strategy = relation->scan_strategy();

        auto&& constants = profile.constants();
        constants.clear();
        for (std::size_t i = 0, n = strategy.prefix().size(); i < n; ++i) {
            constants.emplace(strategy.key_columns()[i]);
        }

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
                auto&& var = it->second;
                if (constants.find(var) != constants.end()) {
                    continue;
                }
                key.emplace(var);
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
                auto&& var = it->second;
                if (constants.find(var) != constants.end()) {
                    continue;
                }
                order.emplace_back(var, column.direction());
            }
            profile.order() = std::move(order);
        }
        // type information is unchanged
    }

    void exit(model::expression::relation::RenameExpression* node) override {
        auto parent = relation_of(node->operand());
        auto relation = relation_of(node);

        if (!node->columns().empty()) {
            std::vector<std::size_t> indices {};
            indices.reserve(parent->output().columns().size());
            for (auto&& column : parent->output().columns()) {
                auto index = relation->process().index_of(*column);
                assert(index.has_value());  // NOLINT
                indices.emplace_back(index.value());
            }
            // high watermark of columns to keep their positions from the first
            std::size_t kept = 0;
            std::size_t moved = 0;
            for (std::size_t i = 0, n = indices.size(); i < n; ++i) {
                // adjust index on origin considering moved columns
                auto index_on_origin = indices[i];
                for (std::size_t j = kept; j < i; ++j) {
                    if (indices[j] < indices[i]) {
                        --index_on_origin;
                    }
                }

                // don't move back if the target column is just on the watermark
                if (index_on_origin == kept) {
                    ++kept;
                    continue;
                }

                // move back the column
                auto name = node->columns().release(index_on_origin);
                node->columns().push_back(std::move(name));
                ++moved;
            }

            // remove between the [high watermark of columns to keep, column size - moved back columns)
            for (std::size_t i = kept, n = node->columns().size() - moved; i < n; ++i) {
                node->columns().remove(kept);
            }
        }

        auto origin = type_of<common::core::type::Relation>(node->operand());

        std::vector<common::core::Name> qualifiers {};
        qualifiers.reserve(1);
        qualifiers.emplace_back(*node->name());

        std::vector<common::core::type::Relation::Column> columns {};
        columns.reserve(origin->columns().size());

        std::size_t column_index = 0;
        for (auto&& column : origin->columns()) {
            std::string const* name;
            if (node->columns().empty()) {
                name = &column.name();
            } else {
                name = &node->columns()[column_index]->token();
            }
            columns.emplace_back(qualifiers, *name, make_clone(column.type()));
            ++column_index;
        }

        relation->process() = parent->output();
        relation->output() = parent->output();

        node->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Relation>(std::move(columns))));
    }

    void exit(model::expression::relation::SelectionExpression* node) override {
        auto parent = relation_of(node->operand());
        auto relation = relation_of(node);
        relation->process() = parent->output();
        relation->output() = parent->output();
        extract_constants(node->condition(), relation->output());
        inherit_type(node, type_of<common::core::type::Relation>(node->operand()));
    }

    void exit(model::expression::relation::LimitExpression* node) override {
        auto parent = relation_of(node->operand());
        auto relation = relation_of(node);
        relation->process() = parent->output();
        relation->output() = parent->output();
        inherit_type(node, type_of<common::core::type::Relation>(node->operand()));
    }

    void exit(model::expression::relation::DistinctExpression* node) override {
        auto parent = relation_of(node->operand());
        if (context_.options().redundancy.distinct && parent->output().distinct()) {
            node->replace_with(node->release_operand());
            return;
        }

        auto relation = relation_of(node);
        relation->process() = parent->output();
        relation->output() = parent->output();

        if (auto&& output = relation->output(); !output.distinct()) {
            output.unique_keys().clear();
            std::set<std::shared_ptr<binding::VariableBinding>> key {};
            auto&& constants = relation->output().constants();
            for (auto&& column : output.columns()) {
                if (constants.find(column) != constants.end()) {
                    continue;
                }
                key.emplace(column);
            }
            output.unique_keys().emplace(std::move(key));
        }
        inherit_type(node, type_of<common::core::type::Relation>(node->operand()));
    }

    void exit(model::expression::relation::JoinExpression* node) override {
        auto left = relation_of(node->left());
        auto right = relation_of(node->right());
        std::vector<std::shared_ptr<binding::VariableBinding>> input {};
        input.reserve(left->output().columns().size() + right->output().columns().size());
        for (auto&& v : left->output().columns()) {
            input.emplace_back(v);
        }
        for (auto&& v : right->output().columns()) {
            input.emplace_back(v);
        }

        auto relation = relation_of(node);
        relation->process() = input;

        auto&& output = relation->output();
        auto&& strategy = relation->join_strategy();

        output.columns().clear();
        for (auto&& column : strategy.columns()) {
            output.columns().emplace_back(column.output());
        }

        auto&& constants = output.constants();
        constants.clear();
        {
            auto&& lconsts = left->output().constants();
            auto&& rconsts = right->output().constants();
            for (auto&& column : strategy.columns()) {
                if (!strategy.right_outer()) {
                    if (auto&& c = column.left_source(); is_valid(c) && lconsts.find(c) != lconsts.end()) {
                        constants.emplace(column.output());
                        continue;
                    }
                }
                if (!strategy.left_outer()) {
                    if (auto&& c = column.right_source(); is_valid(c) && rconsts.find(c) != rconsts.end()) {
                        constants.emplace(column.output());
                        continue;
                    }
                }
            }
        }

        // FIXME: ordering?
        output.order().clear();

        output.unique_keys().clear();
        if (left->output().distinct() && right->output().distinct()
                && !strategy.left_outer() && !strategy.right_outer()) {
            // FIXME: re-think uniqueness for outer joins
            std::map<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>> iomap {};
            for (auto&& column : strategy.columns()) {
                if (is_valid(column.left_source())) {
                    iomap.emplace(column.left_source(), column.output());
                }
                if (is_valid(column.right_source())) {
                    iomap.emplace(column.right_source(), column.output());
                }
            }
            std::map<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>> eqmap {};
            std::vector<std::pair<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>>> pairs {};
            if (is_defined(node->condition())) {
                auto terms = ComparisonTerm::collect(context_.bindings(), node->condition());
                for (auto&& term : terms) {
                    if (term.op() == ComparisonTerm::Operator::EQ
                            && term.left().is_variable() && term.right().is_variable()) {
                        pairs.emplace_back(term.left().variable(), term.right().variable());
                    }
                }
            }
            for (auto&& pair : strategy.equalities()) {
                pairs.emplace_back(pair);
            }
            for (auto&& [a, b] : pairs) {
                auto ita = iomap.find(a);
                if (ita == iomap.end()) {
                    continue;
                }
                auto itb = iomap.find(b);
                if (itb == iomap.end()) {
                    continue;
                }
                if (ita->second == itb->second) {
                    continue;
                }
                // if LEFT OUTER JOIN, the replacement must be come from left operand
                if (!strategy.left_outer() || left->output().index_of(*itb->first).has_value()) {
                    eqmap.emplace(ita->second, itb->second);
                }
                // if RIGHT OUTER JOIN, the replacement must be come from right operand
                if (!strategy.right_outer() || right->output().index_of(*ita->first).has_value()) {
                    eqmap.emplace(itb->second, ita->second);
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
            if (!output.unique_keys().empty() && !eqmap.empty()) {
                // reduces unique key factors using equivalent relations

                // first, we remove each element from eqmap which does not appear in unique keys
                for (auto it = eqmap.begin(); it != eqmap.end();) {
                    bool found = false;
                    for (auto&& key : output.unique_keys()) {
                        if (key.find(it->first) != key.end()) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        it = eqmap.erase(it);
                    } else {
                        ++it;
                    }
                }
                // then, we replace unique key elements with equivalent pairs
                for (auto&& [k, v] : eqmap) {
                    std::vector<std::set<std::shared_ptr<binding::VariableBinding>>> increased {};
                    auto&& current = output.unique_keys();
                    for (auto it = current.begin(); it != current.end();) {
                        auto&& key = *it;
                        bool erased = false;
                        if (key.find(k) != key.end()) {
                            auto copy = key;
                            if (auto itc = copy.find(k); itc != copy.end()) {
                                copy.erase(itc);
                                copy.emplace(v);
                                if (current.find(copy) == current.end()) {
                                    increased.emplace_back(std::move(copy));
                                }
                            }
                            if (key.find(v) != key.end()) {
                                // both pair exists
                                it = current.erase(it);
                                erased = true;
                            }
                        }
                        if (!erased) {
                            ++it;
                        }
                    }
                    for (auto&& key : increased) {
                        current.emplace(std::move(key));
                    }
                }
            }
        }

        std::vector<common::core::type::Relation::Column> type_columns {};
        for (auto&& column : strategy.columns()) {
            // FIXME: naming rules
            auto&& orig = column.output()->name();
            std::string name;
            if (!orig.empty()) {
                name = *orig.segments().rbegin();
            }
            type_columns.emplace_back(
                column.qualifiers(),
                std::move(name),
                make_clone(column.output()->type()));
        }
        node->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Relation>(std::move(type_columns))));
    }

    void exit(model::expression::relation::OrderExpression* node) override {
        auto parent = relation_of(node->operand());
        auto relation = relation_of(node);
        relation->process() = parent->output();
        relation->output() = parent->output();
        auto&& output = relation->output();
        auto&& constants = output.constants();
        output.order().clear();
        bool saw_unresolved = false;
        auto&& elements = node->elements();
        for (std::size_t i = 0; i < elements.size();) {
            auto* element = elements[i];
            auto expr = binding_of(element->key()->expression_key());
            auto var = binding_of(element->variable_key());

            // remove constant columns
            if (expr->constant()) {
                elements.remove(i);
                continue;
            }

            // just a column reference
            if (parent->output().index_of(*var).has_value()) {
                if (constants.find(var) == constants.end()) {
                    output.order().emplace_back(
                        std::move(var),
                        element->direction() == model::expression::relation::OrderExpression::Direction::ASCENDANT
                            ? common::core::Direction::ASCENDANT
                            : common::core::Direction::DESCENDANT);
                }
                ++i;
                continue;
            }

            // cannot resolve in compile time..
            saw_unresolved = true;
            break;
        }
        if (node->elements().empty()) {
            node->replace_with(node->release_operand());
            return;
        }
        if (auto&& input = parent->output();
                context_.options().redundancy.sort
                && input.order().size() >= output.order().size()
                && !saw_unresolved) {
            bool diff = false;
            for (std::size_t i = 0, n = output.order().size(); i < n; ++i) {
                auto&& in = input.order()[i];
                auto&& out = output.order()[i];
                if (in.column() != out.column() || in.direction() != out.direction()) {
                    diff = true;
                    break;
                }
            }
            if (!diff) {
                node->replace_with(node->release_operand());
                return;
            }
        }
        inherit_type(node, type_of<common::core::type::Relation>(node->operand()));
    }

    void exit(model::expression::relation::ProjectionExpression* node) override {
        auto parent = relation_of(node->operand());
        auto relation = relation_of(node);
        relation->process() = parent->output();
        relation->output() = {};
        auto&& output = relation->output();
        auto&& columns = output.columns();
        columns.reserve(node->columns().size());
        auto&& constants = output.constants();
        std::map<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>> iomap;
        for (auto* column : node->columns()) {
            auto out = binding_of(column->variable_key());
            columns.emplace_back(out);
            if (auto in = collect_variable(column->value()); is_valid(in)) {
                iomap.emplace(in, out);
                if (auto it = parent->output().constants().find(in); it != parent->output().constants().end()) {
                    constants.emplace(out);
                }
            }
            if (auto expr = binding_of(column->value()->expression_key()); is_valid(expr) && expr->constant()) {
                constants.emplace(out);
            }
        }

        if (auto&& source = parent->output().order(); !source.empty()) {
            output.order().clear();
            std::vector<binding::RelationBinding::Order> order {};
            for (auto&& element : source) {
                if (auto it = iomap.find(element.column()); it != iomap.end()) {
                    auto&& out = it->second;
                    if (constants.find(out) != constants.end()) {
                        continue;
                    }
                    output.order().emplace_back(out, element.direction());
                } else {
                    break;
                }
            }
        }
        for (auto&& source : parent->output().unique_keys()) {
            bool broken = false;
            std::set<std::shared_ptr<binding::VariableBinding>> key {};
            for (auto&& var : source) {
                if (auto it = iomap.find(var); it != iomap.end()) {
                    auto&& out = it->second;
                    if (constants.find(out) != constants.end()) {
                        continue;
                    }
                    key.emplace(out);
                } else {
                    broken = true;
                    break;
                }
            }
            if (!broken) {
                output.unique_keys().emplace(std::move(key));
            }
        }

        std::vector<common::core::type::Relation::Column> type_columns {};
        type_columns.reserve(node->columns().size());
        for (auto* column : node->columns()) {
            std::string_view name {};
            if (column->alias()) {
                name = column->alias()->token();
            }
            auto var = binding_of(column->variable_key());
            type_columns.emplace_back(name, make_clone(var->type()));
        }
        node->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Relation>(std::move(type_columns))));
    }

    void exit(model::expression::relation::AggregationExpression* node) override {
        auto parent = relation_of(node->operand());
        auto relation = relation_of(node);
        relation->process() = parent->output();

        std::vector<std::shared_ptr<binding::VariableBinding>> columns {};
        columns.reserve(node->columns().size());
        for (auto* column : node->columns()) {
            auto out = binding_of(column->variable_key());
            columns.emplace_back(out);
        }
        binding::RelationBinding::Profile output { std::move(columns) };
        relation->output() = std::move(output);
        rebuild_type(node, type_of<common::core::type::Relation>(node->operand()), {});
    }

    void exit(model::statement::dml::EmitStatement* node) override {
        auto parent = relation_of(node->source());
        auto relation = relation_of(node);
        relation->process() = parent->output();
    }

    void exit(model::statement::dml::InsertValuesStatement* node) override {
        // nothing to do
        (void) node;
    }

    void exit(model::statement::dml::InsertRelationStatement* node) override {
        auto parent = relation_of(node->source());
        auto relation = relation_of(node);
        relation->process() = parent->output();
    }

    void exit(model::statement::dml::UpdateStatement* node) override {
        auto parent = relation_of(node->source());
        auto relation = relation_of(node);
        relation->process() = parent->output();
    }

    void exit(model::statement::dml::DeleteStatement* node) override {
        auto parent = relation_of(node->source());
        auto relation = relation_of(node);
        relation->process() = parent->output();
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
