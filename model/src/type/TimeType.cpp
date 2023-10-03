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
#include "shakujo/model/type/TimeType.h"

#include <cstddef>


namespace shakujo::model::type {

class TimeType::Impl {
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

TimeType::TimeType(std::unique_ptr<TimeType::Impl> impl) noexcept : impl_(std::move(impl)) {}

TimeType::TimeType() : TimeType(std::make_unique<TimeType::Impl>()) {}

TimeType::~TimeType() noexcept = default;

TimeType::TimeType(TimeType&&) noexcept = default;

TimeType& TimeType::operator=(TimeType&&) noexcept = default;

bool TimeType::has_time_zone() const {
    return impl_->has_time_zone_;
}

TimeType& TimeType::has_time_zone(bool enabled) {
    impl_->has_time_zone_ = enabled;
    return *this;
}

TimeType* TimeType::clone() const & {
    return new TimeType(impl_->clone());  // NOLINT
}

TimeType* TimeType::clone() && {
    return new TimeType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

