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
#ifndef SHAKUJO_COMMON_CORE_TYPE_FLOAT_H_
#define SHAKUJO_COMMON_CORE_TYPE_FLOAT_H_

#include <stdexcept>
#include <utility>

#include "Numeric.h"
#include "../value/Float.h"

namespace shakujo::common::core::type {

/**
 * @brief floatint point number type binding.
 */
class Float final : public Numeric {
public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::FLOAT;

    /**
     * @brief the corresponded value type.
     */
    using value_type = value::Float;

    /**
     * @brief Constructs a new object.
     * @param size the bit width, must be 32 or 64
     * @param nullity the nullity
     */
    constexpr explicit Float(std::size_t size, Type::Nullity nullity = Type::Nullity::NULLABLE)
        : Numeric(size, nullity)
    {
        if (size != 32U && size != 64U) {
            throw std::invalid_argument("Float size must be either 32 or 64");
        }
    }

    /**
     * @brief destructs this object.
     */
    ~Float() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Float(Float const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Float(Float&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Float& operator=(Float const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Float& operator=(Float&& other) noexcept = default;

    Float* clone() const & override {
        return new Float(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Float* clone() && override {
        return new Float(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Type::Kind kind() const override {
        return tag;
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_FLOAT_H_
