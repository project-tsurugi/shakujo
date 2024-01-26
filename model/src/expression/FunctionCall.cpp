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
#include "shakujo/model/expression/FunctionCall.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression {

class FunctionCall::Impl {
public:
    std::unique_ptr<name::Name> name_;
    util::ManagedNodeList<Expression> arguments_;
    FunctionCall::Quantifier quantifier_ { FunctionCall::Quantifier::ABSENT };
    std::unique_ptr<key::ExpressionKey> expression_key_;
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
        if (!arguments_.empty()) {
            other->arguments_.reserve(arguments_.size());
            for (auto e : arguments_) {
                other->arguments_.push_back(common::util::make_clone(e));
            }
        }
        other->quantifier_ = quantifier_;
        return other;
    }
};

FunctionCall::FunctionCall(std::unique_ptr<FunctionCall::Impl> impl) noexcept : impl_(std::move(impl)) {}

FunctionCall::FunctionCall() : FunctionCall(std::make_unique<FunctionCall::Impl>()) {}

FunctionCall::~FunctionCall() noexcept = default;

FunctionCall::FunctionCall(FunctionCall&&) noexcept = default;

FunctionCall& FunctionCall::operator=(FunctionCall&&) noexcept = default;

name::Name* FunctionCall::name() {
    return impl_->name_.get();
}

FunctionCall& FunctionCall::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> FunctionCall::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

util::ManagedNodeList<Expression>& FunctionCall::arguments() {
    return impl_->arguments_;
}

FunctionCall::Quantifier FunctionCall::quantifier() const {
    return impl_->quantifier_;
}

FunctionCall& FunctionCall::quantifier(FunctionCall::Quantifier quantifier) {
    impl_->quantifier_ = quantifier;
    return *this;
}

key::ExpressionKey* FunctionCall::expression_key() {
    return impl_->expression_key_.get();
}

FunctionCall& FunctionCall::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::FunctionKey* FunctionCall::function_key() {
    return impl_->function_key_.get();
}

FunctionCall& FunctionCall::function_key(std::unique_ptr<key::FunctionKey> function_key) {
    impl_->function_key_ = std::move(function_key);
    return *this;
}

FunctionCall* FunctionCall::clone() const & {
    return new FunctionCall(impl_->clone());  // NOLINT
}

FunctionCall* FunctionCall::clone() && {
    return new FunctionCall(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

