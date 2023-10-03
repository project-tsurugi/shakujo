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
#include "shakujo/model/name/SimpleName.h"

#include <utility>
#include <string>


namespace shakujo::model::name {

class SimpleName::Impl {
public:
    std::string token_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->token_ = token_;
        return other;
    }
};

SimpleName::SimpleName(std::unique_ptr<SimpleName::Impl> impl) noexcept : impl_(std::move(impl)) {}

SimpleName::SimpleName() : SimpleName(std::make_unique<SimpleName::Impl>()) {}

SimpleName::~SimpleName() noexcept = default;

SimpleName::SimpleName(SimpleName&&) noexcept = default;

SimpleName& SimpleName::operator=(SimpleName&&) noexcept = default;

std::string const& SimpleName::token() const {
    return impl_->token_;
}

SimpleName& SimpleName::token(std::string token) {
    impl_->token_ = std::move(token);
    return *this;
}

SimpleName* SimpleName::clone() const & {
    return new SimpleName(impl_->clone());  // NOLINT
}

SimpleName* SimpleName::clone() && {
    return new SimpleName(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::name

