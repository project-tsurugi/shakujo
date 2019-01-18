/*
 * Copyright 2018-2019 shakujo project.
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
#include <iostream>

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

std::ostream& operator<<(std::ostream& out, BinaryOperator::Kind value) {
    switch (value) {
    case BinaryOperator::Kind::ADD:
        out << "ADD";
        break;
    case BinaryOperator::Kind::SUBTRACT:
        out << "SUBTRACT";
        break;
    case BinaryOperator::Kind::MULTIPLY:
        out << "MULTIPLY";
        break;
    case BinaryOperator::Kind::DIVIDE:
        out << "DIVIDE";
        break;
    case BinaryOperator::Kind::REMAINDER:
        out << "REMAINDER";
        break;
    case BinaryOperator::Kind::BITWISE_AND:
        out << "BITWISE_AND";
        break;
    case BinaryOperator::Kind::BITWISE_OR:
        out << "BITWISE_OR";
        break;
    case BinaryOperator::Kind::BITWISE_XOR:
        out << "BITWISE_XOR";
        break;
    case BinaryOperator::Kind::SHIFT_LEFT:
        out << "SHIFT_LEFT";
        break;
    case BinaryOperator::Kind::SHIFT_RIGHT_ARITHMETIC:
        out << "SHIFT_RIGHT_ARITHMETIC";
        break;
    case BinaryOperator::Kind::SHIFT_RIGHT_LOGICAL:
        out << "SHIFT_RIGHT_LOGICAL";
        break;
    case BinaryOperator::Kind::EQUAL:
        out << "EQUAL";
        break;
    case BinaryOperator::Kind::NOT_EQUAL:
        out << "NOT_EQUAL";
        break;
    case BinaryOperator::Kind::LESS_THAN:
        out << "LESS_THAN";
        break;
    case BinaryOperator::Kind::GREATER_THAN:
        out << "GREATER_THAN";
        break;
    case BinaryOperator::Kind::LESS_THAN_OR_EQUAL:
        out << "LESS_THAN_OR_EQUAL";
        break;
    case BinaryOperator::Kind::GREATER_THAN_OR_EQUAL:
        out << "GREATER_THAN_OR_EQUAL";
        break;
    case BinaryOperator::Kind::IN:
        out << "IN";
        break;
    case BinaryOperator::Kind::LOGICAL_AND:
        out << "LOGICAL_AND";
        break;
    case BinaryOperator::Kind::LOGICAL_OR:
        out << "LOGICAL_OR";
        break;
    case BinaryOperator::Kind::LOGICAL_XOR:
        out << "LOGICAL_XOR";
        break;
    case BinaryOperator::Kind::CONDITIONAL_AND:
        out << "CONDITIONAL_AND";
        break;
    case BinaryOperator::Kind::CONDITIONAL_OR:
        out << "CONDITIONAL_OR";
        break;
    case BinaryOperator::Kind::RELATION_IN:
        out << "RELATION_IN";
        break;
    case BinaryOperator::Kind::RELATION_UNION:
        out << "RELATION_UNION";
        break;
    case BinaryOperator::Kind::RELATION_INTERSECTION:
        out << "RELATION_INTERSECTION";
        break;
    case BinaryOperator::Kind::RELATION_DIFFERENCE:
        out << "RELATION_DIFFERENCE";
        break;
    case BinaryOperator::Kind::RELATION_UNION_ALL:
        out << "RELATION_UNION_ALL";
        break;
    case BinaryOperator::Kind::RELATION_INTERSECTION_ALL:
        out << "RELATION_INTERSECTION_ALL";
        break;
    case BinaryOperator::Kind::RELATION_DIFFERENCE_ALL:
        out << "RELATION_DIFFERENCE_ALL";
        break;
    case BinaryOperator::Kind::INVALID:
        out << "INVALID";
        break;
    }
    return out;
}

}  // namespace shakujo::model::expression

