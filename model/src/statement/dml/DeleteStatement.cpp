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
#include "shakujo/model/statement/dml/DeleteStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::statement::dml {

class DeleteStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> source_;
    std::unique_ptr<name::Name> table_;
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
        return other;
    }
};

DeleteStatement::DeleteStatement(std::unique_ptr<DeleteStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

DeleteStatement::DeleteStatement() : DeleteStatement(std::make_unique<DeleteStatement::Impl>()) {}

DeleteStatement::~DeleteStatement() noexcept = default;

DeleteStatement::DeleteStatement(DeleteStatement&&) noexcept = default;

DeleteStatement& DeleteStatement::operator=(DeleteStatement&&) noexcept = default;

expression::Expression* DeleteStatement::source() {
    return impl_->source_.get();
}

DeleteStatement& DeleteStatement::source(std::unique_ptr<expression::Expression> source) {
    impl_->source_ = std::move(source);
    return *this;
}

std::unique_ptr<expression::Expression> DeleteStatement::release_source() {
    return impl_->source_.release();
}

name::Name* DeleteStatement::table() {
    return impl_->table_.get();
}

DeleteStatement& DeleteStatement::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> DeleteStatement::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

key::RelationKey* DeleteStatement::relation_key() {
    return impl_->relation_key_.get();
}

DeleteStatement& DeleteStatement::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

DeleteStatement* DeleteStatement::clone() const & {
    return new DeleteStatement(impl_->clone());  // NOLINT
}

DeleteStatement* DeleteStatement::clone() && {
    return new DeleteStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::dml

