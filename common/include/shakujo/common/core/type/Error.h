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
#ifndef SHAKUJO_COMMON_CORE_TYPE_ERROR_H_
#define SHAKUJO_COMMON_CORE_TYPE_ERROR_H_

#include <utility>

#include "../Type.h"

namespace shakujo::common::core::type {

/**
 * @brief erroneous type binding.
 */
class Error final : public Type {
public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::ERROR;

    /**
     * @brief Constructs a new object.
     */
    constexpr Error() noexcept = default;

    /**
     * @brief destructs this object.
     */
    ~Error() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Error(Error const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Error(Error&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Error& operator=(Error const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Error& operator=(Error&& other) noexcept = default;

    Type::Kind kind() const override {
        return tag;
    }

    Nullity nullity() const override {
        return Nullity::NULLABLE;
    }

    Error* clone() const & override {
        return new Error(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Error* clone() && override {
        return new Error(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_ERROR_H_
