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
#include "shakujo/model/expression/relation/RenameExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::expression::relation {

class RenameExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
    std::unique_ptr<name::SimpleName> name_;
    util::NodeList<name::SimpleName> columns_;
    std::unique_ptr<key::ExpressionKey> expression_key_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->operand_ = common::util::make_clone(operand_);
        other->name_ = common::util::make_clone(name_);
        if (!columns_.empty()) {
            other->columns_.reserve(columns_.size());
            for (auto e : columns_) {
                other->columns_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

RenameExpression::RenameExpression(std::unique_ptr<RenameExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

RenameExpression::RenameExpression() : RenameExpression(std::make_unique<RenameExpression::Impl>()) {}

RenameExpression::~RenameExpression() noexcept = default;

RenameExpression::RenameExpression(RenameExpression&&) noexcept = default;

RenameExpression& RenameExpression::operator=(RenameExpression&&) noexcept = default;

Expression* RenameExpression::operand() {
    return impl_->operand_.get();
}

RenameExpression& RenameExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> RenameExpression::release_operand() {
    return impl_->operand_.release();
}

name::SimpleName* RenameExpression::name() {
    return impl_->name_.get();
}

RenameExpression& RenameExpression::name(std::unique_ptr<name::SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::SimpleName> RenameExpression::release_name() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

util::NodeList<name::SimpleName>& RenameExpression::columns() {
    return impl_->columns_;
}

key::ExpressionKey* RenameExpression::expression_key() {
    return impl_->expression_key_.get();
}

RenameExpression& RenameExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* RenameExpression::relation_key() {
    return impl_->relation_key_.get();
}

RenameExpression& RenameExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

RenameExpression* RenameExpression::clone() const & {
    return new RenameExpression(impl_->clone());  // NOLINT
}

RenameExpression* RenameExpression::clone() && {
    return new RenameExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

