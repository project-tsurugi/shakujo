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
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see ExpressionKind
 */
std::ostream& operator<<(std::ostream& out, ExpressionKind value);

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_EXPRESSION_KIND_H_
