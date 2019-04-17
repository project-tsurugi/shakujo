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
#include "shakujo/model/util/ConstNodeWalker.h"

#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/name/NameKind.h"
#include "shakujo/model/program/GlobalDeclarationKind.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::util {
void ConstNodeWalker::walk(Node const* node) {
    if (auto ptr = dynamic_cast<expression::Expression const*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<name::Index const*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<name::Name const*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<program::GlobalDeclaration const*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<program::Program const*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<statement::Statement const*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<type::Type const*>(node)) {
        walk(ptr);
        return;
    }
    std::abort();
}

void ConstNodeWalker::walk(program::GlobalDeclaration const* node) {
    switch (node->kind()) {
    case program::GlobalDeclarationKind::GLOBAL_FUNCTION_DECLARATION:
        walk(static_cast<program::GlobalFunctionDeclaration const*>(node));  // NOLINT
        return;
    case program::GlobalDeclarationKind::GLOBAL_VARIABLE_DECLARATION:
        walk(static_cast<program::GlobalVariableDeclaration const*>(node));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void ConstNodeWalker::walk(program::GlobalFunctionDeclaration const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->type()) walk(node->type());
    for (auto child : node->parameters()) {
        if (child->name()) walk(child->name());
        if (child->type()) walk(child->type());
    }
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(program::GlobalVariableDeclaration const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->type()) walk(node->type());
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(name::Name const* node) {
    switch (node->kind()) {
    case name::NameKind::QUALIFIED_NAME:
        walk(static_cast<name::QualifiedName const*>(node));  // NOLINT
        return;
    case name::NameKind::SIMPLE_NAME:
        walk(static_cast<name::SimpleName const*>(node));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void ConstNodeWalker::walk(name::QualifiedName const* node) {
    if (!enter(node)) return;
    if (node->qualifier()) walk(node->qualifier());
    if (node->name()) walk(node->name());
    exit(node);
}

void ConstNodeWalker::walk(name::SimpleName const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(expression::Expression const* node) {
    switch (node->kind()) {
    case expression::ExpressionKind::ARRAY_CREATION_EXPRESSION:
        walk(static_cast<expression::ArrayCreationExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::ARRAY_ELEMENT_LOAD_EXPRESSION:
        walk(static_cast<expression::ArrayElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::ARRAY_ELEMENT_STORE_EXPRESSION:
        walk(static_cast<expression::ArrayElementStoreExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::ASSIGN_EXPRESSION:
        walk(static_cast<expression::AssignExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::BINARY_OPERATOR:
        walk(static_cast<expression::BinaryOperator const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::BLOCK_EXPRESSION:
        walk(static_cast<expression::BlockExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::CASE_EXPRESSION:
        walk(static_cast<expression::CaseExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::CURSOR_ADVANCE_EXPRESSION:
        walk(static_cast<expression::CursorAdvanceExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::CURSOR_CREATION_EXPRESSION:
        walk(static_cast<expression::CursorCreationExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::CURSOR_ELEMENT_LOAD_EXPRESSION:
        walk(static_cast<expression::CursorElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::FUNCTION_CALL:
        walk(static_cast<expression::FunctionCall const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::IMPLICIT_CAST:
        walk(static_cast<expression::ImplicitCast const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::LITERAL:
        walk(static_cast<expression::Literal const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::PLACEHOLDER:
        walk(static_cast<expression::Placeholder const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::STRING_OPERATOR:
        walk(static_cast<expression::StringOperator const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::TUPLE_CREATION_EXPRESSION:
        walk(static_cast<expression::TupleCreationExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::TUPLE_ELEMENT_LOAD_EXPRESSION:
        walk(static_cast<expression::TupleElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::TUPLE_ELEMENT_STORE_EXPRESSION:
        walk(static_cast<expression::TupleElementStoreExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::TYPE_OPERATOR:
        walk(static_cast<expression::TypeOperator const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::UNARY_OPERATOR:
        walk(static_cast<expression::UnaryOperator const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::VARIABLE_REFERENCE:
        walk(static_cast<expression::VariableReference const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::VECTOR_CREATION_EXPRESSION:
        walk(static_cast<expression::VectorCreationExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::VECTOR_ELEMENT_LOAD_EXPRESSION:
        walk(static_cast<expression::VectorElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::VECTOR_ELEMENT_STORE_EXPRESSION:
        walk(static_cast<expression::VectorElementStoreExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::VECTOR_LENGTH_EXPRESSION:
        walk(static_cast<expression::VectorLengthExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::AGGREGATION_EXPRESSION:
        walk(static_cast<expression::relation::AggregationExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::DISTINCT_EXPRESSION:
        walk(static_cast<expression::relation::DistinctExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::GROUP_EXPRESSION:
        walk(static_cast<expression::relation::GroupExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::JOIN_EXPRESSION:
        walk(static_cast<expression::relation::JoinExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::LIMIT_EXPRESSION:
        walk(static_cast<expression::relation::LimitExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::ORDER_EXPRESSION:
        walk(static_cast<expression::relation::OrderExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::PROJECTION_EXPRESSION:
        walk(static_cast<expression::relation::ProjectionExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::SCAN_EXPRESSION:
        walk(static_cast<expression::relation::ScanExpression const*>(node));  // NOLINT
        return;
    case expression::ExpressionKind::SELECTION_EXPRESSION:
        walk(static_cast<expression::relation::SelectionExpression const*>(node));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void ConstNodeWalker::walk(expression::ArrayCreationExpression const* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(expression::ArrayElementLoadExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    exit(node);
}

void ConstNodeWalker::walk(expression::ArrayElementStoreExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(expression::AssignExpression const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(expression::BinaryOperator const* node) {
    if (!enter(node)) return;
    if (node->left()) walk(node->left());
    if (node->right()) walk(node->right());
    exit(node);
}

void ConstNodeWalker::walk(expression::BlockExpression const* node) {
    if (!enter(node)) return;
    for (auto child : node->head()) {
        if (child) walk(child);
    }
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(expression::CaseExpression const* node) {
    if (!enter(node)) return;
    for (auto child : node->cases()) {
        if (child->condition()) walk(child->condition());
        if (child->body()) walk(child->body());
    }
    if (node->default_expression()) walk(node->default_expression());
    exit(node);
}

void ConstNodeWalker::walk(expression::CursorAdvanceExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void ConstNodeWalker::walk(expression::CursorCreationExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void ConstNodeWalker::walk(expression::CursorElementLoadExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void ConstNodeWalker::walk(expression::FunctionCall const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    for (auto child : node->arguments()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(expression::ImplicitCast const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void ConstNodeWalker::walk(expression::Literal const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(expression::Placeholder const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(expression::StringOperator const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void ConstNodeWalker::walk(expression::TupleCreationExpression const* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(expression::TupleElementLoadExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    exit(node);
}

void ConstNodeWalker::walk(expression::TupleElementStoreExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(expression::TypeOperator const* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    if (node->operand()) walk(node->operand());
    exit(node);
}

void ConstNodeWalker::walk(expression::UnaryOperator const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void ConstNodeWalker::walk(expression::VariableReference const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    exit(node);
}

void ConstNodeWalker::walk(expression::VectorCreationExpression const* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(expression::VectorElementLoadExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    exit(node);
}

void ConstNodeWalker::walk(expression::VectorElementStoreExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(expression::VectorLengthExpression const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::AggregationExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->keys()) {
        if (child) walk(child);
    }
    for (auto child : node->columns()) {
        if (child->function()) walk(child->function());
        if (child->operand()) walk(child->operand());
        if (child->alias()) walk(child->alias());
    }
    if (node->alias()) walk(node->alias());
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::DistinctExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::GroupExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->keys()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::JoinExpression const* node) {
    if (!enter(node)) return;
    if (node->left()) walk(node->left());
    if (node->right()) walk(node->right());
    if (node->condition()) walk(node->condition());
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::LimitExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::OrderExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->elements()) {
        if (child->key()) walk(child->key());
    }
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::ProjectionExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->columns()) {
        if (child->value()) walk(child->value());
        if (child->alias()) walk(child->alias());
    }
    if (node->alias()) walk(node->alias());
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::ScanExpression const* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    if (node->alias()) walk(node->alias());
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::SelectionExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    if (node->condition()) walk(node->condition());
    exit(node);
}

void ConstNodeWalker::walk(statement::Statement const* node) {
    switch (node->kind()) {
    case statement::StatementKind::ANCHOR_DECLARATION:
        walk(static_cast<statement::AnchorDeclaration const*>(node));  // NOLINT
        return;
    case statement::StatementKind::BLOCK_STATEMENT:
        walk(static_cast<statement::BlockStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::BRANCH_STATEMENT:
        walk(static_cast<statement::BranchStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::BREAK_STATEMENT:
        walk(static_cast<statement::BreakStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::CONTINUE_STATEMENT:
        walk(static_cast<statement::ContinueStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::EMPTY_STATEMENT:
        walk(static_cast<statement::EmptyStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::EXPRESSION_STATEMENT:
        walk(static_cast<statement::ExpressionStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::FOR_EACH_STATEMENT:
        walk(static_cast<statement::ForEachStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::FOR_STATEMENT:
        walk(static_cast<statement::ForStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::LOCAL_VARIABLE_DECLARATION:
        walk(static_cast<statement::LocalVariableDeclaration const*>(node));  // NOLINT
        return;
    case statement::StatementKind::LOG_STATEMENT:
        walk(static_cast<statement::LogStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::RAISE_STATEMENT:
        walk(static_cast<statement::RaiseStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::RETURN_STATEMENT:
        walk(static_cast<statement::ReturnStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::VECTOR_ELEMENT_DELETE_STATEMENT:
        walk(static_cast<statement::VectorElementDeleteStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::VECTOR_ELEMENT_INSERT_STATEMENT:
        walk(static_cast<statement::VectorElementInsertStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::WHILE_STATEMENT:
        walk(static_cast<statement::WhileStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::CREATE_TABLE_STATEMENT:
        walk(static_cast<statement::ddl::CreateTableStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::DROP_TABLE_STATEMENT:
        walk(static_cast<statement::ddl::DropTableStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::DELETE_STATEMENT:
        walk(static_cast<statement::dml::DeleteStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::EMIT_STATEMENT:
        walk(static_cast<statement::dml::EmitStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::INSERT_RELATION_STATEMENT:
        walk(static_cast<statement::dml::InsertRelationStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::INSERT_VALUES_STATEMENT:
        walk(static_cast<statement::dml::InsertValuesStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::UPDATE_STATEMENT:
        walk(static_cast<statement::dml::UpdateStatement const*>(node));  // NOLINT
        return;
    case statement::StatementKind::TRANSACTION_BLOCK_STATEMENT:
        walk(static_cast<statement::transaction::TransactionBlockStatement const*>(node));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void ConstNodeWalker::walk(statement::AnchorDeclaration const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(statement::BlockStatement const* node) {
    if (!enter(node)) return;
    for (auto child : node->elements()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(statement::BranchStatement const* node) {
    if (!enter(node)) return;
    if (node->condition()) walk(node->condition());
    if (node->then_statement()) walk(node->then_statement());
    if (node->else_statement()) walk(node->else_statement());
    exit(node);
}

void ConstNodeWalker::walk(statement::BreakStatement const* node) {
    if (!enter(node)) return;
    if (node->anchor()) walk(node->anchor());
    exit(node);
}

void ConstNodeWalker::walk(statement::ContinueStatement const* node) {
    if (!enter(node)) return;
    if (node->anchor()) walk(node->anchor());
    exit(node);
}

void ConstNodeWalker::walk(statement::EmptyStatement const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(statement::ExpressionStatement const* node) {
    if (!enter(node)) return;
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(statement::ForEachStatement const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->type()) walk(node->type());
    if (node->enumeration()) walk(node->enumeration());
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(statement::ForStatement const* node) {
    if (!enter(node)) return;
    for (auto child : node->initialize()) {
        if (child) walk(child);
    }
    if (node->condition()) walk(node->condition());
    for (auto child : node->update()) {
        if (child) walk(child);
    }
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(statement::LocalVariableDeclaration const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->type()) walk(node->type());
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(statement::LogStatement const* node) {
    if (!enter(node)) return;
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(statement::RaiseStatement const* node) {
    if (!enter(node)) return;
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(statement::ReturnStatement const* node) {
    if (!enter(node)) return;
    if (node->value()) walk(node->value());
    exit(node);
}

void ConstNodeWalker::walk(statement::VectorElementDeleteStatement const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->begin()) walk(node->begin());
    if (node->end()) walk(node->end());
    exit(node);
}

void ConstNodeWalker::walk(statement::VectorElementInsertStatement const* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(statement::WhileStatement const* node) {
    if (!enter(node)) return;
    if (node->condition()) walk(node->condition());
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(statement::ddl::CreateTableStatement const* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    for (auto child : node->columns()) {
        if (child->name()) walk(child->name());
        if (child->type()) walk(child->type());
        if (child->value()) walk(child->value());
    }
    for (auto child : node->primary_keys()) {
        if (child->name()) walk(child->name());
    }
    exit(node);
}

void ConstNodeWalker::walk(statement::ddl::DropTableStatement const* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    exit(node);
}

void ConstNodeWalker::walk(statement::dml::DeleteStatement const* node) {
    if (!enter(node)) return;
    if (node->source()) walk(node->source());
    if (node->table()) walk(node->table());
    exit(node);
}

void ConstNodeWalker::walk(statement::dml::EmitStatement const* node) {
    if (!enter(node)) return;
    if (node->source()) walk(node->source());
    exit(node);
}

void ConstNodeWalker::walk(statement::dml::InsertRelationStatement const* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    for (auto child : node->columns()) {
        if (child) walk(child);
    }
    if (node->source()) walk(node->source());
    exit(node);
}

void ConstNodeWalker::walk(statement::dml::InsertValuesStatement const* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    for (auto child : node->initialize()) {
        if (child) walk(child);
    }
    for (auto child : node->columns()) {
        if (child->name()) walk(child->name());
        if (child->value()) walk(child->value());
    }
    exit(node);
}

void ConstNodeWalker::walk(statement::dml::UpdateStatement const* node) {
    if (!enter(node)) return;
    if (node->source()) walk(node->source());
    if (node->table()) walk(node->table());
    for (auto child : node->initialize()) {
        if (child) walk(child);
    }
    for (auto child : node->columns()) {
        if (child->name()) walk(child->name());
        if (child->value()) walk(child->value());
    }
    exit(node);
}

void ConstNodeWalker::walk(statement::transaction::TransactionBlockStatement const* node) {
    if (!enter(node)) return;
    if (node->body()) walk(node->body());
    exit(node);
}

void ConstNodeWalker::walk(type::Type const* node) {
    switch (node->kind()) {
    case type::TypeKind::ARRAY_TYPE:
        walk(static_cast<type::ArrayType const*>(node));  // NOLINT
        return;
    case type::TypeKind::BOOLEAN_TYPE:
        walk(static_cast<type::BooleanType const*>(node));  // NOLINT
        return;
    case type::TypeKind::CHAR_TYPE:
        walk(static_cast<type::CharType const*>(node));  // NOLINT
        return;
    case type::TypeKind::FLOAT32_TYPE:
        walk(static_cast<type::Float32Type const*>(node));  // NOLINT
        return;
    case type::TypeKind::FLOAT64_TYPE:
        walk(static_cast<type::Float64Type const*>(node));  // NOLINT
        return;
    case type::TypeKind::INT32_TYPE:
        walk(static_cast<type::Int32Type const*>(node));  // NOLINT
        return;
    case type::TypeKind::INT64_TYPE:
        walk(static_cast<type::Int64Type const*>(node));  // NOLINT
        return;
    case type::TypeKind::NULL_TYPE:
        walk(static_cast<type::NullType const*>(node));  // NOLINT
        return;
    case type::TypeKind::RELATION_TYPE:
        walk(static_cast<type::RelationType const*>(node));  // NOLINT
        return;
    case type::TypeKind::STRING_TYPE:
        walk(static_cast<type::StringType const*>(node));  // NOLINT
        return;
    case type::TypeKind::TUPLE_TYPE:
        walk(static_cast<type::TupleType const*>(node));  // NOLINT
        return;
    case type::TypeKind::VAR_CHAR_TYPE:
        walk(static_cast<type::VarCharType const*>(node));  // NOLINT
        return;
    case type::TypeKind::VECTOR_TYPE:
        walk(static_cast<type::VectorType const*>(node));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void ConstNodeWalker::walk(type::ArrayType const* node) {
    if (!enter(node)) return;
    if (node->element_type()) walk(node->element_type());
    exit(node);
}

void ConstNodeWalker::walk(type::BooleanType const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::CharType const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::Float32Type const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::Float64Type const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::Int32Type const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::Int64Type const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::NullType const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::RelationType const* node) {
    if (!enter(node)) return;
    if (node->element_type()) walk(node->element_type());
    exit(node);
}

void ConstNodeWalker::walk(type::StringType const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::TupleType const* node) {
    if (!enter(node)) return;
    for (auto child : node->elements()) {
        if (child->name()) walk(child->name());
        if (child->type()) walk(child->type());
    }
    exit(node);
}

void ConstNodeWalker::walk(type::VarCharType const* node) {
    if (!enter(node)) return;
    exit(node);
}

void ConstNodeWalker::walk(type::VectorType const* node) {
    if (!enter(node)) return;
    if (node->element_type()) walk(node->element_type());
    exit(node);
}

void ConstNodeWalker::walk(name::Index const* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    exit(node);
}

void ConstNodeWalker::walk(program::Program const* node) {
    if (!enter(node)) return;
    for (auto child : node->declarations()) {
        if (child) walk(child);
    }
    if (node->main()) walk(node->main());
    exit(node);
}

}  // namespace shakujo::model::util

