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
#include "shakujo/model/statement/ExpressionStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::model::statement {

class ExpressionStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> body_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->body_ = common::util::make_clone(body_);
        return other;
    }
};

ExpressionStatement::ExpressionStatement(std::unique_ptr<ExpressionStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

ExpressionStatement::ExpressionStatement() : ExpressionStatement(std::make_unique<ExpressionStatement::Impl>()) {}

ExpressionStatement::~ExpressionStatement() noexcept = default;

ExpressionStatement::ExpressionStatement(ExpressionStatement&&) noexcept = default;

ExpressionStatement& ExpressionStatement::operator=(ExpressionStatement&&) noexcept = default;

expression::Expression* ExpressionStatement::body() {
    return impl_->body_.get();
}

ExpressionStatement& ExpressionStatement::body(std::unique_ptr<expression::Expression> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<expression::Expression> ExpressionStatement::release_body() {
    return impl_->body_.release();
}

ExpressionStatement* ExpressionStatement::clone() const & {
    return new ExpressionStatement(impl_->clone());  // NOLINT
}

ExpressionStatement* ExpressionStatement::clone() && {
    return new ExpressionStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

