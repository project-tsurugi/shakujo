/*
 * Copyright 2018-2023 tsurugi project..
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
#ifndef SHAKUJO_MODEL_EXPRESSION_CONST_EXPRESSION_VISITOR_H_
#define SHAKUJO_MODEL_EXPRESSION_CONST_EXPRESSION_VISITOR_H_

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
#include "shakujo/model/expression/Placeholder.h"
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
#include "shakujo/model/expression/relation/GroupExpression.h"
#include "shakujo/model/expression/relation/JoinExpression.h"
#include "shakujo/model/expression/relation/LimitExpression.h"
#include "shakujo/model/expression/relation/OrderExpression.h"
#include "shakujo/model/expression/relation/ProjectionExpression.h"
#include "shakujo/model/expression/relation/RenameExpression.h"
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
class ConstExpressionVisitor {
public:
    /**
     * @brief Processes the default Expression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Expression
     */
    virtual Return visitDefault(Expression const* node, [[maybe_unused]] Args... args) {
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
    virtual Return visit(ArrayCreationExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes ArrayElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ArrayElementLoadExpression
     */
    virtual Return visit(ArrayElementLoadExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes ArrayElementStoreExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ArrayElementStoreExpression
     */
    virtual Return visit(ArrayElementStoreExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes AssignExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see AssignExpression
     */
    virtual Return visit(AssignExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes BinaryOperator.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BinaryOperator
     */
    virtual Return visit(BinaryOperator const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes BlockExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BlockExpression
     */
    virtual Return visit(BlockExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes CaseExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CaseExpression
     */
    virtual Return visit(CaseExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes CursorAdvanceExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CursorAdvanceExpression
     */
    virtual Return visit(CursorAdvanceExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes CursorCreationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CursorCreationExpression
     */
    virtual Return visit(CursorCreationExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes CursorElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CursorElementLoadExpression
     */
    virtual Return visit(CursorElementLoadExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes FunctionCall.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see FunctionCall
     */
    virtual Return visit(FunctionCall const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes ImplicitCast.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ImplicitCast
     */
    virtual Return visit(ImplicitCast const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Literal.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Literal
     */
    virtual Return visit(Literal const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Placeholder.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Placeholder
     */
    virtual Return visit(Placeholder const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes TupleCreationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TupleCreationExpression
     */
    virtual Return visit(TupleCreationExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes TupleElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TupleElementLoadExpression
     */
    virtual Return visit(TupleElementLoadExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes TupleElementStoreExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TupleElementStoreExpression
     */
    virtual Return visit(TupleElementStoreExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes TypeOperator.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TypeOperator
     */
    virtual Return visit(TypeOperator const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes UnaryOperator.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see UnaryOperator
     */
    virtual Return visit(UnaryOperator const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes VariableReference.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VariableReference
     */
    virtual Return visit(VariableReference const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes VectorCreationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorCreationExpression
     */
    virtual Return visit(VectorCreationExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes VectorElementLoadExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorElementLoadExpression
     */
    virtual Return visit(VectorElementLoadExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes VectorElementStoreExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorElementStoreExpression
     */
    virtual Return visit(VectorElementStoreExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes VectorLengthExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorLengthExpression
     */
    virtual Return visit(VectorLengthExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes AggregationExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::AggregationExpression
     */
    virtual Return visit(relation::AggregationExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes DistinctExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::DistinctExpression
     */
    virtual Return visit(relation::DistinctExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes GroupExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::GroupExpression
     */
    virtual Return visit(relation::GroupExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes JoinExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::JoinExpression
     */
    virtual Return visit(relation::JoinExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes LimitExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::LimitExpression
     */
    virtual Return visit(relation::LimitExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes OrderExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::OrderExpression
     */
    virtual Return visit(relation::OrderExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes ProjectionExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::ProjectionExpression
     */
    virtual Return visit(relation::ProjectionExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes RenameExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::RenameExpression
     */
    virtual Return visit(relation::RenameExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes ScanExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::ScanExpression
     */
    virtual Return visit(relation::ScanExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes SelectionExpression.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see relation::SelectionExpression
     */
    virtual Return visit(relation::SelectionExpression const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Expression const* node, Args... args) {
        switch (node->kind()) {
        case ArrayCreationExpression::tag:
            return visit(static_cast<ArrayCreationExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case ArrayElementLoadExpression::tag:
            return visit(static_cast<ArrayElementLoadExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case ArrayElementStoreExpression::tag:
            return visit(static_cast<ArrayElementStoreExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case AssignExpression::tag:
            return visit(static_cast<AssignExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case BinaryOperator::tag:
            return visit(static_cast<BinaryOperator const*>(node), std::forward<Args>(args)...);  // NOLINT
        case BlockExpression::tag:
            return visit(static_cast<BlockExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case CaseExpression::tag:
            return visit(static_cast<CaseExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case CursorAdvanceExpression::tag:
            return visit(static_cast<CursorAdvanceExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case CursorCreationExpression::tag:
            return visit(static_cast<CursorCreationExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case CursorElementLoadExpression::tag:
            return visit(static_cast<CursorElementLoadExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case FunctionCall::tag:
            return visit(static_cast<FunctionCall const*>(node), std::forward<Args>(args)...);  // NOLINT
        case ImplicitCast::tag:
            return visit(static_cast<ImplicitCast const*>(node), std::forward<Args>(args)...);  // NOLINT
        case Literal::tag:
            return visit(static_cast<Literal const*>(node), std::forward<Args>(args)...);  // NOLINT
        case Placeholder::tag:
            return visit(static_cast<Placeholder const*>(node), std::forward<Args>(args)...);  // NOLINT
        case TupleCreationExpression::tag:
            return visit(static_cast<TupleCreationExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case TupleElementLoadExpression::tag:
            return visit(static_cast<TupleElementLoadExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case TupleElementStoreExpression::tag:
            return visit(static_cast<TupleElementStoreExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case TypeOperator::tag:
            return visit(static_cast<TypeOperator const*>(node), std::forward<Args>(args)...);  // NOLINT
        case UnaryOperator::tag:
            return visit(static_cast<UnaryOperator const*>(node), std::forward<Args>(args)...);  // NOLINT
        case VariableReference::tag:
            return visit(static_cast<VariableReference const*>(node), std::forward<Args>(args)...);  // NOLINT
        case VectorCreationExpression::tag:
            return visit(static_cast<VectorCreationExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case VectorElementLoadExpression::tag:
            return visit(static_cast<VectorElementLoadExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case VectorElementStoreExpression::tag:
            return visit(static_cast<VectorElementStoreExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case VectorLengthExpression::tag:
            return visit(static_cast<VectorLengthExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::AggregationExpression::tag:
            return visit(static_cast<relation::AggregationExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::DistinctExpression::tag:
            return visit(static_cast<relation::DistinctExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::GroupExpression::tag:
            return visit(static_cast<relation::GroupExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::JoinExpression::tag:
            return visit(static_cast<relation::JoinExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::LimitExpression::tag:
            return visit(static_cast<relation::LimitExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::OrderExpression::tag:
            return visit(static_cast<relation::OrderExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::ProjectionExpression::tag:
            return visit(static_cast<relation::ProjectionExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::RenameExpression::tag:
            return visit(static_cast<relation::RenameExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::ScanExpression::tag:
            return visit(static_cast<relation::ScanExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        case relation::SelectionExpression::tag:
            return visit(static_cast<relation::SelectionExpression const*>(node), std::forward<Args>(args)...);  // NOLINT
        }
        // may not occur
        return visitDefault(node, std::forward<Args>(args)...);
    }

};

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_CONST_EXPRESSION_VISITOR_H_
