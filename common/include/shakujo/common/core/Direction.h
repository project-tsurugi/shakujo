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

#ifndef SHAKUJO_COMMON_CORE_DIRECTION_H_
#define SHAKUJO_COMMON_CORE_DIRECTION_H_

#include <iostream>
#include <string>
#include <string_view>

namespace shakujo::common::core {

/**
 * @brief represents sort direction of elements.
 */
enum class Direction {

    /**
     * @brief ascendant order.
     */
    ASCENDANT,

    /**
     * @brief descendant order.
     */
    DESCENDANT,
};

/**
 * @brief returns the string representation of Direction.
 * @param value the enum value
 * @return the corresponded string representation
 */
constexpr std::string_view to_string_view(Direction value) {
    switch (value) {
        case Direction::ASCENDANT: return "ASCENDANT";
        case Direction::DESCENDANT: return "DESCENDANT";
        default: return "UNDEFINED";
    }
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 */
inline std::ostream& operator<<(std::ostream& out, Direction value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::common::core

#endif  // SHAKUJO_COMMON_CORE_DIRECTION_H_
