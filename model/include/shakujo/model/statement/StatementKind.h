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
#ifndef SHAKUJO_MODEL_STATEMENT_STATEMENT_KIND_H_
#define SHAKUJO_MODEL_STATEMENT_STATEMENT_KIND_H_

#include <string>
#include <string_view>
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
     * @brief tag for CreateIndexStatement.
     * @see ddl::CreateIndexStatement
     */
    CREATE_INDEX_STATEMENT,
    /**
     * @brief tag for DropTableStatement.
     * @see ddl::DropTableStatement
     */
    DROP_TABLE_STATEMENT,
    /**
     * @brief tag for DropIndexStatement.
     * @see ddl::DropIndexStatement
     */
    DROP_INDEX_STATEMENT,
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
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see StatementKind
 */
inline constexpr std::string_view to_string_view(StatementKind value) {
    switch (value) {
        case StatementKind::ANCHOR_DECLARATION: return "ANCHOR_DECLARATION";
        case StatementKind::BLOCK_STATEMENT: return "BLOCK_STATEMENT";
        case StatementKind::BRANCH_STATEMENT: return "BRANCH_STATEMENT";
        case StatementKind::BREAK_STATEMENT: return "BREAK_STATEMENT";
        case StatementKind::CONTINUE_STATEMENT: return "CONTINUE_STATEMENT";
        case StatementKind::EMPTY_STATEMENT: return "EMPTY_STATEMENT";
        case StatementKind::EXPRESSION_STATEMENT: return "EXPRESSION_STATEMENT";
        case StatementKind::FOR_EACH_STATEMENT: return "FOR_EACH_STATEMENT";
        case StatementKind::FOR_STATEMENT: return "FOR_STATEMENT";
        case StatementKind::LOCAL_VARIABLE_DECLARATION: return "LOCAL_VARIABLE_DECLARATION";
        case StatementKind::LOG_STATEMENT: return "LOG_STATEMENT";
        case StatementKind::RAISE_STATEMENT: return "RAISE_STATEMENT";
        case StatementKind::RETURN_STATEMENT: return "RETURN_STATEMENT";
        case StatementKind::VECTOR_ELEMENT_DELETE_STATEMENT: return "VECTOR_ELEMENT_DELETE_STATEMENT";
        case StatementKind::VECTOR_ELEMENT_INSERT_STATEMENT: return "VECTOR_ELEMENT_INSERT_STATEMENT";
        case StatementKind::WHILE_STATEMENT: return "WHILE_STATEMENT";
        case StatementKind::CREATE_TABLE_STATEMENT: return "CREATE_TABLE_STATEMENT";
        case StatementKind::CREATE_INDEX_STATEMENT: return "CREATE_INDEX_STATEMENT";
        case StatementKind::DROP_TABLE_STATEMENT: return "DROP_TABLE_STATEMENT";
        case StatementKind::DROP_INDEX_STATEMENT: return "DROP_INDEX_STATEMENT";
        case StatementKind::DELETE_STATEMENT: return "DELETE_STATEMENT";
        case StatementKind::EMIT_STATEMENT: return "EMIT_STATEMENT";
        case StatementKind::INSERT_RELATION_STATEMENT: return "INSERT_RELATION_STATEMENT";
        case StatementKind::INSERT_VALUES_STATEMENT: return "INSERT_VALUES_STATEMENT";
        case StatementKind::UPDATE_STATEMENT: return "UPDATE_STATEMENT";
        case StatementKind::TRANSACTION_BLOCK_STATEMENT: return "TRANSACTION_BLOCK_STATEMENT";
    }
    return "(unknown)";
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see StatementKind
 */
inline std::ostream& operator<<(std::ostream& out, StatementKind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_STATEMENT_KIND_H_
