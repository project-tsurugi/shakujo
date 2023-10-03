/*
 * Copyright 2018-2023 Project Tsurugi.
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
// DON'T EDIT THIS GENERATED FILE //
#include "shakujo/model/expression/AssignExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::expression {

class AssignExpression::Impl {
public:
    std::unique_ptr<name::Name> name_;
    common::util::ManagedPtr<Expression> value_;
    std::unique_ptr<key::ExpressionKey> expression_key_;
    std::unique_ptr<key::VariableKey> variable_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = common::util::make_clone(name_);
        other->value_ = common::util::make_clone(value_);
        return other;
    }
};

AssignExpression::AssignExpression(std::unique_ptr<AssignExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

AssignExpression::AssignExpression() : AssignExpression(std::make_unique<AssignExpression::Impl>()) {}

AssignExpression::~AssignExpression() noexcept = default;

AssignExpression::AssignExpression(AssignExpression&&) noexcept = default;

AssignExpression& AssignExpression::operator=(AssignExpression&&) noexcept = default;

name::Name* AssignExpression::name() {
    return impl_->name_.get();
}

AssignExpression& AssignExpression::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> AssignExpression::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

Expression* AssignExpression::value() {
    return impl_->value_.get();
}

AssignExpression& AssignExpression::value(std::unique_ptr<Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<Expression> AssignExpression::release_value() {
    return impl_->value_.release();
}

key::ExpressionKey* AssignExpression::expression_key() {
    return impl_->expression_key_.get();
}

AssignExpression& AssignExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::VariableKey* AssignExpression::variable_key() {
    return impl_->variable_key_.get();
}

AssignExpression& AssignExpression::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

AssignExpression* AssignExpression::clone() const & {
    return new AssignExpression(impl_->clone());  // NOLINT
}

AssignExpression* AssignExpression::clone() && {
    return new AssignExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

