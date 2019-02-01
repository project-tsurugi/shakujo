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
#include "RelationScope.h"

#include <cassert>

namespace shakujo::analyzer::impl {

RelationScope::RelationScope(
        binding::BindingContext& context,
        scope::Scope<binding::VariableBinding> const* parent,
        common::core::type::Relation const* relation,
        std::vector<std::shared_ptr<binding::VariableBinding>> const& columns)
    : parent_(parent)
{
    assert(columns.empty() || relation->columns().size() == columns.size());  // NOLINT
    std::size_t index = 0;
    for (auto& column : relation->columns()) {
        std::shared_ptr<binding::VariableBinding> binding;
        if (!columns.empty()) {
            binding = columns[index];
        } else {
            binding = std::make_shared<binding::VariableBinding>(
                context.next_variable_id(),
                common::core::Name(column.name()),
                column.type());
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

scope::Result<binding::VariableBinding> RelationScope::find(model::name::Name const* name) const {
    if (auto result = table_.find(name)) {
        return result;
    }
    if (parent_) {
        return parent_->find(name);
    }
    return {};
}
}  // namespace shakujo::analyzer::impl
