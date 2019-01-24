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
#ifndef SHAKUJO_MODEL_UTIL_NODE_SERIALIZER_BASE_H_
#define SHAKUJO_MODEL_UTIL_NODE_SERIALIZER_BASE_H_

#include <memory>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/util/DataSerializer.h"
#include "shakujo/model/Node.h"
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
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/key/VariableKey.h"
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

namespace shakujo::model::util {
/**
 * @brief An abstract implementation of IR node serializer.
 */
class NodeSerializerBase {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief constructs a new object.
     */
    NodeSerializerBase();

    /**
     * @brief destroys this object.
     */
    virtual ~NodeSerializerBase() noexcept;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    NodeSerializerBase(NodeSerializerBase const& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    NodeSerializerBase& operator=(NodeSerializerBase const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    NodeSerializerBase(NodeSerializerBase&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    NodeSerializerBase& operator=(NodeSerializerBase&& other) noexcept = default;

    /**
     * @brief returns whether or not show qualified kind for nested models.
     * @return true if show qualified kind for nested models
     * @return false otherwise
     */
    virtual bool show_qualified_kind() const;

    /**
     * @brief sets whether or not show qualified kind for nested models.
     * @param on whether or not show qualified kind (default: false)
     * @return this
     */
    virtual NodeSerializerBase& show_qualified_kind(bool on);

    /**
     * @brief returns whether or not show kind for enumerations.
     * @return true if show kind for enumerations
     * @return false otherwise
     */
    virtual bool show_enum_kind() const;

    /**
     * @brief sets whether or not show kind for enumerations.
     * @param on whether or not show enum kind (default: false)
     * @return this
     */
    virtual NodeSerializerBase& show_enum_kind(bool on);

    /**
     * @brief returns whether or not show kind for fragment models.
     * @return true if show kind for fragment models
     * @return false otherwise
     */
    virtual bool show_fragment_kind() const;

    /**
     * @brief sets whether or not show kind for fragment models.
     * @param on whether or not show fragment kind (default: false)
     * @return this
     */
    virtual NodeSerializerBase& show_fragment_kind(bool on);

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::Type const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::Value const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, Node const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::ArrayCreationExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::ArrayElementLoadExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::ArrayElementStoreExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::AssignExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::BinaryOperator const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::BinaryOperator::Kind value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::BlockExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::CaseExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::CaseExpression::Clause const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::CursorAdvanceExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::CursorCreationExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::CursorElementLoadExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::Expression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::FunctionCall const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::ImplicitCast const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::Literal const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::StringOperator const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::StringOperator::Kind value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::TupleCreationExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::TupleElementLoadExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::TupleElementStoreExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::TypeOperator const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::TypeOperator::Kind value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::UnaryOperator const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::UnaryOperator::Kind value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::VariableReference const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::VectorCreationExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::VectorElementLoadExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::VectorElementStoreExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::VectorLengthExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::AggregationExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::AggregationExpression::Column const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::DistinctExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::JoinExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::JoinExpression::Kind value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::LimitExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::OrderExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::OrderExpression::Direction value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::OrderExpression::Element const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::ProjectionExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::ProjectionExpression::Column const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::ScanExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, expression::relation::SelectionExpression const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, key::ExpressionKey const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, key::FunctionKey const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, key::RelationKey const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, key::VariableKey const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, name::Index const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, name::Name const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, name::QualifiedName const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, name::SimpleName const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::GlobalDeclaration const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration::Attribute value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration::Parameter const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration::Parameter::Attribute value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::GlobalVariableDeclaration const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::GlobalVariableDeclaration::Attribute value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, program::Program const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::AnchorDeclaration const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::BlockStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::BranchStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::BreakStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ContinueStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::EmptyStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ExpressionStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ForEachStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ForStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::LocalVariableDeclaration const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::LocalVariableDeclaration::Attribute value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::LogStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::LogStatement::Level value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::RaiseStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ReturnStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::Statement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::VectorElementDeleteStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::VectorElementInsertStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::WhileStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::Attribute value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::Column const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::Column::Attribute value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::PrimaryKey const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::PrimaryKey::Direction value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::DropTableStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::ddl::DropTableStatement::Attribute value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::dml::DeleteStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::dml::EmitStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::dml::InsertRelationStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::dml::InsertValuesStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::dml::InsertValuesStatement::Column const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::dml::UpdateStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::dml::UpdateStatement::Column const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, statement::transaction::TransactionBlockStatement const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::ArrayType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::BooleanType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::CharType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::Float32Type const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::Float64Type const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::Int32Type const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::Int64Type const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::NullType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::RelationType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::StringType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::TupleType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::TupleType::Element const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::Type const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::VarCharType const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, type::VectorType const* value);  // NOLINT

};

}  // namespace shakujo::model::util

#endif  // SHAKUJO_MODEL_UTIL_NODE_SERIALIZER_BASE_H_
