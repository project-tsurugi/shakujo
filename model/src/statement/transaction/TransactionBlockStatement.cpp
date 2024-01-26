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
#include "shakujo/model/statement/transaction/TransactionBlockStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/statement/Statement.h"

namespace shakujo::model::statement::transaction {

class TransactionBlockStatement::Impl {
public:
    std::unique_ptr<Statement> body_;

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

TransactionBlockStatement::TransactionBlockStatement(std::unique_ptr<TransactionBlockStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

TransactionBlockStatement::TransactionBlockStatement() : TransactionBlockStatement(std::make_unique<TransactionBlockStatement::Impl>()) {}

TransactionBlockStatement::~TransactionBlockStatement() noexcept = default;

TransactionBlockStatement::TransactionBlockStatement(TransactionBlockStatement&&) noexcept = default;

TransactionBlockStatement& TransactionBlockStatement::operator=(TransactionBlockStatement&&) noexcept = default;

Statement* TransactionBlockStatement::body() {
    return impl_->body_.get();
}

TransactionBlockStatement& TransactionBlockStatement::body(std::unique_ptr<Statement> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<Statement> TransactionBlockStatement::release_body() {
    std::unique_ptr<Statement> ret { std::move(impl_->body_) };
    impl_->body_ = {};
    return ret;
}

TransactionBlockStatement* TransactionBlockStatement::clone() const & {
    return new TransactionBlockStatement(impl_->clone());  // NOLINT
}

TransactionBlockStatement* TransactionBlockStatement::clone() && {
    return new TransactionBlockStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::transaction

