/*
 * Copyright 2018-2019 shakujo project.
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
#ifndef SHAKUJO_MODEL_TYPE_TIME_TYPE_H_
#define SHAKUJO_MODEL_TYPE_TIME_TYPE_H_

#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief Represents time of day type.
 */
class TimeType
        : public Type {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit TimeType(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    TimeType();

    /**
     * @brief Destroys this object.
     */
    ~TimeType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    TimeType(TimeType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    TimeType& operator=(TimeType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    TimeType(TimeType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    TimeType& operator=(TimeType&& other) noexcept;

public:
    /**
     * @brief Returns whether or not this considers time zone offset.
     * @return true this considers time zone offset
     * @return false otherwise
     */
    [[nodiscard]] bool has_time_zone() const;

    /**
     * @brief Sets whether or not this considers time zone offset.
     * @param enabled true to consider offset, or false to not
     * @return this
     */
    TimeType& has_time_zone(bool enabled);

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TimeType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TimeType* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = TypeKind::TIME_TYPE;

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

#endif  // SHAKUJO_MODEL_TYPE_TIME_TYPE_H_
