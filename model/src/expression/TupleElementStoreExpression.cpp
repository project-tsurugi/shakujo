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
#include "shakujo/model/expression/TupleElementStoreExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/name/Index.h"

namespace shakujo::model::expression {

class TupleElementStoreExpression::Impl {
public:
    common::util::ManagedPtr<Expression> data_;
    std::unique_ptr<name::Index> index_;
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

TupleElementStoreExpression::TupleElementStoreExpression(std::unique_ptr<TupleElementStoreExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

TupleElementStoreExpression::TupleElementStoreExpression() : TupleElementStoreExpression(std::make_unique<TupleElementStoreExpression::Impl>()) {}

TupleElementStoreExpression::~TupleElementStoreExpression() noexcept = default;

TupleElementStoreExpression::TupleElementStoreExpression(TupleElementStoreExpression&&) noexcept = default;

TupleElementStoreExpression& TupleElementStoreExpression::operator=(TupleElementStoreExpression&&) noexcept = default;

Expression* TupleElementStoreExpression::data() {
    return impl_->data_.get();
}

TupleElementStoreExpression& TupleElementStoreExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> TupleElementStoreExpression::release_data() {
    return impl_->data_.release();
}

name::Index* TupleElementStoreExpression::index() {
    return impl_->index_.get();
}

TupleElementStoreExpression& TupleElementStoreExpression::index(std::unique_ptr<name::Index> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<name::Index> TupleElementStoreExpression::release_index() {
    std::unique_ptr<name::Index> ret { std::move(impl_->index_) };
    impl_->index_ = {};
    return ret;
}

Expression* TupleElementStoreExpression::value() {
    return impl_->value_.get();
}

TupleElementStoreExpression& TupleElementStoreExpression::value(std::unique_ptr<Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<Expression> TupleElementStoreExpression::release_value() {
    return impl_->value_.release();
}

key::ExpressionKey* TupleElementStoreExpression::expression_key() {
    return impl_->expression_key_.get();
}

TupleElementStoreExpression& TupleElementStoreExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

TupleElementStoreExpression* TupleElementStoreExpression::clone() const & {
    return new TupleElementStoreExpression(impl_->clone());  // NOLINT
}

TupleElementStoreExpression* TupleElementStoreExpression::clone() && {
    return new TupleElementStoreExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

