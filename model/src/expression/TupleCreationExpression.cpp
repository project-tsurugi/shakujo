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
#include "shakujo/model/expression/TupleCreationExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression {

class TupleCreationExpression::Impl {
public:
    std::unique_ptr<type::Type> type_;
    util::ManagedNodeList<Expression> values_;
    std::unique_ptr<key::ExpressionKey> expression_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->type_ = common::util::make_clone(type_);
        if (!values_.empty()) {
            other->values_.reserve(values_.size());
            for (auto e : values_) {
                other->values_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

TupleCreationExpression::TupleCreationExpression(std::unique_ptr<TupleCreationExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

TupleCreationExpression::TupleCreationExpression() : TupleCreationExpression(std::make_unique<TupleCreationExpression::Impl>()) {}

TupleCreationExpression::~TupleCreationExpression() noexcept = default;

TupleCreationExpression::TupleCreationExpression(TupleCreationExpression&&) noexcept = default;

TupleCreationExpression& TupleCreationExpression::operator=(TupleCreationExpression&&) noexcept = default;

type::Type* TupleCreationExpression::type() {
    return impl_->type_.get();
}

TupleCreationExpression& TupleCreationExpression::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> TupleCreationExpression::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

util::ManagedNodeList<Expression>& TupleCreationExpression::values() {
    return impl_->values_;
}

key::ExpressionKey* TupleCreationExpression::expression_key() {
    return impl_->expression_key_.get();
}

TupleCreationExpression& TupleCreationExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

TupleCreationExpression* TupleCreationExpression::clone() const & {
    return new TupleCreationExpression(impl_->clone());  // NOLINT
}

TupleCreationExpression* TupleCreationExpression::clone() && {
    return new TupleCreationExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

