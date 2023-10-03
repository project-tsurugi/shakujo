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
#include "shakujo/model/expression/TypeOperator.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::expression {

class TypeOperator::Impl {
public:
    TypeOperator::Kind operator_kind_ { TypeOperator::Kind::INVALID };
    std::unique_ptr<type::Type> type_;
    common::util::ManagedPtr<Expression> operand_;
    std::unique_ptr<key::ExpressionKey> expression_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->operator_kind_ = operator_kind_;
        other->type_ = common::util::make_clone(type_);
        other->operand_ = common::util::make_clone(operand_);
        return other;
    }
};

TypeOperator::TypeOperator(std::unique_ptr<TypeOperator::Impl> impl) noexcept : impl_(std::move(impl)) {}

TypeOperator::TypeOperator() : TypeOperator(std::make_unique<TypeOperator::Impl>()) {}

TypeOperator::~TypeOperator() noexcept = default;

TypeOperator::TypeOperator(TypeOperator&&) noexcept = default;

TypeOperator& TypeOperator::operator=(TypeOperator&&) noexcept = default;

TypeOperator::Kind TypeOperator::operator_kind() const {
    return impl_->operator_kind_;
}

TypeOperator& TypeOperator::operator_kind(TypeOperator::Kind operator_kind) {
    impl_->operator_kind_ = operator_kind;
    return *this;
}

type::Type* TypeOperator::type() {
    return impl_->type_.get();
}

TypeOperator& TypeOperator::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> TypeOperator::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

Expression* TypeOperator::operand() {
    return impl_->operand_.get();
}

TypeOperator& TypeOperator::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> TypeOperator::release_operand() {
    return impl_->operand_.release();
}

key::ExpressionKey* TypeOperator::expression_key() {
    return impl_->expression_key_.get();
}

TypeOperator& TypeOperator::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

TypeOperator* TypeOperator::clone() const & {
    return new TypeOperator(impl_->clone());  // NOLINT
}

TypeOperator* TypeOperator::clone() && {
    return new TypeOperator(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

