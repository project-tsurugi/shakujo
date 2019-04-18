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
#include "shakujo/model/expression/relation/AggregationExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/FunctionCall.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/util/FragmentList.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression::relation {

class AggregationExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
    util::ManagedNodeList<Expression> keys_;
    util::FragmentList<AggregationExpression::Column> columns_;
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
        if (!keys_.empty()) {
            other->keys_.reserve(keys_.size());
            for (auto e : keys_) {
                other->keys_.push_back(common::util::make_clone(e));
            }
        }
        if (!columns_.empty()) {
            other->columns_.reserve(columns_.size());
            for (auto e : columns_) {
                other->columns_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

class AggregationExpression::Column::Impl {
public:
    std::unique_ptr<name::Name> function_;
    FunctionCall::Quantifier quantifier_ { FunctionCall::Quantifier::ABSENT };
    common::util::ManagedPtr<Expression> operand_;
    std::unique_ptr<name::SimpleName> alias_;
    std::unique_ptr<key::FunctionKey> function_key_;
    std::unique_ptr<key::VariableKey> variable_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->function_ = common::util::make_clone(function_);
        other->quantifier_ = quantifier_;
        other->operand_ = common::util::make_clone(operand_);
        other->alias_ = common::util::make_clone(alias_);
        return other;
    }
};

AggregationExpression::AggregationExpression(std::unique_ptr<AggregationExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

AggregationExpression::AggregationExpression() : AggregationExpression(std::make_unique<AggregationExpression::Impl>()) {}

AggregationExpression::~AggregationExpression() noexcept = default;

AggregationExpression::AggregationExpression(AggregationExpression&&) noexcept = default;

AggregationExpression& AggregationExpression::operator=(AggregationExpression&&) noexcept = default;

Expression* AggregationExpression::operand() {
    return impl_->operand_.get();
}

AggregationExpression& AggregationExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> AggregationExpression::release_operand() {
    return impl_->operand_.release();
}

util::ManagedNodeList<Expression>& AggregationExpression::keys() {
    return impl_->keys_;
}

util::FragmentList<AggregationExpression::Column>& AggregationExpression::columns() {
    return impl_->columns_;
}

key::ExpressionKey* AggregationExpression::expression_key() {
    return impl_->expression_key_.get();
}

AggregationExpression& AggregationExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* AggregationExpression::relation_key() {
    return impl_->relation_key_.get();
}

AggregationExpression& AggregationExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

AggregationExpression* AggregationExpression::clone() const & {
    return new AggregationExpression(impl_->clone());  // NOLINT
}

AggregationExpression* AggregationExpression::clone() && {
    return new AggregationExpression(std::move(impl_));  // NOLINT
}

AggregationExpression::Column::Column(std::unique_ptr<AggregationExpression::Column::Impl> impl) noexcept : impl_(std::move(impl)) {}

AggregationExpression::Column::Column() : AggregationExpression::Column(std::make_unique<AggregationExpression::Column::Impl>()) {}

AggregationExpression::Column::~Column() noexcept = default;

AggregationExpression::Column::Column(AggregationExpression::Column&&) noexcept = default;

AggregationExpression::Column& AggregationExpression::Column::operator=(AggregationExpression::Column&&) noexcept = default;

name::Name* AggregationExpression::Column::function() {
    return impl_->function_.get();
}

AggregationExpression::Column& AggregationExpression::Column::function(std::unique_ptr<name::Name> function) {
    impl_->function_ = std::move(function);
    return *this;
}

std::unique_ptr<name::Name> AggregationExpression::Column::release_function() {
    std::unique_ptr<name::Name> ret { std::move(impl_->function_) };
    impl_->function_ = {};
    return ret;
}

FunctionCall::Quantifier AggregationExpression::Column::quantifier() const {
    return impl_->quantifier_;
}

AggregationExpression::Column& AggregationExpression::Column::quantifier(FunctionCall::Quantifier quantifier) {
    impl_->quantifier_ = quantifier;
    return *this;
}

Expression* AggregationExpression::Column::operand() {
    return impl_->operand_.get();
}

AggregationExpression::Column& AggregationExpression::Column::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> AggregationExpression::Column::release_operand() {
    return impl_->operand_.release();
}

name::SimpleName* AggregationExpression::Column::alias() {
    return impl_->alias_.get();
}

AggregationExpression::Column& AggregationExpression::Column::alias(std::unique_ptr<name::SimpleName> alias) {
    impl_->alias_ = std::move(alias);
    return *this;
}

std::unique_ptr<name::SimpleName> AggregationExpression::Column::release_alias() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->alias_) };
    impl_->alias_ = {};
    return ret;
}

key::FunctionKey* AggregationExpression::Column::function_key() {
    return impl_->function_key_.get();
}

AggregationExpression::Column& AggregationExpression::Column::function_key(std::unique_ptr<key::FunctionKey> function_key) {
    impl_->function_key_ = std::move(function_key);
    return *this;
}

key::VariableKey* AggregationExpression::Column::variable_key() {
    return impl_->variable_key_.get();
}

AggregationExpression::Column& AggregationExpression::Column::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

AggregationExpression::Column* AggregationExpression::Column::clone() const & {
    return new AggregationExpression::Column(impl_->clone());  // NOLINT
}

AggregationExpression::Column* AggregationExpression::Column::clone() && {
    return new AggregationExpression::Column(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

