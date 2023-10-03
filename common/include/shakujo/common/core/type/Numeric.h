/*
 * Copyright 2018-2023 Project Tsurugi.
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
#ifndef SHAKUJO_COMMON_CORE_TYPE_NUMERIC_H_
#define SHAKUJO_COMMON_CORE_TYPE_NUMERIC_H_

#include "../Type.h"

namespace shakujo::common::core::type {

/**
 * @brief numric types.
 */
class Numeric : public Type {
private:
    std::size_t size_;
    Type::Nullity nullity_;

protected:
    /**
     * @brief Constructs a new object.
     * @param size the bit width
     * @param nullity the nullity
     */
    explicit constexpr Numeric(std::size_t size, Type::Nullity nullity) noexcept
        : size_(size), nullity_(nullity)
    {}

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Numeric(Numeric const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Numeric(Numeric&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Numeric& operator=(Numeric const& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Numeric& operator=(Numeric&& other) noexcept = default;

public:
    /**
     * @brief destroys this object.
     */
    ~Numeric() noexcept override = default;

    Numeric* clone() const & override = 0;

    Numeric* clone() && override = 0;

    Nullity nullity() const final {
        return nullity_;
    }

    /**
     * @brief returns the bit width of this type.
     * @return the bit width
     */
    constexpr std::size_t size() const {
        return size_;
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_NUMERIC_H_
