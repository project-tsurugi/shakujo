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
#ifndef SHAKUJO_COMMON_CORE_VALUE_STRING_H_
#define SHAKUJO_COMMON_CORE_VALUE_STRING_H_

#include <string>
#include <string_view>
#include <utility>

#include "../Value.h"

namespace shakujo::common::core::value {

/**
 * @brief String type binding.
 */
class String final : public Value {
public:
    /**
     * @brief the value type.
     */
    using type = std::string;

private:
    type value_;

public:
    /**
     * @brief Constructs a new object.
     * @param value the value
     */
    explicit String(std::string_view value = {}) : value_(value) {}

    /**
     * @brief Destroys this object.
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

    Value::Kind kind() const override {
        return Value::Kind::STRING;
    }

    String* clone() const & override {
        return new String(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    String* clone() && override {
        return new String(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the value which this binding represents.
     * @return the value
     */
    type const& get() const noexcept {
        return value_;
    }

    /**
     * @brief returns the value which this binding represents.
     * @return the value
     */
    type const& operator*() const noexcept {
        return get();
    }
};

}  // namespace shakujo::common::core::value


#endif  // SHAKUJO_COMMON_CORE_VALUE_STRING_H_
