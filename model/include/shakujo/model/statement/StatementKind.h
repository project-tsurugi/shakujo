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
#ifndef SHAKUJO_MODEL_STATEMENT_STATEMENT_KIND_H_
#define SHAKUJO_MODEL_STATEMENT_STATEMENT_KIND_H_

#include <iostream>


namespace shakujo::model::statement {
/**
 * @brief Represents the catalog of Statement.
 * @see Statement::kind()
 */
enum class StatementKind {
    /**
     * @brief tag for AnchorDeclaration.
     * @see AnchorDeclaration
     */
    ANCHOR_DECLARATION,
    /**
     * @brief tag for BlockStatement.
     * @see BlockStatement
     */
    BLOCK_STATEMENT,
    /**
     * @brief tag for BranchStatement.
     * @see BranchStatement
     */
    BRANCH_STATEMENT,
    /**
     * @brief tag for BreakStatement.
     * @see BreakStatement
     */
    BREAK_STATEMENT,
    /**
     * @brief tag for ContinueStatement.
     * @see ContinueStatement
     */
    CONTINUE_STATEMENT,
    /**
     * @brief tag for EmptyStatement.
     * @see EmptyStatement
     */
    EMPTY_STATEMENT,
    /**
     * @brief tag for ExpressionStatement.
     * @see ExpressionStatement
     */
    EXPRESSION_STATEMENT,
    /**
     * @brief tag for ForEachStatement.
     * @see ForEachStatement
     */
    FOR_EACH_STATEMENT,
    /**
     * @brief tag for ForStatement.
     * @see ForStatement
     */
    FOR_STATEMENT,
    /**
     * @brief tag for LocalVariableDeclaration.
     * @see LocalVariableDeclaration
     */
    LOCAL_VARIABLE_DECLARATION,
    /**
     * @brief tag for LogStatement.
     * @see LogStatement
     */
    LOG_STATEMENT,
    /**
     * @brief tag for RaiseStatement.
     * @see RaiseStatement
     */
    RAISE_STATEMENT,
    /**
     * @brief tag for ReturnStatement.
     * @see ReturnStatement
     */
    RETURN_STATEMENT,
    /**
     * @brief tag for VectorElementDeleteStatement.
     * @see VectorElementDeleteStatement
     */
    VECTOR_ELEMENT_DELETE_STATEMENT,
    /**
     * @brief tag for VectorElementInsertStatement.
     * @see VectorElementInsertStatement
     */
    VECTOR_ELEMENT_INSERT_STATEMENT,
    /**
     * @brief tag for WhileStatement.
     * @see WhileStatement
     */
    WHILE_STATEMENT,
    /**
     * @brief tag for CreateTableStatement.
     * @see ddl::CreateTableStatement
     */
    CREATE_TABLE_STATEMENT,
    /**
     * @brief tag for DropTableStatement.
     * @see ddl::DropTableStatement
     */
    DROP_TABLE_STATEMENT,
    /**
     * @brief tag for DeleteStatement.
     * @see dml::DeleteStatement
     */
    DELETE_STATEMENT,
    /**
     * @brief tag for EmitStatement.
     * @see dml::EmitStatement
     */
    EMIT_STATEMENT,
    /**
     * @brief tag for InsertRelationStatement.
     * @see dml::InsertRelationStatement
     */
    INSERT_RELATION_STATEMENT,
    /**
     * @brief tag for InsertValuesStatement.
     * @see dml::InsertValuesStatement
     */
    INSERT_VALUES_STATEMENT,
    /**
     * @brief tag for UpdateStatement.
     * @see dml::UpdateStatement
     */
    UPDATE_STATEMENT,
    /**
     * @brief tag for TransactionBlockStatement.
     * @see transaction::TransactionBlockStatement
     */
    TRANSACTION_BLOCK_STATEMENT,
};

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see StatementKind
 */
std::ostream& operator<<(std::ostream& out, StatementKind value);

}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_STATEMENT_KIND_H_
