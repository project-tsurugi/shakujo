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
#include "shakujo/model/statement/ddl/CreateTableStatement.h"

#include <utility>
#include <memory>
#include <set>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::statement::ddl {

class CreateTableStatement::Impl {
public:
    std::unique_ptr<name::Name> table_;
    util::FragmentList<CreateTableStatement::Column> columns_;
    std::set<CreateTableStatement::Attribute> attributes_;
    util::FragmentList<CreateTableStatement::PrimaryKey> primary_keys_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->table_ = common::util::make_clone(table_);
        if (!columns_.empty()) {
            other->columns_.reserve(columns_.size());
            for (auto e : columns_) {
                other->columns_.push_back(common::util::make_clone(e));
            }
        }
        other->attributes_ = attributes_;
        if (!primary_keys_.empty()) {
            other->primary_keys_.reserve(primary_keys_.size());
            for (auto e : primary_keys_) {
                other->primary_keys_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

class CreateTableStatement::Column::Impl {
public:
    std::unique_ptr<name::SimpleName> name_;
    std::unique_ptr<type::Type> type_;
    common::util::ManagedPtr<expression::Expression> value_;
    std::set<CreateTableStatement::Column::Attribute> attributes_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = common::util::make_clone(name_);
        other->type_ = common::util::make_clone(type_);
        other->value_ = common::util::make_clone(value_);
        other->attributes_ = attributes_;
        return other;
    }
};

class CreateTableStatement::PrimaryKey::Impl {
public:
    std::unique_ptr<name::SimpleName> name_;
    CreateTableStatement::PrimaryKey::Direction direction_ { CreateTableStatement::PrimaryKey::Direction::DONT_CARE };

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = common::util::make_clone(name_);
        other->direction_ = direction_;
        return other;
    }
};

CreateTableStatement::CreateTableStatement(std::unique_ptr<CreateTableStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

CreateTableStatement::CreateTableStatement() : CreateTableStatement(std::make_unique<CreateTableStatement::Impl>()) {}

CreateTableStatement::~CreateTableStatement() noexcept = default;

CreateTableStatement::CreateTableStatement(CreateTableStatement&&) noexcept = default;

CreateTableStatement& CreateTableStatement::operator=(CreateTableStatement&&) noexcept = default;

name::Name* CreateTableStatement::table() {
    return impl_->table_.get();
}

CreateTableStatement& CreateTableStatement::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> CreateTableStatement::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

util::FragmentList<CreateTableStatement::Column>& CreateTableStatement::columns() {
    return impl_->columns_;
}

std::set<CreateTableStatement::Attribute>& CreateTableStatement::attributes() {
    return impl_->attributes_;
}

util::FragmentList<CreateTableStatement::PrimaryKey>& CreateTableStatement::primary_keys() {
    return impl_->primary_keys_;
}

key::RelationKey* CreateTableStatement::relation_key() {
    return impl_->relation_key_.get();
}

CreateTableStatement& CreateTableStatement::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

CreateTableStatement* CreateTableStatement::clone() const & {
    return new CreateTableStatement(impl_->clone());  // NOLINT
}

CreateTableStatement* CreateTableStatement::clone() && {
    return new CreateTableStatement(std::move(impl_));  // NOLINT
}

CreateTableStatement::Column::Column(std::unique_ptr<CreateTableStatement::Column::Impl> impl) noexcept : impl_(std::move(impl)) {}

CreateTableStatement::Column::Column() : CreateTableStatement::Column(std::make_unique<CreateTableStatement::Column::Impl>()) {}

CreateTableStatement::Column::~Column() noexcept = default;

CreateTableStatement::Column::Column(CreateTableStatement::Column&&) noexcept = default;

CreateTableStatement::Column& CreateTableStatement::Column::operator=(CreateTableStatement::Column&&) noexcept = default;

name::SimpleName* CreateTableStatement::Column::name() {
    return impl_->name_.get();
}

CreateTableStatement::Column& CreateTableStatement::Column::name(std::unique_ptr<name::SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::SimpleName> CreateTableStatement::Column::release_name() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

type::Type* CreateTableStatement::Column::type() {
    return impl_->type_.get();
}

CreateTableStatement::Column& CreateTableStatement::Column::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> CreateTableStatement::Column::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

expression::Expression* CreateTableStatement::Column::value() {
    return impl_->value_.get();
}

CreateTableStatement::Column& CreateTableStatement::Column::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> CreateTableStatement::Column::release_value() {
    return impl_->value_.release();
}

std::set<CreateTableStatement::Column::Attribute>& CreateTableStatement::Column::attributes() {
    return impl_->attributes_;
}

CreateTableStatement::Column* CreateTableStatement::Column::clone() const & {
    return new CreateTableStatement::Column(impl_->clone());  // NOLINT
}

CreateTableStatement::Column* CreateTableStatement::Column::clone() && {
    return new CreateTableStatement::Column(std::move(impl_));  // NOLINT
}

CreateTableStatement::PrimaryKey::PrimaryKey(std::unique_ptr<CreateTableStatement::PrimaryKey::Impl> impl) noexcept : impl_(std::move(impl)) {}

CreateTableStatement::PrimaryKey::PrimaryKey() : CreateTableStatement::PrimaryKey(std::make_unique<CreateTableStatement::PrimaryKey::Impl>()) {}

CreateTableStatement::PrimaryKey::~PrimaryKey() noexcept = default;

CreateTableStatement::PrimaryKey::PrimaryKey(CreateTableStatement::PrimaryKey&&) noexcept = default;

CreateTableStatement::PrimaryKey& CreateTableStatement::PrimaryKey::operator=(CreateTableStatement::PrimaryKey&&) noexcept = default;

name::SimpleName* CreateTableStatement::PrimaryKey::name() {
    return impl_->name_.get();
}

CreateTableStatement::PrimaryKey& CreateTableStatement::PrimaryKey::name(std::unique_ptr<name::SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::SimpleName> CreateTableStatement::PrimaryKey::release_name() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

CreateTableStatement::PrimaryKey::Direction CreateTableStatement::PrimaryKey::direction() const {
    return impl_->direction_;
}

CreateTableStatement::PrimaryKey& CreateTableStatement::PrimaryKey::direction(CreateTableStatement::PrimaryKey::Direction direction) {
    impl_->direction_ = direction;
    return *this;
}

CreateTableStatement::PrimaryKey* CreateTableStatement::PrimaryKey::clone() const & {
    return new CreateTableStatement::PrimaryKey(impl_->clone());  // NOLINT
}

CreateTableStatement::PrimaryKey* CreateTableStatement::PrimaryKey::clone() && {
    return new CreateTableStatement::PrimaryKey(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::ddl

