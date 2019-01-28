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
#include "shakujo/model/expression/ExpressionKind.h"

#include <iostream>


namespace shakujo::model::expression {
std::ostream& operator<<(std::ostream& out, ExpressionKind value) {
    switch (value) {
    case ExpressionKind::ARRAY_CREATION_EXPRESSION:
        out << "ARRAY_CREATION_EXPRESSION";
        break;
    case ExpressionKind::ARRAY_ELEMENT_LOAD_EXPRESSION:
        out << "ARRAY_ELEMENT_LOAD_EXPRESSION";
        break;
    case ExpressionKind::ARRAY_ELEMENT_STORE_EXPRESSION:
        out << "ARRAY_ELEMENT_STORE_EXPRESSION";
        break;
    case ExpressionKind::ASSIGN_EXPRESSION:
        out << "ASSIGN_EXPRESSION";
        break;
    case ExpressionKind::BINARY_OPERATOR:
        out << "BINARY_OPERATOR";
        break;
    case ExpressionKind::BLOCK_EXPRESSION:
        out << "BLOCK_EXPRESSION";
        break;
    case ExpressionKind::CASE_EXPRESSION:
        out << "CASE_EXPRESSION";
        break;
    case ExpressionKind::CURSOR_ADVANCE_EXPRESSION:
        out << "CURSOR_ADVANCE_EXPRESSION";
        break;
    case ExpressionKind::CURSOR_CREATION_EXPRESSION:
        out << "CURSOR_CREATION_EXPRESSION";
        break;
    case ExpressionKind::CURSOR_ELEMENT_LOAD_EXPRESSION:
        out << "CURSOR_ELEMENT_LOAD_EXPRESSION";
        break;
    case ExpressionKind::FUNCTION_CALL:
        out << "FUNCTION_CALL";
        break;
    case ExpressionKind::IMPLICIT_CAST:
        out << "IMPLICIT_CAST";
        break;
    case ExpressionKind::LITERAL:
        out << "LITERAL";
        break;
    case ExpressionKind::PLACEHOLDER:
        out << "PLACEHOLDER";
        break;
    case ExpressionKind::STRING_OPERATOR:
        out << "STRING_OPERATOR";
        break;
    case ExpressionKind::TUPLE_CREATION_EXPRESSION:
        out << "TUPLE_CREATION_EXPRESSION";
        break;
    case ExpressionKind::TUPLE_ELEMENT_LOAD_EXPRESSION:
        out << "TUPLE_ELEMENT_LOAD_EXPRESSION";
        break;
    case ExpressionKind::TUPLE_ELEMENT_STORE_EXPRESSION:
        out << "TUPLE_ELEMENT_STORE_EXPRESSION";
        break;
    case ExpressionKind::TYPE_OPERATOR:
        out << "TYPE_OPERATOR";
        break;
    case ExpressionKind::UNARY_OPERATOR:
        out << "UNARY_OPERATOR";
        break;
    case ExpressionKind::VARIABLE_REFERENCE:
        out << "VARIABLE_REFERENCE";
        break;
    case ExpressionKind::VECTOR_CREATION_EXPRESSION:
        out << "VECTOR_CREATION_EXPRESSION";
        break;
    case ExpressionKind::VECTOR_ELEMENT_LOAD_EXPRESSION:
        out << "VECTOR_ELEMENT_LOAD_EXPRESSION";
        break;
    case ExpressionKind::VECTOR_ELEMENT_STORE_EXPRESSION:
        out << "VECTOR_ELEMENT_STORE_EXPRESSION";
        break;
    case ExpressionKind::VECTOR_LENGTH_EXPRESSION:
        out << "VECTOR_LENGTH_EXPRESSION";
        break;
    case ExpressionKind::AGGREGATION_EXPRESSION:
        out << "AGGREGATION_EXPRESSION";
        break;
    case ExpressionKind::DISTINCT_EXPRESSION:
        out << "DISTINCT_EXPRESSION";
        break;
    case ExpressionKind::JOIN_EXPRESSION:
        out << "JOIN_EXPRESSION";
        break;
    case ExpressionKind::LIMIT_EXPRESSION:
        out << "LIMIT_EXPRESSION";
        break;
    case ExpressionKind::ORDER_EXPRESSION:
        out << "ORDER_EXPRESSION";
        break;
    case ExpressionKind::PROJECTION_EXPRESSION:
        out << "PROJECTION_EXPRESSION";
        break;
    case ExpressionKind::SCAN_EXPRESSION:
        out << "SCAN_EXPRESSION";
        break;
    case ExpressionKind::SELECTION_EXPRESSION:
        out << "SELECTION_EXPRESSION";
        break;
    }
    return out;
}

}  // namespace shakujo::model::expression

