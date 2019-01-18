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
#ifndef SHAKUJO_COMMON_CORE_VALUE_NULL_H_
#define SHAKUJO_COMMON_CORE_VALUE_NULL_H_

#include <cstddef>
#include <utility>

#include "../Value.h"

namespace shakujo::common::core::value {

/**
 * @brief Null type binding.
 */
class Null final : public Value {
public:
    /**
     * @brief the value type.
     */
    using type = std::nullptr_t;

    /**
     * @brief Constructs a new object.
     */
    constexpr explicit Null(type = {}) noexcept {};  // NOLINT

    /**
     * @brief Destroys this object.
     */
    ~Null() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Null(Null const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Null(Null&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Null& operator=(Null const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Null& operator=(Null&& other) noexcept = default;

    Value::Kind kind() const override {
        return Value::Kind::NULL_;
    }

    Null* clone() const & override {
        return new Null(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Null* clone() && override {
        return new Null(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }
};

}  // namespace shakujo::common::core::value


#endif  // SHAKUJO_COMMON_CORE_VALUE_NULL_H_
