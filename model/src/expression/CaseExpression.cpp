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
#include "shakujo/model/expression/CaseExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::expression {

class CaseExpression::Impl {
public:
    util::FragmentList<CaseExpression::Clause> cases_;
    common::util::ManagedPtr<Expression> default_expression_;
    std::unique_ptr<key::ExpressionKey> expression_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        if (!cases_.empty()) {
            other->cases_.reserve(cases_.size());
            for (auto e : cases_) {
                other->cases_.push_back(common::util::make_clone(e));
            }
        }
        other->default_expression_ = common::util::make_clone(default_expression_);
        return other;
    }
};

class CaseExpression::Clause::Impl {
public:
    common::util::ManagedPtr<Expression> condition_;
    common::util::ManagedPtr<Expression> body_;

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

CaseExpression::CaseExpression(std::unique_ptr<CaseExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

CaseExpression::CaseExpression() : CaseExpression(std::make_unique<CaseExpression::Impl>()) {}

CaseExpression::~CaseExpression() noexcept = default;

CaseExpression::CaseExpression(CaseExpression&&) noexcept = default;

CaseExpression& CaseExpression::operator=(CaseExpression&&) noexcept = default;

util::FragmentList<CaseExpression::Clause>& CaseExpression::cases() {
    return impl_->cases_;
}

Expression* CaseExpression::default_expression() {
    return impl_->default_expression_.get();
}

CaseExpression& CaseExpression::default_expression(std::unique_ptr<Expression> default_expression) {
    impl_->default_expression_ = std::move(default_expression);
    return *this;
}

std::unique_ptr<Expression> CaseExpression::release_default_expression() {
    return impl_->default_expression_.release();
}

key::ExpressionKey* CaseExpression::expression_key() {
    return impl_->expression_key_.get();
}

CaseExpression& CaseExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

CaseExpression* CaseExpression::clone() const & {
    return new CaseExpression(impl_->clone());  // NOLINT
}

CaseExpression* CaseExpression::clone() && {
    return new CaseExpression(std::move(impl_));  // NOLINT
}

CaseExpression::Clause::Clause(std::unique_ptr<CaseExpression::Clause::Impl> impl) noexcept : impl_(std::move(impl)) {}

CaseExpression::Clause::Clause() : CaseExpression::Clause(std::make_unique<CaseExpression::Clause::Impl>()) {}

CaseExpression::Clause::~Clause() noexcept = default;

CaseExpression::Clause::Clause(CaseExpression::Clause&&) noexcept = default;

CaseExpression::Clause& CaseExpression::Clause::operator=(CaseExpression::Clause&&) noexcept = default;

Expression* CaseExpression::Clause::condition() {
    return impl_->condition_.get();
}

CaseExpression::Clause& CaseExpression::Clause::condition(std::unique_ptr<Expression> condition) {
    impl_->condition_ = std::move(condition);
    return *this;
}

std::unique_ptr<Expression> CaseExpression::Clause::release_condition() {
    return impl_->condition_.release();
}

Expression* CaseExpression::Clause::body() {
    return impl_->body_.get();
}

CaseExpression::Clause& CaseExpression::Clause::body(std::unique_ptr<Expression> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<Expression> CaseExpression::Clause::release_body() {
    return impl_->body_.release();
}

CaseExpression::Clause* CaseExpression::Clause::clone() const & {
    return new CaseExpression::Clause(impl_->clone());  // NOLINT
}

CaseExpression::Clause* CaseExpression::Clause::clone() && {
    return new CaseExpression::Clause(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

