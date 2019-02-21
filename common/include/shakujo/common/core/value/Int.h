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
#ifndef SHAKUJO_COMMON_CORE_VALUE_INT_H_
#define SHAKUJO_COMMON_CORE_VALUE_INT_H_

#include <cstdint>
#include <utility>

#include "../Value.h"

namespace shakujo::common::core::value {

/**
 * @brief Int type binding.
 */
class Int final : public Value {
public:
    /**
     * @brief the value type.
     */
    using type = intmax_t;

    /**
     * @brief the kind of this value.
     */
    static inline constexpr auto tag = Kind::INT;

private:
    type value_;

public:
    /**
     * @brief Constructs a new object.
     * @param value the value
     */
    constexpr explicit Int(type value = {}) noexcept : value_(value) {}

    /**
     * @brief Destroys this object.
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

    Kind kind() const override {
        return tag;
    }

    Int* clone() const & override {
        return new Int(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Int* clone() && override {
        return new Int(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the value which this binding represents.
     * @return the value
     */
    constexpr type get() const noexcept {
        return value_;
    }

    /**
     * @brief returns the value which this binding represents.
     * @return the value
     */
    constexpr type operator*() const noexcept {
        return get();
    }
};

}  // namespace shakujo::common::core::value


#endif  // SHAKUJO_COMMON_CORE_VALUE_INT_H_
