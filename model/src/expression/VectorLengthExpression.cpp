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
#include "shakujo/model/expression/VectorLengthExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class VectorLengthExpression::Impl {
public:
    common::util::ManagedPtr<Expression> data_;
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
        return other;
    }
};

VectorLengthExpression::VectorLengthExpression(std::unique_ptr<VectorLengthExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

VectorLengthExpression::VectorLengthExpression() : VectorLengthExpression(std::make_unique<VectorLengthExpression::Impl>()) {}

VectorLengthExpression::~VectorLengthExpression() noexcept = default;

VectorLengthExpression::VectorLengthExpression(VectorLengthExpression&&) noexcept = default;

VectorLengthExpression& VectorLengthExpression::operator=(VectorLengthExpression&&) noexcept = default;

Expression* VectorLengthExpression::data() {
    return impl_->data_.get();
}

VectorLengthExpression& VectorLengthExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> VectorLengthExpression::release_data() {
    return impl_->data_.release();
}

key::ExpressionKey* VectorLengthExpression::expression_key() {
    return impl_->expression_key_.get();
}

VectorLengthExpression& VectorLengthExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

VectorLengthExpression* VectorLengthExpression::clone() const & {
    return new VectorLengthExpression(impl_->clone());  // NOLINT
}

VectorLengthExpression* VectorLengthExpression::clone() && {
    return new VectorLengthExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

