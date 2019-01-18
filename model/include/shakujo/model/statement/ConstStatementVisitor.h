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
#ifndef SHAKUJO_MODEL_STATEMENT_CONST_STATEMENT_VISITOR_H_
#define SHAKUJO_MODEL_STATEMENT_CONST_STATEMENT_VISITOR_H_

#include <stdexcept>
#include <utility>
#include <sstream>

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
#include "shakujo/model/statement/StatementKind.h"
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

namespace shakujo::model::statement {
/**
 * @brief A visitor for Statement.
 * The default implementation of visit() will be redirected to visitDefault().
 * @tparam Return the result type
 * @tparam Args the parameter types
 * @see Statement
 */
template <class Return, class... Args>
class ConstStatementVisitor {
public:
    /**
     * @brief Processes the default Statement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Statement
     */
    virtual Return visitDefault(Statement const* node, [[maybe_unused]] Args... args) {
        std::ostringstream ss;
        ss << "visit for " << node->kind() << " is not implemented.";
        throw std::runtime_error(ss.str());
    }

    /**
     * @brief Processes AnchorDeclaration.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see AnchorDeclaration
     */
    virtual Return visit(AnchorDeclaration const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes BlockStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BlockStatement
     */
    virtual Return visit(BlockStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes BranchStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BranchStatement
     */
    virtual Return visit(BranchStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes BreakStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BreakStatement
     */
    virtual Return visit(BreakStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ContinueStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ContinueStatement
     */
    virtual Return visit(ContinueStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes EmptyStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see EmptyStatement
     */
    virtual Return visit(EmptyStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ExpressionStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ExpressionStatement
     */
    virtual Return visit(ExpressionStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ForEachStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ForEachStatement
     */
    virtual Return visit(ForEachStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ForStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ForStatement
     */
    virtual Return visit(ForStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes LocalVariableDeclaration.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see LocalVariableDeclaration
     */
    virtual Return visit(LocalVariableDeclaration const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes LogStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see LogStatement
     */
    virtual Return visit(LogStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes RaiseStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see RaiseStatement
     */
    virtual Return visit(RaiseStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes ReturnStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ReturnStatement
     */
    virtual Return visit(ReturnStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VectorElementDeleteStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorElementDeleteStatement
     */
    virtual Return visit(VectorElementDeleteStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VectorElementInsertStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorElementInsertStatement
     */
    virtual Return visit(VectorElementInsertStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes WhileStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see WhileStatement
     */
    virtual Return visit(WhileStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes CreateTableStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ddl::CreateTableStatement
     */
    virtual Return visit(ddl::CreateTableStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes DropTableStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ddl::DropTableStatement
     */
    virtual Return visit(ddl::DropTableStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes DeleteStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see dml::DeleteStatement
     */
    virtual Return visit(dml::DeleteStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes EmitStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see dml::EmitStatement
     */
    virtual Return visit(dml::EmitStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes InsertRelationStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see dml::InsertRelationStatement
     */
    virtual Return visit(dml::InsertRelationStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes InsertValuesStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see dml::InsertValuesStatement
     */
    virtual Return visit(dml::InsertValuesStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes UpdateStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see dml::UpdateStatement
     */
    virtual Return visit(dml::UpdateStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes TransactionBlockStatement.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see transaction::TransactionBlockStatement
     */
    virtual Return visit(transaction::TransactionBlockStatement const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Statement const* node, Args... args) {
        switch (node->kind()) {
        case StatementKind::ANCHOR_DECLARATION:
            return visit(dynamic_cast<AnchorDeclaration const*>(node), args...);
        case StatementKind::BLOCK_STATEMENT:
            return visit(dynamic_cast<BlockStatement const*>(node), args...);
        case StatementKind::BRANCH_STATEMENT:
            return visit(dynamic_cast<BranchStatement const*>(node), args...);
        case StatementKind::BREAK_STATEMENT:
            return visit(dynamic_cast<BreakStatement const*>(node), args...);
        case StatementKind::CONTINUE_STATEMENT:
            return visit(dynamic_cast<ContinueStatement const*>(node), args...);
        case StatementKind::EMPTY_STATEMENT:
            return visit(dynamic_cast<EmptyStatement const*>(node), args...);
        case StatementKind::EXPRESSION_STATEMENT:
            return visit(dynamic_cast<ExpressionStatement const*>(node), args...);
        case StatementKind::FOR_EACH_STATEMENT:
            return visit(dynamic_cast<ForEachStatement const*>(node), args...);
        case StatementKind::FOR_STATEMENT:
            return visit(dynamic_cast<ForStatement const*>(node), args...);
        case StatementKind::LOCAL_VARIABLE_DECLARATION:
            return visit(dynamic_cast<LocalVariableDeclaration const*>(node), args...);
        case StatementKind::LOG_STATEMENT:
            return visit(dynamic_cast<LogStatement const*>(node), args...);
        case StatementKind::RAISE_STATEMENT:
            return visit(dynamic_cast<RaiseStatement const*>(node), args...);
        case StatementKind::RETURN_STATEMENT:
            return visit(dynamic_cast<ReturnStatement const*>(node), args...);
        case StatementKind::VECTOR_ELEMENT_DELETE_STATEMENT:
            return visit(dynamic_cast<VectorElementDeleteStatement const*>(node), args...);
        case StatementKind::VECTOR_ELEMENT_INSERT_STATEMENT:
            return visit(dynamic_cast<VectorElementInsertStatement const*>(node), args...);
        case StatementKind::WHILE_STATEMENT:
            return visit(dynamic_cast<WhileStatement const*>(node), args...);
        case StatementKind::CREATE_TABLE_STATEMENT:
            return visit(dynamic_cast<ddl::CreateTableStatement const*>(node), args...);
        case StatementKind::DROP_TABLE_STATEMENT:
            return visit(dynamic_cast<ddl::DropTableStatement const*>(node), args...);
        case StatementKind::DELETE_STATEMENT:
            return visit(dynamic_cast<dml::DeleteStatement const*>(node), args...);
        case StatementKind::EMIT_STATEMENT:
            return visit(dynamic_cast<dml::EmitStatement const*>(node), args...);
        case StatementKind::INSERT_RELATION_STATEMENT:
            return visit(dynamic_cast<dml::InsertRelationStatement const*>(node), args...);
        case StatementKind::INSERT_VALUES_STATEMENT:
            return visit(dynamic_cast<dml::InsertValuesStatement const*>(node), args...);
        case StatementKind::UPDATE_STATEMENT:
            return visit(dynamic_cast<dml::UpdateStatement const*>(node), args...);
        case StatementKind::TRANSACTION_BLOCK_STATEMENT:
            return visit(dynamic_cast<transaction::TransactionBlockStatement const*>(node), args...);
        }
        // may not occur
        return visitDefault(node, args...);
    }

};

}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_CONST_STATEMENT_VISITOR_H_
