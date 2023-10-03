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
#include "shakujo/model/statement/RaiseStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::model::statement {

class RaiseStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> value_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->value_ = common::util::make_clone(value_);
        return other;
    }
};

RaiseStatement::RaiseStatement(std::unique_ptr<RaiseStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

RaiseStatement::RaiseStatement() : RaiseStatement(std::make_unique<RaiseStatement::Impl>()) {}

RaiseStatement::~RaiseStatement() noexcept = default;

RaiseStatement::RaiseStatement(RaiseStatement&&) noexcept = default;

RaiseStatement& RaiseStatement::operator=(RaiseStatement&&) noexcept = default;

expression::Expression* RaiseStatement::value() {
    return impl_->value_.get();
}

RaiseStatement& RaiseStatement::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> RaiseStatement::release_value() {
    return impl_->value_.release();
}

RaiseStatement* RaiseStatement::clone() const & {
    return new RaiseStatement(impl_->clone());  // NOLINT
}

RaiseStatement* RaiseStatement::clone() && {
    return new RaiseStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

