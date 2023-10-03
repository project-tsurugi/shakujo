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
#ifndef SHAKUJO_COMMON_UTIL_MOVE_INITIALIZER_LIST_H_
#define SHAKUJO_COMMON_UTIL_MOVE_INITIALIZER_LIST_H_

#include <memory>
#include <vector>

namespace shakujo::common::util {

/**
 * @brief a variant of initializer_list which can accept move only types.
 * @tparam E the element type
 */
template<class E>
class MoveInitializerList {
private:
    std::vector<E> entity_;

public:
    /**
     * @brief Construct a new empty object.
     */
    MoveInitializerList() = default;

    /**
     * @brief Construct a new empty object.
     * @param elements the elements
     */
    MoveInitializerList(std::vector<E>&& elements)   // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : entity_(std::move(elements))
    {}

    /**
     * @brief Construct a new object.
     * @tparam Args the elements type
     * @param args the elements
     */
    template<class... Args>
    MoveInitializerList(Args&&... args) {  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        entity_.reserve(sizeof...(args));
        append(std::forward<Args>(args)...);
    }

    /**
     * @brief returns whether or not this list is empty.
     * @return true if this is empty
     * @return otherwise
     */
    bool empty() const {
        return entity_.empty();
    }

    /**
     * @brief returns the elements as std::vector.
     * @return the elements
     */
    std::vector<E> build() && {
        return std::move(entity_);
    }

    /**
     * @brief returns the view of entity.
     * @return the entity view
     */
    std::vector<E>& view() {
        return entity_;
    }

private:
    inline void append() {}
    template<typename... Args>
    inline void append(E&& first, Args&&... rest) {
        entity_.emplace_back(std::move(first));
        append(std::forward<Args>(rest)...);
    }
};

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_MOVE_INITIALIZER_LIST_H_
