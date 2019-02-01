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
#include "shakujo/model/statement/ddl/DropTableStatement.h"

#include <utility>
#include <memory>
#include <set>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::statement::ddl {

class DropTableStatement::Impl {
public:
    std::unique_ptr<name::Name> table_;
    std::set<DropTableStatement::Attribute> attributes_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->table_ = common::util::make_clone(table_);
        other->attributes_ = attributes_;
        return other;
    }
};

DropTableStatement::DropTableStatement(std::unique_ptr<DropTableStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

DropTableStatement::DropTableStatement() : DropTableStatement(std::make_unique<DropTableStatement::Impl>()) {}

DropTableStatement::~DropTableStatement() noexcept = default;

DropTableStatement::DropTableStatement(DropTableStatement&&) noexcept = default;

DropTableStatement& DropTableStatement::operator=(DropTableStatement&&) noexcept = default;

name::Name* DropTableStatement::table() {
    return impl_->table_.get();
}

DropTableStatement& DropTableStatement::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> DropTableStatement::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

std::set<DropTableStatement::Attribute>& DropTableStatement::attributes() {
    return impl_->attributes_;
}

DropTableStatement* DropTableStatement::clone() const & {
    return new DropTableStatement(impl_->clone());  // NOLINT
}

DropTableStatement* DropTableStatement::clone() && {
    return new DropTableStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::ddl

