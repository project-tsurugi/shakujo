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
#include "shakujo/model/expression/ArrayElementLoadExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class ArrayElementLoadExpression::Impl {
public:
    common::util::ManagedPtr<Expression> data_;
    common::util::ManagedPtr<Expression> index_;
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
        return other;
    }
};

ArrayElementLoadExpression::ArrayElementLoadExpression(std::unique_ptr<ArrayElementLoadExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

ArrayElementLoadExpression::ArrayElementLoadExpression() : ArrayElementLoadExpression(std::make_unique<ArrayElementLoadExpression::Impl>()) {}

ArrayElementLoadExpression::~ArrayElementLoadExpression() noexcept = default;

ArrayElementLoadExpression::ArrayElementLoadExpression(ArrayElementLoadExpression&&) noexcept = default;

ArrayElementLoadExpression& ArrayElementLoadExpression::operator=(ArrayElementLoadExpression&&) noexcept = default;

Expression* ArrayElementLoadExpression::data() {
    return impl_->data_.get();
}

ArrayElementLoadExpression& ArrayElementLoadExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> ArrayElementLoadExpression::release_data() {
    return impl_->data_.release();
}

Expression* ArrayElementLoadExpression::index() {
    return impl_->index_.get();
}

ArrayElementLoadExpression& ArrayElementLoadExpression::index(std::unique_ptr<Expression> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<Expression> ArrayElementLoadExpression::release_index() {
    return impl_->index_.release();
}

key::ExpressionKey* ArrayElementLoadExpression::expression_key() {
    return impl_->expression_key_.get();
}

ArrayElementLoadExpression& ArrayElementLoadExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

ArrayElementLoadExpression* ArrayElementLoadExpression::clone() const & {
    return new ArrayElementLoadExpression(impl_->clone());  // NOLINT
}

ArrayElementLoadExpression* ArrayElementLoadExpression::clone() && {
    return new ArrayElementLoadExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

