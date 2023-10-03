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
#include "shakujo/model/statement/ddl/CreateIndexStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::statement::ddl {

class CreateIndexStatement::Impl {
public:
    std::unique_ptr<name::Name> index_;
    std::unique_ptr<name::Name> table_;
    util::FragmentList<CreateIndexStatement::Column> columns_;
    std::set<CreateIndexStatement::Attribute> attributes_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->index_ = common::util::make_clone(index_);
        other->table_ = common::util::make_clone(table_);
        if (!columns_.empty()) {
            other->columns_.reserve(columns_.size());
            for (auto e : columns_) {
                other->columns_.push_back(common::util::make_clone(e));
            }
        }
        other->attributes_ = attributes_;
        return other;
    }
};

class CreateIndexStatement::Column::Impl {
public:
    std::unique_ptr<name::SimpleName> name_;
    CreateIndexStatement::Column::Direction direction_ { CreateIndexStatement::Column::Direction::DONT_CARE };

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

CreateIndexStatement::CreateIndexStatement(std::unique_ptr<CreateIndexStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

CreateIndexStatement::CreateIndexStatement() : CreateIndexStatement(std::make_unique<CreateIndexStatement::Impl>()) {}

CreateIndexStatement::~CreateIndexStatement() noexcept = default;

CreateIndexStatement::CreateIndexStatement(CreateIndexStatement&&) noexcept = default;

CreateIndexStatement& CreateIndexStatement::operator=(CreateIndexStatement&&) noexcept = default;

name::Name* CreateIndexStatement::index() {
    return impl_->index_.get();
}

CreateIndexStatement& CreateIndexStatement::index(std::unique_ptr<name::Name> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<name::Name> CreateIndexStatement::release_index() {
    std::unique_ptr<name::Name> ret { std::move(impl_->index_) };
    impl_->index_ = {};
    return ret;
}

name::Name* CreateIndexStatement::table() {
    return impl_->table_.get();
}

CreateIndexStatement& CreateIndexStatement::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> CreateIndexStatement::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

util::FragmentList<CreateIndexStatement::Column>& CreateIndexStatement::columns() {
    return impl_->columns_;
}

std::set<CreateIndexStatement::Attribute>& CreateIndexStatement::attributes() {
    return impl_->attributes_;
}

key::RelationKey* CreateIndexStatement::relation_key() {
    return impl_->relation_key_.get();
}

CreateIndexStatement& CreateIndexStatement::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

CreateIndexStatement* CreateIndexStatement::clone() const & {
    return new CreateIndexStatement(impl_->clone());  // NOLINT
}

CreateIndexStatement* CreateIndexStatement::clone() && {
    return new CreateIndexStatement(std::move(impl_));  // NOLINT
}

CreateIndexStatement::Column::Column(std::unique_ptr<CreateIndexStatement::Column::Impl> impl) noexcept : impl_(std::move(impl)) {}

CreateIndexStatement::Column::Column() : CreateIndexStatement::Column(std::make_unique<CreateIndexStatement::Column::Impl>()) {}

CreateIndexStatement::Column::~Column() noexcept = default;

CreateIndexStatement::Column::Column(CreateIndexStatement::Column&&) noexcept = default;

CreateIndexStatement::Column& CreateIndexStatement::Column::operator=(CreateIndexStatement::Column&&) noexcept = default;

name::SimpleName* CreateIndexStatement::Column::name() {
    return impl_->name_.get();
}

CreateIndexStatement::Column& CreateIndexStatement::Column::name(std::unique_ptr<name::SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::SimpleName> CreateIndexStatement::Column::release_name() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

CreateIndexStatement::Column::Direction CreateIndexStatement::Column::direction() const {
    return impl_->direction_;
}

CreateIndexStatement::Column& CreateIndexStatement::Column::direction(CreateIndexStatement::Column::Direction direction) {
    impl_->direction_ = direction;
    return *this;
}

CreateIndexStatement::Column* CreateIndexStatement::Column::clone() const & {
    return new CreateIndexStatement::Column(impl_->clone());  // NOLINT
}

CreateIndexStatement::Column* CreateIndexStatement::Column::clone() && {
    return new CreateIndexStatement::Column(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::ddl

