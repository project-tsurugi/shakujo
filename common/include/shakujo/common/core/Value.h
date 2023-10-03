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

#ifndef SHAKUJO_COMMON_CORE_VALUE_H_
#define SHAKUJO_COMMON_CORE_VALUE_H_

#include <iostream>
#include <memory>
#include <string>
#include <string_view>

#include "shakujo/common/util/Clonable.h"
#include "shakujo/common/util/operators.h"

namespace shakujo::common::core {

namespace value {}

/**
 * @brief Provides semantic information of values.
 */
class Value : public util::Clonable, private util::Equivalent<Value> {
public:

    /**
     * @brief represents a value kind.
     */
    enum class Kind {

        /**
         * @brief boolean.
         */
        BOOL,

        /**
         * @brief integers.
         */
        INT,

        /**
         * @brief floating point numbers.
         */
        FLOAT,

        /**
         * @brief string value.
         */
        STRING,

        /**
         * @brief tuple value
         */
        TUPLE,

        /**
         * @brief null value.
         */
        NULL_,

        /**
         * @brief unresolved value placeholder.
         */
        PLACEHOLDER,

        /**
         * @brief erroneous type.
         */
        ERROR,
    };

protected:
    constexpr Value() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    Value(Value const& other) = default;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Value(Value&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Value& operator=(Value const& other) = default;
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Value& operator=(Value&& other) noexcept = default;

public:
    /**
     * @brief Destroys this object.
     */
    ~Value() noexcept override = default;

    /**
     * @brief returns the kind of this value.
     * @return the value kind
     */
    virtual Kind kind() const = 0;

    /**
     * @brief returns whether or not this value is equivalent to the given one.
     * @param other the target value
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(Value const& other) const;

    /**
     * @brief returns whether or not this is a valid value.
     * @return true if this is a valid value
     * @return false if this is or has erroneous value
     */
    inline bool is_valid() const {
        return kind() != Kind::ERROR;
    }

    /**
     * @brief returns a new copy of this object.
     * @return a clone of this
     */
    Value* clone() const & override = 0;

    /**
     * @brief returns a new copy of this object.
     * @return a clone of this
     */
    Value* clone() && override = 0;

    /**
     * @brief Appends the object information into the given output stream.
     * @param out the target output stream
     * @param value the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, Value const& value);
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(Value::Kind value) {
    using Kind = Value::Kind;
    switch (value) {
        case Kind::BOOL: return "BOOL";
        case Kind::INT: return "INT";
        case Kind::FLOAT: return "FLOAT";
        case Kind::STRING: return "STRING";
        case Kind::TUPLE: return "TUPLE";
        case Kind::NULL_: return "NULL";
        case Kind::PLACEHOLDER: return "PLACEHOLDER";
        case Kind::ERROR: return "ERROR";
    }
    std::abort();
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 */
inline std::ostream& operator<<(std::ostream& out, Value::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::common::core

#endif  // SHAKUJO_COMMON_CORE_VALUE_H_
