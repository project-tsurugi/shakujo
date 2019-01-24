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
#ifndef SHAKUJO_MODEL_I_R_FACTORY_BASE_H_
#define SHAKUJO_MODEL_I_R_FACTORY_BASE_H_

#include <cstddef>
#include <memory>
#include <string>
#include <initializer_list>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/util/MoveInitializerList.h"
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
#include "shakujo/model/expression/Expression.h"
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
#include "shakujo/model/name/Index.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/QualifiedName.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/program/GlobalDeclaration.h"
#include "shakujo/model/program/GlobalFunctionDeclaration.h"
#include "shakujo/model/program/GlobalVariableDeclaration.h"
#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/AnchorDeclaration.h"
#include "shakujo/model/statement/BlockStatement.h"
#include "shakujo/model/statement/BranchStatement.h"
#include "shakujo/model/statement/BreakStatement.h"
#include "shakujo/model/statement/ContinueStatement.h"
#include "shakujo/model/statement/EmptyStatement.h"
#include "shakujo/model/statement/ExpressionStatement.h"
#include "shakujo/model/statement/ForEachStatement.h"
#include "shakujo/model/statement/ForStatement.h"
#include "shakujo/model/statement/LocalVariableDeclaration.h"
#include "shakujo/model/statement/LogStatement.h"
#include "shakujo/model/statement/RaiseStatement.h"
#include "shakujo/model/statement/ReturnStatement.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/VectorElementDeleteStatement.h"
#include "shakujo/model/statement/VectorElementInsertStatement.h"
#include "shakujo/model/statement/WhileStatement.h"
#include "shakujo/model/statement/ddl/CreateTableStatement.h"
#include "shakujo/model/statement/ddl/DropTableStatement.h"
#include "shakujo/model/statement/dml/DeleteStatement.h"
#include "shakujo/model/statement/dml/EmitStatement.h"
#include "shakujo/model/statement/dml/InsertRelationStatement.h"
#include "shakujo/model/statement/dml/InsertValuesStatement.h"
#include "shakujo/model/statement/dml/UpdateStatement.h"
#include "shakujo/model/statement/transaction/TransactionBlockStatement.h"
#include "shakujo/model/type/ArrayType.h"
#include "shakujo/model/type/BooleanType.h"
#include "shakujo/model/type/CharType.h"
#include "shakujo/model/type/Float32Type.h"
#include "shakujo/model/type/Float64Type.h"
#include "shakujo/model/type/Int32Type.h"
#include "shakujo/model/type/Int64Type.h"
#include "shakujo/model/type/NullType.h"
#include "shakujo/model/type/RelationType.h"
#include "shakujo/model/type/StringType.h"
#include "shakujo/model/type/TupleType.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/VarCharType.h"
#include "shakujo/model/type/VectorType.h"

namespace shakujo::model {
/**
 * @brief a base class of IR model factory.
 */
class IRFactoryBase {
public:
    /**
     * @brief returns a new empty expression::ArrayCreationExpression.
     * @return a created empty node
     * @see expression::ArrayCreationExpression
     */
    virtual std::unique_ptr<expression::ArrayCreationExpression> ArrayCreationExpression();

    /**
     * @brief returns a new expression::ArrayCreationExpression.
     * @param type array type
     * @param values initial array elements
     * @return a created node
     * @see expression::ArrayCreationExpression
     */
    std::unique_ptr<expression::ArrayCreationExpression> ArrayCreationExpression(
            std::unique_ptr<type::Type> type,
            common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values = {});

    /**
     * @brief returns a new empty expression::ArrayElementLoadExpression.
     * @return a created empty node
     * @see expression::ArrayElementLoadExpression
     */
    virtual std::unique_ptr<expression::ArrayElementLoadExpression> ArrayElementLoadExpression();

    /**
     * @brief returns a new expression::ArrayElementLoadExpression.
     * @param data source array
     * @param index element position
     * @return a created node
     * @see expression::ArrayElementLoadExpression
     */
    std::unique_ptr<expression::ArrayElementLoadExpression> ArrayElementLoadExpression(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<expression::Expression> index);

    /**
     * @brief returns a new empty expression::ArrayElementStoreExpression.
     * @return a created empty node
     * @see expression::ArrayElementStoreExpression
     */
    virtual std::unique_ptr<expression::ArrayElementStoreExpression> ArrayElementStoreExpression();

    /**
     * @brief returns a new expression::ArrayElementStoreExpression.
     * @param data target array
     * @param index array element position
     * @param value storing value
     * @return a created node
     * @see expression::ArrayElementStoreExpression
     */
    std::unique_ptr<expression::ArrayElementStoreExpression> ArrayElementStoreExpression(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<expression::Expression> index,
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty expression::AssignExpression.
     * @return a created empty node
     * @see expression::AssignExpression
     */
    virtual std::unique_ptr<expression::AssignExpression> AssignExpression();

    /**
     * @brief returns a new expression::AssignExpression.
     * @param name target variable name
     * @param value assignment value
     * @return a created node
     * @see expression::AssignExpression
     */
    std::unique_ptr<expression::AssignExpression> AssignExpression(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty expression::BinaryOperator.
     * @return a created empty node
     * @see expression::BinaryOperator
     */
    virtual std::unique_ptr<expression::BinaryOperator> BinaryOperator();

    /**
     * @brief returns a new expression::BinaryOperator.
     * @param operator_kind binary operator kind
     * @param left left term
     * @param right right term
     * @return a created node
     * @see expression::BinaryOperator
     */
    std::unique_ptr<expression::BinaryOperator> BinaryOperator(
            expression::BinaryOperator::Kind operator_kind,
            std::unique_ptr<expression::Expression> left,
            std::unique_ptr<expression::Expression> right);

    /**
     * @brief returns a new empty expression::BlockExpression.
     * @return a created empty node
     * @see expression::BlockExpression
     */
    virtual std::unique_ptr<expression::BlockExpression> BlockExpression();

    /**
     * @brief returns a new expression::BlockExpression.
     * @param head initialization statements
     * @param body body expression
     * @return a created node
     * @see expression::BlockExpression
     */
    std::unique_ptr<expression::BlockExpression> BlockExpression(
            common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> head,
            std::unique_ptr<expression::Expression> body);

    /**
     * @brief returns a new empty expression::CaseExpression.
     * @return a created empty node
     * @see expression::CaseExpression
     */
    virtual std::unique_ptr<expression::CaseExpression> CaseExpression();

    /**
     * @brief returns a new expression::CaseExpression.
     * @param cases case clauses
     * @param default_expression default expression
     * @return a created node
     * @see expression::CaseExpression
     */
    std::unique_ptr<expression::CaseExpression> CaseExpression(
            common::util::MoveInitializerList<std::unique_ptr<expression::CaseExpression::Clause>> cases,
            std::unique_ptr<expression::Expression> default_expression);

    /**
     * @brief returns a new empty expression::CaseExpression::Clause.
     * @return a created empty node
     * @see expression::CaseExpression::Clause
     */
    virtual std::unique_ptr<expression::CaseExpression::Clause> CaseExpressionClause();

    /**
     * @brief returns a new expression::CaseExpression::Clause.
     * @param condition case condition
     * @param body body expression
     * @return a created node
     * @see expression::CaseExpression::Clause
     */
    std::unique_ptr<expression::CaseExpression::Clause> CaseExpressionClause(
            std::unique_ptr<expression::Expression> condition,
            std::unique_ptr<expression::Expression> body);

    /**
     * @brief returns a new empty expression::CursorAdvanceExpression.
     * @return a created empty node
     * @see expression::CursorAdvanceExpression
     */
    virtual std::unique_ptr<expression::CursorAdvanceExpression> CursorAdvanceExpression();

    /**
     * @brief returns a new expression::CursorAdvanceExpression.
     * @param data source cursor
     * @return a created node
     * @see expression::CursorAdvanceExpression
     */
    std::unique_ptr<expression::CursorAdvanceExpression> CursorAdvanceExpression(
            std::unique_ptr<expression::Expression> data);

    /**
     * @brief returns a new empty expression::CursorCreationExpression.
     * @return a created empty node
     * @see expression::CursorCreationExpression
     */
    virtual std::unique_ptr<expression::CursorCreationExpression> CursorCreationExpression();

    /**
     * @brief returns a new expression::CursorCreationExpression.
     * @param data source relation
     * @return a created node
     * @see expression::CursorCreationExpression
     */
    std::unique_ptr<expression::CursorCreationExpression> CursorCreationExpression(
            std::unique_ptr<expression::Expression> data);

    /**
     * @brief returns a new empty expression::CursorElementLoadExpression.
     * @return a created empty node
     * @see expression::CursorElementLoadExpression
     */
    virtual std::unique_ptr<expression::CursorElementLoadExpression> CursorElementLoadExpression();

    /**
     * @brief returns a new expression::CursorElementLoadExpression.
     * @param data source cursor
     * @return a created node
     * @see expression::CursorElementLoadExpression
     */
    std::unique_ptr<expression::CursorElementLoadExpression> CursorElementLoadExpression(
            std::unique_ptr<expression::Expression> data);

    /**
     * @brief returns a new empty expression::FunctionCall.
     * @return a created empty node
     * @see expression::FunctionCall
     */
    virtual std::unique_ptr<expression::FunctionCall> FunctionCall();

    /**
     * @brief returns a new expression::FunctionCall.
     * @param name function name
     * @param arguments function arguments
     * @return a created node
     * @see expression::FunctionCall
     */
    std::unique_ptr<expression::FunctionCall> FunctionCall(
            std::unique_ptr<name::Name> name,
            common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> arguments = {});

    /**
     * @brief returns a new empty expression::ImplicitCast.
     * @return a created empty node
     * @see expression::ImplicitCast
     */
    virtual std::unique_ptr<expression::ImplicitCast> ImplicitCast();

    /**
     * @brief returns a new expression::ImplicitCast.
     * @param type target type
     * @param operand operand
     * @return a created node
     * @see expression::ImplicitCast
     */
    std::unique_ptr<expression::ImplicitCast> ImplicitCast(
            std::unique_ptr<common::core::Type> type,
            std::unique_ptr<expression::Expression> operand);

    /**
     * @brief returns a new empty expression::Literal.
     * @return a created empty node
     * @see expression::Literal
     */
    virtual std::unique_ptr<expression::Literal> Literal();

    /**
     * @brief returns a new expression::Literal.
     * @param type literal type
     * @param value literal value
     * @return a created node
     * @see expression::Literal
     */
    std::unique_ptr<expression::Literal> Literal(
            std::unique_ptr<common::core::Type> type,
            std::unique_ptr<common::core::Value> value);

    /**
     * @brief returns a new empty expression::StringOperator.
     * @return a created empty node
     * @see expression::StringOperator
     */
    virtual std::unique_ptr<expression::StringOperator> StringOperator();

    /**
     * @brief returns a new expression::StringOperator.
     * @param operator_kind string operator kind
     * @param string string argument
     * @param operand operand
     * @return a created node
     * @see expression::StringOperator
     */
    std::unique_ptr<expression::StringOperator> StringOperator(
            expression::StringOperator::Kind operator_kind,
            std::string string,
            std::unique_ptr<expression::Expression> operand);

    /**
     * @brief returns a new empty expression::TupleCreationExpression.
     * @return a created empty node
     * @see expression::TupleCreationExpression
     */
    virtual std::unique_ptr<expression::TupleCreationExpression> TupleCreationExpression();

    /**
     * @brief returns a new expression::TupleCreationExpression.
     * @param type tuple type
     * @param values initial tuple elements
     * @return a created node
     * @see expression::TupleCreationExpression
     */
    std::unique_ptr<expression::TupleCreationExpression> TupleCreationExpression(
            std::unique_ptr<type::Type> type,
            common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values = {});

    /**
     * @brief returns a new empty expression::TupleElementLoadExpression.
     * @return a created empty node
     * @see expression::TupleElementLoadExpression
     */
    virtual std::unique_ptr<expression::TupleElementLoadExpression> TupleElementLoadExpression();

    /**
     * @brief returns a new expression::TupleElementLoadExpression.
     * @param data source tuple
     * @param index element index
     * @return a created node
     * @see expression::TupleElementLoadExpression
     */
    std::unique_ptr<expression::TupleElementLoadExpression> TupleElementLoadExpression(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<name::Index> index);

    /**
     * @brief returns a new empty expression::TupleElementStoreExpression.
     * @return a created empty node
     * @see expression::TupleElementStoreExpression
     */
    virtual std::unique_ptr<expression::TupleElementStoreExpression> TupleElementStoreExpression();

    /**
     * @brief returns a new expression::TupleElementStoreExpression.
     * @param data target tuple
     * @param index element index
     * @param value storing value
     * @return a created node
     * @see expression::TupleElementStoreExpression
     */
    std::unique_ptr<expression::TupleElementStoreExpression> TupleElementStoreExpression(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<name::Index> index,
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty expression::TypeOperator.
     * @return a created empty node
     * @see expression::TypeOperator
     */
    virtual std::unique_ptr<expression::TypeOperator> TypeOperator();

    /**
     * @brief returns a new expression::TypeOperator.
     * @param operator_kind type operator kind
     * @param type type argument
     * @param operand operand
     * @return a created node
     * @see expression::TypeOperator
     */
    std::unique_ptr<expression::TypeOperator> TypeOperator(
            expression::TypeOperator::Kind operator_kind,
            std::unique_ptr<type::Type> type,
            std::unique_ptr<expression::Expression> operand);

    /**
     * @brief returns a new empty expression::UnaryOperator.
     * @return a created empty node
     * @see expression::UnaryOperator
     */
    virtual std::unique_ptr<expression::UnaryOperator> UnaryOperator();

    /**
     * @brief returns a new expression::UnaryOperator.
     * @param operator_kind unary operator kind
     * @param operand operand
     * @return a created node
     * @see expression::UnaryOperator
     */
    std::unique_ptr<expression::UnaryOperator> UnaryOperator(
            expression::UnaryOperator::Kind operator_kind,
            std::unique_ptr<expression::Expression> operand);

    /**
     * @brief returns a new empty expression::VariableReference.
     * @return a created empty node
     * @see expression::VariableReference
     */
    virtual std::unique_ptr<expression::VariableReference> VariableReference();

    /**
     * @brief returns a new expression::VariableReference.
     * @param name variable name
     * @return a created node
     * @see expression::VariableReference
     */
    std::unique_ptr<expression::VariableReference> VariableReference(
            std::unique_ptr<name::Name> name);

    /**
     * @brief returns a new empty expression::VectorCreationExpression.
     * @return a created empty node
     * @see expression::VectorCreationExpression
     */
    virtual std::unique_ptr<expression::VectorCreationExpression> VectorCreationExpression();

    /**
     * @brief returns a new expression::VectorCreationExpression.
     * @param type vector type
     * @param values initial vector elements
     * @return a created node
     * @see expression::VectorCreationExpression
     */
    std::unique_ptr<expression::VectorCreationExpression> VectorCreationExpression(
            std::unique_ptr<type::Type> type,
            common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values = {});

    /**
     * @brief returns a new empty expression::VectorElementLoadExpression.
     * @return a created empty node
     * @see expression::VectorElementLoadExpression
     */
    virtual std::unique_ptr<expression::VectorElementLoadExpression> VectorElementLoadExpression();

    /**
     * @brief returns a new expression::VectorElementLoadExpression.
     * @param data source vector
     * @param index element position
     * @return a created node
     * @see expression::VectorElementLoadExpression
     */
    std::unique_ptr<expression::VectorElementLoadExpression> VectorElementLoadExpression(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<expression::Expression> index);

    /**
     * @brief returns a new empty expression::VectorElementStoreExpression.
     * @return a created empty node
     * @see expression::VectorElementStoreExpression
     */
    virtual std::unique_ptr<expression::VectorElementStoreExpression> VectorElementStoreExpression();

    /**
     * @brief returns a new expression::VectorElementStoreExpression.
     * @param data target vector
     * @param index element index
     * @param value storing value
     * @return a created node
     * @see expression::VectorElementStoreExpression
     */
    std::unique_ptr<expression::VectorElementStoreExpression> VectorElementStoreExpression(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<expression::Expression> index,
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty expression::VectorLengthExpression.
     * @return a created empty node
     * @see expression::VectorLengthExpression
     */
    virtual std::unique_ptr<expression::VectorLengthExpression> VectorLengthExpression();

    /**
     * @brief returns a new expression::VectorLengthExpression.
     * @param data source vector
     * @return a created node
     * @see expression::VectorLengthExpression
     */
    std::unique_ptr<expression::VectorLengthExpression> VectorLengthExpression(
            std::unique_ptr<expression::Expression> data);

    /**
     * @brief returns a new empty expression::relation::AggregationExpression.
     * @return a created empty node
     * @see expression::relation::AggregationExpression
     */
    virtual std::unique_ptr<expression::relation::AggregationExpression> AggregationExpression();

    /**
     * @brief returns a new expression::relation::AggregationExpression.
     * @param operand source relation
     * @param keys group keys
     * @param initialize initialization statements
     * @param columns aggregation columns
     * @return a created node
     * @see expression::relation::AggregationExpression
     */
    std::unique_ptr<expression::relation::AggregationExpression> AggregationExpression(
            std::unique_ptr<expression::Expression> operand,
            common::util::MoveInitializerList<std::unique_ptr<name::Index>> keys,
            common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> initialize,
            common::util::MoveInitializerList<std::unique_ptr<expression::relation::AggregationExpression::Column>> columns);

    /**
     * @brief returns a new empty expression::relation::AggregationExpression::Column.
     * @return a created empty node
     * @see expression::relation::AggregationExpression::Column
     */
    virtual std::unique_ptr<expression::relation::AggregationExpression::Column> AggregationExpressionColumn();

    /**
     * @brief returns a new expression::relation::AggregationExpression::Column.
     * @param name column name
     * @param type column type
     * @param function aggregation function
     * @param arguments function arguments
     * @return a created node
     * @see expression::relation::AggregationExpression::Column
     */
    std::unique_ptr<expression::relation::AggregationExpression::Column> AggregationExpressionColumn(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<type::Type> type,
            std::unique_ptr<name::Name> function,
            common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> arguments);

    /**
     * @brief returns a new empty expression::relation::DistinctExpression.
     * @return a created empty node
     * @see expression::relation::DistinctExpression
     */
    virtual std::unique_ptr<expression::relation::DistinctExpression> DistinctExpression();

    /**
     * @brief returns a new expression::relation::DistinctExpression.
     * @param operand source relation
     * @param subsets grouping column indices
     * @return a created node
     * @see expression::relation::DistinctExpression
     */
    std::unique_ptr<expression::relation::DistinctExpression> DistinctExpression(
            std::unique_ptr<expression::Expression> operand,
            common::util::MoveInitializerList<std::unique_ptr<name::Index>> subsets = {});

    /**
     * @brief returns a new empty expression::relation::JoinExpression.
     * @return a created empty node
     * @see expression::relation::JoinExpression
     */
    virtual std::unique_ptr<expression::relation::JoinExpression> JoinExpression();

    /**
     * @brief returns a new expression::relation::JoinExpression.
     * @param operator_kind join operator kind
     * @param left left term
     * @param right right term
     * @param condition join condition
     * @return a created node
     * @see expression::relation::JoinExpression
     */
    std::unique_ptr<expression::relation::JoinExpression> JoinExpression(
            expression::relation::JoinExpression::Kind operator_kind,
            std::unique_ptr<expression::Expression> left,
            std::unique_ptr<expression::Expression> right,
            std::unique_ptr<expression::Expression> condition = {});

    /**
     * @brief returns a new empty expression::relation::LimitExpression.
     * @return a created empty node
     * @see expression::relation::LimitExpression
     */
    virtual std::unique_ptr<expression::relation::LimitExpression> LimitExpression();

    /**
     * @brief returns a new expression::relation::LimitExpression.
     * @param operand source relation
     * @param count the number of limit records
     * @return a created node
     * @see expression::relation::LimitExpression
     */
    std::unique_ptr<expression::relation::LimitExpression> LimitExpression(
            std::unique_ptr<expression::Expression> operand,
            std::size_t count);

    /**
     * @brief returns a new empty expression::relation::OrderExpression.
     * @return a created empty node
     * @see expression::relation::OrderExpression
     */
    virtual std::unique_ptr<expression::relation::OrderExpression> OrderExpression();

    /**
     * @brief returns a new expression::relation::OrderExpression.
     * @param operand source relation
     * @param elements sort orders specification
     * @return a created node
     * @see expression::relation::OrderExpression
     */
    std::unique_ptr<expression::relation::OrderExpression> OrderExpression(
            std::unique_ptr<expression::Expression> operand,
            common::util::MoveInitializerList<std::unique_ptr<expression::relation::OrderExpression::Element>> elements);

    /**
     * @brief returns a new empty expression::relation::OrderExpression::Element.
     * @return a created empty node
     * @see expression::relation::OrderExpression::Element
     */
    virtual std::unique_ptr<expression::relation::OrderExpression::Element> OrderExpressionElement();

    /**
     * @brief returns a new expression::relation::OrderExpression::Element.
     * @param column column index
     * @param direction sort direction
     * @return a created node
     * @see expression::relation::OrderExpression::Element
     */
    std::unique_ptr<expression::relation::OrderExpression::Element> OrderExpressionElement(
            std::unique_ptr<name::Index> column,
            expression::relation::OrderExpression::Direction direction);

    /**
     * @brief returns a new empty expression::relation::ProjectionExpression.
     * @return a created empty node
     * @see expression::relation::ProjectionExpression
     */
    virtual std::unique_ptr<expression::relation::ProjectionExpression> ProjectionExpression();

    /**
     * @brief returns a new expression::relation::ProjectionExpression.
     * @param operand source relation
     * @param initialize initialization statements
     * @param columns projection columns
     * @param alias alias name
     * @return a created node
     * @see expression::relation::ProjectionExpression
     */
    std::unique_ptr<expression::relation::ProjectionExpression> ProjectionExpression(
            std::unique_ptr<expression::Expression> operand,
            common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> initialize,
            common::util::MoveInitializerList<std::unique_ptr<expression::relation::ProjectionExpression::Column>> columns,
            std::unique_ptr<name::SimpleName> alias = {});

    /**
     * @brief returns a new empty expression::relation::ProjectionExpression::Column.
     * @return a created empty node
     * @see expression::relation::ProjectionExpression::Column
     */
    virtual std::unique_ptr<expression::relation::ProjectionExpression::Column> ProjectionExpressionColumn();

    /**
     * @brief returns a new expression::relation::ProjectionExpression::Column.
     * @param value column value
     * @param alias column name
     * @return a created node
     * @see expression::relation::ProjectionExpression::Column
     */
    std::unique_ptr<expression::relation::ProjectionExpression::Column> ProjectionExpressionColumn(
            std::unique_ptr<expression::Expression> value,
            std::unique_ptr<name::SimpleName> alias = {});

    /**
     * @brief returns a new empty expression::relation::ScanExpression.
     * @return a created empty node
     * @see expression::relation::ScanExpression
     */
    virtual std::unique_ptr<expression::relation::ScanExpression> ScanExpression();

    /**
     * @brief returns a new expression::relation::ScanExpression.
     * @param table table name
     * @param alias alias name
     * @return a created node
     * @see expression::relation::ScanExpression
     */
    std::unique_ptr<expression::relation::ScanExpression> ScanExpression(
            std::unique_ptr<name::Name> table,
            std::unique_ptr<name::SimpleName> alias = {});

    /**
     * @brief returns a new empty expression::relation::SelectionExpression.
     * @return a created empty node
     * @see expression::relation::SelectionExpression
     */
    virtual std::unique_ptr<expression::relation::SelectionExpression> SelectionExpression();

    /**
     * @brief returns a new expression::relation::SelectionExpression.
     * @param operand source relation
     * @param condition record filter predicate expression
     * @return a created node
     * @see expression::relation::SelectionExpression
     */
    std::unique_ptr<expression::relation::SelectionExpression> SelectionExpression(
            std::unique_ptr<expression::Expression> operand,
            std::unique_ptr<expression::Expression> condition);

    /**
     * @brief returns a new empty name::Index.
     * @return a created empty node
     * @see name::Index
     */
    virtual std::unique_ptr<name::Index> Index();

    /**
     * @brief returns a new name::Index.
     * @param name element name
     * @param position element position
     * @return a created node
     * @see name::Index
     */
    std::unique_ptr<name::Index> Index(
            std::unique_ptr<name::SimpleName> name,
            std::size_t position);

    /**
     * @brief returns a new empty name::QualifiedName.
     * @return a created empty node
     * @see name::QualifiedName
     */
    virtual std::unique_ptr<name::QualifiedName> QualifiedName();

    /**
     * @brief returns a new name::QualifiedName.
     * @param qualifier qualifier
     * @param name name
     * @return a created node
     * @see name::QualifiedName
     */
    std::unique_ptr<name::QualifiedName> QualifiedName(
            std::unique_ptr<name::Name> qualifier,
            std::unique_ptr<name::SimpleName> name);

    /**
     * @brief returns a new empty name::SimpleName.
     * @return a created empty node
     * @see name::SimpleName
     */
    virtual std::unique_ptr<name::SimpleName> SimpleName();

    /**
     * @brief returns a new name::SimpleName.
     * @param token name token
     * @return a created node
     * @see name::SimpleName
     */
    std::unique_ptr<name::SimpleName> SimpleName(
            std::string token);

    /**
     * @brief returns a new empty program::GlobalFunctionDeclaration.
     * @return a created empty node
     * @see program::GlobalFunctionDeclaration
     */
    virtual std::unique_ptr<program::GlobalFunctionDeclaration> GlobalFunctionDeclaration();

    /**
     * @brief returns a new program::GlobalFunctionDeclaration.
     * @param name function name
     * @param type function return type
     * @param parameters function parameters specification
     * @param body function body
     * @param attributes function attributes
     * @return a created node
     * @see program::GlobalFunctionDeclaration
     */
    std::unique_ptr<program::GlobalFunctionDeclaration> GlobalFunctionDeclaration(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<type::Type> type,
            common::util::MoveInitializerList<std::unique_ptr<program::GlobalFunctionDeclaration::Parameter>> parameters,
            std::unique_ptr<statement::Statement> body,
            std::initializer_list<program::GlobalFunctionDeclaration::Attribute> attributes = {});

    /**
     * @brief returns a new empty program::GlobalFunctionDeclaration::Parameter.
     * @return a created empty node
     * @see program::GlobalFunctionDeclaration::Parameter
     */
    virtual std::unique_ptr<program::GlobalFunctionDeclaration::Parameter> GlobalFunctionDeclarationParameter();

    /**
     * @brief returns a new program::GlobalFunctionDeclaration::Parameter.
     * @param name parameter name
     * @param type parameter type
     * @param attributes parameter attributes
     * @return a created node
     * @see program::GlobalFunctionDeclaration::Parameter
     */
    std::unique_ptr<program::GlobalFunctionDeclaration::Parameter> GlobalFunctionDeclarationParameter(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<type::Type> type,
            std::initializer_list<program::GlobalFunctionDeclaration::Parameter::Attribute> attributes = {});

    /**
     * @brief returns a new empty program::GlobalVariableDeclaration.
     * @return a created empty node
     * @see program::GlobalVariableDeclaration
     */
    virtual std::unique_ptr<program::GlobalVariableDeclaration> GlobalVariableDeclaration();

    /**
     * @brief returns a new program::GlobalVariableDeclaration.
     * @param name variable name
     * @param type variable type
     * @param value initial value
     * @param attributes variable attributes
     * @return a created node
     * @see program::GlobalVariableDeclaration
     */
    std::unique_ptr<program::GlobalVariableDeclaration> GlobalVariableDeclaration(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<type::Type> type,
            std::unique_ptr<expression::Expression> value = {},
            std::initializer_list<program::GlobalVariableDeclaration::Attribute> attributes = {});

    /**
     * @brief returns a new empty program::Program.
     * @return a created empty node
     * @see program::Program
     */
    virtual std::unique_ptr<program::Program> Program();

    /**
     * @brief returns a new program::Program.
     * @param declarations global declarations
     * @param main program entry
     * @return a created node
     * @see program::Program
     */
    std::unique_ptr<program::Program> Program(
            common::util::MoveInitializerList<std::unique_ptr<program::GlobalDeclaration>> declarations,
            std::unique_ptr<statement::Statement> main);

    /**
     * @brief returns a new empty statement::AnchorDeclaration.
     * @return a created empty node
     * @see statement::AnchorDeclaration
     */
    virtual std::unique_ptr<statement::AnchorDeclaration> AnchorDeclaration();

    /**
     * @brief returns a new statement::AnchorDeclaration.
     * @param name anchor name
     * @param body body statement
     * @return a created node
     * @see statement::AnchorDeclaration
     */
    std::unique_ptr<statement::AnchorDeclaration> AnchorDeclaration(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<statement::Statement> body);

    /**
     * @brief returns a new empty statement::BlockStatement.
     * @return a created empty node
     * @see statement::BlockStatement
     */
    virtual std::unique_ptr<statement::BlockStatement> BlockStatement();

    /**
     * @brief returns a new statement::BlockStatement.
     * @param elements body statements
     * @return a created node
     * @see statement::BlockStatement
     */
    std::unique_ptr<statement::BlockStatement> BlockStatement(
            common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> elements);

    /**
     * @brief returns a new empty statement::BranchStatement.
     * @return a created empty node
     * @see statement::BranchStatement
     */
    virtual std::unique_ptr<statement::BranchStatement> BranchStatement();

    /**
     * @brief returns a new statement::BranchStatement.
     * @param condition branch condition
     * @param then_statement true statement
     * @param else_statement false statement
     * @return a created node
     * @see statement::BranchStatement
     */
    std::unique_ptr<statement::BranchStatement> BranchStatement(
            std::unique_ptr<expression::Expression> condition,
            std::unique_ptr<statement::Statement> then_statement,
            std::unique_ptr<statement::Statement> else_statement = {});

    /**
     * @brief returns a new empty statement::BreakStatement.
     * @return a created empty node
     * @see statement::BreakStatement
     */
    virtual std::unique_ptr<statement::BreakStatement> BreakStatement();

    /**
     * @brief returns a new statement::BreakStatement.
     * @param anchor target anchor
     * @return a created node
     * @see statement::BreakStatement
     */
    std::unique_ptr<statement::BreakStatement> BreakStatement(
            std::unique_ptr<name::Name> anchor);

    /**
     * @brief returns a new empty statement::ContinueStatement.
     * @return a created empty node
     * @see statement::ContinueStatement
     */
    virtual std::unique_ptr<statement::ContinueStatement> ContinueStatement();

    /**
     * @brief returns a new statement::ContinueStatement.
     * @param anchor target anchor
     * @return a created node
     * @see statement::ContinueStatement
     */
    std::unique_ptr<statement::ContinueStatement> ContinueStatement(
            std::unique_ptr<name::Name> anchor);

    /**
     * @brief returns a new empty statement::EmptyStatement.
     * @return a created empty node
     * @see statement::EmptyStatement
     */
    virtual std::unique_ptr<statement::EmptyStatement> EmptyStatement();

    /**
     * @brief returns a new empty statement::ExpressionStatement.
     * @return a created empty node
     * @see statement::ExpressionStatement
     */
    virtual std::unique_ptr<statement::ExpressionStatement> ExpressionStatement();

    /**
     * @brief returns a new statement::ExpressionStatement.
     * @param body body expression
     * @return a created node
     * @see statement::ExpressionStatement
     */
    std::unique_ptr<statement::ExpressionStatement> ExpressionStatement(
            std::unique_ptr<expression::Expression> body);

    /**
     * @brief returns a new empty statement::ForEachStatement.
     * @return a created empty node
     * @see statement::ForEachStatement
     */
    virtual std::unique_ptr<statement::ForEachStatement> ForEachStatement();

    /**
     * @brief returns a new statement::ForEachStatement.
     * @param name variable name
     * @param type variable type
     * @param enumeration target sequence
     * @param body body statement
     * @return a created node
     * @see statement::ForEachStatement
     */
    std::unique_ptr<statement::ForEachStatement> ForEachStatement(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<type::Type> type,
            std::unique_ptr<expression::Expression> enumeration,
            std::unique_ptr<statement::Statement> body);

    /**
     * @brief returns a new empty statement::ForStatement.
     * @return a created empty node
     * @see statement::ForStatement
     */
    virtual std::unique_ptr<statement::ForStatement> ForStatement();

    /**
     * @brief returns a new statement::ForStatement.
     * @param initialize initialization statements
     * @param condition loop condition
     * @param update update statements
     * @param body body statement
     * @return a created node
     * @see statement::ForStatement
     */
    std::unique_ptr<statement::ForStatement> ForStatement(
            common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> initialize,
            std::unique_ptr<expression::Expression> condition,
            common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> update,
            std::unique_ptr<statement::Statement> body);

    /**
     * @brief returns a new empty statement::LocalVariableDeclaration.
     * @return a created empty node
     * @see statement::LocalVariableDeclaration
     */
    virtual std::unique_ptr<statement::LocalVariableDeclaration> LocalVariableDeclaration();

    /**
     * @brief returns a new statement::LocalVariableDeclaration.
     * @param name variable name
     * @param type variable type
     * @param value initial value
     * @param attributes variable attributes
     * @return a created node
     * @see statement::LocalVariableDeclaration
     */
    std::unique_ptr<statement::LocalVariableDeclaration> LocalVariableDeclaration(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<type::Type> type,
            std::unique_ptr<expression::Expression> value = {},
            std::initializer_list<statement::LocalVariableDeclaration::Attribute> attributes = {});

    /**
     * @brief returns a new empty statement::LogStatement.
     * @return a created empty node
     * @see statement::LogStatement
     */
    virtual std::unique_ptr<statement::LogStatement> LogStatement();

    /**
     * @brief returns a new statement::LogStatement.
     * @param level log level
     * @param value log message
     * @return a created node
     * @see statement::LogStatement
     */
    std::unique_ptr<statement::LogStatement> LogStatement(
            statement::LogStatement::Level level,
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty statement::RaiseStatement.
     * @return a created empty node
     * @see statement::RaiseStatement
     */
    virtual std::unique_ptr<statement::RaiseStatement> RaiseStatement();

    /**
     * @brief returns a new statement::RaiseStatement.
     * @param value exception
     * @return a created node
     * @see statement::RaiseStatement
     */
    std::unique_ptr<statement::RaiseStatement> RaiseStatement(
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty statement::ReturnStatement.
     * @return a created empty node
     * @see statement::ReturnStatement
     */
    virtual std::unique_ptr<statement::ReturnStatement> ReturnStatement();

    /**
     * @brief returns a new statement::ReturnStatement.
     * @param value return value
     * @return a created node
     * @see statement::ReturnStatement
     */
    std::unique_ptr<statement::ReturnStatement> ReturnStatement(
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty statement::VectorElementDeleteStatement.
     * @return a created empty node
     * @see statement::VectorElementDeleteStatement
     */
    virtual std::unique_ptr<statement::VectorElementDeleteStatement> VectorElementDeleteStatement();

    /**
     * @brief returns a new statement::VectorElementDeleteStatement.
     * @param data target vector
     * @param begin begining index
     * @param end ending index
     * @return a created node
     * @see statement::VectorElementDeleteStatement
     */
    std::unique_ptr<statement::VectorElementDeleteStatement> VectorElementDeleteStatement(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<expression::Expression> begin,
            std::unique_ptr<expression::Expression> end);

    /**
     * @brief returns a new empty statement::VectorElementInsertStatement.
     * @return a created empty node
     * @see statement::VectorElementInsertStatement
     */
    virtual std::unique_ptr<statement::VectorElementInsertStatement> VectorElementInsertStatement();

    /**
     * @brief returns a new statement::VectorElementInsertStatement.
     * @param data target vector
     * @param index insertion position
     * @param values inserting values
     * @return a created node
     * @see statement::VectorElementInsertStatement
     */
    std::unique_ptr<statement::VectorElementInsertStatement> VectorElementInsertStatement(
            std::unique_ptr<expression::Expression> data,
            std::unique_ptr<expression::Expression> index,
            common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values);

    /**
     * @brief returns a new empty statement::WhileStatement.
     * @return a created empty node
     * @see statement::WhileStatement
     */
    virtual std::unique_ptr<statement::WhileStatement> WhileStatement();

    /**
     * @brief returns a new statement::WhileStatement.
     * @param condition loop condition
     * @param body body statement
     * @return a created node
     * @see statement::WhileStatement
     */
    std::unique_ptr<statement::WhileStatement> WhileStatement(
            std::unique_ptr<expression::Expression> condition,
            std::unique_ptr<statement::Statement> body);

    /**
     * @brief returns a new empty statement::ddl::CreateTableStatement.
     * @return a created empty node
     * @see statement::ddl::CreateTableStatement
     */
    virtual std::unique_ptr<statement::ddl::CreateTableStatement> CreateTableStatement();

    /**
     * @brief returns a new statement::ddl::CreateTableStatement.
     * @param table table name
     * @param columns columns specification
     * @param attributes create table attributes
     * @param primary_keys primary key settings
     * @return a created node
     * @see statement::ddl::CreateTableStatement
     */
    std::unique_ptr<statement::ddl::CreateTableStatement> CreateTableStatement(
            std::unique_ptr<name::Name> table,
            common::util::MoveInitializerList<std::unique_ptr<statement::ddl::CreateTableStatement::Column>> columns,
            std::initializer_list<statement::ddl::CreateTableStatement::Attribute> attributes = {},
            common::util::MoveInitializerList<std::unique_ptr<statement::ddl::CreateTableStatement::PrimaryKey>> primary_keys = {});

    /**
     * @brief returns a new empty statement::ddl::CreateTableStatement::Column.
     * @return a created empty node
     * @see statement::ddl::CreateTableStatement::Column
     */
    virtual std::unique_ptr<statement::ddl::CreateTableStatement::Column> CreateTableStatementColumn();

    /**
     * @brief returns a new statement::ddl::CreateTableStatement::Column.
     * @param name column name
     * @param type column type
     * @param value column default value
     * @param attributes column attributes
     * @return a created node
     * @see statement::ddl::CreateTableStatement::Column
     */
    std::unique_ptr<statement::ddl::CreateTableStatement::Column> CreateTableStatementColumn(
            std::unique_ptr<name::SimpleName> name,
            std::unique_ptr<type::Type> type,
            std::unique_ptr<expression::Expression> value = {},
            std::initializer_list<statement::ddl::CreateTableStatement::Column::Attribute> attributes = {});

    /**
     * @brief returns a new empty statement::ddl::CreateTableStatement::PrimaryKey.
     * @return a created empty node
     * @see statement::ddl::CreateTableStatement::PrimaryKey
     */
    virtual std::unique_ptr<statement::ddl::CreateTableStatement::PrimaryKey> CreateTableStatementPrimaryKey();

    /**
     * @brief returns a new statement::ddl::CreateTableStatement::PrimaryKey.
     * @param name key column name
     * @param direction column direction
     * @return a created node
     * @see statement::ddl::CreateTableStatement::PrimaryKey
     */
    std::unique_ptr<statement::ddl::CreateTableStatement::PrimaryKey> CreateTableStatementPrimaryKey(
            std::unique_ptr<name::SimpleName> name,
            statement::ddl::CreateTableStatement::PrimaryKey::Direction direction = statement::ddl::CreateTableStatement::PrimaryKey::Direction::DONT_CARE);

    /**
     * @brief returns a new empty statement::ddl::DropTableStatement.
     * @return a created empty node
     * @see statement::ddl::DropTableStatement
     */
    virtual std::unique_ptr<statement::ddl::DropTableStatement> DropTableStatement();

    /**
     * @brief returns a new statement::ddl::DropTableStatement.
     * @param table table name
     * @param attributes drop table attributes
     * @return a created node
     * @see statement::ddl::DropTableStatement
     */
    std::unique_ptr<statement::ddl::DropTableStatement> DropTableStatement(
            std::unique_ptr<name::Name> table,
            std::initializer_list<statement::ddl::DropTableStatement::Attribute> attributes = {});

    /**
     * @brief returns a new empty statement::dml::DeleteStatement.
     * @return a created empty node
     * @see statement::dml::DeleteStatement
     */
    virtual std::unique_ptr<statement::dml::DeleteStatement> DeleteStatement();

    /**
     * @brief returns a new statement::dml::DeleteStatement.
     * @param table table name
     * @param condition row filter predicate expression
     * @return a created node
     * @see statement::dml::DeleteStatement
     */
    std::unique_ptr<statement::dml::DeleteStatement> DeleteStatement(
            std::unique_ptr<name::Name> table,
            std::unique_ptr<expression::Expression> condition = {});

    /**
     * @brief returns a new empty statement::dml::EmitStatement.
     * @return a created empty node
     * @see statement::dml::EmitStatement
     */
    virtual std::unique_ptr<statement::dml::EmitStatement> EmitStatement();

    /**
     * @brief returns a new statement::dml::EmitStatement.
     * @param source target relation
     * @return a created node
     * @see statement::dml::EmitStatement
     */
    std::unique_ptr<statement::dml::EmitStatement> EmitStatement(
            std::unique_ptr<expression::Expression> source);

    /**
     * @brief returns a new empty statement::dml::InsertRelationStatement.
     * @return a created empty node
     * @see statement::dml::InsertRelationStatement
     */
    virtual std::unique_ptr<statement::dml::InsertRelationStatement> InsertRelationStatement();

    /**
     * @brief returns a new statement::dml::InsertRelationStatement.
     * @param table table name
     * @param columns destination column names
     * @param source source relation
     * @return a created node
     * @see statement::dml::InsertRelationStatement
     */
    std::unique_ptr<statement::dml::InsertRelationStatement> InsertRelationStatement(
            std::unique_ptr<name::Name> table,
            common::util::MoveInitializerList<std::unique_ptr<name::SimpleName>> columns,
            std::unique_ptr<expression::Expression> source);

    /**
     * @brief returns a new empty statement::dml::InsertValuesStatement.
     * @return a created empty node
     * @see statement::dml::InsertValuesStatement
     */
    virtual std::unique_ptr<statement::dml::InsertValuesStatement> InsertValuesStatement();

    /**
     * @brief returns a new statement::dml::InsertValuesStatement.
     * @param table table name
     * @param columns destination column specifications
     * @return a created node
     * @see statement::dml::InsertValuesStatement
     */
    std::unique_ptr<statement::dml::InsertValuesStatement> InsertValuesStatement(
            std::unique_ptr<name::Name> table,
            common::util::MoveInitializerList<std::unique_ptr<statement::dml::InsertValuesStatement::Column>> columns);

    /**
     * @brief returns a new empty statement::dml::InsertValuesStatement::Column.
     * @return a created empty node
     * @see statement::dml::InsertValuesStatement::Column
     */
    virtual std::unique_ptr<statement::dml::InsertValuesStatement::Column> InsertValuesStatementColumn();

    /**
     * @brief returns a new statement::dml::InsertValuesStatement::Column.
     * @param name destination column name
     * @param value column value
     * @return a created node
     * @see statement::dml::InsertValuesStatement::Column
     */
    std::unique_ptr<statement::dml::InsertValuesStatement::Column> InsertValuesStatementColumn(
            std::unique_ptr<name::SimpleName> name,
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty statement::dml::UpdateStatement.
     * @return a created empty node
     * @see statement::dml::UpdateStatement
     */
    virtual std::unique_ptr<statement::dml::UpdateStatement> UpdateStatement();

    /**
     * @brief returns a new statement::dml::UpdateStatement.
     * @param table table name
     * @param columns destination column specification
     * @param condition record filter predicate expression
     * @return a created node
     * @see statement::dml::UpdateStatement
     */
    std::unique_ptr<statement::dml::UpdateStatement> UpdateStatement(
            std::unique_ptr<name::Name> table,
            common::util::MoveInitializerList<std::unique_ptr<statement::dml::UpdateStatement::Column>> columns,
            std::unique_ptr<expression::Expression> condition = {});

    /**
     * @brief returns a new empty statement::dml::UpdateStatement::Column.
     * @return a created empty node
     * @see statement::dml::UpdateStatement::Column
     */
    virtual std::unique_ptr<statement::dml::UpdateStatement::Column> UpdateStatementColumn();

    /**
     * @brief returns a new statement::dml::UpdateStatement::Column.
     * @param name destination column name
     * @param value column value
     * @return a created node
     * @see statement::dml::UpdateStatement::Column
     */
    std::unique_ptr<statement::dml::UpdateStatement::Column> UpdateStatementColumn(
            std::unique_ptr<name::Name> name,
            std::unique_ptr<expression::Expression> value);

    /**
     * @brief returns a new empty statement::transaction::TransactionBlockStatement.
     * @return a created empty node
     * @see statement::transaction::TransactionBlockStatement
     */
    virtual std::unique_ptr<statement::transaction::TransactionBlockStatement> TransactionBlockStatement();

    /**
     * @brief returns a new statement::transaction::TransactionBlockStatement.
     * @param body body statement
     * @return a created node
     * @see statement::transaction::TransactionBlockStatement
     */
    std::unique_ptr<statement::transaction::TransactionBlockStatement> TransactionBlockStatement(
            std::unique_ptr<statement::Statement> body);

    /**
     * @brief returns a new empty type::ArrayType.
     * @return a created empty node
     * @see type::ArrayType
     */
    virtual std::unique_ptr<type::ArrayType> ArrayType();

    /**
     * @brief returns a new type::ArrayType.
     * @param element_type element type
     * @param size number of elements
     * @return a created node
     * @see type::ArrayType
     */
    std::unique_ptr<type::ArrayType> ArrayType(
            std::unique_ptr<type::Type> element_type,
            std::size_t size);

    /**
     * @brief returns a new empty type::BooleanType.
     * @return a created empty node
     * @see type::BooleanType
     */
    virtual std::unique_ptr<type::BooleanType> BooleanType();

    /**
     * @brief returns a new empty type::CharType.
     * @return a created empty node
     * @see type::CharType
     */
    virtual std::unique_ptr<type::CharType> CharType();

    /**
     * @brief returns a new type::CharType.
     * @param size bytes length
     * @return a created node
     * @see type::CharType
     */
    std::unique_ptr<type::CharType> CharType(
            std::size_t size);

    /**
     * @brief returns a new empty type::Float32Type.
     * @return a created empty node
     * @see type::Float32Type
     */
    virtual std::unique_ptr<type::Float32Type> Float32Type();

    /**
     * @brief returns a new empty type::Float64Type.
     * @return a created empty node
     * @see type::Float64Type
     */
    virtual std::unique_ptr<type::Float64Type> Float64Type();

    /**
     * @brief returns a new empty type::Int32Type.
     * @return a created empty node
     * @see type::Int32Type
     */
    virtual std::unique_ptr<type::Int32Type> Int32Type();

    /**
     * @brief returns a new empty type::Int64Type.
     * @return a created empty node
     * @see type::Int64Type
     */
    virtual std::unique_ptr<type::Int64Type> Int64Type();

    /**
     * @brief returns a new empty type::NullType.
     * @return a created empty node
     * @see type::NullType
     */
    virtual std::unique_ptr<type::NullType> NullType();

    /**
     * @brief returns a new empty type::RelationType.
     * @return a created empty node
     * @see type::RelationType
     */
    virtual std::unique_ptr<type::RelationType> RelationType();

    /**
     * @brief returns a new type::RelationType.
     * @param element_type element type
     * @return a created node
     * @see type::RelationType
     */
    std::unique_ptr<type::RelationType> RelationType(
            std::unique_ptr<type::TupleType> element_type);

    /**
     * @brief returns a new empty type::StringType.
     * @return a created empty node
     * @see type::StringType
     */
    virtual std::unique_ptr<type::StringType> StringType();

    /**
     * @brief returns a new empty type::TupleType.
     * @return a created empty node
     * @see type::TupleType
     */
    virtual std::unique_ptr<type::TupleType> TupleType();

    /**
     * @brief returns a new type::TupleType.
     * @param elements tuple elements
     * @return a created node
     * @see type::TupleType
     */
    std::unique_ptr<type::TupleType> TupleType(
            common::util::MoveInitializerList<std::unique_ptr<type::TupleType::Element>> elements);

    /**
     * @brief returns a new empty type::TupleType::Element.
     * @return a created empty node
     * @see type::TupleType::Element
     */
    virtual std::unique_ptr<type::TupleType::Element> TupleTypeElement();

    /**
     * @brief returns a new type::TupleType::Element.
     * @param name element name
     * @param type element type
     * @return a created node
     * @see type::TupleType::Element
     */
    std::unique_ptr<type::TupleType::Element> TupleTypeElement(
            std::unique_ptr<name::SimpleName> name,
            std::unique_ptr<type::Type> type);

    /**
     * @brief returns a new empty type::VarCharType.
     * @return a created empty node
     * @see type::VarCharType
     */
    virtual std::unique_ptr<type::VarCharType> VarCharType();

    /**
     * @brief returns a new type::VarCharType.
     * @param size bytes length
     * @return a created node
     * @see type::VarCharType
     */
    std::unique_ptr<type::VarCharType> VarCharType(
            std::size_t size);

    /**
     * @brief returns a new empty type::VectorType.
     * @return a created empty node
     * @see type::VectorType
     */
    virtual std::unique_ptr<type::VectorType> VectorType();

    /**
     * @brief returns a new type::VectorType.
     * @param element_type element type
     * @return a created node
     * @see type::VectorType
     */
    std::unique_ptr<type::VectorType> VectorType(
            std::unique_ptr<type::Type> element_type);

};
}  // namespace shakujo::model

#endif  // SHAKUJO_MODEL_I_R_FACTORY_BASE_H_
