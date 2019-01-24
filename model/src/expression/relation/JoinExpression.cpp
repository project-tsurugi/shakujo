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
#include "shakujo/model/expression/relation/JoinExpression.h"

#include <utility>
#include <memory>
#include <iostream>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::model::expression::relation {

class JoinExpression::Impl {
public:
    JoinExpression::Kind operator_kind_ { JoinExpression::Kind::INVALID };
    common::util::ManagedPtr<Expression> left_;
    common::util::ManagedPtr<Expression> right_;
    common::util::ManagedPtr<Expression> condition_;
    std::unique_ptr<key::ExpressionKey> expression_key_;
    std::unique_ptr<key::RelationKey> relation_key_;

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
        other->condition_ = common::util::make_clone(condition_);
        return other;
    }
};

JoinExpression::JoinExpression(std::unique_ptr<JoinExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

JoinExpression::JoinExpression() : JoinExpression(std::make_unique<JoinExpression::Impl>()) {}

JoinExpression::~JoinExpression() noexcept = default;

JoinExpression::JoinExpression(JoinExpression&&) noexcept = default;

JoinExpression& JoinExpression::operator=(JoinExpression&&) noexcept = default;

JoinExpression::Kind JoinExpression::operator_kind() const {
    return impl_->operator_kind_;
}

JoinExpression& JoinExpression::operator_kind(JoinExpression::Kind operator_kind) {
    impl_->operator_kind_ = operator_kind;
    return *this;
}

Expression* JoinExpression::left() {
    return impl_->left_.get();
}

JoinExpression& JoinExpression::left(std::unique_ptr<Expression> left) {
    impl_->left_ = std::move(left);
    return *this;
}

std::unique_ptr<Expression> JoinExpression::release_left() {
    return impl_->left_.release();
}

Expression* JoinExpression::right() {
    return impl_->right_.get();
}

JoinExpression& JoinExpression::right(std::unique_ptr<Expression> right) {
    impl_->right_ = std::move(right);
    return *this;
}

std::unique_ptr<Expression> JoinExpression::release_right() {
    return impl_->right_.release();
}

Expression* JoinExpression::condition() {
    return impl_->condition_.get();
}

JoinExpression& JoinExpression::condition(std::unique_ptr<Expression> condition) {
    impl_->condition_ = std::move(condition);
    return *this;
}

std::unique_ptr<Expression> JoinExpression::release_condition() {
    return impl_->condition_.release();
}

key::ExpressionKey* JoinExpression::expression_key() {
    return impl_->expression_key_.get();
}

JoinExpression& JoinExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* JoinExpression::relation_key() {
    return impl_->relation_key_.get();
}

JoinExpression& JoinExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

JoinExpression* JoinExpression::clone() const & {
    return new JoinExpression(impl_->clone());  // NOLINT
}

JoinExpression* JoinExpression::clone() && {
    return new JoinExpression(std::move(impl_));  // NOLINT
}

std::ostream& operator<<(std::ostream& out, JoinExpression::Kind value) {
    switch (value) {
    case JoinExpression::Kind::CROSS:
        out << "CROSS";
        break;
    case JoinExpression::Kind::INNER:
        out << "INNER";
        break;
    case JoinExpression::Kind::LEFT_OUTER:
        out << "LEFT_OUTER";
        break;
    case JoinExpression::Kind::RIGHT_OUTER:
        out << "RIGHT_OUTER";
        break;
    case JoinExpression::Kind::FULL_OUTER:
        out << "FULL_OUTER";
        break;
    case JoinExpression::Kind::NATURAL_INNER:
        out << "NATURAL_INNER";
        break;
    case JoinExpression::Kind::NATURAL_LEFT_OUTER:
        out << "NATURAL_LEFT_OUTER";
        break;
    case JoinExpression::Kind::NATURAL_RIGHT_OUTER:
        out << "NATURAL_RIGHT_OUTER";
        break;
    case JoinExpression::Kind::NATURAL_FULL_OUTER:
        out << "NATURAL_FULL_OUTER";
        break;
    case JoinExpression::Kind::UNION_OUTER:
        out << "UNION_OUTER";
        break;
    case JoinExpression::Kind::LEFT_SEMI:
        out << "LEFT_SEMI";
        break;
    case JoinExpression::Kind::RIGHT_SEMI:
        out << "RIGHT_SEMI";
        break;
    case JoinExpression::Kind::INVALID:
        out << "INVALID";
        break;
    }
    return out;
}

}  // namespace shakujo::model::expression::relation

