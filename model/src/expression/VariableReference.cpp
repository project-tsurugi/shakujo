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
#include "shakujo/model/expression/VariableReference.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::expression {

class VariableReference::Impl {
public:
    std::unique_ptr<name::Name> name_;
    std::unique_ptr<key::ExpressionKey> expression_key_;
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
        return other;
    }
};

VariableReference::VariableReference(std::unique_ptr<VariableReference::Impl> impl) noexcept : impl_(std::move(impl)) {}

VariableReference::VariableReference() : VariableReference(std::make_unique<VariableReference::Impl>()) {}

VariableReference::~VariableReference() noexcept = default;

VariableReference::VariableReference(VariableReference&&) noexcept = default;

VariableReference& VariableReference::operator=(VariableReference&&) noexcept = default;

name::Name* VariableReference::name() {
    return impl_->name_.get();
}

VariableReference& VariableReference::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> VariableReference::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

key::ExpressionKey* VariableReference::expression_key() {
    return impl_->expression_key_.get();
}

VariableReference& VariableReference::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::VariableKey* VariableReference::variable_key() {
    return impl_->variable_key_.get();
}

VariableReference& VariableReference::variable_key(std::unique_ptr<key::VariableKey> variable_key) {
    impl_->variable_key_ = std::move(variable_key);
    return *this;
}

VariableReference* VariableReference::clone() const & {
    return new VariableReference(impl_->clone());  // NOLINT
}

VariableReference* VariableReference::clone() && {
    return new VariableReference(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

