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
#include "shakujo/model/statement/ForEachStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::statement {

class ForEachStatement::Impl {
public:
    std::unique_ptr<name::Name> name_;
    std::unique_ptr<type::Type> type_;
    common::util::ManagedPtr<expression::Expression> enumeration_;
    std::unique_ptr<Statement> body_;
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
        other->type_ = common::util::make_clone(type_);
        other->enumeration_ = common::util::make_clone(enumeration_);
        other->body_ = common::util::make_clone(body_);
        return other;
    }
};

ForEachStatement::ForEachStatement(std::unique_ptr<ForEachStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

ForEachStatement::ForEachStatement() : ForEachStatement(std::make_unique<ForEachStatement::Impl>()) {}

ForEachStatement::~ForEachStatement() noexcept = default;

ForEachStatement::ForEachStatement(ForEachStatement&&) noexcept = default;

ForEachStatement& ForEachStatement::operator=(ForEachStatement&&) noexcept = default;

name::Name* ForEachStatement::name() {
    return impl_->name_.get();
}

ForEachStatement& ForEachStatement::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> ForEachStatement::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

type::Type* ForEachStatement::type() {
    return impl_->type_.get();
}

ForEachStatement& ForEachStatement::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> ForEachStatement::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

expression::Expression* ForEachStatement::enumeration() {
    return impl_->enumeration_.get();
}

ForEachStatement& ForEachStatement::enumeration(std::unique_ptr<expression::Expression> enumeration) {
    impl_->enumeration_ = std::move(enumeration);
    return *this;
}

std::unique_ptr<expression::Expression> ForEachStatement::release_enumeration() {
    return impl_->enumeration_.release();
}

Statement* ForEachStatement::body() {
    return impl_->body_.get();
}

ForEachStatement& ForEachStatement::body(std::unique_ptr<Statement> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<Statement> ForEachStatement::release_body() {
    std::unique_ptr<Statement> ret { std::move(impl_->body_) };
    impl_->body_ = {};
    return ret;
}

key::VariableKey* ForEachStatement::variable_key() {
    return impl_->variable_key_.get();
}

ForEachStatement& ForEachStatement::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

ForEachStatement* ForEachStatement::clone() const & {
    return new ForEachStatement(impl_->clone());  // NOLINT
}

ForEachStatement* ForEachStatement::clone() && {
    return new ForEachStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

