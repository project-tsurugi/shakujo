/*
 * Copyright 2018 shakujo project.
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
#ifndef SHAKUJO_ANALYZER_DIAGNOSTIC_H_
#define SHAKUJO_ANALYZER_DIAGNOSTIC_H_

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include "shakujo/common/core/DocumentRegion.h"

namespace shakujo::analyzer {

/**
 * @brief represents diagnostic information.
 */
class Diagnostic final {
private:
    using code_type = uint16_t;

public:
    /**
     * @brief represents severity of diagnostic.
     */
    enum class Severity : code_type {

        /**
         * @brief only information.
         */
        INFORMATION = 0,

        /**
         * @brief warning.
         */
        WARNING = 10000,

        /**
         * @brief semantic error.
         */
        ERROR = 20000,

        /**
         * @brief internal error.
         */
        CRITICAL = 30000,
    };

public:
    /**
     * @brief represents a diagnostic kind.
     */
    enum class Code : code_type {

        /**
         * @brief unknown information.
         */
        UNKNOWN_INFORMATION = static_cast<code_type>(Severity::INFORMATION),

        /**
         * @brief just a message.
         */
        MESSAGE,

        /**
         * @brief unknown warning.
         */
        UNKNOWN_WARNING = static_cast<code_type>(Severity::WARNING),

        /**
         * @brief unknown error.
         */
        UNKNOWN_ERROR = static_cast<code_type>(Severity::ERROR),

        /**
         * @brief mandatory element is not defined.
         */
        UNDEFINED_ELEMENT,

        /**
         * @brief unexpected element.
         */
        UNEXPECTED_ELEMENT,

        /**
         * @brief unexpected name format.
         */
        INVALID_NAME,

        /**
         * @brief literal type is not valid.
         */
        INVALID_LITERAL_TYPE,

        /**
         * @brief literal value is not valid.
         */
        INVALID_LITERAL_VALUE,

        /**
         * @brief set quantifier is inconsistent for the function.
         */
        INVALID_SET_QUANTIFIER,

        /**
         * @brief target variable is not found.
         */
        VARIABLE_NOT_FOUND,

        /**
         * @brief target variable is not identical.
         */
        VARIABLE_NOT_IDENTICAL,

        /**
         * @brief target function is not found.
         */
        FUNCTION_NOT_FOUND,

        /**
         * @brief target function is not identical.
         */
        FUNCTION_NOT_IDENTICAL,

        /**
         * @brief mismatch between function parameters and arguments.
         */
        INCOMPATIBLE_FUNCTION_ARGUMENT_COUNT,

        /**
         * @brief expression type is not compatible for function parameter.
         */
        INCOMPATIBLE_FUNCTION_ARGUMENT_TYPE,

        /**
         * @brief function quantifier is mismatched (may not occur).
         */
        INCOMPATIBLE_FUNCTION_QUANTIFIER,

        /**
         * @brief target tuple element is not found.
         */
        TUPLE_ELEMENT_NOT_FOUND,

        /**
         * @brief expression type is not compatible for operand.
         */
        INCOMPATIBLE_EXPRESSION_TYPE,

        /**
         * @brief expression type is not compatible for variable.
         */
        INCOMPATIBLE_VARIABLE_TYPE,

        /**
         * @brief invalid variable type.
         */
        INVALID_VARIABLE_TYPE,

        /**
         * @brief conflict symbol in the block.
         */
        DUPLICATE_SYMBOL_IN_BLOCK,

        /**
         * @brief variable declaration is restricted in the block.
         */
        RESTRICTED_VARIABLE_DECLARATION,

        /**
         * @brief variable declaration is restricted in the block.
         */
        RESTRICTED_FUNCTION_DECLARATION,

        /**
         * @brief target database table is not found.
         */
        TABLE_NOT_FOUND,

        /**
         * @brief target table schema is not compatible for the operation.
         */
        INCOMPATIBLE_TABLE_SCHEMA,

        /**
         * @brief the table expression is not valid.
         */
        INVALID_TABLE_EXPRESSION,

        /**
         * @brief target table column is not found.
         */
        COLUMN_NOT_FOUND,

        /**
         * @brief each table does not have any common columns.
         */
        MISSING_NATURAL_JOIN_PAIR,

        /**
         * @brief mandatory column is not specified.
         */
        MISSING_MANDATORY_COLUMN,

        /**
         * @brief target column type is not supported.
         */
        INVALID_COLUMN_TYPE,

        /**
         * @brief duplicate column name.
         */
        DUPLICATE_COLUMN,

        /**
         * @brief duplicate primary key.
         */
        DUPLICATE_PRIMARY_KEY,

        /**
         * @brief unknown critical error.
         */
        UNKNOWN_CRITICAL = static_cast<code_type>(Severity::CRITICAL),

        /**
         * @brief yet not implemented.
         */
        NOT_IMPLEMENTED
    };

private:
    common::core::DocumentRegion region_;
    Code code_;
    std::string message_;

public:
    /**
     * @brief constructs a new object.
     * @param code the diagnostic kind
     * @param message the message
     */
    Diagnostic(Code code, std::string_view message)
        : Diagnostic(common::core::DocumentRegion(), code, message)
    {}

    /**
     * @brief constructs a new object.
     * @param region the corresponded document region
     * @param code the diagnostic kind
     * @param message the message
     */
    Diagnostic(common::core::DocumentRegion region, Code code, std::string_view message)
        : region_(std::move(region)), code_(code), message_(message)
    {}

    /**
     * @brief destroys this object.
     */
    ~Diagnostic() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    Diagnostic(const Diagnostic& other) = default;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Diagnostic(Diagnostic&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Diagnostic& operator=(const Diagnostic& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Diagnostic& operator=(Diagnostic &&other) noexcept = default;

    /**
     * @brief returns the corresponded document region of this diagnostic.
     * @return the document region
     */
    inline common::core::DocumentRegion const& region() const {
        return region_;
    }

    /**
     * @brief returns the code of this diagnostic.
     * @return the diagnostic code
     */
    inline Code code() const {
        return code_;
    }

    /**
     * @brief returns the severity of this diagnostic.
     * @return the severity
     */
    Severity severity() const;

    /**
     * @brief returns the message of this diagnostic.
     * @return the message
     */
    inline std::string const& message() const {
        return message_;
    }

    /**
     * @brief append textual representation of Diagnostic.
     * @param out the target output stream
     * @param value the target value
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, Diagnostic const& value);
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(Diagnostic::Code value) {
    using Code = Diagnostic::Code;
    switch (value) {
        case Code::UNKNOWN_INFORMATION:
            return "UNKNOWN_INFORMATION";
        case Code::UNKNOWN_WARNING:
            return "UNKNOWN_WARNING";
        case Code::UNKNOWN_ERROR:
            return "UNKNOWN_ERROR";
        case Code::UNKNOWN_CRITICAL:
            return "UNKNOWN_CRITICAL";
        case Code::MESSAGE:
            return "message";
        case Code::UNDEFINED_ELEMENT:
            return "mandatory element is not defined";
        case Code::UNEXPECTED_ELEMENT:
            return "element is unexpected";
        case Code::INVALID_NAME:
            return "invalid name";
        case Code::INVALID_LITERAL_TYPE:
            return "invalid literal type";
        case Code::INVALID_LITERAL_VALUE:
            return "invalid literal value";
        case Code::INVALID_SET_QUANTIFIER:
            return "set quantifier is inconsistent for the function";
        case Code::VARIABLE_NOT_FOUND:
            return "target variable is not found";
        case Code::VARIABLE_NOT_IDENTICAL:
            return "target variable is ambiguous";
        case Code::FUNCTION_NOT_FOUND:
            return "target function is not found";
        case Code::FUNCTION_NOT_IDENTICAL:
            return "target function is ambiguous";
        case Code::TUPLE_ELEMENT_NOT_FOUND:
            return "tuple element is not found";
        case Code::DUPLICATE_SYMBOL_IN_BLOCK:
            return "duplicate symbol in block";
        case Code::RESTRICTED_VARIABLE_DECLARATION:
            return "variable declaration is restricted in here";
        case Code::RESTRICTED_FUNCTION_DECLARATION:
            return "function declaration is restricted in here";
        case Code::INCOMPATIBLE_EXPRESSION_TYPE:
            return "expression type is now allowed";
        case Code::INCOMPATIBLE_VARIABLE_TYPE:
            return "variable type is not allowed";
        case Code::INCOMPATIBLE_FUNCTION_QUANTIFIER:
            return "function quantifier is not valid";
        case Code::INCOMPATIBLE_FUNCTION_ARGUMENT_COUNT:
            return "function argument count is not valid";
        case Code::INCOMPATIBLE_FUNCTION_ARGUMENT_TYPE:
            return "function argument type is not valid";
        case Code::INVALID_VARIABLE_TYPE:
            return "variable type is not valid";
        case Code::TABLE_NOT_FOUND:
            return "target table is not found";
        case Code::INCOMPATIBLE_TABLE_SCHEMA:
            return "inconsistent number of columns";
        case Code::INVALID_TABLE_EXPRESSION:
            return "target must be a table expression";
        case Code::COLUMN_NOT_FOUND:
            return "target column is not found";
        case Code::MISSING_NATURAL_JOIN_PAIR:
            return "no natural join pairs";
        case Code::MISSING_MANDATORY_COLUMN:
            return "missing mandatory column";
        case Code::INVALID_COLUMN_TYPE:
            return "invalid column type";
        case Code::DUPLICATE_COLUMN:
            return "duplicate column in table";
        case Code::DUPLICATE_PRIMARY_KEY:
            return "duplicate primary key in table";
        case Code::NOT_IMPLEMENTED:
            return "YET NOT IMPLEMENTED";
    }
    return "(unknown)";
}

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(Diagnostic::Severity value) {
    using Severity = Diagnostic::Severity;
    switch (value) {
        case Severity::INFORMATION:
            return "INFORMATION";
        case Severity::WARNING:
            return "WARNING";
        case Severity::ERROR:
            return "ERROR";
        case Severity::CRITICAL:
            return "CRITICAL";
    }
    return "(unknown)";
}

/**
 * @brief append textual representation of Diagnostic::Code.
 * @param out the target output stream
 * @param value the target value
 * @return the output stream
 */
std::ostream& operator<<(std::ostream& out, Diagnostic::Code value);

/**
 * @brief append textual representation of Diagnostic::Severity.
 * @param out the target output stream
 * @param value the target value
 * @return the output stream
 */
std::ostream& operator<<(std::ostream& out, Diagnostic::Severity value);

}  // namespace shakujo::analyzer

#endif  //SHAKUJO_ANALYZER_DIAGNOSTIC_H_
