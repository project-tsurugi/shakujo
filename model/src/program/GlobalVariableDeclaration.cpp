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
#include "shakujo/model/program/GlobalVariableDeclaration.h"

#include <utility>
#include <memory>
#include <set>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::program {

class GlobalVariableDeclaration::Impl {
public:
    std::unique_ptr<name::Name> name_;
    std::unique_ptr<type::Type> type_;
    common::util::ManagedPtr<expression::Expression> value_;
    std::set<GlobalVariableDeclaration::Attribute> attributes_;
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
        other->value_ = common::util::make_clone(value_);
        other->attributes_ = attributes_;
        return other;
    }
};

GlobalVariableDeclaration::GlobalVariableDeclaration(std::unique_ptr<GlobalVariableDeclaration::Impl> impl) noexcept : impl_(std::move(impl)) {}

GlobalVariableDeclaration::GlobalVariableDeclaration() : GlobalVariableDeclaration(std::make_unique<GlobalVariableDeclaration::Impl>()) {}

GlobalVariableDeclaration::~GlobalVariableDeclaration() noexcept = default;

GlobalVariableDeclaration::GlobalVariableDeclaration(GlobalVariableDeclaration&&) noexcept = default;

GlobalVariableDeclaration& GlobalVariableDeclaration::operator=(GlobalVariableDeclaration&&) noexcept = default;

name::Name* GlobalVariableDeclaration::name() {
    return impl_->name_.get();
}

GlobalVariableDeclaration& GlobalVariableDeclaration::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> GlobalVariableDeclaration::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

type::Type* GlobalVariableDeclaration::type() {
    return impl_->type_.get();
}

GlobalVariableDeclaration& GlobalVariableDeclaration::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> GlobalVariableDeclaration::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

expression::Expression* GlobalVariableDeclaration::value() {
    return impl_->value_.get();
}

GlobalVariableDeclaration& GlobalVariableDeclaration::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> GlobalVariableDeclaration::release_value() {
    return impl_->value_.release();
}

std::set<GlobalVariableDeclaration::Attribute>& GlobalVariableDeclaration::attributes() {
    return impl_->attributes_;
}

key::VariableKey* GlobalVariableDeclaration::variable_key() {
    return impl_->variable_key_.get();
}

GlobalVariableDeclaration& GlobalVariableDeclaration::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

GlobalVariableDeclaration* GlobalVariableDeclaration::clone() const & {
    return new GlobalVariableDeclaration(impl_->clone());  // NOLINT
}

GlobalVariableDeclaration* GlobalVariableDeclaration::clone() && {
    return new GlobalVariableDeclaration(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::program

