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
#include "shakujo/model/expression/VectorCreationExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression {

class VectorCreationExpression::Impl {
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

VectorCreationExpression::VectorCreationExpression(std::unique_ptr<VectorCreationExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

VectorCreationExpression::VectorCreationExpression() : VectorCreationExpression(std::make_unique<VectorCreationExpression::Impl>()) {}

VectorCreationExpression::~VectorCreationExpression() noexcept = default;

VectorCreationExpression::VectorCreationExpression(VectorCreationExpression&&) noexcept = default;

VectorCreationExpression& VectorCreationExpression::operator=(VectorCreationExpression&&) noexcept = default;

type::Type* VectorCreationExpression::type() {
    return impl_->type_.get();
}

VectorCreationExpression& VectorCreationExpression::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> VectorCreationExpression::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

util::ManagedNodeList<Expression>& VectorCreationExpression::values() {
    return impl_->values_;
}

key::ExpressionKey* VectorCreationExpression::expression_key() {
    return impl_->expression_key_.get();
}

VectorCreationExpression& VectorCreationExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

VectorCreationExpression* VectorCreationExpression::clone() const & {
    return new VectorCreationExpression(impl_->clone());  // NOLINT
}

VectorCreationExpression* VectorCreationExpression::clone() && {
    return new VectorCreationExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

