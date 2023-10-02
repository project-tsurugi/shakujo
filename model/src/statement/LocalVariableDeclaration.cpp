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
#include "shakujo/model/statement/LocalVariableDeclaration.h"

#include <utility>
#include <memory>
#include <set>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::statement {

class LocalVariableDeclaration::Impl {
public:
    std::unique_ptr<name::Name> name_;
    std::unique_ptr<type::Type> type_;
    common::util::ManagedPtr<expression::Expression> value_;
    std::set<LocalVariableDeclaration::Attribute> attributes_;
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

LocalVariableDeclaration::LocalVariableDeclaration(std::unique_ptr<LocalVariableDeclaration::Impl> impl) noexcept : impl_(std::move(impl)) {}

LocalVariableDeclaration::LocalVariableDeclaration() : LocalVariableDeclaration(std::make_unique<LocalVariableDeclaration::Impl>()) {}

LocalVariableDeclaration::~LocalVariableDeclaration() noexcept = default;

LocalVariableDeclaration::LocalVariableDeclaration(LocalVariableDeclaration&&) noexcept = default;

LocalVariableDeclaration& LocalVariableDeclaration::operator=(LocalVariableDeclaration&&) noexcept = default;

name::Name* LocalVariableDeclaration::name() {
    return impl_->name_.get();
}

LocalVariableDeclaration& LocalVariableDeclaration::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> LocalVariableDeclaration::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

type::Type* LocalVariableDeclaration::type() {
    return impl_->type_.get();
}

LocalVariableDeclaration& LocalVariableDeclaration::type(std::unique_ptr<type::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<type::Type> LocalVariableDeclaration::release_type() {
    std::unique_ptr<type::Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

expression::Expression* LocalVariableDeclaration::value() {
    return impl_->value_.get();
}

LocalVariableDeclaration& LocalVariableDeclaration::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> LocalVariableDeclaration::release_value() {
    return impl_->value_.release();
}

std::set<LocalVariableDeclaration::Attribute>& LocalVariableDeclaration::attributes() {
    return impl_->attributes_;
}

key::VariableKey* LocalVariableDeclaration::variable_key() {
    return impl_->variable_key_.get();
}

LocalVariableDeclaration& LocalVariableDeclaration::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

LocalVariableDeclaration* LocalVariableDeclaration::clone() const & {
    return new LocalVariableDeclaration(impl_->clone());  // NOLINT
}

LocalVariableDeclaration* LocalVariableDeclaration::clone() && {
    return new LocalVariableDeclaration(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

