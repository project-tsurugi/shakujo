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
#include "shakujo/model/statement/LogStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::model::statement {

class LogStatement::Impl {
public:
    LogStatement::Level level_ { LogStatement::Level::INFO };
    common::util::ManagedPtr<expression::Expression> value_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->level_ = level_;
        other->value_ = common::util::make_clone(value_);
        return other;
    }
};

LogStatement::LogStatement(std::unique_ptr<LogStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

LogStatement::LogStatement() : LogStatement(std::make_unique<LogStatement::Impl>()) {}

LogStatement::~LogStatement() noexcept = default;

LogStatement::LogStatement(LogStatement&&) noexcept = default;

LogStatement& LogStatement::operator=(LogStatement&&) noexcept = default;

LogStatement::Level LogStatement::level() const {
    return impl_->level_;
}

LogStatement& LogStatement::level(LogStatement::Level level) {
    impl_->level_ = level;
    return *this;
}

expression::Expression* LogStatement::value() {
    return impl_->value_.get();
}

LogStatement& LogStatement::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> LogStatement::release_value() {
    return impl_->value_.release();
}

LogStatement* LogStatement::clone() const & {
    return new LogStatement(impl_->clone());  // NOLINT
}

LogStatement* LogStatement::clone() && {
    return new LogStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

