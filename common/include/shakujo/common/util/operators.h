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
#ifndef SHAKUJO_COMMON_UTIL_OPERATORS_H_
#define SHAKUJO_COMMON_UTIL_OPERATORS_H_

#include <type_traits>

namespace shakujo::common::util {

/**
 * @brief defines equivalent operators from T == T.
 * @tparam T the target type
 */
template<typename T>
class Equivalent {
public:
    /**
     * @brief returns whether or not the first element is not equivalent to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if the both are not equivalent
     * @return false otherwise
     */
    friend inline bool operator!=(T const & a, T const & b) {
        return !static_cast<bool>(a == b);
    }
};

/**
 * @brief defines equivalent operators from T == U.
 * @tparam T the first type
 * @tparam U the second type
 */
template<typename T, typename U>
class Equivalent2 {
    static_assert(!std::is_same_v<T, U>, "T must not be as same as U");

public:
    /**
     * @brief returns whether or not the first element is not equivalent to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if the both are not equivalent
     * @return false otherwise
     */
    friend inline bool operator==(U const & a, T const & b) {
        return static_cast<bool>(b == a);
    }

    /**
     * @brief returns whether or not the first element is not equivalent to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if the both are not equivalent
     * @return false otherwise
     */
    friend inline bool operator!=(T const & a, U const & b) {
        return !static_cast<bool>(a == b);
    }

    /**
     * @brief returns whether or not the first element is not equivalent to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if the both are not equivalent
     * @return false otherwise
     */
    friend inline bool operator!=(U const & a, T const & b) {
        return !static_cast<bool>(b == a);
    }
};

/**
 * @brief defines boolean operators from boolean cast operator.
 * @tparam T the object type
 */
template<typename T>
class Boolean {
public:
    /**
     * @brief returns whether or not this object is evaluated as false.
     * @param value the target element
     * @return true if the object is evaluated as false
     * @return false otherwise
     */
    friend inline bool operator!(T const & value) {
        return !static_cast<bool>(value);
    }
};

/**
 * @brief defines relation operators from T < T.
 * @tparam T the target type
 */
template<typename T>
class Ordered {
public:
    /**
     * @brief returns whether or not the first element is larger than the second one.
     * @param a the first element
     * @param b the second element
     * @return true if a > b
     * @return otherwise
     */
    friend inline bool operator>(T const & a, T const & b) {
        return b < a;
    }

    /**
     * @brief returns whether or not the first element is smaller than or equal to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if a <= b
     * @return otherwise
     */
    friend inline bool operator<=(T const & a, T const & b) {
        return !static_cast<bool>(a > b);
    }

    /**
     * @brief returns whether or not the first element is greater than or equal to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if a >= b
     * @return otherwise
     */
    friend inline bool operator>=(T const & a, T const & b) {
        return !static_cast<bool>(a < b);
    }
};

/**
 * @brief defines relation operators from T.compare(T).
 * @tparam T the target type
 */
template<typename T>
class Comparable {
public:
    /**
     * @brief returns whether or not the first element is smaller than the second one.
     * @param a the first element
     * @param b the second element
     * @return true if a < b
     * @return otherwise
     */
    friend inline bool operator<(T const & a, T const & b) {
        return a.compare(b) < 0;
    }

    /**
     * @brief returns whether or not the first element is larger than the second one.
     * @param a the first element
     * @param b the second element
     * @return true if a > b
     * @return otherwise
     */
    friend inline bool operator>(T const & a, T const & b) {
        return a.compare(b) > 0;
    }

    /**
     * @brief returns whether or not the first element is smaller than or equal to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if a <= b
     * @return otherwise
     */
    friend inline bool operator<=(T const & a, T const & b) {
        return a.compare(b) <= 0;
    }

    /**
     * @brief returns whether or not the first element is greater than or equal to the second one.
     * @param a the first element
     * @param b the second element
     * @return true if a >= b
     * @return otherwise
     */
    friend inline bool operator>=(T const & a, T const & b) {
        return a.compare(b) >= 0;
    }
};

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_OPERATORS_H_
