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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_TYPE_DECIMAL_TYPE_H_
#define SHAKUJO_MODEL_TYPE_DECIMAL_TYPE_H_

#include <cstddef>
#include <optional>
#include <utility>
#include <memory>

#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief Represents decimal number type.
 */
class DecimalType
        : public Type {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit DecimalType(std::unique_ptr<Impl>) noexcept;

public:
    /// @brief represents don't care values.
    static constexpr std::size_t dont_care = static_cast<std::size_t>(-1);

    /**
     * @brief Constructs a new empty object.
     */
    DecimalType();

    /**
     * @brief Destroys this object.
     */
    ~DecimalType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    DecimalType(DecimalType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    DecimalType& operator=(DecimalType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    DecimalType(DecimalType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    DecimalType& operator=(DecimalType&& other) noexcept;

public:

    /**
     * @brief returns the max number of decimal digits in integral and fractional part.
     * @return the max number of digits
     * @return dont_care if the number of digits are defaults
     * @return empty if it is not defined
     */
    [[nodiscard]] std::optional<std::size_t> precision() const;

    /**
     * @brief Sets the precision.
     * @param value the precision value
     * @return this
     */
    DecimalType& precision(std::optional<std::size_t> value);

    /**
     * @brief returns the number of digits in the fractional part.
     * @return the number of digits in the fractional part
     * @return dont_care if the fractional part is flexible
     * @return empty if it is not defined
     */
    [[nodiscard]] std::optional<std::size_t> scale() const;

    /**
     * @brief Sets the scale.
     * @param value the scale value
     * @return this
     */
    DecimalType& scale(std::optional<std::size_t> value);

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DecimalType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DecimalType* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = TypeKind::DECIMAL_TYPE;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */
    TypeKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_DECIMAL_TYPE_H_
