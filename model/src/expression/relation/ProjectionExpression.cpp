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
#include "shakujo/model/expression/relation/ProjectionExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/FragmentList.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::expression::relation {

class ProjectionExpression::Impl {
public:
    common::util::ManagedPtr<Expression> operand_;
    util::NodeList<statement::Statement> initialize_;
    util::FragmentList<ProjectionExpression::Column> columns_;
    std::unique_ptr<name::SimpleName> alias_;
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
        if (!initialize_.empty()) {
            other->initialize_.reserve(initialize_.size());
            for (auto e : initialize_) {
                other->initialize_.push_back(common::util::make_clone(e));
            }
        }
        if (!columns_.empty()) {
            other->columns_.reserve(columns_.size());
            for (auto e : columns_) {
                other->columns_.push_back(common::util::make_clone(e));
            }
        }
        other->alias_ = common::util::make_clone(alias_);
        return other;
    }
};

class ProjectionExpression::Column::Impl {
public:
    common::util::ManagedPtr<Expression> value_;
    std::unique_ptr<name::SimpleName> alias_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->value_ = common::util::make_clone(value_);
        other->alias_ = common::util::make_clone(alias_);
        return other;
    }
};

ProjectionExpression::ProjectionExpression(std::unique_ptr<ProjectionExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

ProjectionExpression::ProjectionExpression() : ProjectionExpression(std::make_unique<ProjectionExpression::Impl>()) {}

ProjectionExpression::~ProjectionExpression() noexcept = default;

ProjectionExpression::ProjectionExpression(ProjectionExpression&&) noexcept = default;

ProjectionExpression& ProjectionExpression::operator=(ProjectionExpression&&) noexcept = default;

Expression* ProjectionExpression::operand() {
    return impl_->operand_.get();
}

ProjectionExpression& ProjectionExpression::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> ProjectionExpression::release_operand() {
    return impl_->operand_.release();
}

util::NodeList<statement::Statement>& ProjectionExpression::initialize() {
    return impl_->initialize_;
}

util::FragmentList<ProjectionExpression::Column>& ProjectionExpression::columns() {
    return impl_->columns_;
}

name::SimpleName* ProjectionExpression::alias() {
    return impl_->alias_.get();
}

ProjectionExpression& ProjectionExpression::alias(std::unique_ptr<name::SimpleName> alias) {
    impl_->alias_ = std::move(alias);
    return *this;
}

std::unique_ptr<name::SimpleName> ProjectionExpression::release_alias() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->alias_) };
    impl_->alias_ = {};
    return ret;
}

key::ExpressionKey* ProjectionExpression::expression_key() {
    return impl_->expression_key_.get();
}

ProjectionExpression& ProjectionExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* ProjectionExpression::relation_key() {
    return impl_->relation_key_.get();
}

ProjectionExpression& ProjectionExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

ProjectionExpression* ProjectionExpression::clone() const & {
    return new ProjectionExpression(impl_->clone());  // NOLINT
}

ProjectionExpression* ProjectionExpression::clone() && {
    return new ProjectionExpression(std::move(impl_));  // NOLINT
}

ProjectionExpression::Column::Column(std::unique_ptr<ProjectionExpression::Column::Impl> impl) noexcept : impl_(std::move(impl)) {}

ProjectionExpression::Column::Column() : ProjectionExpression::Column(std::make_unique<ProjectionExpression::Column::Impl>()) {}

ProjectionExpression::Column::~Column() noexcept = default;

ProjectionExpression::Column::Column(ProjectionExpression::Column&&) noexcept = default;

ProjectionExpression::Column& ProjectionExpression::Column::operator=(ProjectionExpression::Column&&) noexcept = default;

Expression* ProjectionExpression::Column::value() {
    return impl_->value_.get();
}

ProjectionExpression::Column& ProjectionExpression::Column::value(std::unique_ptr<Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<Expression> ProjectionExpression::Column::release_value() {
    return impl_->value_.release();
}

name::SimpleName* ProjectionExpression::Column::alias() {
    return impl_->alias_.get();
}

ProjectionExpression::Column& ProjectionExpression::Column::alias(std::unique_ptr<name::SimpleName> alias) {
    impl_->alias_ = std::move(alias);
    return *this;
}

std::unique_ptr<name::SimpleName> ProjectionExpression::Column::release_alias() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->alias_) };
    impl_->alias_ = {};
    return ret;
}

ProjectionExpression::Column* ProjectionExpression::Column::clone() const & {
    return new ProjectionExpression::Column(impl_->clone());  // NOLINT
}

ProjectionExpression::Column* ProjectionExpression::Column::clone() && {
    return new ProjectionExpression::Column(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

