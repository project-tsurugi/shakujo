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
#include "shakujo/analyzer/binding/FunctionBinding.h"

#include "shakujo/analyzer/binding/Id.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/PtrList.h"
#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"

namespace shakujo::analyzer::binding {

class FunctionBinding::Impl {
public:
    Id<FunctionBinding> id_;
    common::core::Name name_;
    common::util::ClonablePtr<common::core::Type> type_;
    std::vector<std::shared_ptr<VariableBinding>> parameters_;

    Impl(Id<FunctionBinding> &&id,
             common::core::Name&& name,
             std::unique_ptr<common::core::Type>&& type,
             std::vector<std::shared_ptr<VariableBinding>>&& parameters)
        : id_(std::move(id)), name_(std::move(name)), type_(std::move(type)), parameters_(std::move(parameters))
    {}
};

FunctionBinding::~FunctionBinding() noexcept = default;

FunctionBinding::FunctionBinding(
        Id<FunctionBinding> &&id,
        common::core::Name name,
        std::unique_ptr<common::core::Type> type,
        std::vector<std::shared_ptr<VariableBinding>> parameters)
    : impl_(new Impl { std::move(id), std::move(name), std::move(type), std::move(parameters) })
{}

Id<FunctionBinding> const& FunctionBinding::id() const {
    return impl_->id_;
}

common::core::Name const& FunctionBinding::name() const {
    return impl_->name_;
}

common::core::Type const* FunctionBinding::type() const {
    return impl_->type_.get();
}

FunctionBinding &FunctionBinding::type(std::unique_ptr<common::core::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::vector<std::shared_ptr<VariableBinding>>& FunctionBinding::parameters() {
    return impl_->parameters_;
}
}  // namespace shakujo::analyzer::binding
