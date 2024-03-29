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
#include "shakujo/model/expression/relation/LimitExpression.h"

#include <cstddef>
#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::model::expression::relation {

class LimitExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
    std::size_t count_ { static_cast<std::size_t>(-1) };
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
        other->count_ = count_;
        return other;
    }
};

LimitExpression::LimitExpression(std::unique_ptr<LimitExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

LimitExpression::LimitExpression() : LimitExpression(std::make_unique<LimitExpression::Impl>()) {}

LimitExpression::~LimitExpression() noexcept = default;

LimitExpression::LimitExpression(LimitExpression&&) noexcept = default;

LimitExpression& LimitExpression::operator=(LimitExpression&&) noexcept = default;

Expression* LimitExpression::operand() {
    return impl_->operand_.get();
}

LimitExpression& LimitExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> LimitExpression::release_operand() {
    return impl_->operand_.release();
}

std::size_t LimitExpression::count() const {
    return impl_->count_;
}

LimitExpression& LimitExpression::count(std::size_t count) {
    impl_->count_ = count;
    return *this;
}

key::ExpressionKey* LimitExpression::expression_key() {
    return impl_->expression_key_.get();
}

LimitExpression& LimitExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* LimitExpression::relation_key() {
    return impl_->relation_key_.get();
}

LimitExpression& LimitExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

LimitExpression* LimitExpression::clone() const & {
    return new LimitExpression(impl_->clone());  // NOLINT
}

LimitExpression* LimitExpression::clone() && {
    return new LimitExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

