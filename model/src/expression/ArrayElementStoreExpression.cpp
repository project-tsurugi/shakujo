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
#include "shakujo/model/expression/ArrayElementStoreExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class ArrayElementStoreExpression::Impl {
public:
    common::util::ManagedPtr<Expression> data_;
    common::util::ManagedPtr<Expression> index_;
    common::util::ManagedPtr<Expression> value_;
    std::unique_ptr<key::ExpressionKey> expression_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->data_ = common::util::make_clone(data_);
        other->index_ = common::util::make_clone(index_);
        other->value_ = common::util::make_clone(value_);
        return other;
    }
};

ArrayElementStoreExpression::ArrayElementStoreExpression(std::unique_ptr<ArrayElementStoreExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

ArrayElementStoreExpression::ArrayElementStoreExpression() : ArrayElementStoreExpression(std::make_unique<ArrayElementStoreExpression::Impl>()) {}

ArrayElementStoreExpression::~ArrayElementStoreExpression() noexcept = default;

ArrayElementStoreExpression::ArrayElementStoreExpression(ArrayElementStoreExpression&&) noexcept = default;

ArrayElementStoreExpression& ArrayElementStoreExpression::operator=(ArrayElementStoreExpression&&) noexcept = default;

Expression* ArrayElementStoreExpression::data() {
    return impl_->data_.get();
}

ArrayElementStoreExpression& ArrayElementStoreExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> ArrayElementStoreExpression::release_data() {
    return impl_->data_.release();
}

Expression* ArrayElementStoreExpression::index() {
    return impl_->index_.get();
}

ArrayElementStoreExpression& ArrayElementStoreExpression::index(std::unique_ptr<Expression> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<Expression> ArrayElementStoreExpression::release_index() {
    return impl_->index_.release();
}

Expression* ArrayElementStoreExpression::value() {
    return impl_->value_.get();
}

ArrayElementStoreExpression& ArrayElementStoreExpression::value(std::unique_ptr<Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<Expression> ArrayElementStoreExpression::release_value() {
    return impl_->value_.release();
}

key::ExpressionKey* ArrayElementStoreExpression::expression_key() {
    return impl_->expression_key_.get();
}

ArrayElementStoreExpression& ArrayElementStoreExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

ArrayElementStoreExpression* ArrayElementStoreExpression::clone() const & {
    return new ArrayElementStoreExpression(impl_->clone());  // NOLINT
}

ArrayElementStoreExpression* ArrayElementStoreExpression::clone() && {
    return new ArrayElementStoreExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

