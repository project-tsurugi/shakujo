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
#include "shakujo/model/statement/dml/UpdateStatement.h"

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

class UpdateStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> source_;
    std::unique_ptr<name::Name> table_;
    util::NodeList<Statement> initialize_;
    util::FragmentList<UpdateStatement::Column> columns_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->source_ = common::util::make_clone(source_);
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

class UpdateStatement::Column::Impl {
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

UpdateStatement::UpdateStatement(std::unique_ptr<UpdateStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

UpdateStatement::UpdateStatement() : UpdateStatement(std::make_unique<UpdateStatement::Impl>()) {}

UpdateStatement::~UpdateStatement() noexcept = default;

UpdateStatement::UpdateStatement(UpdateStatement&&) noexcept = default;

UpdateStatement& UpdateStatement::operator=(UpdateStatement&&) noexcept = default;

expression::Expression* UpdateStatement::source() {
    return impl_->source_.get();
}

UpdateStatement& UpdateStatement::source(std::unique_ptr<expression::Expression> source) {
    impl_->source_ = std::move(source);
    return *this;
}

std::unique_ptr<expression::Expression> UpdateStatement::release_source() {
    return impl_->source_.release();
}

name::Name* UpdateStatement::table() {
    return impl_->table_.get();
}

UpdateStatement& UpdateStatement::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> UpdateStatement::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

util::NodeList<Statement>& UpdateStatement::initialize() {
    return impl_->initialize_;
}

util::FragmentList<UpdateStatement::Column>& UpdateStatement::columns() {
    return impl_->columns_;
}

key::RelationKey* UpdateStatement::relation_key() {
    return impl_->relation_key_.get();
}

UpdateStatement& UpdateStatement::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

UpdateStatement* UpdateStatement::clone() const & {
    return new UpdateStatement(impl_->clone());  // NOLINT
}

UpdateStatement* UpdateStatement::clone() && {
    return new UpdateStatement(std::move(impl_));  // NOLINT
}

UpdateStatement::Column::Column(std::unique_ptr<UpdateStatement::Column::Impl> impl) noexcept : impl_(std::move(impl)) {}

UpdateStatement::Column::Column() : UpdateStatement::Column(std::make_unique<UpdateStatement::Column::Impl>()) {}

UpdateStatement::Column::~Column() noexcept = default;

UpdateStatement::Column::Column(UpdateStatement::Column&&) noexcept = default;

UpdateStatement::Column& UpdateStatement::Column::operator=(UpdateStatement::Column&&) noexcept = default;

name::SimpleName* UpdateStatement::Column::name() {
    return impl_->name_.get();
}

UpdateStatement::Column& UpdateStatement::Column::name(std::unique_ptr<name::SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::SimpleName> UpdateStatement::Column::release_name() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

expression::Expression* UpdateStatement::Column::value() {
    return impl_->value_.get();
}

UpdateStatement::Column& UpdateStatement::Column::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> UpdateStatement::Column::release_value() {
    return impl_->value_.release();
}

key::VariableKey* UpdateStatement::Column::variable_key() {
    return impl_->variable_key_.get();
}

UpdateStatement::Column& UpdateStatement::Column::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

UpdateStatement::Column* UpdateStatement::Column::clone() const & {
    return new UpdateStatement::Column(impl_->clone());  // NOLINT
}

UpdateStatement::Column* UpdateStatement::Column::clone() && {
    return new UpdateStatement::Column(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::dml

