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
#include "shakujo/model/statement/dml/InsertRelationStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::statement::dml {

class InsertRelationStatement::Impl {
public:
    std::unique_ptr<name::Name> table_;
    util::NodeList<name::SimpleName> columns_;
    common::util::ManagedPtr<expression::Expression> source_;

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
        other->source_ = common::util::make_clone(source_);
        return other;
    }
};

InsertRelationStatement::InsertRelationStatement(std::unique_ptr<InsertRelationStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

InsertRelationStatement::InsertRelationStatement() : InsertRelationStatement(std::make_unique<InsertRelationStatement::Impl>()) {}

InsertRelationStatement::~InsertRelationStatement() noexcept = default;

InsertRelationStatement::InsertRelationStatement(InsertRelationStatement&&) noexcept = default;

InsertRelationStatement& InsertRelationStatement::operator=(InsertRelationStatement&&) noexcept = default;

name::Name* InsertRelationStatement::table() {
    return impl_->table_.get();
}

InsertRelationStatement& InsertRelationStatement::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> InsertRelationStatement::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

util::NodeList<name::SimpleName>& InsertRelationStatement::columns() {
    return impl_->columns_;
}

expression::Expression* InsertRelationStatement::source() {
    return impl_->source_.get();
}

InsertRelationStatement& InsertRelationStatement::source(std::unique_ptr<expression::Expression> source) {
    impl_->source_ = std::move(source);
    return *this;
}

std::unique_ptr<expression::Expression> InsertRelationStatement::release_source() {
    return impl_->source_.release();
}

InsertRelationStatement* InsertRelationStatement::clone() const & {
    return new InsertRelationStatement(impl_->clone());  // NOLINT
}

InsertRelationStatement* InsertRelationStatement::clone() && {
    return new InsertRelationStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::dml

