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
#ifndef SHAKUJO_ANALYZER_IMPL_RELATION_SCOPE_H_
#define SHAKUJO_ANALYZER_IMPL_RELATION_SCOPE_H_

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "shakujo/analyzer/scope/Scope.h"
#include "shakujo/analyzer/scope/Result.h"
#include "shakujo/analyzer/scope/Table.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"
#include "shakujo/analyzer/binding/BindingContext.h"
#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/type/Relation.h"

namespace shakujo::analyzer::impl {

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
            std::vector<std::shared_ptr<binding::VariableBinding>> const& columns);

    ~RelationScope() noexcept override = default;
    RelationScope(const RelationScope& other) = delete;
    RelationScope(RelationScope&& other) noexcept = default;
    RelationScope& operator=(const RelationScope& other) = delete;
    RelationScope& operator=(RelationScope&& other) noexcept = default;

    binding::RelationBinding::Profile profile() {
        return { columns_ };
    }

    scope::Result<binding::VariableBinding> find(model::name::Name const* name) const override;
};
}  // namespace shakujo::analyzer::impl

#endif  //SHAKUJO_ANALYZER_IMPL_RELATION_SCOPE_H_
