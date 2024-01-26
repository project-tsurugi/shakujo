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
#ifndef SHAKUJO_COMMON_UTIL_TERNARY_H_
#define SHAKUJO_COMMON_UTIL_TERNARY_H_

#include <iostream>
#include <string>
#include <string_view>

namespace shakujo::common::util {

/**
 * @brief represents an element of three-valued logic.
 */
enum class Ternary {

    /**
     * @brief true value.
     */
    TRUE,

    /**
     * @brief false value.
     */
    FALSE,

    /**
     * @brief unknown value.
     */
    UNKNOWN,
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(Ternary value) {
    switch (value) {
        case Ternary::TRUE: return "TRUE";
        case Ternary::FALSE: return "FALSE";
        case Ternary::UNKNOWN: return "UNKNOWN";
    }
    return "(unknown)";
}

/**
 * @brief append textual representation of the given value.
 * @param out the target output stream
 * @param value the target value
 * @return the output stream
 */
inline std::ostream& operator<<(std::ostream& out, Ternary value) {
    return out << to_string_view(value);
}

/**
 * @brief returns ternary logical NOT of the given value.
 * @param value the target value
 * @return the result
 */
inline constexpr Ternary operator~(Ternary value) {
    switch (value) {
        case Ternary::TRUE: return Ternary::FALSE;
        case Ternary::FALSE: return Ternary::TRUE;
        case Ternary::UNKNOWN: return Ternary::UNKNOWN;
    }
    return Ternary::UNKNOWN;
}

/**
 * @brief returns ternary logical AND of the given values.
 * @param a the first value
 * @param b the second value
 * @return the result
 */
inline constexpr Ternary operator&(Ternary a, Ternary b) {
    if (a == Ternary::FALSE || b == Ternary::FALSE) {
        return Ternary::FALSE;
    }
    if (a == Ternary::TRUE && b == Ternary::TRUE) {
        return Ternary::TRUE;
    }
    return Ternary::UNKNOWN;
}

/**
 * @brief returns ternary logical OR of the given values.
 * @param a the first value
 * @param b the second value
 * @return the result
 */
inline constexpr Ternary operator|(Ternary a, Ternary b) {
    if (a == Ternary::TRUE || b == Ternary::TRUE) {
        return Ternary::TRUE;
    }
    if (a == Ternary::FALSE && b == Ternary::FALSE) {
        return Ternary::FALSE;
    }
    return Ternary::UNKNOWN;
}

/**
 * @brief returns whether or not the two values are equivalent.
 * @param a the first value
 * @param b the second value
 * @return true if the both are equivalent
 * @return false otherwise
 */
inline constexpr bool operator==(Ternary a, bool b) {
    if (b) {
        return a == Ternary::TRUE;
    }
    return a == Ternary::FALSE;
}

/**
 * @brief returns whether or not the two values are different.
 * @param a the first value
 * @param b the second value
 * @return true if the both are different
 * @return false otherwise
 */
inline constexpr bool operator!=(Ternary a, bool b) {
    return !operator==(a, b);
}

/**
 * @brief returns whether or not the two values are equivalent.
 * @param a the first value
 * @param b the second value
 * @return true if the both are equivalent
 * @return false otherwise
 */
inline constexpr bool operator==(bool a, Ternary b) {
    return operator==(b, a);
}

/**
 * @brief returns whether or not the two values are different.
 * @param a the first value
 * @param b the second value
 * @return true if the both are different
 * @return false otherwise
 */
inline constexpr bool operator!=(bool a, Ternary b) {
    return operator!=(b, a);
}

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_TERNARY_H_
