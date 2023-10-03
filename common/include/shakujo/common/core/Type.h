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

#ifndef SHAKUJO_COMMON_CORE_TYPE_H_
#define SHAKUJO_COMMON_CORE_TYPE_H_

#include <iostream>
#include <memory>
#include <string>
#include <string_view>

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
     * @param test_nullity test with their nullity
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool equals(Type const& other, bool test_nullity = true) const;

    /**
     * @brief returns whether or not this type is equivalent to the given one.
     * @param other the target type
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(Type const& other) const {
        return equals(other);
    }

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
     * @brief returns whether or not this is a nullable type.
     * @return true if this is a nullable type
     * @return false otherwise
     */
    bool nullable() const {
        return nullity() == Nullity::NULLABLE;
    }

    /**
     * @brief Appends the object information into the given output stream.
     * @param out the target output stream
     * @param value the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, Type const & value);
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(Type::Kind value) {
    using Kind = Type::Kind;
    switch (value) {
        case Kind::INT: return "INT";
        case Kind::FLOAT: return "FLOAT";
        case Kind::CHAR: return "CHAR";
        case Kind::STRING: return "STRING";
        case Kind::BOOL: return "BOOL";
        case Kind::NULL_: return "NULL";
        case Kind::TUPLE: return "TUPLE";
        case Kind::ARRAY: return "ARRAY";
        case Kind::VECTOR: return "VECTOR";
        case Kind::RELATION: return "RELATION";
        case Kind::CURSOR: return "CURSOR";
        case Kind::ERROR: return "ERROR";
    }
    std::abort();
}

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(Type::Nullity value) {
    using Kind = Type::Nullity;
    switch (value) {
        case Kind::NULLABLE: return "NULLABLE";
        case Kind::NEVER_NULL: return "NEVER_NULL";
    }
    std::abort();
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see Type::Nullity
 */
inline std::ostream& operator<<(std::ostream& out, Type::Kind value) {
    return out << to_string_view(value);
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see Type::Nullity
 */
inline std::ostream& operator<<(std::ostream& out, Type::Nullity value) {
    return out << to_string_view(value);
}

/**
 * @brief merges two nullity.
 * @param a the first nullity
 * @param b the second nullity
 * @return Type::Nullity::NULLABLE if either one is Type::Nullity::NULLABLE
 * @return Type::Nullity::NEVER_NULL otherwise
 */
inline constexpr Type::Nullity operator|(Type::Nullity a, Type::Nullity b) {
    return static_cast<Type::Nullity>(static_cast<bool>(a) || static_cast<bool>(b));
}

/**
 * @brief merges two nullity.
 * @param a the first nullity
 * @param b the second nullity
 * @return Type::Nullity::NULLABLE if both are Type::Nullity::NULLABLE
 * @return Type::Nullity::NEVER_NULL otherwise
 */
inline Type::Nullity operator&(Type::Nullity a, Type::Nullity b) {
    return static_cast<Type::Nullity>(static_cast<bool>(a) && static_cast<bool>(b));
}

/**
 * @brief returns the flipped nullity.
 * @param nullity the target nullity
 * @return Type::Nullity::NULLABLE if it is NEVER_NULL
 * @return Type::Nullity::NEVER_NULL otherwise
 */
inline Type::Nullity operator~(Type::Nullity nullity) {
    return static_cast<Type::Nullity>(!static_cast<bool>(nullity));
}

}  // namespace shakujo::common::core

#endif  // SHAKUJO_COMMON_CORE_TYPE_H_
