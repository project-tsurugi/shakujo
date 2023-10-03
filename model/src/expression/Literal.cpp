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
#include "shakujo/model/expression/Literal.h"

#include <utility>
#include <memory>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/util/utility.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class Literal::Impl {
public:
    std::unique_ptr<common::core::Type> type_;
    std::unique_ptr<common::core::Value> value_;
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
        other->value_ = common::util::make_clone(value_);
        return other;
    }
};

Literal::Literal(std::unique_ptr<Literal::Impl> impl) noexcept : impl_(std::move(impl)) {}

Literal::Literal() : Literal(std::make_unique<Literal::Impl>()) {}

Literal::~Literal() noexcept = default;

Literal::Literal(Literal&&) noexcept = default;

Literal& Literal::operator=(Literal&&) noexcept = default;

common::core::Type* Literal::type() {
    return impl_->type_.get();
}

Literal& Literal::type(std::unique_ptr<common::core::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

common::core::Value* Literal::value() {
    return impl_->value_.get();
}

Literal& Literal::value(std::unique_ptr<common::core::Value> value) {
    impl_->value_ = std::move(value);
    return *this;
}

key::ExpressionKey* Literal::expression_key() {
    return impl_->expression_key_.get();
}

Literal& Literal::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

Literal* Literal::clone() const & {
    return new Literal(impl_->clone());  // NOLINT
}

Literal* Literal::clone() && {
    return new Literal(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

