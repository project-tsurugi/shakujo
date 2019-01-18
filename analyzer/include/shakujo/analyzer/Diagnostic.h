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
        LITERAL_INVALID_TYPE,

        /**
         * @brief literal value is not valid.
         */
        LITERAL_INVALID_VALUE,

        /**
         * @brief target variable is not found.
         */
        VARIABLE_NOT_FOUND,

        /**
         * @brief target variable is not identical.
         */
        VARIABLE_NOT_IDENTICAL,

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
    common::core::DocumentRegion const& region() const;

    /**
     * @brief returns the code of this diagnostic.
     * @return the diagnostic code
     */
    Code code() const;

    /**
     * @brief returns the severity of this diagnostic.
     * @return the severity
     */
    Severity severity() const;

    /**
     * @brief returns the message of this diagnostic.
     * @return the message
     */
    std::string const& message() const;

    /**
     * @brief append textual representation of Diagnostic.
     * @param out the target output stream
     * @param value the target value
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, Diagnostic const& value);
};

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
