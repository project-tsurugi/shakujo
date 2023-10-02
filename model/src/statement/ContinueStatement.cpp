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
#include "shakujo/model/statement/ContinueStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::statement {

class ContinueStatement::Impl {
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

ContinueStatement::ContinueStatement(std::unique_ptr<ContinueStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

ContinueStatement::ContinueStatement() : ContinueStatement(std::make_unique<ContinueStatement::Impl>()) {}

ContinueStatement::~ContinueStatement() noexcept = default;

ContinueStatement::ContinueStatement(ContinueStatement&&) noexcept = default;

ContinueStatement& ContinueStatement::operator=(ContinueStatement&&) noexcept = default;

name::Name* ContinueStatement::anchor() {
    return impl_->anchor_.get();
}

ContinueStatement& ContinueStatement::anchor(std::unique_ptr<name::Name> anchor) {
    impl_->anchor_ = std::move(anchor);
    return *this;
}

std::unique_ptr<name::Name> ContinueStatement::release_anchor() {
    std::unique_ptr<name::Name> ret { std::move(impl_->anchor_) };
    impl_->anchor_ = {};
    return ret;
}

ContinueStatement* ContinueStatement::clone() const & {
    return new ContinueStatement(impl_->clone());  // NOLINT
}

ContinueStatement* ContinueStatement::clone() && {
    return new ContinueStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

