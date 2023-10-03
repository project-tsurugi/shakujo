/*
 * Copyright 2018-2023 Project Tsurugi.
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
#include "shakujo/model/expression/VectorElementStoreExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class VectorElementStoreExpression::Impl {
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

VectorElementStoreExpression::VectorElementStoreExpression(std::unique_ptr<VectorElementStoreExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

VectorElementStoreExpression::VectorElementStoreExpression() : VectorElementStoreExpression(std::make_unique<VectorElementStoreExpression::Impl>()) {}

VectorElementStoreExpression::~VectorElementStoreExpression() noexcept = default;

VectorElementStoreExpression::VectorElementStoreExpression(VectorElementStoreExpression&&) noexcept = default;

VectorElementStoreExpression& VectorElementStoreExpression::operator=(VectorElementStoreExpression&&) noexcept = default;

Expression* VectorElementStoreExpression::data() {
    return impl_->data_.get();
}

VectorElementStoreExpression& VectorElementStoreExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> VectorElementStoreExpression::release_data() {
    return impl_->data_.release();
}

Expression* VectorElementStoreExpression::index() {
    return impl_->index_.get();
}

VectorElementStoreExpression& VectorElementStoreExpression::index(std::unique_ptr<Expression> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<Expression> VectorElementStoreExpression::release_index() {
    return impl_->index_.release();
}

Expression* VectorElementStoreExpression::value() {
    return impl_->value_.get();
}

VectorElementStoreExpression& VectorElementStoreExpression::value(std::unique_ptr<Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<Expression> VectorElementStoreExpression::release_value() {
    return impl_->value_.release();
}

key::ExpressionKey* VectorElementStoreExpression::expression_key() {
    return impl_->expression_key_.get();
}

VectorElementStoreExpression& VectorElementStoreExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

VectorElementStoreExpression* VectorElementStoreExpression::clone() const & {
    return new VectorElementStoreExpression(impl_->clone());  // NOLINT
}

VectorElementStoreExpression* VectorElementStoreExpression::clone() && {
    return new VectorElementStoreExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

