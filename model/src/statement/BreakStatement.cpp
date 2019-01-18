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
#include "shakujo/model/statement/BreakStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::statement {

class BreakStatement::Impl {
public:
    std::unique_ptr<name::Name> anchor_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->anchor_ = common::util::make_clone(anchor_);
        return other;
    }
};

BreakStatement::BreakStatement(std::unique_ptr<BreakStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

BreakStatement::BreakStatement() : BreakStatement(std::make_unique<BreakStatement::Impl>()) {}

BreakStatement::~BreakStatement() noexcept = default;

BreakStatement::BreakStatement(BreakStatement&&) noexcept = default;

BreakStatement& BreakStatement::operator=(BreakStatement&&) noexcept = default;

name::Name* BreakStatement::anchor() {
    return impl_->anchor_.get();
}

BreakStatement& BreakStatement::anchor(std::unique_ptr<name::Name> anchor) {
    impl_->anchor_ = std::move(anchor);
    return *this;
}

std::unique_ptr<name::Name> BreakStatement::release_anchor() {
    std::unique_ptr<name::Name> ret { std::move(impl_->anchor_) };
    impl_->anchor_ = {};
    return ret;
}

BreakStatement* BreakStatement::clone() const & {
    return new BreakStatement(impl_->clone());  // NOLINT
}

BreakStatement* BreakStatement::clone() && {
    return new BreakStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

