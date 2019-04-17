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
#ifndef SHAKUJO_MODEL_TYPE_STRING_TYPE_H_
#define SHAKUJO_MODEL_TYPE_STRING_TYPE_H_

#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief Represents flexible length character sequence type.
 */
class StringType
        : public Type {
public:
    /**
     * @brief Constructs a new empty object.
     */
    StringType() noexcept;

    /**
     * @brief Destroys this object.
     */
    ~StringType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    StringType(StringType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    StringType& operator=(StringType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    StringType(StringType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    StringType& operator=(StringType&& other) noexcept;

public:
    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    StringType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    StringType* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = TypeKind::STRING_TYPE;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */TypeKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_STRING_TYPE_H_
