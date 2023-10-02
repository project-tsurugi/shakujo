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
#include "shakujo/model/statement/ForStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::statement {

class ForStatement::Impl {
public:
    util::NodeList<Statement> initialize_;
    common::util::ManagedPtr<expression::Expression> condition_;
    util::NodeList<Statement> update_;
    std::unique_ptr<Statement> body_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        if (!initialize_.empty()) {
            other->initialize_.reserve(initialize_.size());
            for (auto e : initialize_) {
                other->initialize_.push_back(common::util::make_clone(e));
            }
        }
        other->condition_ = common::util::make_clone(condition_);
        if (!update_.empty()) {
            other->update_.reserve(update_.size());
            for (auto e : update_) {
                other->update_.push_back(common::util::make_clone(e));
            }
        }
        other->body_ = common::util::make_clone(body_);
        return other;
    }
};

ForStatement::ForStatement(std::unique_ptr<ForStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

ForStatement::ForStatement() : ForStatement(std::make_unique<ForStatement::Impl>()) {}

ForStatement::~ForStatement() noexcept = default;

ForStatement::ForStatement(ForStatement&&) noexcept = default;

ForStatement& ForStatement::operator=(ForStatement&&) noexcept = default;

util::NodeList<Statement>& ForStatement::initialize() {
    return impl_->initialize_;
}

expression::Expression* ForStatement::condition() {
    return impl_->condition_.get();
}

ForStatement& ForStatement::condition(std::unique_ptr<expression::Expression> condition) {
    impl_->condition_ = std::move(condition);
    return *this;
}

std::unique_ptr<expression::Expression> ForStatement::release_condition() {
    return impl_->condition_.release();
}

util::NodeList<Statement>& ForStatement::update() {
    return impl_->update_;
}

Statement* ForStatement::body() {
    return impl_->body_.get();
}

ForStatement& ForStatement::body(std::unique_ptr<Statement> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<Statement> ForStatement::release_body() {
    std::unique_ptr<Statement> ret { std::move(impl_->body_) };
    impl_->body_ = {};
    return ret;
}

ForStatement* ForStatement::clone() const & {
    return new ForStatement(impl_->clone());  // NOLINT
}

ForStatement* ForStatement::clone() && {
    return new ForStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

