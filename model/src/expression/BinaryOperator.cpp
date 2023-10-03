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
#include "shakujo/model/expression/BinaryOperator.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class BinaryOperator::Impl {
public:
    BinaryOperator::Kind operator_kind_ { BinaryOperator::Kind::INVALID };
    common::util::ManagedPtr<Expression> left_;
    common::util::ManagedPtr<Expression> right_;
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
        other->left_ = common::util::make_clone(left_);
        other->right_ = common::util::make_clone(right_);
        return other;
    }
};

BinaryOperator::BinaryOperator(std::unique_ptr<BinaryOperator::Impl> impl) noexcept : impl_(std::move(impl)) {}

BinaryOperator::BinaryOperator() : BinaryOperator(std::make_unique<BinaryOperator::Impl>()) {}

BinaryOperator::~BinaryOperator() noexcept = default;

BinaryOperator::BinaryOperator(BinaryOperator&&) noexcept = default;

BinaryOperator& BinaryOperator::operator=(BinaryOperator&&) noexcept = default;

BinaryOperator::Kind BinaryOperator::operator_kind() const {
    return impl_->operator_kind_;
}

BinaryOperator& BinaryOperator::operator_kind(BinaryOperator::Kind operator_kind) {
    impl_->operator_kind_ = operator_kind;
    return *this;
}

Expression* BinaryOperator::left() {
    return impl_->left_.get();
}

BinaryOperator& BinaryOperator::left(std::unique_ptr<Expression> left) {
    impl_->left_ = std::move(left);
    return *this;
}

std::unique_ptr<Expression> BinaryOperator::release_left() {
    return impl_->left_.release();
}

Expression* BinaryOperator::right() {
    return impl_->right_.get();
}

BinaryOperator& BinaryOperator::right(std::unique_ptr<Expression> right) {
    impl_->right_ = std::move(right);
    return *this;
}

std::unique_ptr<Expression> BinaryOperator::release_right() {
    return impl_->right_.release();
}

key::ExpressionKey* BinaryOperator::expression_key() {
    return impl_->expression_key_.get();
}

BinaryOperator& BinaryOperator::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

BinaryOperator* BinaryOperator::clone() const & {
    return new BinaryOperator(impl_->clone());  // NOLINT
}

BinaryOperator* BinaryOperator::clone() && {
    return new BinaryOperator(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

