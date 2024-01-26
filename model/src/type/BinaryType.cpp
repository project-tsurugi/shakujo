/*
 * Copyright 2018-2024 Project Tsurugi.
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
#include "shakujo/model/type/BinaryType.h"

#include <cstddef>


namespace shakujo::model::type {

class BinaryType::Impl {
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

BinaryType::BinaryType(std::unique_ptr<BinaryType::Impl> impl) noexcept : impl_(std::move(impl)) {}

BinaryType::BinaryType() : BinaryType(std::make_unique<BinaryType::Impl>()) {}

BinaryType::~BinaryType() noexcept = default;

BinaryType::BinaryType(BinaryType&&) noexcept = default;

BinaryType& BinaryType::operator=(BinaryType&&) noexcept = default;

std::size_t BinaryType::size() const {
    return impl_->size_;
}

BinaryType& BinaryType::size(std::size_t size) {
    impl_->size_ = size;
    return *this;
}

BinaryType* BinaryType::clone() const & {
    return new BinaryType(impl_->clone());  // NOLINT
}

BinaryType* BinaryType::clone() && {
    return new BinaryType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

