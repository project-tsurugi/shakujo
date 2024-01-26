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
#include "shakujo/model/expression/VectorElementLoadExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class VectorElementLoadExpression::Impl {
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

VectorElementLoadExpression::VectorElementLoadExpression(std::unique_ptr<VectorElementLoadExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

VectorElementLoadExpression::VectorElementLoadExpression() : VectorElementLoadExpression(std::make_unique<VectorElementLoadExpression::Impl>()) {}

VectorElementLoadExpression::~VectorElementLoadExpression() noexcept = default;

VectorElementLoadExpression::VectorElementLoadExpression(VectorElementLoadExpression&&) noexcept = default;

VectorElementLoadExpression& VectorElementLoadExpression::operator=(VectorElementLoadExpression&&) noexcept = default;

Expression* VectorElementLoadExpression::data() {
    return impl_->data_.get();
}

VectorElementLoadExpression& VectorElementLoadExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> VectorElementLoadExpression::release_data() {
    return impl_->data_.release();
}

Expression* VectorElementLoadExpression::index() {
    return impl_->index_.get();
}

VectorElementLoadExpression& VectorElementLoadExpression::index(std::unique_ptr<Expression> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<Expression> VectorElementLoadExpression::release_index() {
    return impl_->index_.release();
}

key::ExpressionKey* VectorElementLoadExpression::expression_key() {
    return impl_->expression_key_.get();
}

VectorElementLoadExpression& VectorElementLoadExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

VectorElementLoadExpression* VectorElementLoadExpression::clone() const & {
    return new VectorElementLoadExpression(impl_->clone());  // NOLINT
}

VectorElementLoadExpression* VectorElementLoadExpression::clone() && {
    return new VectorElementLoadExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

