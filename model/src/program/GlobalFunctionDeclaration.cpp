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
#include "shakujo/model/program/GlobalFunctionDeclaration.h"

#include <utility>
#include <memory>
#include <set>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::program {

class GlobalFunctionDeclaration::Impl {
public:
    std::unique_ptr<name::Name> name_;
    std::unique_ptr<type::Type> type_;
    util::FragmentList<GlobalFunctionDeclaration::Parameter> parameters_;
    std::unique_ptr<statement::Statement> body_;
    std::set<GlobalFunctionDeclaration::Attribute> attributes_;
    std::unique_ptr<key::FunctionKey> function_key_;

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
        if (!parameters_.empty()) {
            other->parameters_.reserve(parameters_.size());
            for (auto e : parameters_) {
                other->parameters_.push_back(common::util::make_clone(e));
            }
        }
        other->body_ = common::util::make_clone(body_);
        other->attributes_ = attributes_;
        return other;
    }
};

class GlobalFunctionDeclaration::Parameter::Impl {
public:
    std::unique_ptr<name::Name> name_;
    std::unique_ptr<type::Type> type_;
    std::set<GlobalFunctionDeclaration::Parameter::Attribute> attributes_;
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
        other->attributes_ = attributes_;
        return other;
    }
};

GlobalFunctionDeclaration::GlobalFunctionDeclaration(std::unique_ptr<GlobalFunctionDeclaration::Impl> impl) noexcept : impl_(std::move(impl)) {}

GlobalFunctionDeclaration::GlobalFunctionDeclaration() : GlobalFunctionDeclaration(std::make_unique<GlobalFunctionDeclaration::Impl>()) {}

GlobalFunctionDeclaration::~GlobalFunctionDeclaration() noexcept = default;

GlobalFunctionDeclaration::GlobalFunctionDeclaration(GlobalFunctionDeclaration&&) noexcept = default;

GlobalFunctionDeclaration& GlobalFunctionDeclaration::operator=(GlobalFunctionDeclaration&&) noexcept = default;

name::Name* GlobalFunctionDeclaration::name() {
    return impl_->name_.get();
}

GlobalFunctionDeclaration& GlobalFunctionDeclaration::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> GlobalFunctionDeclaration::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

type::Type* GlobalFunctionDeclaration::type() {
    return impl_->type_.get();
}

GlobalFunctionDeclaration& GlobalFunctionDeclaration::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> GlobalFunctionDeclaration::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

util::FragmentList<GlobalFunctionDeclaration::Parameter>& GlobalFunctionDeclaration::parameters() {
    return impl_->parameters_;
}

statement::Statement* GlobalFunctionDeclaration::body() {
    return impl_->body_.get();
}

GlobalFunctionDeclaration& GlobalFunctionDeclaration::body(std::unique_ptr<statement::Statement> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<statement::Statement> GlobalFunctionDeclaration::release_body() {
    std::unique_ptr<statement::Statement> ret { std::move(impl_->body_) };
    impl_->body_ = {};
    return ret;
}

std::set<GlobalFunctionDeclaration::Attribute>& GlobalFunctionDeclaration::attributes() {
    return impl_->attributes_;
}

key::FunctionKey* GlobalFunctionDeclaration::function_key() {
    return impl_->function_key_.get();
}

GlobalFunctionDeclaration& GlobalFunctionDeclaration::function_key(std::unique_ptr<key::FunctionKey> function_key) {
    impl_->function_key_ = std::move(function_key);
    return *this;
}

GlobalFunctionDeclaration* GlobalFunctionDeclaration::clone() const & {
    return new GlobalFunctionDeclaration(impl_->clone());  // NOLINT
}

GlobalFunctionDeclaration* GlobalFunctionDeclaration::clone() && {
    return new GlobalFunctionDeclaration(std::move(impl_));  // NOLINT
}

GlobalFunctionDeclaration::Parameter::Parameter(std::unique_ptr<GlobalFunctionDeclaration::Parameter::Impl> impl) noexcept : impl_(std::move(impl)) {}

GlobalFunctionDeclaration::Parameter::Parameter() : GlobalFunctionDeclaration::Parameter(std::make_unique<GlobalFunctionDeclaration::Parameter::Impl>()) {}

GlobalFunctionDeclaration::Parameter::~Parameter() noexcept = default;

GlobalFunctionDeclaration::Parameter::Parameter(GlobalFunctionDeclaration::Parameter&&) noexcept = default;

GlobalFunctionDeclaration::Parameter& GlobalFunctionDeclaration::Parameter::operator=(GlobalFunctionDeclaration::Parameter&&) noexcept = default;

name::Name* GlobalFunctionDeclaration::Parameter::name() {
    return impl_->name_.get();
}

GlobalFunctionDeclaration::Parameter& GlobalFunctionDeclaration::Parameter::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> GlobalFunctionDeclaration::Parameter::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

type::Type* GlobalFunctionDeclaration::Parameter::type() {
    return impl_->type_.get();
}

GlobalFunctionDeclaration::Parameter& GlobalFunctionDeclaration::Parameter::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> GlobalFunctionDeclaration::Parameter::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

std::set<GlobalFunctionDeclaration::Parameter::Attribute>& GlobalFunctionDeclaration::Parameter::attributes() {
    return impl_->attributes_;
}

key::VariableKey* GlobalFunctionDeclaration::Parameter::variable_key() {
    return impl_->variable_key_.get();
}

GlobalFunctionDeclaration::Parameter& GlobalFunctionDeclaration::Parameter::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

GlobalFunctionDeclaration::Parameter* GlobalFunctionDeclaration::Parameter::clone() const & {
    return new GlobalFunctionDeclaration::Parameter(impl_->clone());  // NOLINT
}

GlobalFunctionDeclaration::Parameter* GlobalFunctionDeclaration::Parameter::clone() && {
    return new GlobalFunctionDeclaration::Parameter(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::program

