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
#ifndef SHAKUJO_COMMON_CORE_TYPE_INT_H_
#define SHAKUJO_COMMON_CORE_TYPE_INT_H_

#include <stdexcept>
#include <limits>
#include <cstdint>
#include <utility>

#include "Numeric.h"
#include "../value/Int.h"

namespace shakujo::common::core::type {

/**
 * @brief integer type binding.
 */
class Int final : public Numeric {
public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::INT;

    /**
     * @brief the corresponded value type.
     */
    using value_type = value::Int;

    /**
     * @brief Constructs a new object.
     * @param size the bit width, must be 8, 16, 32, 64
     * @param nullity the nullity
     */
    constexpr explicit Int(std::size_t size, Type::Nullity nullity = Type::Nullity::NULLABLE)
        : Numeric(size, nullity)
    {
        if (size != 8U && size != 16U && size != 32U && size != 64U) {
            throw std::invalid_argument("Int size must be one of 8, 16, 32, 64");
        }
    }

    /**
     * @brief destructs this object.
     */
    ~Int() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Int(Int const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Int(Int&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Int& operator=(Int const& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Int& operator=(Int&& other) noexcept = default;

    Int* clone() const & override {
        return new Int(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Int* clone() && override {
        return new Int(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Type::Kind kind() const override {
        return tag;
    }

    /**
     * @brief the maximum value of this type.
     * @return the maximum value
     */
    constexpr value::Int::type max_value() const {
        switch (size()) {
        case 8U: return std::numeric_limits<int8_t>::max();
        case 16U: return std::numeric_limits<int16_t>::max();
        case 32U: return std::numeric_limits<int32_t>::max();
        case 64U: return std::numeric_limits<int64_t>::max();
        default: throw std::domain_error("unknown size");
        }
    }

    /**
     * @brief the minimum value of this type.
     * @return the minimum value
     */
    constexpr value::Int::type min_value() const {
        switch (size()) {
        case 8U: return std::numeric_limits<int8_t>::min();
        case 16U: return std::numeric_limits<int16_t>::min();
        case 32U: return std::numeric_limits<int32_t>::min();
        case 64U: return std::numeric_limits<int64_t>::min();
        default: throw std::domain_error("unknown size");
        }
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_INT_H_
