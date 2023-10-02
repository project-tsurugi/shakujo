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
#include "shakujo/model/type/VarBinaryType.h"

#include <cstddef>


namespace shakujo::model::type {

class VarBinaryType::Impl {
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

VarBinaryType::VarBinaryType(std::unique_ptr<VarBinaryType::Impl> impl) noexcept : impl_(std::move(impl)) {}

VarBinaryType::VarBinaryType() : VarBinaryType(std::make_unique<VarBinaryType::Impl>()) {}

VarBinaryType::~VarBinaryType() noexcept = default;

VarBinaryType::VarBinaryType(VarBinaryType&&) noexcept = default;

VarBinaryType& VarBinaryType::operator=(VarBinaryType&&) noexcept = default;

std::size_t VarBinaryType::size() const {
    return impl_->size_;
}

VarBinaryType& VarBinaryType::size(std::size_t size) {
    impl_->size_ = size;
    return *this;
}

VarBinaryType* VarBinaryType::clone() const & {
    return new VarBinaryType(impl_->clone());  // NOLINT
}

VarBinaryType* VarBinaryType::clone() && {
    return new VarBinaryType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

