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
        binding::BindingContext &context,
        scope::Scope<binding::VariableBinding> const* parent,
        common::core::type::Relation const* relation)
    : parent_(parent)
    , binding_(new binding::VariableBinding {
            context.next_variable_id(),
            common::core::Name(), // empty name
            relation->row_type(),
    })
{
    std::size_t position = 0;
    for (auto& column : relation->columns()) {
        column_set_.emplace(column.name());
        for (auto& qualifier : column.qualifiers()) {
            auto ptr = qualifier_map_.get(qualifier.segments());
            if (!ptr) {
                ptr = std::make_shared<std::map<std::string, std::vector<std::size_t>>>();
                qualifier_map_.put(qualifier.segments(), ptr);
            }
            auto it = ptr->find(column.name());
            if (it != ptr->end()) {
                it->second.push_back(position);
            } else {
                ptr->emplace(column.name(), std::vector<std::size_t> { position });
            }
        }
        ++position;
    }
}

static model::name::SimpleName const* name_at(model::name::Name const* name, std::size_t position) {
    std::size_t size = name->size();
    if (position >= size) {
        throw std::out_of_range(common::util::to_string("invalid position ", position, ": ", *name));
    }
    model::name::Name const* current = name;
    std::size_t rest = size - position - 1;
    while (current) {
        switch (current->kind()) {
            case model::name::NameKind::QUALIFIED_NAME:
                if (rest == 0) {
                    return dynamic_cast<model::name::QualifiedName const*>(current)->name();
                } else {
                    current = dynamic_cast<model::name::QualifiedName const*>(current)->qualifier();
                    break;
                }
            case model::name::NameKind::SIMPLE_NAME:
                if (rest == 0) {
                    return dynamic_cast<model::name::SimpleName const*>(current);
                } else {
                    throw std::domain_error("out of range");
                }
        }
        --rest;
    }
    throw std::domain_error("out of range");
}

std::shared_ptr<binding::VariableBinding> RelationScope::binding() {
    return binding_;
}

scope::Result<binding::VariableBinding> RelationScope::find(model::name::Name const* name) const {
    if (auto r = find_relation(name)) {
        return r;
    }
    if (parent_) {
        return parent_->find(name);
    }
    return {};
}

scope::Result<binding::VariableBinding> RelationScope::find_relation(model::name::Name const* name) const {
    if (!name) {
        return {};
    }
    {
        // search column qualifier
        auto r = qualifier_map_.find(name);
        if (r) {
            std::size_t matched = r.name()->size();
            // if perfect match, we don't have any column information
            if (matched == name->size()) {
                return scope::Result<binding::VariableBinding>(r.name(), binding_);
            }
            auto column_name = name_at(name, matched);
            auto& available_columns = *r.element();
            auto it = available_columns.find(column_name->token());
            if (it != available_columns.end()) {
                assert(it->second.size() >= 1);  // NOLINT
                if (it->second.size() == 1) {
                    // column is unambiguous for the leading qualifier
                    return { r.name(), binding_, it->second };
                }
                // we don't determine the succeeding column name
                return {
                    name,  // FIXME matched + 1
                    std::make_shared<binding::VariableBinding>(),  // invalid binding for unambiguous variables
                    {},
                };
            }
            // found valid qualifier, but the following column name is not found
            return {};
        }
    }
    {
        // search bare column
        auto column_name = name_at(name, 0U);
        if (column_set_.find(column_name->token()) != column_set_.end()) {
            return scope::Result<binding::VariableBinding>(binding_);
        };
    }
    return {};
}
}  // namespace shakujo::analyzer::impl
