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
#ifndef SHAKUJO_MODEL_EXPRESSION_EXPRESSION_KIND_H_
#define SHAKUJO_MODEL_EXPRESSION_EXPRESSION_KIND_H_

#include <string>
#include <string_view>
#include <iostream>


namespace shakujo::model::expression {
/**
 * @brief Represents the catalog of Expression.
 * @see Expression::kind()
 */
enum class ExpressionKind {
    /**
     * @brief tag for ArrayCreationExpression.
     * @see ArrayCreationExpression
     */
    ARRAY_CREATION_EXPRESSION,
    /**
     * @brief tag for ArrayElementLoadExpression.
     * @see ArrayElementLoadExpression
     */
    ARRAY_ELEMENT_LOAD_EXPRESSION,
    /**
     * @brief tag for ArrayElementStoreExpression.
     * @see ArrayElementStoreExpression
     */
    ARRAY_ELEMENT_STORE_EXPRESSION,
    /**
     * @brief tag for AssignExpression.
     * @see AssignExpression
     */
    ASSIGN_EXPRESSION,
    /**
     * @brief tag for BinaryOperator.
     * @see BinaryOperator
     */
    BINARY_OPERATOR,
    /**
     * @brief tag for BlockExpression.
     * @see BlockExpression
     */
    BLOCK_EXPRESSION,
    /**
     * @brief tag for CaseExpression.
     * @see CaseExpression
     */
    CASE_EXPRESSION,
    /**
     * @brief tag for CursorAdvanceExpression.
     * @see CursorAdvanceExpression
     */
    CURSOR_ADVANCE_EXPRESSION,
    /**
     * @brief tag for CursorCreationExpression.
     * @see CursorCreationExpression
     */
    CURSOR_CREATION_EXPRESSION,
    /**
     * @brief tag for CursorElementLoadExpression.
     * @see CursorElementLoadExpression
     */
    CURSOR_ELEMENT_LOAD_EXPRESSION,
    /**
     * @brief tag for FunctionCall.
     * @see FunctionCall
     */
    FUNCTION_CALL,
    /**
     * @brief tag for ImplicitCast.
     * @see ImplicitCast
     */
    IMPLICIT_CAST,
    /**
     * @brief tag for Literal.
     * @see Literal
     */
    LITERAL,
    /**
     * @brief tag for Placeholder.
     * @see Placeholder
     */
    PLACEHOLDER,
    /**
     * @brief tag for StringOperator.
     * @see StringOperator
     */
    STRING_OPERATOR,
    /**
     * @brief tag for TupleCreationExpression.
     * @see TupleCreationExpression
     */
    TUPLE_CREATION_EXPRESSION,
    /**
     * @brief tag for TupleElementLoadExpression.
     * @see TupleElementLoadExpression
     */
    TUPLE_ELEMENT_LOAD_EXPRESSION,
    /**
     * @brief tag for TupleElementStoreExpression.
     * @see TupleElementStoreExpression
     */
    TUPLE_ELEMENT_STORE_EXPRESSION,
    /**
     * @brief tag for TypeOperator.
     * @see TypeOperator
     */
    TYPE_OPERATOR,
    /**
     * @brief tag for UnaryOperator.
     * @see UnaryOperator
     */
    UNARY_OPERATOR,
    /**
     * @brief tag for VariableReference.
     * @see VariableReference
     */
    VARIABLE_REFERENCE,
    /**
     * @brief tag for VectorCreationExpression.
     * @see VectorCreationExpression
     */
    VECTOR_CREATION_EXPRESSION,
    /**
     * @brief tag for VectorElementLoadExpression.
     * @see VectorElementLoadExpression
     */
    VECTOR_ELEMENT_LOAD_EXPRESSION,
    /**
     * @brief tag for VectorElementStoreExpression.
     * @see VectorElementStoreExpression
     */
    VECTOR_ELEMENT_STORE_EXPRESSION,
    /**
     * @brief tag for VectorLengthExpression.
     * @see VectorLengthExpression
     */
    VECTOR_LENGTH_EXPRESSION,
    /**
     * @brief tag for AggregationExpression.
     * @see relation::AggregationExpression
     */
    AGGREGATION_EXPRESSION,
    /**
     * @brief tag for DistinctExpression.
     * @see relation::DistinctExpression
     */
    DISTINCT_EXPRESSION,
    /**
     * @brief tag for GroupExpression.
     * @see relation::GroupExpression
     */
    GROUP_EXPRESSION,
    /**
     * @brief tag for JoinExpression.
     * @see relation::JoinExpression
     */
    JOIN_EXPRESSION,
    /**
     * @brief tag for LimitExpression.
     * @see relation::LimitExpression
     */
    LIMIT_EXPRESSION,
    /**
     * @brief tag for OrderExpression.
     * @see relation::OrderExpression
     */
    ORDER_EXPRESSION,
    /**
     * @brief tag for ProjectionExpression.
     * @see relation::ProjectionExpression
     */
    PROJECTION_EXPRESSION,
    /**
     * @brief tag for ScanExpression.
     * @see relation::ScanExpression
     */
    SCAN_EXPRESSION,
    /**
     * @brief tag for SelectionExpression.
     * @see relation::SelectionExpression
     */
    SELECTION_EXPRESSION,
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see ExpressionKind
 */
inline constexpr std::string_view to_string_view(ExpressionKind value) {
    switch (value) {
        case ExpressionKind::ARRAY_CREATION_EXPRESSION: return "ARRAY_CREATION_EXPRESSION";
        case ExpressionKind::ARRAY_ELEMENT_LOAD_EXPRESSION: return "ARRAY_ELEMENT_LOAD_EXPRESSION";
        case ExpressionKind::ARRAY_ELEMENT_STORE_EXPRESSION: return "ARRAY_ELEMENT_STORE_EXPRESSION";
        case ExpressionKind::ASSIGN_EXPRESSION: return "ASSIGN_EXPRESSION";
        case ExpressionKind::BINARY_OPERATOR: return "BINARY_OPERATOR";
        case ExpressionKind::BLOCK_EXPRESSION: return "BLOCK_EXPRESSION";
        case ExpressionKind::CASE_EXPRESSION: return "CASE_EXPRESSION";
        case ExpressionKind::CURSOR_ADVANCE_EXPRESSION: return "CURSOR_ADVANCE_EXPRESSION";
        case ExpressionKind::CURSOR_CREATION_EXPRESSION: return "CURSOR_CREATION_EXPRESSION";
        case ExpressionKind::CURSOR_ELEMENT_LOAD_EXPRESSION: return "CURSOR_ELEMENT_LOAD_EXPRESSION";
        case ExpressionKind::FUNCTION_CALL: return "FUNCTION_CALL";
        case ExpressionKind::IMPLICIT_CAST: return "IMPLICIT_CAST";
        case ExpressionKind::LITERAL: return "LITERAL";
        case ExpressionKind::PLACEHOLDER: return "PLACEHOLDER";
        case ExpressionKind::STRING_OPERATOR: return "STRING_OPERATOR";
        case ExpressionKind::TUPLE_CREATION_EXPRESSION: return "TUPLE_CREATION_EXPRESSION";
        case ExpressionKind::TUPLE_ELEMENT_LOAD_EXPRESSION: return "TUPLE_ELEMENT_LOAD_EXPRESSION";
        case ExpressionKind::TUPLE_ELEMENT_STORE_EXPRESSION: return "TUPLE_ELEMENT_STORE_EXPRESSION";
        case ExpressionKind::TYPE_OPERATOR: return "TYPE_OPERATOR";
        case ExpressionKind::UNARY_OPERATOR: return "UNARY_OPERATOR";
        case ExpressionKind::VARIABLE_REFERENCE: return "VARIABLE_REFERENCE";
        case ExpressionKind::VECTOR_CREATION_EXPRESSION: return "VECTOR_CREATION_EXPRESSION";
        case ExpressionKind::VECTOR_ELEMENT_LOAD_EXPRESSION: return "VECTOR_ELEMENT_LOAD_EXPRESSION";
        case ExpressionKind::VECTOR_ELEMENT_STORE_EXPRESSION: return "VECTOR_ELEMENT_STORE_EXPRESSION";
        case ExpressionKind::VECTOR_LENGTH_EXPRESSION: return "VECTOR_LENGTH_EXPRESSION";
        case ExpressionKind::AGGREGATION_EXPRESSION: return "AGGREGATION_EXPRESSION";
        case ExpressionKind::DISTINCT_EXPRESSION: return "DISTINCT_EXPRESSION";
        case ExpressionKind::GROUP_EXPRESSION: return "GROUP_EXPRESSION";
        case ExpressionKind::JOIN_EXPRESSION: return "JOIN_EXPRESSION";
        case ExpressionKind::LIMIT_EXPRESSION: return "LIMIT_EXPRESSION";
        case ExpressionKind::ORDER_EXPRESSION: return "ORDER_EXPRESSION";
        case ExpressionKind::PROJECTION_EXPRESSION: return "PROJECTION_EXPRESSION";
        case ExpressionKind::SCAN_EXPRESSION: return "SCAN_EXPRESSION";
        case ExpressionKind::SELECTION_EXPRESSION: return "SELECTION_EXPRESSION";
    }
    return "(unknown)";
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see ExpressionKind
 */
inline std::ostream& operator<<(std::ostream& out, ExpressionKind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_EXPRESSION_KIND_H_
