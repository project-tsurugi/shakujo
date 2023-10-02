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
#include "shakujo/model/type/VarCharType.h"

#include <cstddef>


namespace shakujo::model::type {

class VarCharType::Impl {
public:
    std::size_t size_ { static_cast<std::size_t>(-1) };

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->size_ = size_;
        return other;
    }
};

VarCharType::VarCharType(std::unique_ptr<VarCharType::Impl> impl) noexcept : impl_(std::move(impl)) {}

VarCharType::VarCharType() : VarCharType(std::make_unique<VarCharType::Impl>()) {}

VarCharType::~VarCharType() noexcept = default;

VarCharType::VarCharType(VarCharType&&) noexcept = default;

VarCharType& VarCharType::operator=(VarCharType&&) noexcept = default;

std::size_t VarCharType::size() const {
    return impl_->size_;
}

VarCharType& VarCharType::size(std::size_t size) {
    impl_->size_ = size;
    return *this;
}

VarCharType* VarCharType::clone() const & {
    return new VarCharType(impl_->clone());  // NOLINT
}

VarCharType* VarCharType::clone() && {
    return new VarCharType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

