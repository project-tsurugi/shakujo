/*
 * Copyright 2018-2018 shakujo project.
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
#include "shakujo/analyzer/binding/VariableBinding.h"

#include "shakujo/analyzer/binding/Id.h"
#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"

namespace shakujo::analyzer::binding {

namespace util = common::util;

class VariableBinding::Impl {
public:
    Id<VariableBinding> id_;
    common::core::Name name_;
    std::unique_ptr<common::core::Type> type_;
    std::unique_ptr<common::core::Value> value_;
    Impl(
            Id<VariableBinding>&& id,
            common::core::Name&& name,
            std::unique_ptr<common::core::Type>&& type,
            std::unique_ptr<common::core::Value>&& value)
        : id_(std::move(id)), name_(std::move(name)), type_(std::move(type)), value_(std::move(value))
    {}
};

VariableBinding::VariableBinding(
        Id<VariableBinding> &&id,
        common::core::Name name,
        std::unique_ptr<common::core::Type> type,
        std::unique_ptr<common::core::Value> value)
    : impl_(new Impl { std::move(id), std::move(name), std::move(type), std::move(value) })
{}

VariableBinding::~VariableBinding() noexcept = default;

const Id<VariableBinding>& VariableBinding::id() const {
    return impl_->id_;
}

const common::core::Name& VariableBinding::name() const {
    return impl_->name_;
}

common::core::Type const* VariableBinding::type() const {
    return impl_->type_.get();
}

VariableBinding &VariableBinding::type(std::unique_ptr<common::core::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

common::core::Value const* VariableBinding::value() const {
    return impl_->value_.get();
}

VariableBinding &VariableBinding::value(std::unique_ptr<common::core::Value> value) {
    impl_->value_ = std::move(value);
    return *this;
}

bool VariableBinding::is_valid() const {
    return has_id()
        && util::is_valid(type())
        && (!util::is_defined(value()) || util::is_valid(value()));
}
}  // namespace shakujo::analyzer::binding
