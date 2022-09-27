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
#include "shakujo/model/type/TimestampType.h"

#include <cstddef>


namespace shakujo::model::type {

class TimestampType::Impl {
public:
    bool has_time_zone_ {};

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->has_time_zone_ = has_time_zone_;
        return other;
    }
};

TimestampType::TimestampType(std::unique_ptr<TimestampType::Impl> impl) noexcept : impl_(std::move(impl)) {}

TimestampType::TimestampType() : TimestampType(std::make_unique<TimestampType::Impl>()) {}

TimestampType::~TimestampType() noexcept = default;

TimestampType::TimestampType(TimestampType&&) noexcept = default;

TimestampType& TimestampType::operator=(TimestampType&&) noexcept = default;

bool TimestampType::has_time_zone() const {
    return impl_->has_time_zone_;
}

TimestampType& TimestampType::has_time_zone(bool enabled) {
    impl_->has_time_zone_ = enabled;
    return *this;
}

TimestampType* TimestampType::clone() const & {
    return new TimestampType(impl_->clone());  // NOLINT
}

TimestampType* TimestampType::clone() && {
    return new TimestampType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

