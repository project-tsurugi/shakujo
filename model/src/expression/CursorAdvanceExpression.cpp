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
#include "shakujo/model/expression/CursorAdvanceExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class CursorAdvanceExpression::Impl {
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

CursorAdvanceExpression::CursorAdvanceExpression(std::unique_ptr<CursorAdvanceExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

CursorAdvanceExpression::CursorAdvanceExpression() : CursorAdvanceExpression(std::make_unique<CursorAdvanceExpression::Impl>()) {}

CursorAdvanceExpression::~CursorAdvanceExpression() noexcept = default;

CursorAdvanceExpression::CursorAdvanceExpression(CursorAdvanceExpression&&) noexcept = default;

CursorAdvanceExpression& CursorAdvanceExpression::operator=(CursorAdvanceExpression&&) noexcept = default;

Expression* CursorAdvanceExpression::data() {
    return impl_->data_.get();
}

CursorAdvanceExpression& CursorAdvanceExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> CursorAdvanceExpression::release_data() {
    return impl_->data_.release();
}

key::ExpressionKey* CursorAdvanceExpression::expression_key() {
    return impl_->expression_key_.get();
}

CursorAdvanceExpression& CursorAdvanceExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

CursorAdvanceExpression* CursorAdvanceExpression::clone() const & {
    return new CursorAdvanceExpression(impl_->clone());  // NOLINT
}

CursorAdvanceExpression* CursorAdvanceExpression::clone() && {
    return new CursorAdvanceExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

