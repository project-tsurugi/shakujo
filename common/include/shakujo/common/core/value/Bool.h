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
#ifndef SHAKUJO_COMMON_CORE_VALUE_BOOL_H_
#define SHAKUJO_COMMON_CORE_VALUE_BOOL_H_

#include <utility>

#include "../Value.h"

namespace shakujo::common::core::value {

/**
 * @brief Bool type binding.
 */
class Bool final : public Value {
public:
    /**
     * @brief the value type.
     */
    using type = bool;

    /**
     * @brief the kind of this value.
     */
    static inline constexpr auto tag = Kind::BOOL;

private:
    type value_;

public:
    /**
     * @brief Constructs a new object.
     * @param value the value
     */
    constexpr explicit Bool(type value = {}) noexcept : value_(value) {}

    /**
     * @brief Destroys this object.
     */
    ~Bool() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Bool(Bool const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Bool(Bool&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Bool& operator=(Bool const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Bool& operator=(Bool&& other) noexcept = default;

    Kind kind() const override {
        return tag;
    }

    Bool* clone() const & override {
        return new Bool(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Bool* clone() && override {
        return new Bool(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
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


#endif  // SHAKUJO_COMMON_CORE_VALUE_BOOL_H_
