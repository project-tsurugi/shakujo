/*
 * Copyright 2018-2023 tsurugi project..
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
#ifndef SHAKUJO_COMMON_CORE_TYPE_NULL_H_
#define SHAKUJO_COMMON_CORE_TYPE_NULL_H_

#include <utility>

#include "../Type.h"
#include "../value/Null.h"

namespace shakujo::common::core::type {

/**
 * @brief null type binding.
 */
class Null final : public Type {
public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::NULL_;

    /**
     * @brief the corresponded value type.
     */
    using value_type = value::Null;

    /**
     * @brief Constructs a new object.
     */
    constexpr Null() noexcept = default;

    /**
     * @brief destructs this object.
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

    Type::Kind kind() const override {
        return tag;
    }

    Nullity nullity() const override {
        return Nullity::NULLABLE;
    }

    Null* clone() const & override {
        return new Null(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Null* clone() && override {
        return new Null(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_NULL_H_