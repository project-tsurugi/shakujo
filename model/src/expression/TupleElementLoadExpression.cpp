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
#include "shakujo/model/expression/TupleElementLoadExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/name/Index.h"

namespace shakujo::model::expression {

class TupleElementLoadExpression::Impl {
public:
    common::util::ManagedPtr<Expression> data_;
    std::unique_ptr<name::Index> index_;
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

TupleElementLoadExpression::TupleElementLoadExpression(std::unique_ptr<TupleElementLoadExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

TupleElementLoadExpression::TupleElementLoadExpression() : TupleElementLoadExpression(std::make_unique<TupleElementLoadExpression::Impl>()) {}

TupleElementLoadExpression::~TupleElementLoadExpression() noexcept = default;

TupleElementLoadExpression::TupleElementLoadExpression(TupleElementLoadExpression&&) noexcept = default;

TupleElementLoadExpression& TupleElementLoadExpression::operator=(TupleElementLoadExpression&&) noexcept = default;

Expression* TupleElementLoadExpression::data() {
    return impl_->data_.get();
}

TupleElementLoadExpression& TupleElementLoadExpression::data(std::unique_ptr<Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<Expression> TupleElementLoadExpression::release_data() {
    return impl_->data_.release();
}

name::Index* TupleElementLoadExpression::index() {
    return impl_->index_.get();
}

TupleElementLoadExpression& TupleElementLoadExpression::index(std::unique_ptr<name::Index> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<name::Index> TupleElementLoadExpression::release_index() {
    std::unique_ptr<name::Index> ret { std::move(impl_->index_) };
    impl_->index_ = {};
    return ret;
}

key::ExpressionKey* TupleElementLoadExpression::expression_key() {
    return impl_->expression_key_.get();
}

TupleElementLoadExpression& TupleElementLoadExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

TupleElementLoadExpression* TupleElementLoadExpression::clone() const & {
    return new TupleElementLoadExpression(impl_->clone());  // NOLINT
}

TupleElementLoadExpression* TupleElementLoadExpression::clone() && {
    return new TupleElementLoadExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

