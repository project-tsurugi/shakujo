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
#include "shakujo/analyzer/binding/ExpressionBinding.h"

#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"

namespace shakujo::analyzer::binding {

using common::util::is_defined;

class ExpressionBinding::Impl {
public:
    common::util::ClonablePtr<common::core::Type> type_;
    common::util::ClonablePtr<common::core::Value> value_;
    bool constant_;
    Impl(
            std::unique_ptr<common::core::Type> type,
            std::unique_ptr<common::core::Value> value,
            bool constant)
        : type_(std::move(type)), value_(std::move(value)), constant_(constant)
    {}
};

ExpressionBinding::ExpressionBinding(
        std::unique_ptr<common::core::Type> type,
        std::unique_ptr<common::core::Value> value,
        bool constant)
    : impl_(new Impl(std::move(type), std::move(value), constant))
{}

ExpressionBinding::~ExpressionBinding() noexcept = default;

common::core::Type const* ExpressionBinding::type() const {
    return impl_->type_.get();
}

ExpressionBinding &ExpressionBinding::type(std::unique_ptr<common::core::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

common::core::Value const* ExpressionBinding::value() const {
    return impl_->value_.get();
}

ExpressionBinding &ExpressionBinding::value(std::unique_ptr<common::core::Value> value) {
    impl_->value_ = std::move(value);
    return *this;
}

bool ExpressionBinding::constant() const {
    return impl_->constant_;
}

ExpressionBinding &ExpressionBinding::constant(bool constant) {
    impl_->constant_ = constant;
    return *this;
}

bool ExpressionBinding::is_valid() const {
    return (is_defined(type()) && type()->kind() != common::core::Type::Kind::ERROR)
        && (!is_defined(value()) || value()->kind() != common::core::Value::Kind::ERROR);
}
}  // namespace shakujo::analyzer::binding
