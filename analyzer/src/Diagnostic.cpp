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
#include "shakujo/analyzer/Diagnostic.h"

namespace shakujo::analyzer {

const common::core::DocumentRegion& Diagnostic::region() const {
    return region_;
}

Diagnostic::Code Diagnostic::code() const {
    return code_;
}

const std::string &Diagnostic::message() const {
    return message_;
}

Diagnostic::Severity Diagnostic::severity() const {
    using severity_t = typename std::underlying_type<Severity>::type;
    using code_t = typename std::underlying_type<Code>::type;
    static_assert(
            std::is_same<severity_t, code_t>::value,
            "underlying type of Severity and Code must be same"
    );
    auto v = static_cast<code_t>(code_);
    if (v >= static_cast<severity_t>(Severity::CRITICAL)) {
        return Severity::CRITICAL;
    }
    if (v >= static_cast<severity_t>(Severity::ERROR)) {
        return Severity::ERROR;
    }
    if (v >= static_cast<severity_t>(Severity::WARNING)) {
        return Severity::WARNING;
    }
    return Severity::INFORMATION;
}

std::ostream& operator<<(std::ostream& out, const Diagnostic& value) {
    out << "[" << value.severity() << ": " << value.code() << "]"
        << " " << value.message();
    if (value.region()) {
        out << " (" << value.region() << ")";
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, Diagnostic::Severity value) {
    switch (value) {
    case Diagnostic::Severity::INFORMATION:
        out << "INFORMATION";
        break;
    case Diagnostic::Severity::WARNING:
        out << "WARNING";
        break;
    case Diagnostic::Severity::ERROR:
        out << "ERROR";
        break;
    case Diagnostic::Severity::CRITICAL:
        out << "CRITICAL";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, Diagnostic::Code value) {
    switch (value) {
    case Diagnostic::Code::UNKNOWN_INFORMATION:
        out << "UNKNOWN_INFORMATION";
        break;
    case Diagnostic::Code::UNKNOWN_WARNING:
        out << "UNKNOWN_WARNING";
        break;
    case Diagnostic::Code::UNKNOWN_ERROR:
        out << "UNKNOWN_ERROR";
        break;
    case Diagnostic::Code::UNKNOWN_CRITICAL:
        out << "UNKNOWN_CRITICAL";
        break;
    case Diagnostic::Code::MESSAGE:
        out << "message";
        break;
    case Diagnostic::Code::UNDEFINED_ELEMENT:
        out << "mandatory element is not defined";
        break;
    case Diagnostic::Code::UNEXPECTED_ELEMENT:
        out << "element is unexpected";
        break;
    case Diagnostic::Code::INVALID_NAME:
        out << "invalid name";
        break;
    case Diagnostic::Code::LITERAL_INVALID_TYPE:
        out << "invalid literal type";
        break;
    case Diagnostic::Code::LITERAL_INVALID_VALUE:
        out << "invalid literal value";
        break;
    case Diagnostic::Code::VARIABLE_NOT_FOUND:
        out << "target variable is not found";
        break;
    case Diagnostic::Code::VARIABLE_NOT_IDENTICAL:
        out << "target variable is ambiguous";
        break;
    case Diagnostic::Code::TUPLE_ELEMENT_NOT_FOUND:
        out << "tuple element is not found";
        break;
    case Diagnostic::Code::DUPLICATE_SYMBOL_IN_BLOCK:
        out << "duplicate symbol in block";
        break;
    case Diagnostic::Code::RESTRICTED_VARIABLE_DECLARATION:
        out << "variable declaration is restricted in here";
        break;
    case Diagnostic::Code::RESTRICTED_FUNCTION_DECLARATION:
        out << "function declaration is restricted in here";
        break;
    case Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE:
        out << "expression type is now allowed";
        break;
    case Diagnostic::Code::INCOMPATIBLE_VARIABLE_TYPE:
        out << "variable type is not allowed";
        break;
    case Diagnostic::Code::INVALID_VARIABLE_TYPE:
        out << "variable type is not valid";
        break;
    case Diagnostic::Code::TABLE_NOT_FOUND:
        out << "target table is not found";
        break;
    case Diagnostic::Code::INCOMPATIBLE_TABLE_SCHEMA:
        out << "inconsistent number of columns";
        break;
    case Diagnostic::Code::COLUMN_NOT_FOUND:
        out << "target column is not found";
        break;
    case Diagnostic::Code::MISSING_NATURAL_JOIN_PAIR:
        out << "no natural join pairs";
        break;
    case Diagnostic::Code::MISSING_MANDATORY_COLUMN:
        out << "missing mandatory column";
        break;
    case Diagnostic::Code::INVALID_COLUMN_TYPE:
        out << "invalid column type";
        break;
    case Diagnostic::Code::DUPLICATE_COLUMN:
        out << "duplicate column in table";
        break;
    case Diagnostic::Code::DUPLICATE_PRIMARY_KEY:
        out << "duplicate primary key in table";
        break;
    case Diagnostic::Code::NOT_IMPLEMENTED:
        out << "YET NOT IMPLEMENTED";
        break;
    }
    return out;
}
}  // namespace shakujo::analyzer
