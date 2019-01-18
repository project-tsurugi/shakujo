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
#include "shakujo/model/expression/CursorElementLoadExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class CursorElementLoadExpression::Impl {
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

CursorElementLoadExpression::CursorElementLoadExpression(std::unique_ptr<CursorElementLoadExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

CursorElementLoadExpression::CursorElementLoadExpression() : CursorElementLoadExpression(std::make_unique<CursorElementLoadExpression::Impl>()) {}

CursorElementLoadExpression::~CursorElementLoadExpression() noexcept = default;

CursorElementLoadExpression::CursorElementLoadExpression(CursorElementLoadExpression&&) noexcept = default;

CursorElementLoadExpression& CursorElementLoadExpression::operator=(CursorElementLoadExpression&&) noexcept = default;

Expression* CursorElementLoadExpression::data() {
    return impl_->data_.get();
}

CursorElementLoadExpression& CursorElementLoadExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> CursorElementLoadExpression::release_data() {
    return impl_->data_.release();
}

key::ExpressionKey* CursorElementLoadExpression::expression_key() {
    return impl_->expression_key_.get();
}

CursorElementLoadExpression& CursorElementLoadExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

CursorElementLoadExpression* CursorElementLoadExpression::clone() const & {
    return new CursorElementLoadExpression(impl_->clone());  // NOLINT
}

CursorElementLoadExpression* CursorElementLoadExpression::clone() && {
    return new CursorElementLoadExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

