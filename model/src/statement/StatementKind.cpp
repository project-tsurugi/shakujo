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
#include "shakujo/model/statement/StatementKind.h"

#include <iostream>


namespace shakujo::model::statement {
std::ostream& operator<<(std::ostream& out, StatementKind value) {
    switch (value) {
    case StatementKind::ANCHOR_DECLARATION:
        out << "ANCHOR_DECLARATION";
        break;
    case StatementKind::BLOCK_STATEMENT:
        out << "BLOCK_STATEMENT";
        break;
    case StatementKind::BRANCH_STATEMENT:
        out << "BRANCH_STATEMENT";
        break;
    case StatementKind::BREAK_STATEMENT:
        out << "BREAK_STATEMENT";
        break;
    case StatementKind::CONTINUE_STATEMENT:
        out << "CONTINUE_STATEMENT";
        break;
    case StatementKind::EMPTY_STATEMENT:
        out << "EMPTY_STATEMENT";
        break;
    case StatementKind::EXPRESSION_STATEMENT:
        out << "EXPRESSION_STATEMENT";
        break;
    case StatementKind::FOR_EACH_STATEMENT:
        out << "FOR_EACH_STATEMENT";
        break;
    case StatementKind::FOR_STATEMENT:
        out << "FOR_STATEMENT";
        break;
    case StatementKind::LOCAL_VARIABLE_DECLARATION:
        out << "LOCAL_VARIABLE_DECLARATION";
        break;
    case StatementKind::LOG_STATEMENT:
        out << "LOG_STATEMENT";
        break;
    case StatementKind::RAISE_STATEMENT:
        out << "RAISE_STATEMENT";
        break;
    case StatementKind::RETURN_STATEMENT:
        out << "RETURN_STATEMENT";
        break;
    case StatementKind::VECTOR_ELEMENT_DELETE_STATEMENT:
        out << "VECTOR_ELEMENT_DELETE_STATEMENT";
        break;
    case StatementKind::VECTOR_ELEMENT_INSERT_STATEMENT:
        out << "VECTOR_ELEMENT_INSERT_STATEMENT";
        break;
    case StatementKind::WHILE_STATEMENT:
        out << "WHILE_STATEMENT";
        break;
    case StatementKind::CREATE_TABLE_STATEMENT:
        out << "CREATE_TABLE_STATEMENT";
        break;
    case StatementKind::DROP_TABLE_STATEMENT:
        out << "DROP_TABLE_STATEMENT";
        break;
    case StatementKind::DELETE_STATEMENT:
        out << "DELETE_STATEMENT";
        break;
    case StatementKind::EMIT_STATEMENT:
        out << "EMIT_STATEMENT";
        break;
    case StatementKind::INSERT_RELATION_STATEMENT:
        out << "INSERT_RELATION_STATEMENT";
        break;
    case StatementKind::INSERT_VALUES_STATEMENT:
        out << "INSERT_VALUES_STATEMENT";
        break;
    case StatementKind::UPDATE_STATEMENT:
        out << "UPDATE_STATEMENT";
        break;
    case StatementKind::TRANSACTION_BLOCK_STATEMENT:
        out << "TRANSACTION_BLOCK_STATEMENT";
        break;
    }
    return out;
}

}  // namespace shakujo::model::statement

