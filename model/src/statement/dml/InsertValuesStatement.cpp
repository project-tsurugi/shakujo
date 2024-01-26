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
#include "shakujo/model/statement/dml/InsertValuesStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/FragmentList.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::statement::dml {

class InsertValuesStatement::Impl {
public:
    ConflictAction conflict_action_;
    std::unique_ptr<name::Name> table_;
    util::NodeList<Statement> initialize_;
    util::FragmentList<InsertValuesStatement::Column> columns_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->conflict_action_ = conflict_action_;
        other->table_ = common::util::make_clone(table_);
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
        return other;
    }
};

class InsertValuesStatement::Column::Impl {
public:
    std::unique_ptr<name::SimpleName> name_;
    common::util::ManagedPtr<expression::Expression> value_;
    std::unique_ptr<key::VariableKey> variable_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = common::util::make_clone(name_);
        other->value_ = common::util::make_clone(value_);
        return other;
    }
};

InsertValuesStatement::InsertValuesStatement(std::unique_ptr<InsertValuesStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

InsertValuesStatement::InsertValuesStatement() : InsertValuesStatement(std::make_unique<InsertValuesStatement::Impl>()) {}

InsertValuesStatement::~InsertValuesStatement() noexcept = default;

InsertValuesStatement::InsertValuesStatement(InsertValuesStatement&&) noexcept = default;

InsertValuesStatement& InsertValuesStatement::operator=(InsertValuesStatement&&) noexcept = default;

InsertValuesStatement::ConflictAction InsertValuesStatement::conflict_action() const {
    return impl_->conflict_action_;
}

InsertValuesStatement& InsertValuesStatement::conflict_action(InsertValuesStatement::ConflictAction action) {
    impl_->conflict_action_ = action;
    return *this;
}

name::Name* InsertValuesStatement::table() {
    return impl_->table_.get();
}

InsertValuesStatement& InsertValuesStatement::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> InsertValuesStatement::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

util::NodeList<Statement>& InsertValuesStatement::initialize() {
    return impl_->initialize_;
}

util::FragmentList<InsertValuesStatement::Column>& InsertValuesStatement::columns() {
    return impl_->columns_;
}

key::RelationKey* InsertValuesStatement::relation_key() {
    return impl_->relation_key_.get();
}

InsertValuesStatement& InsertValuesStatement::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

InsertValuesStatement* InsertValuesStatement::clone() const & {
    return new InsertValuesStatement(impl_->clone());  // NOLINT
}

InsertValuesStatement* InsertValuesStatement::clone() && {
    return new InsertValuesStatement(std::move(impl_));  // NOLINT
}

InsertValuesStatement::Column::Column(std::unique_ptr<InsertValuesStatement::Column::Impl> impl) noexcept : impl_(std::move(impl)) {}

InsertValuesStatement::Column::Column() : InsertValuesStatement::Column(std::make_unique<InsertValuesStatement::Column::Impl>()) {}

InsertValuesStatement::Column::~Column() noexcept = default;

InsertValuesStatement::Column::Column(InsertValuesStatement::Column&&) noexcept = default;

InsertValuesStatement::Column& InsertValuesStatement::Column::operator=(InsertValuesStatement::Column&&) noexcept = default;

name::SimpleName* InsertValuesStatement::Column::name() {
    return impl_->name_.get();
}

InsertValuesStatement::Column& InsertValuesStatement::Column::name(std::unique_ptr<name::SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::SimpleName> InsertValuesStatement::Column::release_name() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

expression::Expression* InsertValuesStatement::Column::value() {
    return impl_->value_.get();
}

InsertValuesStatement::Column& InsertValuesStatement::Column::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> InsertValuesStatement::Column::release_value() {
    return impl_->value_.release();
}

key::VariableKey* InsertValuesStatement::Column::variable_key() {
    return impl_->variable_key_.get();
}

InsertValuesStatement::Column& InsertValuesStatement::Column::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

InsertValuesStatement::Column* InsertValuesStatement::Column::clone() const & {
    return new InsertValuesStatement::Column(impl_->clone());  // NOLINT
}

InsertValuesStatement::Column* InsertValuesStatement::Column::clone() && {
    return new InsertValuesStatement::Column(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::dml

