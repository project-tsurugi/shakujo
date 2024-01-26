/*
 * Copyright 2018-2024 Project Tsurugi.
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
#ifndef SHAKUJO_COMMON_CORE_VALUE_PLACEHOLDER_H_
#define SHAKUJO_COMMON_CORE_VALUE_PLACEHOLDER_H_

#include <string>
#include <string_view>
#include <utility>

#include "../Value.h"

namespace shakujo::common::core::value {

/**
 * @brief an unresolved value placeholder.
 */
class Placeholder final : public Value {
public:
    /**
     * @brief the value type.
     */
    using type = std::string;

    /**
     * @brief the kind of this value.
     */
    static inline constexpr auto tag = Kind::PLACEHOLDER;

private:
    type value_;

public:
    /**
     * @brief Constructs a new object.
     * @param value the value
     */
    explicit Placeholder(std::string_view value) : value_(value) {}

    /**
     * @brief Destroys this object.
     */
    ~Placeholder() noexcept override = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    Placeholder(Placeholder const& other) = default;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Placeholder(Placeholder&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Placeholder& operator=(Placeholder const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Placeholder& operator=(Placeholder&& other) noexcept = default;

    Kind kind() const override {
        return tag;
    }

    Placeholder* clone() const & override {
        return new Placeholder(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Placeholder* clone() && override {
        return new Placeholder(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the value expression of this placeholder.
     * @return the value
     */
    type const& get() const noexcept {
        return value_;
    }

    /**
     * @brief returns the value expression of this placeholder.
     * @return the value
     */
    type const& operator*() const noexcept {
        return get();
    }
};

}  // namespace shakujo::common::core::value


#endif  // SHAKUJO_COMMON_CORE_VALUE_PLACEHOLDER_H_
