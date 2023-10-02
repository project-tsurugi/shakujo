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
#ifndef SHAKUJO_MODEL_TYPE_TYPE_H_
#define SHAKUJO_MODEL_TYPE_TYPE_H_

#include <iostream>

#include "../Node.h"
#include "TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief An abstract super interface of types in IR model.
 * @see TypeKind
 * @see TypeVisitor
 * @see ConstTypeVisitor
 */
class Type : public Node {
protected:
    /**
     * @brief Constructs a new object.
     */
    Type() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Type(const Type& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Type(Type&& other) noexcept = default;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Type& operator=(const Type& other) = default;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Type& operator=(Type&& other) noexcept = default;

public:
    ~Type() noexcept override = default;

    Type* clone() const & override = 0;

    Type* clone() && override = 0;

    /**
     * @brief Returns the kind of this type.
     * @return the type kind
     */
    virtual TypeKind kind() const = 0;

    /**
     * @brief Returns whether or not this is equivalent to the given type.
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(const Type& other) const;

    /**
     * @brief Returns whether or not this is NOT equivalent to the given type.
     * @return true if both are NOT equivalent
     * @return false otherwise
     */
    inline bool operator!=(const Type& other) const {
        return !operator==(other);
    }

    /**
     * @brief Appends type into the given output stream.
     * @param out the target output stream
     * @param value the target type
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, const Type& value);
};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_TYPE_H_
