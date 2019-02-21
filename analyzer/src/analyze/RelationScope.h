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
#ifndef SHAKUJO_ANALYZER_ANALYZE_RELATION_SCOPE_H_
#define SHAKUJO_ANALYZER_ANALYZE_RELATION_SCOPE_H_

#include <algorithm>
#include <map>
#include <memory>
#include <utility>
#include <vector>

#include <cassert>

#include "shakujo/analyzer/scope/Scope.h"
#include "shakujo/analyzer/scope/Result.h"
#include "shakujo/analyzer/scope/Table.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"
#include "shakujo/analyzer/binding/BindingContext.h"
#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/type/Relation.h"

namespace shakujo::analyzer::analyze {

using common::util::make_clone;

class RelationScope final : public scope::Scope<binding::VariableBinding> {
private:
    scope::Scope<binding::VariableBinding> const* parent_;
    std::vector<std::shared_ptr<binding::VariableBinding>> columns_;
    scope::Table<binding::VariableBinding> table_;

public:
    RelationScope(
            binding::BindingContext& context,
            scope::Scope<binding::VariableBinding> const* parent,
            common::core::type::Relation const* relation,
            std::vector<std::shared_ptr<binding::VariableBinding>> const& columns)
        : RelationScope(context, parent, std::vector { relation }, columns)
    {}

    RelationScope(
            binding::BindingContext& context,
            scope::Scope<binding::VariableBinding> const* parent,
            std::vector<common::core::type::Relation const*> relations,
            std::vector<std::shared_ptr<binding::VariableBinding>> const& columns)
        : parent_(parent)
    {
        std::size_t index = 0;
        for (auto* relation : relations) {
            for (auto& column : relation->columns()) {
                std::shared_ptr<binding::VariableBinding> binding;
                if (!columns.empty()) {
                    assert(index < columns.size());  // NOLINT
                    binding = columns[index];
                } else {
                    binding = std::make_shared<binding::VariableBinding>(
                        context.next_variable_id(),
                        common::core::Name(column.name()),
                        make_clone(column.type()));
                }
                columns_.push_back(binding);
                if (!table_.contains(column.name())) {
                    table_.put(column.name(), binding);
                } else {
                    table_.put(
                        column.name(),
                        std::make_shared<binding::VariableBinding>(), // ambiguous
                        true);
                }
                for (auto& qualifier : column.qualifiers()) {
                    auto qname = qualifier.segments(); // take a copy
                    qname.push_back(column.name());
                    if (!table_.contains(qname)) {
                        table_.put(std::move(qname), binding);
                    } else {
                        table_.put(
                            std::move(qname),
                            std::make_shared<binding::VariableBinding>(), // ambiguous
                            true);
                    }
                }
                ++index;
            }
        }
    }

    ~RelationScope() noexcept override = default;
    RelationScope(const RelationScope& other) = delete;
    RelationScope(RelationScope&& other) noexcept = default;
    RelationScope& operator=(const RelationScope& other) = delete;
    RelationScope& operator=(RelationScope&& other) noexcept = default;

    binding::RelationBinding::Profile profile() {
        return { columns_ };
    }

    scope::Result<binding::VariableBinding> find(model::name::Name const* name) const override {
        if (auto result = table_.find(name)) {
            return result;
        }
        if (parent_) {
            return parent_->find(name);
        }
        return {};
    }
};
}  // namespace shakujo::analyzer::analyze

#endif  //SHAKUJO_ANALYZER_ANALYZE_RELATION_SCOPE_H_
