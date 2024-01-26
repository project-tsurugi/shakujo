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
#include "shakujo/model/expression/ImplicitCast.h"

#include <utility>
#include <memory>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class ImplicitCast::Impl {
public:
    std::unique_ptr<common::core::Type> type_;
    common::util::ManagedPtr<Expression> operand_;
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
        other->operand_ = common::util::make_clone(operand_);
        return other;
    }
};

ImplicitCast::ImplicitCast(std::unique_ptr<ImplicitCast::Impl> impl) noexcept : impl_(std::move(impl)) {}

ImplicitCast::ImplicitCast() : ImplicitCast(std::make_unique<ImplicitCast::Impl>()) {}

ImplicitCast::~ImplicitCast() noexcept = default;

ImplicitCast::ImplicitCast(ImplicitCast&&) noexcept = default;

ImplicitCast& ImplicitCast::operator=(ImplicitCast&&) noexcept = default;

common::core::Type* ImplicitCast::type() {
    return impl_->type_.get();
}

ImplicitCast& ImplicitCast::type(std::unique_ptr<common::core::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

Expression* ImplicitCast::operand() {
    return impl_->operand_.get();
}

ImplicitCast& ImplicitCast::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> ImplicitCast::release_operand() {
    return impl_->operand_.release();
}

key::ExpressionKey* ImplicitCast::expression_key() {
    return impl_->expression_key_.get();
}

ImplicitCast& ImplicitCast::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

ImplicitCast* ImplicitCast::clone() const & {
    return new ImplicitCast(impl_->clone());  // NOLINT
}

ImplicitCast* ImplicitCast::clone() && {
    return new ImplicitCast(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

