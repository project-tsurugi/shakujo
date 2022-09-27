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
#ifndef SHAKUJO_MODEL_UTIL_CONST_NODE_WALKER_H_
#define SHAKUJO_MODEL_UTIL_CONST_NODE_WALKER_H_

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
#include "shakujo/model/name/Index.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/QualifiedName.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/program/Comment.h"
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
#include "shakujo/model/type/BinaryType.h"
#include "shakujo/model/type/BooleanType.h"
#include "shakujo/model/type/CharType.h"
#include "shakujo/model/type/DateType.h"
#include "shakujo/model/type/DecimalType.h"
#include "shakujo/model/type/Float32Type.h"
#include "shakujo/model/type/Float64Type.h"
#include "shakujo/model/type/Int32Type.h"
#include "shakujo/model/type/Int64Type.h"
#include "shakujo/model/type/NullType.h"
#include "shakujo/model/type/RelationType.h"
#include "shakujo/model/type/StringType.h"
#include "shakujo/model/type/TimeType.h"
#include "shakujo/model/type/TimestampType.h"
#include "shakujo/model/type/TupleType.h"
#include "shakujo/model/type/VarBinaryType.h"
#include "shakujo/model/type/VarCharType.h"
#include "shakujo/model/type/VectorType.h"

namespace shakujo::model::util {
/**
 * @brief An IR node walker.
 */
class ConstNodeWalker {
protected:
    /**
     * @brief callback on enter into GlobalFunctionDeclaration.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see program::GlobalFunctionDeclaration
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] program::GlobalFunctionDeclaration const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from GlobalFunctionDeclaration.
     * @param node the processing target
     * @see program::GlobalFunctionDeclaration
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] program::GlobalFunctionDeclaration const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into GlobalVariableDeclaration.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see program::GlobalVariableDeclaration
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] program::GlobalVariableDeclaration const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from GlobalVariableDeclaration.
     * @param node the processing target
     * @see program::GlobalVariableDeclaration
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] program::GlobalVariableDeclaration const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into QualifiedName.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see name::QualifiedName
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] name::QualifiedName const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from QualifiedName.
     * @param node the processing target
     * @see name::QualifiedName
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] name::QualifiedName const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into SimpleName.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see name::SimpleName
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] name::SimpleName const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from SimpleName.
     * @param node the processing target
     * @see name::SimpleName
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] name::SimpleName const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ArrayCreationExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::ArrayCreationExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::ArrayCreationExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ArrayCreationExpression.
     * @param node the processing target
     * @see expression::ArrayCreationExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::ArrayCreationExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ArrayElementLoadExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::ArrayElementLoadExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::ArrayElementLoadExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ArrayElementLoadExpression.
     * @param node the processing target
     * @see expression::ArrayElementLoadExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::ArrayElementLoadExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ArrayElementStoreExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::ArrayElementStoreExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::ArrayElementStoreExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ArrayElementStoreExpression.
     * @param node the processing target
     * @see expression::ArrayElementStoreExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::ArrayElementStoreExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into AssignExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::AssignExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::AssignExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from AssignExpression.
     * @param node the processing target
     * @see expression::AssignExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::AssignExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into BinaryOperator.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::BinaryOperator
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::BinaryOperator const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from BinaryOperator.
     * @param node the processing target
     * @see expression::BinaryOperator
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::BinaryOperator const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into BlockExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::BlockExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::BlockExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from BlockExpression.
     * @param node the processing target
     * @see expression::BlockExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::BlockExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into CaseExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::CaseExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::CaseExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from CaseExpression.
     * @param node the processing target
     * @see expression::CaseExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::CaseExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into CursorAdvanceExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::CursorAdvanceExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::CursorAdvanceExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from CursorAdvanceExpression.
     * @param node the processing target
     * @see expression::CursorAdvanceExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::CursorAdvanceExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into CursorCreationExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::CursorCreationExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::CursorCreationExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from CursorCreationExpression.
     * @param node the processing target
     * @see expression::CursorCreationExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::CursorCreationExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into CursorElementLoadExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::CursorElementLoadExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::CursorElementLoadExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from CursorElementLoadExpression.
     * @param node the processing target
     * @see expression::CursorElementLoadExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::CursorElementLoadExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into FunctionCall.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::FunctionCall
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::FunctionCall const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from FunctionCall.
     * @param node the processing target
     * @see expression::FunctionCall
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::FunctionCall const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ImplicitCast.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::ImplicitCast
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::ImplicitCast const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ImplicitCast.
     * @param node the processing target
     * @see expression::ImplicitCast
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::ImplicitCast const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into Literal.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::Literal
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::Literal const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from Literal.
     * @param node the processing target
     * @see expression::Literal
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::Literal const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into Placeholder.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::Placeholder
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::Placeholder const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from Placeholder.
     * @param node the processing target
     * @see expression::Placeholder
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::Placeholder const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into TupleCreationExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::TupleCreationExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::TupleCreationExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TupleCreationExpression.
     * @param node the processing target
     * @see expression::TupleCreationExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::TupleCreationExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into TupleElementLoadExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::TupleElementLoadExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::TupleElementLoadExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TupleElementLoadExpression.
     * @param node the processing target
     * @see expression::TupleElementLoadExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::TupleElementLoadExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into TupleElementStoreExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::TupleElementStoreExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::TupleElementStoreExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TupleElementStoreExpression.
     * @param node the processing target
     * @see expression::TupleElementStoreExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::TupleElementStoreExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into TypeOperator.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::TypeOperator
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::TypeOperator const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TypeOperator.
     * @param node the processing target
     * @see expression::TypeOperator
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::TypeOperator const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into UnaryOperator.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::UnaryOperator
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::UnaryOperator const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from UnaryOperator.
     * @param node the processing target
     * @see expression::UnaryOperator
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::UnaryOperator const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VariableReference.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::VariableReference
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::VariableReference const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VariableReference.
     * @param node the processing target
     * @see expression::VariableReference
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::VariableReference const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VectorCreationExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::VectorCreationExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::VectorCreationExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VectorCreationExpression.
     * @param node the processing target
     * @see expression::VectorCreationExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::VectorCreationExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VectorElementLoadExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::VectorElementLoadExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::VectorElementLoadExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VectorElementLoadExpression.
     * @param node the processing target
     * @see expression::VectorElementLoadExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::VectorElementLoadExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VectorElementStoreExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::VectorElementStoreExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::VectorElementStoreExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VectorElementStoreExpression.
     * @param node the processing target
     * @see expression::VectorElementStoreExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::VectorElementStoreExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VectorLengthExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::VectorLengthExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::VectorLengthExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VectorLengthExpression.
     * @param node the processing target
     * @see expression::VectorLengthExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::VectorLengthExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into AggregationExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::AggregationExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::AggregationExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from AggregationExpression.
     * @param node the processing target
     * @see expression::relation::AggregationExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::AggregationExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into DistinctExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::DistinctExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::DistinctExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from DistinctExpression.
     * @param node the processing target
     * @see expression::relation::DistinctExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::DistinctExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into GroupExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::GroupExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::GroupExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from GroupExpression.
     * @param node the processing target
     * @see expression::relation::GroupExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::GroupExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into JoinExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::JoinExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::JoinExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from JoinExpression.
     * @param node the processing target
     * @see expression::relation::JoinExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::JoinExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into LimitExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::LimitExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::LimitExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from LimitExpression.
     * @param node the processing target
     * @see expression::relation::LimitExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::LimitExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into OrderExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::OrderExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::OrderExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from OrderExpression.
     * @param node the processing target
     * @see expression::relation::OrderExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::OrderExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ProjectionExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::ProjectionExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::ProjectionExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ProjectionExpression.
     * @param node the processing target
     * @see expression::relation::ProjectionExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::ProjectionExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into RenameExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::RenameExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::RenameExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from RenameExpression.
     * @param node the processing target
     * @see expression::relation::RenameExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::RenameExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ScanExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::ScanExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::ScanExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ScanExpression.
     * @param node the processing target
     * @see expression::relation::ScanExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::ScanExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into SelectionExpression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see expression::relation::SelectionExpression
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] expression::relation::SelectionExpression const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from SelectionExpression.
     * @param node the processing target
     * @see expression::relation::SelectionExpression
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] expression::relation::SelectionExpression const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into AnchorDeclaration.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::AnchorDeclaration
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::AnchorDeclaration const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from AnchorDeclaration.
     * @param node the processing target
     * @see statement::AnchorDeclaration
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::AnchorDeclaration const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into BlockStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::BlockStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::BlockStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from BlockStatement.
     * @param node the processing target
     * @see statement::BlockStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::BlockStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into BranchStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::BranchStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::BranchStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from BranchStatement.
     * @param node the processing target
     * @see statement::BranchStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::BranchStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into BreakStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::BreakStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::BreakStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from BreakStatement.
     * @param node the processing target
     * @see statement::BreakStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::BreakStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ContinueStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::ContinueStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::ContinueStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ContinueStatement.
     * @param node the processing target
     * @see statement::ContinueStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::ContinueStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into EmptyStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::EmptyStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::EmptyStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from EmptyStatement.
     * @param node the processing target
     * @see statement::EmptyStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::EmptyStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ExpressionStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::ExpressionStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::ExpressionStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ExpressionStatement.
     * @param node the processing target
     * @see statement::ExpressionStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::ExpressionStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ForEachStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::ForEachStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::ForEachStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ForEachStatement.
     * @param node the processing target
     * @see statement::ForEachStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::ForEachStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ForStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::ForStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::ForStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ForStatement.
     * @param node the processing target
     * @see statement::ForStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::ForStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into LocalVariableDeclaration.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::LocalVariableDeclaration
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::LocalVariableDeclaration const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from LocalVariableDeclaration.
     * @param node the processing target
     * @see statement::LocalVariableDeclaration
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::LocalVariableDeclaration const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into LogStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::LogStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::LogStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from LogStatement.
     * @param node the processing target
     * @see statement::LogStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::LogStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into RaiseStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::RaiseStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::RaiseStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from RaiseStatement.
     * @param node the processing target
     * @see statement::RaiseStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::RaiseStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ReturnStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::ReturnStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::ReturnStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ReturnStatement.
     * @param node the processing target
     * @see statement::ReturnStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::ReturnStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VectorElementDeleteStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::VectorElementDeleteStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::VectorElementDeleteStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VectorElementDeleteStatement.
     * @param node the processing target
     * @see statement::VectorElementDeleteStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::VectorElementDeleteStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VectorElementInsertStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::VectorElementInsertStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::VectorElementInsertStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VectorElementInsertStatement.
     * @param node the processing target
     * @see statement::VectorElementInsertStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::VectorElementInsertStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into WhileStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::WhileStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::WhileStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from WhileStatement.
     * @param node the processing target
     * @see statement::WhileStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::WhileStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into CreateTableStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::ddl::CreateTableStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::ddl::CreateTableStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from CreateTableStatement.
     * @param node the processing target
     * @see statement::ddl::CreateTableStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::ddl::CreateTableStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into DropTableStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::ddl::DropTableStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::ddl::DropTableStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from DropTableStatement.
     * @param node the processing target
     * @see statement::ddl::DropTableStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::ddl::DropTableStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into DeleteStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::dml::DeleteStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::dml::DeleteStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from DeleteStatement.
     * @param node the processing target
     * @see statement::dml::DeleteStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::dml::DeleteStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into EmitStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::dml::EmitStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::dml::EmitStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from EmitStatement.
     * @param node the processing target
     * @see statement::dml::EmitStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::dml::EmitStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into InsertRelationStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::dml::InsertRelationStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::dml::InsertRelationStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from InsertRelationStatement.
     * @param node the processing target
     * @see statement::dml::InsertRelationStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::dml::InsertRelationStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into InsertValuesStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::dml::InsertValuesStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::dml::InsertValuesStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from InsertValuesStatement.
     * @param node the processing target
     * @see statement::dml::InsertValuesStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::dml::InsertValuesStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into UpdateStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::dml::UpdateStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::dml::UpdateStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from UpdateStatement.
     * @param node the processing target
     * @see statement::dml::UpdateStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::dml::UpdateStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into TransactionBlockStatement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see statement::transaction::TransactionBlockStatement
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] statement::transaction::TransactionBlockStatement const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TransactionBlockStatement.
     * @param node the processing target
     * @see statement::transaction::TransactionBlockStatement
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] statement::transaction::TransactionBlockStatement const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into ArrayType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::ArrayType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::ArrayType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from ArrayType.
     * @param node the processing target
     * @see type::ArrayType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::ArrayType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into BooleanType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::BooleanType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::BooleanType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from BooleanType.
     * @param node the processing target
     * @see type::BooleanType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::BooleanType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into CharType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::CharType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::CharType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from CharType.
     * @param node the processing target
     * @see type::CharType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::CharType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into Float32Type.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::Float32Type
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::Float32Type const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from Float32Type.
     * @param node the processing target
     * @see type::Float32Type
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::Float32Type const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into Float64Type.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::Float64Type
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::Float64Type const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from Float64Type.
     * @param node the processing target
     * @see type::Float64Type
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::Float64Type const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into Int32Type.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::Int32Type
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::Int32Type const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from Int32Type.
     * @param node the processing target
     * @see type::Int32Type
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::Int32Type const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into Int64Type.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::Int64Type
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::Int64Type const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from Int64Type.
     * @param node the processing target
     * @see type::Int64Type
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::Int64Type const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into NullType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::NullType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::NullType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from NullType.
     * @param node the processing target
     * @see type::NullType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::NullType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into RelationType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::RelationType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::RelationType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from RelationType.
     * @param node the processing target
     * @see type::RelationType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::RelationType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into StringType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::StringType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::StringType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from StringType.
     * @param node the processing target
     * @see type::StringType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::StringType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into TupleType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::TupleType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::TupleType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TupleType.
     * @param node the processing target
     * @see type::TupleType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::TupleType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VarCharType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VarCharType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::VarCharType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VarCharType.
     * @param node the processing target
     * @see type::VarCharType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::VarCharType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into VectorType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VectorType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::VectorType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VectorType.
     * @param node the processing target
     * @see type::VectorType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::VectorType const* node) {
        exitDefault(node);
    }

    /**
     * @brief callback on enter into DecimalType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VectorType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::DecimalType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from DecimalType.
     * @param node the processing target
     * @see type::VectorType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::DecimalType const* node) {
        return exitDefault(node);
    }

    /**
     * @brief callback on enter into BinaryType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VectorType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::BinaryType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from BinaryType.
     * @param node the processing target
     * @see type::VectorType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::BinaryType const* node) {
        return exitDefault(node);
    }

    /**
     * @brief callback on enter into VarBinaryType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VectorType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::VarBinaryType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from VarBinaryType.
     * @param node the processing target
     * @see type::VectorType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::VarBinaryType const* node) {
        return exitDefault(node);
    }

    /**
     * @brief callback on enter into DateType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VectorType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::DateType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from DateType.
     * @param node the processing target
     * @see type::VectorType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::DateType const* node) {
        return exitDefault(node);
    }

    /**
     * @brief callback on enter into TimeType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VectorType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::TimeType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TimeType.
     * @param node the processing target
     * @see type::VectorType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::TimeType const* node) {
        return exitDefault(node);
    }

    /**
     * @brief callback on enter into TimestampType.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see type::VectorType
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] type::TimestampType const* node) {
        return enterDefault(node);
    }

    /**
     * @brief callback on exit from TimestampType.
     * @param node the processing target
     * @see type::VectorType
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] type::TimestampType const* node) {
        return exitDefault(node);
    }

    /**
     * @brief callback on enter into Index.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see name::Index
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] name::Index const* node) {
        return true;
    }

    /**
     * @brief callback on exit from Index.
     * @param node the processing target
     * @see name::Index
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] name::Index const* node) {}

    /**
     * @brief callback on enter into Comment.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see program::Comment
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] program::Comment const* node) {
        return true;
    }

    /**
     * @brief callback on exit from Comment.
     * @param node the processing target
     * @see program::Comment
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] program::Comment const* node) {}

    /**
     * @brief callback on enter into Program.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements, also the corresponded exit() will not be called
     * @see program::Program
     * @see walk()
     * @see exit()
     */
    virtual bool enter([[maybe_unused]] program::Program const* node) {
        return true;
    }

    /**
     * @brief callback on exit from Program.
     * @param node the processing target
     * @see program::Program
     * @see walk()
     * @see enter()
     */
    virtual void exit([[maybe_unused]] program::Program const* node) {}

    /**
     * @brief callback on enter into GlobalDeclaration.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements
     * @see program::GlobalDeclaration
     * @see walk()
     * @see exitDefault()
     */
    virtual bool enterDefault([[maybe_unused]] program::GlobalDeclaration const* node) {
        return true;
    }

    /**
     * @brief callback on exit from GlobalDeclaration.
     * @param node the processing target
     * @see program::GlobalDeclaration
     * @see walk()
     * @see enterDefault()
     */
    virtual void exitDefault([[maybe_unused]] program::GlobalDeclaration const* node) {}

    /**
     * @brief callback on enter into Name.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements
     * @see name::Name
     * @see walk()
     * @see exitDefault()
     */
    virtual bool enterDefault([[maybe_unused]] name::Name const* node) {
        return true;
    }

    /**
     * @brief callback on exit from Name.
     * @param node the processing target
     * @see name::Name
     * @see walk()
     * @see enterDefault()
     */
    virtual void exitDefault([[maybe_unused]] name::Name const* node) {}

    /**
     * @brief callback on enter into Expression.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements
     * @see expression::Expression
     * @see walk()
     * @see exitDefault()
     */
    virtual bool enterDefault([[maybe_unused]] expression::Expression const* node) {
        return true;
    }

    /**
     * @brief callback on exit from Expression.
     * @param node the processing target
     * @see expression::Expression
     * @see walk()
     * @see enterDefault()
     */
    virtual void exitDefault([[maybe_unused]] expression::Expression const* node) {}

    /**
     * @brief callback on enter into Statement.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements
     * @see statement::Statement
     * @see walk()
     * @see exitDefault()
     */
    virtual bool enterDefault([[maybe_unused]] statement::Statement const* node) {
        return true;
    }

    /**
     * @brief callback on exit from Statement.
     * @param node the processing target
     * @see statement::Statement
     * @see walk()
     * @see enterDefault()
     */
    virtual void exitDefault([[maybe_unused]] statement::Statement const* node) {}

    /**
     * @brief callback on enter into Type.
     * @param node the processing target
     * @return true if continue to enter into child elements of the given node
     * @return false don't walk into child elements
     * @see type::Type
     * @see walk()
     * @see exitDefault()
     */
    virtual bool enterDefault([[maybe_unused]] type::Type const* node) {
        return true;
    }

    /**
     * @brief callback on exit from Type.
     * @param node the processing target
     * @see type::Type
     * @see walk()
     * @see enterDefault()
     */
    virtual void exitDefault([[maybe_unused]] type::Type const* node) {}

public:
    /**
     * @brief Begins to walk Node.
     * @param node the processing target
     * @see Node
     */
    void walk(Node const* node);

    /**
     * @brief Begins to walk GlobalDeclaration.
     * @param node the processing target
     * @see program::GlobalDeclaration
     */
    void walk(program::GlobalDeclaration const* node);

    /**
     * @brief Begins to walk GlobalFunctionDeclaration.
     * @param node the processing target
     * @see program::GlobalFunctionDeclaration
     */
    void walk(program::GlobalFunctionDeclaration const* node);

    /**
     * @brief Begins to walk GlobalVariableDeclaration.
     * @param node the processing target
     * @see program::GlobalVariableDeclaration
     */
    void walk(program::GlobalVariableDeclaration const* node);

    /**
     * @brief Begins to walk Name.
     * @param node the processing target
     * @see name::Name
     */
    void walk(name::Name const* node);

    /**
     * @brief Begins to walk QualifiedName.
     * @param node the processing target
     * @see name::QualifiedName
     */
    void walk(name::QualifiedName const* node);

    /**
     * @brief Begins to walk SimpleName.
     * @param node the processing target
     * @see name::SimpleName
     */
    void walk(name::SimpleName const* node);

    /**
     * @brief Begins to walk Expression.
     * @param node the processing target
     * @see expression::Expression
     */
    void walk(expression::Expression const* node);

    /**
     * @brief Begins to walk ArrayCreationExpression.
     * @param node the processing target
     * @see expression::ArrayCreationExpression
     */
    void walk(expression::ArrayCreationExpression const* node);

    /**
     * @brief Begins to walk ArrayElementLoadExpression.
     * @param node the processing target
     * @see expression::ArrayElementLoadExpression
     */
    void walk(expression::ArrayElementLoadExpression const* node);

    /**
     * @brief Begins to walk ArrayElementStoreExpression.
     * @param node the processing target
     * @see expression::ArrayElementStoreExpression
     */
    void walk(expression::ArrayElementStoreExpression const* node);

    /**
     * @brief Begins to walk AssignExpression.
     * @param node the processing target
     * @see expression::AssignExpression
     */
    void walk(expression::AssignExpression const* node);

    /**
     * @brief Begins to walk BinaryOperator.
     * @param node the processing target
     * @see expression::BinaryOperator
     */
    void walk(expression::BinaryOperator const* node);

    /**
     * @brief Begins to walk BlockExpression.
     * @param node the processing target
     * @see expression::BlockExpression
     */
    void walk(expression::BlockExpression const* node);

    /**
     * @brief Begins to walk CaseExpression.
     * @param node the processing target
     * @see expression::CaseExpression
     */
    void walk(expression::CaseExpression const* node);

    /**
     * @brief Begins to walk CursorAdvanceExpression.
     * @param node the processing target
     * @see expression::CursorAdvanceExpression
     */
    void walk(expression::CursorAdvanceExpression const* node);

    /**
     * @brief Begins to walk CursorCreationExpression.
     * @param node the processing target
     * @see expression::CursorCreationExpression
     */
    void walk(expression::CursorCreationExpression const* node);

    /**
     * @brief Begins to walk CursorElementLoadExpression.
     * @param node the processing target
     * @see expression::CursorElementLoadExpression
     */
    void walk(expression::CursorElementLoadExpression const* node);

    /**
     * @brief Begins to walk FunctionCall.
     * @param node the processing target
     * @see expression::FunctionCall
     */
    void walk(expression::FunctionCall const* node);

    /**
     * @brief Begins to walk ImplicitCast.
     * @param node the processing target
     * @see expression::ImplicitCast
     */
    void walk(expression::ImplicitCast const* node);

    /**
     * @brief Begins to walk Literal.
     * @param node the processing target
     * @see expression::Literal
     */
    void walk(expression::Literal const* node);

    /**
     * @brief Begins to walk Placeholder.
     * @param node the processing target
     * @see expression::Placeholder
     */
    void walk(expression::Placeholder const* node);

    /**
     * @brief Begins to walk TupleCreationExpression.
     * @param node the processing target
     * @see expression::TupleCreationExpression
     */
    void walk(expression::TupleCreationExpression const* node);

    /**
     * @brief Begins to walk TupleElementLoadExpression.
     * @param node the processing target
     * @see expression::TupleElementLoadExpression
     */
    void walk(expression::TupleElementLoadExpression const* node);

    /**
     * @brief Begins to walk TupleElementStoreExpression.
     * @param node the processing target
     * @see expression::TupleElementStoreExpression
     */
    void walk(expression::TupleElementStoreExpression const* node);

    /**
     * @brief Begins to walk TypeOperator.
     * @param node the processing target
     * @see expression::TypeOperator
     */
    void walk(expression::TypeOperator const* node);

    /**
     * @brief Begins to walk UnaryOperator.
     * @param node the processing target
     * @see expression::UnaryOperator
     */
    void walk(expression::UnaryOperator const* node);

    /**
     * @brief Begins to walk VariableReference.
     * @param node the processing target
     * @see expression::VariableReference
     */
    void walk(expression::VariableReference const* node);

    /**
     * @brief Begins to walk VectorCreationExpression.
     * @param node the processing target
     * @see expression::VectorCreationExpression
     */
    void walk(expression::VectorCreationExpression const* node);

    /**
     * @brief Begins to walk VectorElementLoadExpression.
     * @param node the processing target
     * @see expression::VectorElementLoadExpression
     */
    void walk(expression::VectorElementLoadExpression const* node);

    /**
     * @brief Begins to walk VectorElementStoreExpression.
     * @param node the processing target
     * @see expression::VectorElementStoreExpression
     */
    void walk(expression::VectorElementStoreExpression const* node);

    /**
     * @brief Begins to walk VectorLengthExpression.
     * @param node the processing target
     * @see expression::VectorLengthExpression
     */
    void walk(expression::VectorLengthExpression const* node);

    /**
     * @brief Begins to walk AggregationExpression.
     * @param node the processing target
     * @see expression::relation::AggregationExpression
     */
    void walk(expression::relation::AggregationExpression const* node);

    /**
     * @brief Begins to walk DistinctExpression.
     * @param node the processing target
     * @see expression::relation::DistinctExpression
     */
    void walk(expression::relation::DistinctExpression const* node);

    /**
     * @brief Begins to walk GroupExpression.
     * @param node the processing target
     * @see expression::relation::GroupExpression
     */
    void walk(expression::relation::GroupExpression const* node);

    /**
     * @brief Begins to walk JoinExpression.
     * @param node the processing target
     * @see expression::relation::JoinExpression
     */
    void walk(expression::relation::JoinExpression const* node);

    /**
     * @brief Begins to walk LimitExpression.
     * @param node the processing target
     * @see expression::relation::LimitExpression
     */
    void walk(expression::relation::LimitExpression const* node);

    /**
     * @brief Begins to walk OrderExpression.
     * @param node the processing target
     * @see expression::relation::OrderExpression
     */
    void walk(expression::relation::OrderExpression const* node);

    /**
     * @brief Begins to walk ProjectionExpression.
     * @param node the processing target
     * @see expression::relation::ProjectionExpression
     */
    void walk(expression::relation::ProjectionExpression const* node);

    /**
     * @brief Begins to walk RenameExpression.
     * @param node the processing target
     * @see expression::relation::RenameExpression
     */
    void walk(expression::relation::RenameExpression const* node);

    /**
     * @brief Begins to walk ScanExpression.
     * @param node the processing target
     * @see expression::relation::ScanExpression
     */
    void walk(expression::relation::ScanExpression const* node);

    /**
     * @brief Begins to walk SelectionExpression.
     * @param node the processing target
     * @see expression::relation::SelectionExpression
     */
    void walk(expression::relation::SelectionExpression const* node);

    /**
     * @brief Begins to walk Statement.
     * @param node the processing target
     * @see statement::Statement
     */
    void walk(statement::Statement const* node);

    /**
     * @brief Begins to walk AnchorDeclaration.
     * @param node the processing target
     * @see statement::AnchorDeclaration
     */
    void walk(statement::AnchorDeclaration const* node);

    /**
     * @brief Begins to walk BlockStatement.
     * @param node the processing target
     * @see statement::BlockStatement
     */
    void walk(statement::BlockStatement const* node);

    /**
     * @brief Begins to walk BranchStatement.
     * @param node the processing target
     * @see statement::BranchStatement
     */
    void walk(statement::BranchStatement const* node);

    /**
     * @brief Begins to walk BreakStatement.
     * @param node the processing target
     * @see statement::BreakStatement
     */
    void walk(statement::BreakStatement const* node);

    /**
     * @brief Begins to walk ContinueStatement.
     * @param node the processing target
     * @see statement::ContinueStatement
     */
    void walk(statement::ContinueStatement const* node);

    /**
     * @brief Begins to walk EmptyStatement.
     * @param node the processing target
     * @see statement::EmptyStatement
     */
    void walk(statement::EmptyStatement const* node);

    /**
     * @brief Begins to walk ExpressionStatement.
     * @param node the processing target
     * @see statement::ExpressionStatement
     */
    void walk(statement::ExpressionStatement const* node);

    /**
     * @brief Begins to walk ForEachStatement.
     * @param node the processing target
     * @see statement::ForEachStatement
     */
    void walk(statement::ForEachStatement const* node);

    /**
     * @brief Begins to walk ForStatement.
     * @param node the processing target
     * @see statement::ForStatement
     */
    void walk(statement::ForStatement const* node);

    /**
     * @brief Begins to walk LocalVariableDeclaration.
     * @param node the processing target
     * @see statement::LocalVariableDeclaration
     */
    void walk(statement::LocalVariableDeclaration const* node);

    /**
     * @brief Begins to walk LogStatement.
     * @param node the processing target
     * @see statement::LogStatement
     */
    void walk(statement::LogStatement const* node);

    /**
     * @brief Begins to walk RaiseStatement.
     * @param node the processing target
     * @see statement::RaiseStatement
     */
    void walk(statement::RaiseStatement const* node);

    /**
     * @brief Begins to walk ReturnStatement.
     * @param node the processing target
     * @see statement::ReturnStatement
     */
    void walk(statement::ReturnStatement const* node);

    /**
     * @brief Begins to walk VectorElementDeleteStatement.
     * @param node the processing target
     * @see statement::VectorElementDeleteStatement
     */
    void walk(statement::VectorElementDeleteStatement const* node);

    /**
     * @brief Begins to walk VectorElementInsertStatement.
     * @param node the processing target
     * @see statement::VectorElementInsertStatement
     */
    void walk(statement::VectorElementInsertStatement const* node);

    /**
     * @brief Begins to walk WhileStatement.
     * @param node the processing target
     * @see statement::WhileStatement
     */
    void walk(statement::WhileStatement const* node);

    /**
     * @brief Begins to walk CreateTableStatement.
     * @param node the processing target
     * @see statement::ddl::CreateTableStatement
     */
    void walk(statement::ddl::CreateTableStatement const* node);

    /**
     * @brief Begins to walk DropTableStatement.
     * @param node the processing target
     * @see statement::ddl::DropTableStatement
     */
    void walk(statement::ddl::DropTableStatement const* node);

    /**
     * @brief Begins to walk DeleteStatement.
     * @param node the processing target
     * @see statement::dml::DeleteStatement
     */
    void walk(statement::dml::DeleteStatement const* node);

    /**
     * @brief Begins to walk EmitStatement.
     * @param node the processing target
     * @see statement::dml::EmitStatement
     */
    void walk(statement::dml::EmitStatement const* node);

    /**
     * @brief Begins to walk InsertRelationStatement.
     * @param node the processing target
     * @see statement::dml::InsertRelationStatement
     */
    void walk(statement::dml::InsertRelationStatement const* node);

    /**
     * @brief Begins to walk InsertValuesStatement.
     * @param node the processing target
     * @see statement::dml::InsertValuesStatement
     */
    void walk(statement::dml::InsertValuesStatement const* node);

    /**
     * @brief Begins to walk UpdateStatement.
     * @param node the processing target
     * @see statement::dml::UpdateStatement
     */
    void walk(statement::dml::UpdateStatement const* node);

    /**
     * @brief Begins to walk TransactionBlockStatement.
     * @param node the processing target
     * @see statement::transaction::TransactionBlockStatement
     */
    void walk(statement::transaction::TransactionBlockStatement const* node);

    /**
     * @brief Begins to walk Type.
     * @param node the processing target
     * @see type::Type
     */
    void walk(type::Type const* node);

    /**
     * @brief Begins to walk ArrayType.
     * @param node the processing target
     * @see type::ArrayType
     */
    void walk(type::ArrayType const* node);

    /**
     * @brief Begins to walk BooleanType.
     * @param node the processing target
     * @see type::BooleanType
     */
    void walk(type::BooleanType const* node);

    /**
     * @brief Begins to walk CharType.
     * @param node the processing target
     * @see type::CharType
     */
    void walk(type::CharType const* node);

    /**
     * @brief Begins to walk Float32Type.
     * @param node the processing target
     * @see type::Float32Type
     */
    void walk(type::Float32Type const* node);

    /**
     * @brief Begins to walk Float64Type.
     * @param node the processing target
     * @see type::Float64Type
     */
    void walk(type::Float64Type const* node);

    /**
     * @brief Begins to walk Int32Type.
     * @param node the processing target
     * @see type::Int32Type
     */
    void walk(type::Int32Type const* node);

    /**
     * @brief Begins to walk Int64Type.
     * @param node the processing target
     * @see type::Int64Type
     */
    void walk(type::Int64Type const* node);

    /**
     * @brief Begins to walk NullType.
     * @param node the processing target
     * @see type::NullType
     */
    void walk(type::NullType const* node);

    /**
     * @brief Begins to walk RelationType.
     * @param node the processing target
     * @see type::RelationType
     */
    void walk(type::RelationType const* node);

    /**
     * @brief Begins to walk StringType.
     * @param node the processing target
     * @see type::StringType
     */
    void walk(type::StringType const* node);

    /**
     * @brief Begins to walk TupleType.
     * @param node the processing target
     * @see type::TupleType
     */
    void walk(type::TupleType const* node);

    /**
     * @brief Begins to walk VarCharType.
     * @param node the processing target
     * @see type::VarCharType
     */
    void walk(type::VarCharType const* node);

    /**
     * @brief Begins to walk VectorType.
     * @param node the processing target
     * @see type::VectorType
     */
    void walk(type::VectorType const* node);

    /**
     * @brief Begins to walk DecimalType.
     * @param node the processing target
     * @see type::DecimalType
     */
    void walk(type::DecimalType const* node);

    /**
     * @brief Begins to walk BinaryType.
     * @param node the processing target
     * @see type::BinaryType
     */
    void walk(type::BinaryType const* node);

    /**
     * @brief Begins to walk VarBinaryType.
     * @param node the processing target
     * @see type::VarBinaryType
     */
    void walk(type::VarBinaryType const* node);

    /**
     * @brief Begins to walk DateType.
     * @param node the processing target
     * @see type::DateType
     */
    void walk(type::DateType const* node);

    /**
     * @brief Begins to walk TimeType.
     * @param node the processing target
     * @see type::TimeType
     */
    void walk(type::TimeType const* node);

    /**
     * @brief Begins to walk TimestampType.
     * @param node the processing target
     * @see type::TimestampType
     */
    void walk(type::TimestampType const* node);

    /**
     * @brief Begins to walk Index.
     * @param node the processing target
     * @see name::Index
     */
    void walk(name::Index const* node);

    /**
     * @brief Begins to walk Comment.
     * @param node the processing target
     * @see program::Comment
     */
    void walk(program::Comment const* node);

    /**
     * @brief Begins to walk Program.
     * @param node the processing target
     * @see program::Program
     */
    void walk(program::Program const* node);

};

}  // namespace shakujo::model::util

#endif  // SHAKUJO_MODEL_UTIL_CONST_NODE_WALKER_H_
