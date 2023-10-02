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
#include "shakujo/model/statement/ReturnStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::model::statement {

class ReturnStatement::Impl {
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

ReturnStatement::ReturnStatement(std::unique_ptr<ReturnStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

ReturnStatement::ReturnStatement() : ReturnStatement(std::make_unique<ReturnStatement::Impl>()) {}

ReturnStatement::~ReturnStatement() noexcept = default;

ReturnStatement::ReturnStatement(ReturnStatement&&) noexcept = default;

ReturnStatement& ReturnStatement::operator=(ReturnStatement&&) noexcept = default;

expression::Expression* ReturnStatement::value() {
    return impl_->value_.get();
}

ReturnStatement& ReturnStatement::value(std::unique_ptr<expression::Expression> value) {
    impl_->value_ = std::move(value);
    return *this;
}

std::unique_ptr<expression::Expression> ReturnStatement::release_value() {
    return impl_->value_.release();
}

ReturnStatement* ReturnStatement::clone() const & {
    return new ReturnStatement(impl_->clone());  // NOLINT
}

ReturnStatement* ReturnStatement::clone() && {
    return new ReturnStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

