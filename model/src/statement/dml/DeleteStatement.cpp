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
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::statement::dml {

class DeleteStatement::Impl {
public:
    std::unique_ptr<name::Name> table_;
    common::util::ManagedPtr<expression::Expression> condition_;
    std::unique_ptr<key::VariableKey> variable_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->table_ = common::util::make_clone(table_);
        other->condition_ = common::util::make_clone(condition_);
        return other;
    }
};

DeleteStatement::DeleteStatement(std::unique_ptr<DeleteStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

DeleteStatement::DeleteStatement() : DeleteStatement(std::make_unique<DeleteStatement::Impl>()) {}

DeleteStatement::~DeleteStatement() noexcept = default;

DeleteStatement::DeleteStatement(DeleteStatement&&) noexcept = default;

DeleteStatement& DeleteStatement::operator=(DeleteStatement&&) noexcept = default;

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

expression::Expression* DeleteStatement::condition() {
    return impl_->condition_.get();
}

DeleteStatement& DeleteStatement::condition(std::unique_ptr<expression::Expression> condition) {
    impl_->condition_ = std::move(condition);
    return *this;
}

std::unique_ptr<expression::Expression> DeleteStatement::release_condition() {
    return impl_->condition_.release();
}

key::VariableKey* DeleteStatement::variable_key() {
    return impl_->variable_key_.get();
}

DeleteStatement& DeleteStatement::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

DeleteStatement* DeleteStatement::clone() const & {
    return new DeleteStatement(impl_->clone());  // NOLINT
}

DeleteStatement* DeleteStatement::clone() && {
    return new DeleteStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::dml

