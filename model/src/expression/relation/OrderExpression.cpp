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
#include "shakujo/model/expression/relation/OrderExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::expression::relation {

class OrderExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
    util::FragmentList<OrderExpression::Element> elements_;
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
        other->operand_ = common::util::make_clone(operand_);
        if (!elements_.empty()) {
            other->elements_.reserve(elements_.size());
            for (auto e : elements_) {
                other->elements_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

class OrderExpression::Element::Impl {
public:
    common::util::ManagedPtr<Expression> key_;
    OrderExpression::Direction direction_ { OrderExpression::Direction::ASCENDANT };
    std::unique_ptr<key::VariableKey> variable_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->key_ = common::util::make_clone(key_);
        other->direction_ = direction_;
        return other;
    }
};

OrderExpression::OrderExpression(std::unique_ptr<OrderExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

OrderExpression::OrderExpression() : OrderExpression(std::make_unique<OrderExpression::Impl>()) {}

OrderExpression::~OrderExpression() noexcept = default;

OrderExpression::OrderExpression(OrderExpression&&) noexcept = default;

OrderExpression& OrderExpression::operator=(OrderExpression&&) noexcept = default;

Expression* OrderExpression::operand() {
    return impl_->operand_.get();
}

OrderExpression& OrderExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> OrderExpression::release_operand() {
    return impl_->operand_.release();
}

util::FragmentList<OrderExpression::Element>& OrderExpression::elements() {
    return impl_->elements_;
}

key::ExpressionKey* OrderExpression::expression_key() {
    return impl_->expression_key_.get();
}

OrderExpression& OrderExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* OrderExpression::relation_key() {
    return impl_->relation_key_.get();
}

OrderExpression& OrderExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

OrderExpression* OrderExpression::clone() const & {
    return new OrderExpression(impl_->clone());  // NOLINT
}

OrderExpression* OrderExpression::clone() && {
    return new OrderExpression(std::move(impl_));  // NOLINT
}

OrderExpression::Element::Element(std::unique_ptr<OrderExpression::Element::Impl> impl) noexcept : impl_(std::move(impl)) {}

OrderExpression::Element::Element() : OrderExpression::Element(std::make_unique<OrderExpression::Element::Impl>()) {}

OrderExpression::Element::~Element() noexcept = default;

OrderExpression::Element::Element(OrderExpression::Element&&) noexcept = default;

OrderExpression::Element& OrderExpression::Element::operator=(OrderExpression::Element&&) noexcept = default;

Expression* OrderExpression::Element::key() {
    return impl_->key_.get();
}

OrderExpression::Element& OrderExpression::Element::key(std::unique_ptr<Expression> key) {
    impl_->key_ = std::move(key);
    return *this;
}

std::unique_ptr<Expression> OrderExpression::Element::release_key() {
    return impl_->key_.release();
}

OrderExpression::Direction OrderExpression::Element::direction() const {
    return impl_->direction_;
}

OrderExpression::Element& OrderExpression::Element::direction(OrderExpression::Direction direction) {
    impl_->direction_ = direction;
    return *this;
}

key::VariableKey* OrderExpression::Element::variable_key() {
    return impl_->variable_key_.get();
}

OrderExpression::Element& OrderExpression::Element::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

OrderExpression::Element* OrderExpression::Element::clone() const & {
    return new OrderExpression::Element(impl_->clone());  // NOLINT
}

OrderExpression::Element* OrderExpression::Element::clone() && {
    return new OrderExpression::Element(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

