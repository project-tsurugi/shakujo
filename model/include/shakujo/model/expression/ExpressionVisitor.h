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
#ifndef SHAKUJO_MODEL_EXPRESSION_EXPRESSION_VISITOR_H_
#define SHAKUJO_MODEL_EXPRESSION_EXPRESSION_VISITOR_H_

#include <stdexcept>
#include <utility>
#include <sstream>

#include "shakujo/model/expression/ArrayCreationExpression.h"
#include "shakujo/model/expression/ArrayElementLoadExpression.h"
#include "shakujo/model/expression/ArrayElementStoreExpression.h"
#include "shakujo/model/expression/AssignExpression.h"
#include "shakujo/model/expression/BinaryOperator.h"
#include "shakujo/model/expression/BlockExpression.h"
#include "shakujo/model/expression/CaseExpression.h"
#include "shakujo/model/expression/CursorAdvanceExpression.h"
#include "shakujo/model/expression/CursorCreationExpression.h"
#include "shakujo/model/expression/CursorElementLoadExpression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/expression/FunctionCall.h"
#include "shakujo/model/expression/ImplicitCast.h"
#include "shakujo/model/expression/Literal.h"
#include "shakujo/model/expression/StringOperator.h"
#include "shakujo/model/expression/TupleCreationExpression.h"
#include "shakujo/model/expression/TupleElementLoadExpression.h"
#include "shakujo/model/expression/TupleElementStoreExpression.h"
#include "shakujo/model/expression/TypeOperator.h"
#include "shakujo/model/expression/UnaryOperator.h"
#include "shakujo/model/expression/VariableReference.h"
#include "shakujo/model/expression/VectorCreationExpression.h"
#include "shakujo/model/expression/VectorElementLoadExpression.h"
#include "shakujo/model/expression/VectorElementStoreExpression.h"
#include "shakujo/model/expression/VectorLengthExpression.h"
#include "shakujo/model/expression/relation/AggregationExpression.h"
#include "shakujo/model/expression/relation/DistinctExpression.h"
#include "shakujo/model/expression/relation/JoinExpression.h"
#include "shakujo/model/expression/relation/LimitExpression.h"
#include "shakujo/model/expression/relation/OrderExpression.h"
#include "shakujo/model/expression/relation/ProjectionExpression.h"
#include "shakujo/model/expression/relation/ScanExpression.h"
#include "shakujo/model/expression/relation/SelectionExpression.h"

namespace shakujo::model::expression {
/**
 * @brief A visitor for Expression.
 * The default implementation of visit() will be redirected to visitDefault().
 * @tparam Return the result type
 * @tparam Args the parameter types
 * @see Expression
 */
template <class Return, class... Args>
class ExpressionVisitor {
public:
    /**
     * @brief Processes the default Expression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Expression
     */
    virtual Return visitDefault(Expression* node, [[maybe_unused]] Args... args) {  // NOLINT
        std::ostringstream ss;
        ss << "visit for " << node->kind() << " is not implemented.";
        throw std::runtime_error(ss.str());
    }

    /**
     * @brief Processes ArrayCreationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ArrayCreationExpression
     */
    virtual Return visit(ArrayCreationExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ArrayElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ArrayElementLoadExpression
     */
    virtual Return visit(ArrayElementLoadExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ArrayElementStoreExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ArrayElementStoreExpression
     */
    virtual Return visit(ArrayElementStoreExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes AssignExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see AssignExpression
     */
    virtual Return visit(AssignExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes BinaryOperator.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BinaryOperator
     */
    virtual Return visit(BinaryOperator* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes BlockExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BlockExpression
     */
    virtual Return visit(BlockExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes CaseExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CaseExpression
     */
    virtual Return visit(CaseExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes CursorAdvanceExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CursorAdvanceExpression
     */
    virtual Return visit(CursorAdvanceExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes CursorCreationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CursorCreationExpression
     */
    virtual Return visit(CursorCreationExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes CursorElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CursorElementLoadExpression
     */
    virtual Return visit(CursorElementLoadExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes FunctionCall.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see FunctionCall
     */
    virtual Return visit(FunctionCall* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ImplicitCast.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ImplicitCast
     */
    virtual Return visit(ImplicitCast* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Literal.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Literal
     */
    virtual Return visit(Literal* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes StringOperator.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see StringOperator
     */
    virtual Return visit(StringOperator* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes TupleCreationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TupleCreationExpression
     */
    virtual Return visit(TupleCreationExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes TupleElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TupleElementLoadExpression
     */
    virtual Return visit(TupleElementLoadExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes TupleElementStoreExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TupleElementStoreExpression
     */
    virtual Return visit(TupleElementStoreExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes TypeOperator.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TypeOperator
     */
    virtual Return visit(TypeOperator* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes UnaryOperator.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see UnaryOperator
     */
    virtual Return visit(UnaryOperator* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VariableReference.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VariableReference
     */
    virtual Return visit(VariableReference* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VectorCreationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorCreationExpression
     */
    virtual Return visit(VectorCreationExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VectorElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorElementLoadExpression
     */
    virtual Return visit(VectorElementLoadExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VectorElementStoreExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorElementStoreExpression
     */
    virtual Return visit(VectorElementStoreExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VectorLengthExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorLengthExpression
     */
    virtual Return visit(VectorLengthExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes AggregationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::AggregationExpression
     */
    virtual Return visit(relation::AggregationExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes DistinctExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::DistinctExpression
     */
    virtual Return visit(relation::DistinctExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes JoinExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::JoinExpression
     */
    virtual Return visit(relation::JoinExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes LimitExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::LimitExpression
     */
    virtual Return visit(relation::LimitExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes OrderExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::OrderExpression
     */
    virtual Return visit(relation::OrderExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ProjectionExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::ProjectionExpression
     */
    virtual Return visit(relation::ProjectionExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ScanExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::ScanExpression
     */
    virtual Return visit(relation::ScanExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes SelectionExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::SelectionExpression
     */
    virtual Return visit(relation::SelectionExpression* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Expression* node, Args... args) {  // NOLINT
        switch (node->kind()) {
        case ExpressionKind::ARRAY_CREATION_EXPRESSION:
            return visit(dynamic_cast<ArrayCreationExpression*>(node), args...);
        case ExpressionKind::ARRAY_ELEMENT_LOAD_EXPRESSION:
            return visit(dynamic_cast<ArrayElementLoadExpression*>(node), args...);
        case ExpressionKind::ARRAY_ELEMENT_STORE_EXPRESSION:
            return visit(dynamic_cast<ArrayElementStoreExpression*>(node), args...);
        case ExpressionKind::ASSIGN_EXPRESSION:
            return visit(dynamic_cast<AssignExpression*>(node), args...);
        case ExpressionKind::BINARY_OPERATOR:
            return visit(dynamic_cast<BinaryOperator*>(node), args...);
        case ExpressionKind::BLOCK_EXPRESSION:
            return visit(dynamic_cast<BlockExpression*>(node), args...);
        case ExpressionKind::CASE_EXPRESSION:
            return visit(dynamic_cast<CaseExpression*>(node), args...);
        case ExpressionKind::CURSOR_ADVANCE_EXPRESSION:
            return visit(dynamic_cast<CursorAdvanceExpression*>(node), args...);
        case ExpressionKind::CURSOR_CREATION_EXPRESSION:
            return visit(dynamic_cast<CursorCreationExpression*>(node), args...);
        case ExpressionKind::CURSOR_ELEMENT_LOAD_EXPRESSION:
            return visit(dynamic_cast<CursorElementLoadExpression*>(node), args...);
        case ExpressionKind::FUNCTION_CALL:
            return visit(dynamic_cast<FunctionCall*>(node), args...);
        case ExpressionKind::IMPLICIT_CAST:
            return visit(dynamic_cast<ImplicitCast*>(node), args...);
        case ExpressionKind::LITERAL:
            return visit(dynamic_cast<Literal*>(node), args...);
        case ExpressionKind::STRING_OPERATOR:
            return visit(dynamic_cast<StringOperator*>(node), args...);
        case ExpressionKind::TUPLE_CREATION_EXPRESSION:
            return visit(dynamic_cast<TupleCreationExpression*>(node), args...);
        case ExpressionKind::TUPLE_ELEMENT_LOAD_EXPRESSION:
            return visit(dynamic_cast<TupleElementLoadExpression*>(node), args...);
        case ExpressionKind::TUPLE_ELEMENT_STORE_EXPRESSION:
            return visit(dynamic_cast<TupleElementStoreExpression*>(node), args...);
        case ExpressionKind::TYPE_OPERATOR:
            return visit(dynamic_cast<TypeOperator*>(node), args...);
        case ExpressionKind::UNARY_OPERATOR:
            return visit(dynamic_cast<UnaryOperator*>(node), args...);
        case ExpressionKind::VARIABLE_REFERENCE:
            return visit(dynamic_cast<VariableReference*>(node), args...);
        case ExpressionKind::VECTOR_CREATION_EXPRESSION:
            return visit(dynamic_cast<VectorCreationExpression*>(node), args...);
        case ExpressionKind::VECTOR_ELEMENT_LOAD_EXPRESSION:
            return visit(dynamic_cast<VectorElementLoadExpression*>(node), args...);
        case ExpressionKind::VECTOR_ELEMENT_STORE_EXPRESSION:
            return visit(dynamic_cast<VectorElementStoreExpression*>(node), args...);
        case ExpressionKind::VECTOR_LENGTH_EXPRESSION:
            return visit(dynamic_cast<VectorLengthExpression*>(node), args...);
        case ExpressionKind::AGGREGATION_EXPRESSION:
            return visit(dynamic_cast<relation::AggregationExpression*>(node), args...);
        case ExpressionKind::DISTINCT_EXPRESSION:
            return visit(dynamic_cast<relation::DistinctExpression*>(node), args...);
        case ExpressionKind::JOIN_EXPRESSION:
            return visit(dynamic_cast<relation::JoinExpression*>(node), args...);
        case ExpressionKind::LIMIT_EXPRESSION:
            return visit(dynamic_cast<relation::LimitExpression*>(node), args...);
        case ExpressionKind::ORDER_EXPRESSION:
            return visit(dynamic_cast<relation::OrderExpression*>(node), args...);
        case ExpressionKind::PROJECTION_EXPRESSION:
            return visit(dynamic_cast<relation::ProjectionExpression*>(node), args...);
        case ExpressionKind::SCAN_EXPRESSION:
            return visit(dynamic_cast<relation::ScanExpression*>(node), args...);
        case ExpressionKind::SELECTION_EXPRESSION:
            return visit(dynamic_cast<relation::SelectionExpression*>(node), args...);
        }
        // may not occur
        return visitDefault(node, args...);
    }

};

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_EXPRESSION_VISITOR_H_
