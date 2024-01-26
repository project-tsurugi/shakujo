/*
 * Copyright 2018-2024 Project Tsurugi.
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
#include "shakujo/model/expression/UnaryOperator.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class UnaryOperator::Impl {
public:
    UnaryOperator::Kind operator_kind_ { UnaryOperator::Kind::INVALID };
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
        other->operand_ = common::util::make_clone(operand_);
        return other;
    }
};

UnaryOperator::UnaryOperator(std::unique_ptr<UnaryOperator::Impl> impl) noexcept : impl_(std::move(impl)) {}

UnaryOperator::UnaryOperator() : UnaryOperator(std::make_unique<UnaryOperator::Impl>()) {}

UnaryOperator::~UnaryOperator() noexcept = default;

UnaryOperator::UnaryOperator(UnaryOperator&&) noexcept = default;

UnaryOperator& UnaryOperator::operator=(UnaryOperator&&) noexcept = default;

UnaryOperator::Kind UnaryOperator::operator_kind() const {
    return impl_->operator_kind_;
}

UnaryOperator& UnaryOperator::operator_kind(UnaryOperator::Kind operator_kind) {
    impl_->operator_kind_ = operator_kind;
    return *this;
}

Expression* UnaryOperator::operand() {
    return impl_->operand_.get();
}

UnaryOperator& UnaryOperator::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> UnaryOperator::release_operand() {
    return impl_->operand_.release();
}

key::ExpressionKey* UnaryOperator::expression_key() {
    return impl_->expression_key_.get();
}

UnaryOperator& UnaryOperator::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

UnaryOperator* UnaryOperator::clone() const & {
    return new UnaryOperator(impl_->clone());  // NOLINT
}

UnaryOperator* UnaryOperator::clone() && {
    return new UnaryOperator(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

