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
#include "AggregationAnalyzer.h"

#include <algorithm>
#include <set>

#include <cassert>

#include "shakujo/common/core/type/Relation.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::analyze {

using common::util::is_defined;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;
using common::util::make_clone;
using common::util::to_string;

class AggregationAnalyzer::Impl : public model::util::NodeWalker {
public:
    bool enter(model::expression::VariableReference* node) override {
        auto variable = bindings().get(node->variable_key());
        if (group_keys_.find(variable) != group_keys_.end()) {
            // bare group key expression

            auto&& f = ir_factory;
            auto column = f.AggregationExpressionColumn(
                {},
                model::expression::FunctionCall::Quantifier::ABSENT,
                {});

            auto variable_id = bindings().next_variable_id();
            auto variable_name = to_string('#', variable_id.get());
            auto column_binding = std::make_shared<binding::VariableBinding>(
                std::move(variable_id),
                common::core::Name { variable_name },
                make_clone(variable->type()));

            column->variable_key(bindings().create_key(column_binding));
            column->function_key(bindings().create_key<binding::FunctionBinding>());  // no aggregation

            auto replacement = f.VariableReference(f.Name(std::move(variable_name)));
            replacement->variable_key(bindings().create_key(std::move(column_binding)));
            replacement->expression_key(bindings().create_key<binding::ExpressionBinding>(make_clone(variable->type())));

            node->replace([&](auto origin) {
                column->operand(std::move(origin));
                return std::move(replacement);
            });

            columns_.emplace_back(std::move(column));
        }
        return false;
    }

    bool enter(model::expression::FunctionCall* node) override {
        auto function = bindings().get(node->function_key());
        if (!function->is_set_function()) {
            return true;
        }

        auto&& f = ir_factory;
        std::unique_ptr<model::expression::Expression> argument {};
        if (!node->arguments().empty()) {
            assert(node->arguments().size() == 1U);  // NOLINT
            argument = node->arguments().release(0U).release();
        }

        auto column = f.AggregationExpressionColumn(node->release_name(), node->quantifier(), std::move(argument));

        auto variable_id = bindings().next_variable_id();
        auto variable_name = to_string('#', variable_id.get());

        auto variable_binding = std::make_shared<binding::VariableBinding>(
            std::move(variable_id),
            common::core::Name { variable_name },
            make_clone(function->type()));
        column->variable_key(bindings().create_key(variable_binding));
        column->function_key(bindings().create_key(function));
        columns_.emplace_back(std::move(column));

        auto replacement = f.VariableReference(f.Name(std::move(variable_name)));
        replacement->variable_key(bindings().create_key(std::move(variable_binding)));
        replacement->expression_key(bindings().create_key<binding::ExpressionBinding>(make_clone(function->type())));
        node->replace_with(std::move(replacement));

        return false;
    }

    Impl(AnalyzerContext& env, model::expression::Expression* source)
        : env_(env)
        , source_(source)
        , source_profile_(env
            .binding_context()
            .get(dynamic_pointer_cast<model::key::RelationKey::Provider>(source)->relation_key())
            ->output())
    {
        if (auto* group = dynamic_pointer_cast_if<model::expression::relation::GroupExpression>(source); is_defined(group)) {
            group_ = group;
            for (auto* key : group->keys()) {
                if (auto* ref = dynamic_pointer_cast_if<model::expression::VariableReference>(key); is_defined(ref)) {
                    auto variable = bindings().get(ref->variable_key());
                    group_keys_.emplace(std::move(variable));
                }
            }
        }
        // FIXME: pick up having
    }

    virtual ~Impl() = default;
    Impl(const Impl&) = delete;
    Impl(Impl&&) noexcept = delete;
    Impl& operator=(const Impl&) = delete;
    Impl& operator=(Impl&& other) noexcept = delete;

    model::expression::relation::AggregationExpression* build() {
        if (columns_.empty()) {
            return {};
        }
        auto aggregation = dynamic_pointer_cast<model::expression::relation::AggregationExpression>(
            source_->replace([&](auto orig) {
                auto&& f = ir_factory;
                return f.AggregationExpression(std::move(orig), {}, std::move(columns_));
            }));
        if (is_defined(group_)) {
            aggregation->keys().reserve(group_->keys().size());
            while (!group_->keys().empty()) {
                auto first = group_->keys().release(0);
                aggregation->keys().push_back(first.release());
            }
            group_->replace_with(group_->release_operand());
            group_ = nullptr;
        }

        std::vector<std::shared_ptr<binding::VariableBinding>> agg_columns {};
        std::vector<common::core::type::Relation::Column> type_columns {};
        agg_columns.reserve(aggregation->columns().size());
        type_columns.reserve(aggregation->columns().size());
        for (auto&& column : aggregation->columns()) {
            auto variable = bindings().get(column->variable_key());
            std::string_view name;
            if (is_defined(column->alias())) {
                name = column->alias()->token();
            }
            type_columns.emplace_back(name, make_clone(variable->type()));
            agg_columns.emplace_back(std::move(variable));
        }
        aggregation->relation_key(bindings().create_key<binding::RelationBinding>(
            source_profile_,
            binding::RelationBinding::Profile { std::move(agg_columns) }));
        aggregation->expression_key(bindings().create_key<binding::ExpressionBinding>(
            std::make_unique<common::core::type::Relation>(std::move(type_columns))));

        // FIXME: push down for group-by operation

        return aggregation;
    }

    inline binding::BindingContext& bindings() {
        return env_.binding_context();
    }

    AnalyzerContext& env_;
    model::expression::Expression* source_;
    binding::RelationBinding::Profile const& source_profile_;
    model::expression::relation::GroupExpression* group_ {};
    std::set<std::shared_ptr<binding::VariableBinding>> group_keys_ {};
    std::vector<std::unique_ptr<model::expression::relation::AggregationExpression::Column>> columns_ {};
    model::IRFactory ir_factory {};
};

AggregationAnalyzer::AggregationAnalyzer(AnalyzerContext& env, model::expression::Expression* source)
    : impl_(std::make_unique<Impl>(env, source))
{}

AggregationAnalyzer::~AggregationAnalyzer() noexcept = default;

void AggregationAnalyzer::process(model::expression::Expression* node) {
    impl_->walk(node);
}

model::expression::relation::AggregationExpression* AggregationAnalyzer::build() {
    return impl_->build();
}
}  // namespace shakujo::analyzer::analyze
