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
#include "shakujo/model/expression/relation/DistinctExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::model::expression::relation {

class DistinctExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
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
        return other;
    }
};

DistinctExpression::DistinctExpression(std::unique_ptr<DistinctExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

DistinctExpression::DistinctExpression() : DistinctExpression(std::make_unique<DistinctExpression::Impl>()) {}

DistinctExpression::~DistinctExpression() noexcept = default;

DistinctExpression::DistinctExpression(DistinctExpression&&) noexcept = default;

DistinctExpression& DistinctExpression::operator=(DistinctExpression&&) noexcept = default;

Expression* DistinctExpression::operand() {
    return impl_->operand_.get();
}

DistinctExpression& DistinctExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> DistinctExpression::release_operand() {
    return impl_->operand_.release();
}

key::ExpressionKey* DistinctExpression::expression_key() {
    return impl_->expression_key_.get();
}

DistinctExpression& DistinctExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* DistinctExpression::relation_key() {
    return impl_->relation_key_.get();
}

DistinctExpression& DistinctExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

DistinctExpression* DistinctExpression::clone() const & {
    return new DistinctExpression(impl_->clone());  // NOLINT
}

DistinctExpression* DistinctExpression::clone() && {
    return new DistinctExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

