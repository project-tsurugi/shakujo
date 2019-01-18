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
#include "shakujo/model/expression/StringOperator.h"

#include <utility>
#include <memory>
#include <string>
#include <iostream>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class StringOperator::Impl {
public:
    StringOperator::Kind operator_kind_ { StringOperator::Kind::INVALID };
    std::string string_;
    common::util::ManagedPtr<Expression> operand_;
    std::unique_ptr<key::ExpressionKey> expression_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->operator_kind_ = operator_kind_;
        other->string_ = string_;
        other->operand_ = common::util::make_clone(operand_);
        return other;
    }
};

StringOperator::StringOperator(std::unique_ptr<StringOperator::Impl> impl) noexcept : impl_(std::move(impl)) {}

StringOperator::StringOperator() : StringOperator(std::make_unique<StringOperator::Impl>()) {}

StringOperator::~StringOperator() noexcept = default;

StringOperator::StringOperator(StringOperator&&) noexcept = default;

StringOperator& StringOperator::operator=(StringOperator&&) noexcept = default;

StringOperator::Kind StringOperator::operator_kind() const {
    return impl_->operator_kind_;
}

StringOperator& StringOperator::operator_kind(StringOperator::Kind operator_kind) {
    impl_->operator_kind_ = operator_kind;
    return *this;
}

std::string const& StringOperator::string() const {
    return impl_->string_;
}

StringOperator& StringOperator::string(std::string string) {
    impl_->string_ = std::move(string);
    return *this;
}

Expression* StringOperator::operand() {
    return impl_->operand_.get();
}

StringOperator& StringOperator::operand(std::unique_ptr<Expression> operand) {
    impl_->operand_ = std::move(operand);
    return *this;
}

std::unique_ptr<Expression> StringOperator::release_operand() {
    return impl_->operand_.release();
}

key::ExpressionKey* StringOperator::expression_key() {
    return impl_->expression_key_.get();
}

StringOperator& StringOperator::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

StringOperator* StringOperator::clone() const & {
    return new StringOperator(impl_->clone());  // NOLINT
}

StringOperator* StringOperator::clone() && {
    return new StringOperator(std::move(impl_));  // NOLINT
}

std::ostream& operator<<(std::ostream& out, StringOperator::Kind value) {
    switch (value) {
    case StringOperator::Kind::LIKE:
        out << "LIKE";
        break;
    case StringOperator::Kind::LIKE_IGNORECASE:
        out << "LIKE_IGNORECASE";
        break;
    case StringOperator::Kind::REGEX:
        out << "REGEX";
        break;
    case StringOperator::Kind::INVALID:
        out << "INVALID";
        break;
    }
    return out;
}

}  // namespace shakujo::model::expression

