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
#ifndef SHAKUJO_COMMON_CORE_TYPE_STRING_H_
#define SHAKUJO_COMMON_CORE_TYPE_STRING_H_

#include <utility>

#include "Textual.h"
#include "../value/String.h"

namespace shakujo::common::core::type {

/**
 * @brief variable length character sequence type binding.
 */
class String final : public Textual {
public:
    /**
     * @brief the corresponded value type.
     */
    using value_type = value::String;

    /**
     * @brief Constructs a new object.
     * @param nullity the nullity
     */
    constexpr explicit String(Type::Nullity nullity = Type::Nullity::NULLABLE) noexcept : Textual(nullity) {}

    /**
     * @brief destructs this object.
     */
    ~String() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    String(String const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    String(String&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    String& operator=(String const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    String& operator=(String&& other) noexcept = default;

    String* clone() const & override {
        return new String(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    String* clone() && override {
        return new String(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Type::Kind kind() const override {
        return Type::Kind::STRING;
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_STRING_H_
