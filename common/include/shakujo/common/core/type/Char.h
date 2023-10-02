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
#ifndef SHAKUJO_COMMON_CORE_TYPE_CHAR_H_
#define SHAKUJO_COMMON_CORE_TYPE_CHAR_H_

#include <utility>

#include "Textual.h"
#include "../value/String.h"

namespace shakujo::common::core::type {

/**
 * @brief fixed length character sequence type binding.
 */
class Char final : public Textual {
private:
    bool varying_;
    std::size_t size_;

public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::CHAR;

    /**
     * @brief the corresponded value type.
     */
    using value_type = value::String;

    /**
     * @brief Constructs a new object.
     * @param varying true to varying length character
     * @param size the size in bytes
     * @param nullity the nullity
     */
    constexpr Char(bool varying, std::size_t size, Type::Nullity nullity = Type::Nullity::NULLABLE) noexcept
        : Textual(nullity), varying_(varying), size_(size)
    {}

    /**
     * @brief Constructs a new object.
     * @param size the size in bytes
     * @param nullity the nullity
     */
    constexpr explicit Char(std::size_t size, Type::Nullity nullity = Type::Nullity::NULLABLE) noexcept
        : Char(false, size, nullity)
    {}

    /**
     * @brief destructs this object.
     */
    ~Char() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Char(Char const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Char(Char&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Char& operator=(Char const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Char& operator=(Char&& other) noexcept = default;

    Char* clone() const & override {
        return new Char(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Char* clone() && override {
        return new Char(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Type::Kind kind() const override {
        return tag;
    }

    /**
     * @brief returns whether or not the character length is variant.
     * @return true if it is variant length (a.k.a. VARCHAR)
     * @return false otherwise (a.k.a. CHAR)
     */
    constexpr bool variant() const {
        return varying_;
    }

    /**
     * @brief returns whether or not the character length is varying.
     * @return true if it is varying length (a.k.a. VARCHAR)
     * @return false otherwise (a.k.a. CHAR)
     */
    constexpr bool varying() const {
        return varying_;
    }

    /**
     * @brief returns the byte length of this type.
     * @return the byte length
     */
    constexpr std::size_t size() const {
        return size_;
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_CHAR_H_
