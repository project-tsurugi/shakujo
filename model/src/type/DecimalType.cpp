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
#include "shakujo/model/type/DecimalType.h"

#include <cstddef>


namespace shakujo::model::type {

class DecimalType::Impl {
public:
    std::optional<std::size_t> precision_ { std::nullopt };
    std::optional<std::size_t> scale_ { std::nullopt };

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->precision_ = precision_;
        other->scale_ = scale_;
        return other;
    }
};

DecimalType::DecimalType(std::unique_ptr<DecimalType::Impl> impl) noexcept : impl_(std::move(impl)) {}

DecimalType::DecimalType() : DecimalType(std::make_unique<DecimalType::Impl>()) {}

DecimalType::~DecimalType() noexcept = default;

DecimalType::DecimalType(DecimalType&&) noexcept = default;

DecimalType& DecimalType::operator=(DecimalType&&) noexcept = default;


std::optional<std::size_t> DecimalType::precision() const {
    return impl_->precision_;
}

DecimalType& DecimalType::precision(std::optional<std::size_t> value) {
    impl_->precision_ = value;
    return *this;
}

std::optional<std::size_t> DecimalType::scale() const {
    return impl_->scale_;
}

DecimalType& DecimalType::scale(std::optional<std::size_t> value) {
    impl_->scale_ = value;
    return *this;
}

DecimalType* DecimalType::clone() const & {
    return new DecimalType(impl_->clone());  // NOLINT
}

DecimalType* DecimalType::clone() && {
    return new DecimalType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

