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
#include "shakujo/model/expression/relation/SelectionExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::model::expression::relation {

class SelectionExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
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
        other->operand_ = common::util::make_clone(operand_);
        other->condition_ = common::util::make_clone(condition_);
        return other;
    }
};

SelectionExpression::SelectionExpression(std::unique_ptr<SelectionExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

SelectionExpression::SelectionExpression() : SelectionExpression(std::make_unique<SelectionExpression::Impl>()) {}

SelectionExpression::~SelectionExpression() noexcept = default;

SelectionExpression::SelectionExpression(SelectionExpression&&) noexcept = default;

SelectionExpression& SelectionExpression::operator=(SelectionExpression&&) noexcept = default;

Expression* SelectionExpression::operand() {
    return impl_->operand_.get();
}

SelectionExpression& SelectionExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> SelectionExpression::release_operand() {
    return impl_->operand_.release();
}

Expression* SelectionExpression::condition() {
    return impl_->condition_.get();
}

SelectionExpression& SelectionExpression::condition(std::unique_ptr<Expression> condition) {
    impl_->condition_ = std::move(condition);
    return *this;
}

std::unique_ptr<Expression> SelectionExpression::release_condition() {
    return impl_->condition_.release();
}

key::ExpressionKey* SelectionExpression::expression_key() {
    return impl_->expression_key_.get();
}

SelectionExpression& SelectionExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* SelectionExpression::relation_key() {
    return impl_->relation_key_.get();
}

SelectionExpression& SelectionExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

SelectionExpression* SelectionExpression::clone() const & {
    return new SelectionExpression(impl_->clone());  // NOLINT
}

SelectionExpression* SelectionExpression::clone() && {
    return new SelectionExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

