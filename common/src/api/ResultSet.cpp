/*
 * Copyright 2018-2018 shakujo project.
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
#include "shakujo/common/api/ResultSet.h"

#include <algorithm>
#include <cmath>
#include <cstring>

#include "shakujo/common/util/utility.h"

namespace shakujo::common::api {

std::string_view ResultSet::get_text(std::size_t position, void *buffer, std::size_t buffer_size) const {
    if (is_null(position)) {
        return {};
    }
    auto borrowed = get_text(position);
    if (borrowed.empty()) {
        return {};
    }
    std::size_t effective_size = std::min(buffer_size, borrowed.size());
    if (effective_size > 0U) {
        std::memcpy(buffer, borrowed.data(), effective_size);
    }
    return { reinterpret_cast<std::string::value_type*>(buffer), effective_size };  // NOLINT
}

void ResultSet::get_text(std::size_t position, std::string &buffer) const {
    auto result = get_text(position);
    if (result.empty()) {
        buffer.clear();
    } else {
        buffer.assign(result);
    }
}

}  // namespace shakujo::common::api

