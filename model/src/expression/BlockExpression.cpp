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
#include "shakujo/model/expression/BlockExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::expression {

class BlockExpression::Impl {
public:
    util::NodeList<statement::Statement> head_;
    common::util::ManagedPtr<Expression> body_;
    std::unique_ptr<key::ExpressionKey> expression_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        if (!head_.empty()) {
            other->head_.reserve(head_.size());
            for (auto e : head_) {
                other->head_.push_back(common::util::make_clone(e));
            }
        }
        other->body_ = common::util::make_clone(body_);
        return other;
    }
};

BlockExpression::BlockExpression(std::unique_ptr<BlockExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

BlockExpression::BlockExpression() : BlockExpression(std::make_unique<BlockExpression::Impl>()) {}

BlockExpression::~BlockExpression() noexcept = default;

BlockExpression::BlockExpression(BlockExpression&&) noexcept = default;

BlockExpression& BlockExpression::operator=(BlockExpression&&) noexcept = default;

util::NodeList<statement::Statement>& BlockExpression::head() {
    return impl_->head_;
}

Expression* BlockExpression::body() {
    return impl_->body_.get();
}

BlockExpression& BlockExpression::body(std::unique_ptr<Expression> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<Expression> BlockExpression::release_body() {
    return impl_->body_.release();
}

key::ExpressionKey* BlockExpression::expression_key() {
    return impl_->expression_key_.get();
}

BlockExpression& BlockExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

BlockExpression* BlockExpression::clone() const & {
    return new BlockExpression(impl_->clone());  // NOLINT
}

BlockExpression* BlockExpression::clone() && {
    return new BlockExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

