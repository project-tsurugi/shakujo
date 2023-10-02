/*
 * Copyright 2018-2023 tsurugi project..
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
#include "shakujo/model/expression/relation/GroupExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression::relation {

class GroupExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
    util::ManagedNodeList<Expression> keys_;
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
        if (!keys_.empty()) {
            other->keys_.reserve(keys_.size());
            for (auto e : keys_) {
                other->keys_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

GroupExpression::GroupExpression(std::unique_ptr<GroupExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

GroupExpression::GroupExpression() : GroupExpression(std::make_unique<GroupExpression::Impl>()) {}

GroupExpression::~GroupExpression() noexcept = default;

GroupExpression::GroupExpression(GroupExpression&&) noexcept = default;

GroupExpression& GroupExpression::operator=(GroupExpression&&) noexcept = default;

Expression* GroupExpression::operand() {
    return impl_->operand_.get();
}

GroupExpression& GroupExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> GroupExpression::release_operand() {
    return impl_->operand_.release();
}

util::ManagedNodeList<Expression>& GroupExpression::keys() {
    return impl_->keys_;
}

key::ExpressionKey* GroupExpression::expression_key() {
    return impl_->expression_key_.get();
}

GroupExpression& GroupExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* GroupExpression::relation_key() {
    return impl_->relation_key_.get();
}

GroupExpression& GroupExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

GroupExpression* GroupExpression::clone() const & {
    return new GroupExpression(impl_->clone());  // NOLINT
}

GroupExpression* GroupExpression::clone() && {
    return new GroupExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

