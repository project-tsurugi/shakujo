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

#ifndef SHAKUJO_COMMON_CORE_TYPE_H_
#define SHAKUJO_COMMON_CORE_TYPE_H_

#include <iostream>
#include <memory>

#include "shakujo/common/util/Clonable.h"
#include "shakujo/common/util/operators.h"

namespace shakujo::common::core {

namespace type {}

/**
 * @brief Provides semantic information of types.
 */
class Type : public util::Clonable, private util::Equivalent<Type> {
public:

    /**
     * @brief represents a type kind.
     */
    enum class Kind {

        /**
         * @brief integral number type.
         */
        INT,

        /**
         * @brief floating point number type.
         */
        FLOAT,

        /**
         * @brief fixed length character sequence type.
         */
        CHAR,

        /**
         * @brief flexible length character string type.
         */
        STRING,

        /**
         * @brief boolean type.
         */
        BOOL,

        /**
         * @brief NULL type.
         */
        NULL_,

        /**
         * @brief tuple type.
         */
        TUPLE,

        /**
         * @brief fixed length array type.
         */
        ARRAY,

        /**
         * @brief flexible length vector type.
         */
        VECTOR,

        /**
         * @brief relation type.
         */
        RELATION,

        /**
         * @brief cursor type.
         */
        CURSOR,

        /**
         * @brief erroneous type.
         */
        ERROR,
    };

    /**
     * @brief represents nullity of the type.
     */
    enum class Nullity : bool {

        /**
         * @brief the value is never null.
         */
        NEVER_NULL = false,

        /**
         * @brief the value may be or must be null.
         */
        NULLABLE = true,
    };

protected:
    /**
     * @brief Constructs a new object.
     */
    constexpr Type() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Type(Type const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Type(Type&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Type& operator=(Type const& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Type& operator=(Type&& other) noexcept = default;

public:
    /**
     * @brief Destroys this object.
     */
    ~Type() noexcept override = default;

    /**
     * @brief returns a new copy of this object.
     * @return a clone of this
     */
    Type* clone() const & override = 0;

    /**
     * @brief returns a new copy of this object.
     * @return a clone of this
     */
    Type* clone() && override = 0;

    /**
     * @brief returns the kind of this type.
     * @return the type kind
     */
    virtual Kind kind() const = 0;

    /**
     * @brief returns whether or not this type is equivalent to the given one.
     * @param other the target type
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(const Type& other) const;

    /**
     * @brief returns whether or not this is a valid type.
     * @return true if this is a valid type
     * @return false if this is or has erroneous type
     */
    virtual bool is_valid() const {
        return kind() != Kind::ERROR;
    }

    /**
     * @brief Returns nullity of this type.
     * @return nullity of this type
     */
    virtual Nullity nullity() const = 0;

    /**
     * @brief Appends the object information into the given output stream.
     * @param out the target output stream
     * @param value the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, Type const & value);
};

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see Type::Nullity
 */
std::ostream& operator<<(std::ostream& out, Type::Kind value);

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see Type::Nullity
 */
std::ostream& operator<<(std::ostream& out, Type::Nullity value);

/**
 * @brief merges two nullity.
 * @param a the first nullity
 * @param b the second nullity
 * @return Type::Nullity::NULLABLE if either one is Type::Nullity::NULLABLE
 * @return Type::Nullity::NEVER_NULL otherwise
 */
Type::Nullity operator|(Type::Nullity a, Type::Nullity b);

/**
 * @brief merges two nullity.
 * @param a the first nullity
 * @param b the second nullity
 * @return Type::Nullity::NULLABLE if both are Type::Nullity::NULLABLE
 * @return Type::Nullity::NEVER_NULL otherwise
 */
Type::Nullity operator&(Type::Nullity a, Type::Nullity b);

/**
 * @brief returns the flipped nullity.
 * @param nullity the target nullity
 * @return Type::Nullity::NULLABLE if it is NEVER_NULL
 * @return Type::Nullity::NEVER_NULL otherwise
 */
Type::Nullity operator~(Type::Nullity nullity);

}  // namespace shakujo::common::core

#endif  // SHAKUJO_COMMON_CORE_TYPE_H_
