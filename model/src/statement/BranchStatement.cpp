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
#include "shakujo/model/statement/BranchStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"

namespace shakujo::model::statement {

class BranchStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> condition_;
    std::unique_ptr<Statement> then_statement_;
    std::unique_ptr<Statement> else_statement_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->condition_ = common::util::make_clone(condition_);
        other->then_statement_ = common::util::make_clone(then_statement_);
        other->else_statement_ = common::util::make_clone(else_statement_);
        return other;
    }
};

BranchStatement::BranchStatement(std::unique_ptr<BranchStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

BranchStatement::BranchStatement() : BranchStatement(std::make_unique<BranchStatement::Impl>()) {}

BranchStatement::~BranchStatement() noexcept = default;

BranchStatement::BranchStatement(BranchStatement&&) noexcept = default;

BranchStatement& BranchStatement::operator=(BranchStatement&&) noexcept = default;

expression::Expression* BranchStatement::condition() {
    return impl_->condition_.get();
}

BranchStatement& BranchStatement::condition(std::unique_ptr<expression::Expression> condition) {
    impl_->condition_ = std::move(condition);
    return *this;
}

std::unique_ptr<expression::Expression> BranchStatement::release_condition() {
    return impl_->condition_.release();
}

Statement* BranchStatement::then_statement() {
    return impl_->then_statement_.get();
}

BranchStatement& BranchStatement::then_statement(std::unique_ptr<Statement> then_statement) {
    impl_->then_statement_ = std::move(then_statement);
    return *this;
}

std::unique_ptr<Statement> BranchStatement::release_then_statement() {
    std::unique_ptr<Statement> ret { std::move(impl_->then_statement_) };
    impl_->then_statement_ = {};
    return ret;
}

Statement* BranchStatement::else_statement() {
    return impl_->else_statement_.get();
}

BranchStatement& BranchStatement::else_statement(std::unique_ptr<Statement> else_statement) {
    impl_->else_statement_ = std::move(else_statement);
    return *this;
}

std::unique_ptr<Statement> BranchStatement::release_else_statement() {
    std::unique_ptr<Statement> ret { std::move(impl_->else_statement_) };
    impl_->else_statement_ = {};
    return ret;
}

BranchStatement* BranchStatement::clone() const & {
    return new BranchStatement(impl_->clone());  // NOLINT
}

BranchStatement* BranchStatement::clone() && {
    return new BranchStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

