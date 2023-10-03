/*
 * Copyright 2018-2023 Project Tsurugi.
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
#include "shakujo/model/statement/WhileStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"

namespace shakujo::model::statement {

class WhileStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> condition_;
    std::unique_ptr<Statement> body_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->condition_ = common::util::make_clone(condition_);
        other->body_ = common::util::make_clone(body_);
        return other;
    }
};

WhileStatement::WhileStatement(std::unique_ptr<WhileStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

WhileStatement::WhileStatement() : WhileStatement(std::make_unique<WhileStatement::Impl>()) {}

WhileStatement::~WhileStatement() noexcept = default;

WhileStatement::WhileStatement(WhileStatement&&) noexcept = default;

WhileStatement& WhileStatement::operator=(WhileStatement&&) noexcept = default;

expression::Expression* WhileStatement::condition() {
    return impl_->condition_.get();
}

WhileStatement& WhileStatement::condition(std::unique_ptr<expression::Expression> condition) {
    impl_->condition_ = std::move(condition);
    return *this;
}

std::unique_ptr<expression::Expression> WhileStatement::release_condition() {
    return impl_->condition_.release();
}

Statement* WhileStatement::body() {
    return impl_->body_.get();
}

WhileStatement& WhileStatement::body(std::unique_ptr<Statement> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<Statement> WhileStatement::release_body() {
    std::unique_ptr<Statement> ret { std::move(impl_->body_) };
    impl_->body_ = {};
    return ret;
}

WhileStatement* WhileStatement::clone() const & {
    return new WhileStatement(impl_->clone());  // NOLINT
}

WhileStatement* WhileStatement::clone() && {
    return new WhileStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

