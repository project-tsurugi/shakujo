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

#ifndef SHAKUJO_COMMON_CORE_NAME_H_
#define SHAKUJO_COMMON_CORE_NAME_H_

#include <functional>
#include <initializer_list>
#include <iostream>
#include <utility>
#include <string>
#include <string_view>
#include <vector>

#include "shakujo/common/util/operators.h"

namespace shakujo::common::core {

/**
 * @brief Provides semantic information of names.
 */
class Name
        : private util::Equivalent<Name>
        , private util::Equivalent2<Name, std::string_view>
        , private util::Comparable<Name>
        , private util::Boolean<Name> {
private:
    std::vector<std::string> segments_;

public:
    /**
     * @brief Constructs a new empty object.
     */
    Name() noexcept = default;

    /**
     * @brief Constructs a new object.
     * @param segments the name segments
     */
    Name(std::initializer_list<std::string> segments) : segments_(segments) {}

    /**
     * @brief Constructs a new object.
     * @param segments the name segments
     */
    explicit Name(std::vector<std::string> segments) : segments_(std::move(segments)) {}

    /**
     * @brief Constructs a new object.
     * @param name the name
     */
    explicit Name(std::string_view name);

    /**
     * @brief Destroys this object.
     */
    ~Name() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Name(Name const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Name(Name&& other) noexcept = default;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Name& operator=(Name const& other) = default;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Name& operator=(Name&& other) = default;

    /**
     * @brief Returns whether or not the given name is equivalent to this name.
     * @param other the target name
     * @return true if the both are equivalent
     * @return false otherwise
     */
    bool operator==(Name const& other) const {
        return segments_ == other.segments_;
    }

    /**
     * @brief Returns whether or not this is equivalent to the given name.
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(std::string_view other) const {
        if (other.empty()) {
            return segments_.empty();
        }
        if (segments_.size() != 1) {
            return false;
        }
        return segments_.front() == other;
    }

    /**
     * @brief returns whether or not this is an empty name.
     * @return true if this is empty
     * @return false otherwise
     */
    bool empty() const {
        return segments_.empty();
    }

    /**
     * @brief Compares two names by dictionary order.
     * @param other the target name
     * @return == 0 if two names are equivalent
     * @return < 0 if this name is less than the given name
     * @return > 0 if this name is greater than the given name
     */
    int compare(Name const& other) const;

    /**
     * @brief returns whether or not this is a valid value.
     * @return true if this is a valid value
     * @return false if this is or has erroneous or undefined value
     */
    explicit operator bool() const {
        return !empty();
    }

    /**
     * @brief returns the segments of this name.
     * @return the name segments
     */
    std::vector<std::string> const& segments() const & {
        return segments_;
    }

    /**
     * @brief returns the segments of this name.
     * @return the name segments
     */
    std::vector<std::string> segments() && {
        return std::move(segments_);
    }

    /**
     * @brief Appends the object information into the given output stream.
     * @param out the target output stream
     * @param value the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, Name const& value);

    /**
     * @brief provides hash code of Name.
     */
    class Hash {
    public:
        /**
         * @brief the hash code type.
         */
        using result_type = std::size_t;

        /**
         * @brief returns the hash code of the given name.
         * @param key the target name
         * @return the corresponded hash code
         */
        std::size_t operator()(Name const& key) const {
            if (!key) return 0U;
            std::hash<std::string> str_hash;
            std::size_t result = 1;
            for (auto& s : key.segments()) {
                result = result * 31U + str_hash(s);
            }
            return result;
        }
    };
};
}  // namespace shakujo::common::core

#endif  // SHAKUJO_COMMON_CORE_NAME_H_
