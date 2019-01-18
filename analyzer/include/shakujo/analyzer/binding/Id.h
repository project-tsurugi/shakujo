/*
 * Copyright 2018 shakujo project.
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
#ifndef SHAKUJO_ANALYZER_BINDING_ID_H_
#define SHAKUJO_ANALYZER_BINDING_ID_H_

#include <cstddef>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "shakujo/common/util/operators.h"

namespace shakujo::analyzer::binding {

/**
 * @brief Represents an ID of binding.
 * @tparam T the ID kind
 */
template<typename T>
class Id final : private common::util::Equivalent<Id<T>>, common::util::Ordered<Id<T>>, common::util::Boolean<Id<T>> {
public:
    /**
     * @brief ID entity type.
     */
    using type = std::size_t;

    /**
     * @brief the invalid value of ID.
     */
    static constexpr type INVALID_VALUE = 0U;

    /**
     * @brief the minimum value of the ID.
     */
    static constexpr type MIN_VALUE = INVALID_VALUE + 1;

private:
    type value_;

public:
    /**
     * @brief Constructs a new invalid ID object.
     */
    Id() noexcept : value_(INVALID_VALUE) {}

    /**
     * @brief Constructs a new ID object.
     * @param value the ID value.
     * @throws std::invalid_argument if value < MIN_VALUE
     */
    explicit Id(type value) : value_(value) {
        if (value < MIN_VALUE) {
            throw std::invalid_argument("invalid ID value");
        }
    }

    /**
     * @brief destroys this object.
     */
    ~Id() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Id(Id const& other) = delete;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Id(Id&& other) noexcept = default;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Id& operator=(Id const& other) = delete;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Id& operator=(Id&& other) noexcept = default;

    /**
     * @brief returns the ID entity.
     * @return the ID entity.
     * @see INVALID_VALUE
     */
    Id::type get() const {
        return value_;
    }

    /**
     * @brief returns whether or not this is a valid ID.
     * @return true if this is a valid ID
     * @return false otherwise
     */
    explicit operator bool() const {
        return value_ != INVALID_VALUE;
    }

    /**
     * @brief returns whether or not the two IDs are equivalent.
     * @param other the target ID
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(Id const& other) const {
        return get() == other.get();
    }

    /**
     * @brief returns whether this ID is less than the given ID.
     * @param other the target ID
     * @return true if this < other
     * @return false otherwise
     */
    bool operator<(Id const& other) const {
        return get() < other.get();
    }

    /**
     * @brief Appends ID object into the given output stream.
     * @param out the target output stream
     * @param value the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, Id<T> const& value) {
        out << "Id(" << value.value_ << ")";
        return out;
    }

    /**
     * @brief provides hash code of Id.
     */
    class Hash {
    public:
        /**
         * @brief the hash code type.
         */
        using result_type = std::size_t;

        /**
         * @brief returns the hash code of the given ID.
         * @param key the target ID
         * @return the corresponded hash code
         */
        std::size_t operator()(Id<T> const& key) const {
            return key.get();
        }
    };
};
}  // namespace shakujo::analyzer::binding

#endif  //SHAKUJO_ANALYZER_BINDING_ID_H_
