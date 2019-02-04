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
#include "shakujo/model/util/NodeWalker.h"

#include <cstdlib>
#include <stdexcept>
#include <sstream>

#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/name/NameKind.h"
#include "shakujo/model/program/GlobalDeclarationKind.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::util {
void NodeWalker::walk(Node* node) {
    if (auto ptr = dynamic_cast<expression::Expression*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<name::Index*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<name::Name*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<program::GlobalDeclaration*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<program::Program*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<statement::Statement*>(node)) {
        walk(ptr);
        return;
    }
    if (auto ptr = dynamic_cast<type::Type*>(node)) {
        walk(ptr);
        return;
    }
    std::abort();
}

void NodeWalker::walk(program::GlobalDeclaration* node) {
    switch (node->kind()) {
    case program::GlobalDeclarationKind::GLOBAL_FUNCTION_DECLARATION:
        walk(dynamic_cast<program::GlobalFunctionDeclaration*>(node));
        return;
    case program::GlobalDeclarationKind::GLOBAL_VARIABLE_DECLARATION:
        walk(dynamic_cast<program::GlobalVariableDeclaration*>(node));
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void NodeWalker::walk(program::GlobalFunctionDeclaration* node) {
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

void NodeWalker::walk(program::GlobalVariableDeclaration* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->type()) walk(node->type());
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(name::Name* node) {
    switch (node->kind()) {
    case name::NameKind::QUALIFIED_NAME:
        walk(dynamic_cast<name::QualifiedName*>(node));
        return;
    case name::NameKind::SIMPLE_NAME:
        walk(dynamic_cast<name::SimpleName*>(node));
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void NodeWalker::walk(name::QualifiedName* node) {
    if (!enter(node)) return;
    if (node->qualifier()) walk(node->qualifier());
    if (node->name()) walk(node->name());
    exit(node);
}

void NodeWalker::walk(name::SimpleName* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(expression::Expression* node) {
    switch (node->kind()) {
    case expression::ExpressionKind::ARRAY_CREATION_EXPRESSION:
        walk(dynamic_cast<expression::ArrayCreationExpression*>(node));
        return;
    case expression::ExpressionKind::ARRAY_ELEMENT_LOAD_EXPRESSION:
        walk(dynamic_cast<expression::ArrayElementLoadExpression*>(node));
        return;
    case expression::ExpressionKind::ARRAY_ELEMENT_STORE_EXPRESSION:
        walk(dynamic_cast<expression::ArrayElementStoreExpression*>(node));
        return;
    case expression::ExpressionKind::ASSIGN_EXPRESSION:
        walk(dynamic_cast<expression::AssignExpression*>(node));
        return;
    case expression::ExpressionKind::BINARY_OPERATOR:
        walk(dynamic_cast<expression::BinaryOperator*>(node));
        return;
    case expression::ExpressionKind::BLOCK_EXPRESSION:
        walk(dynamic_cast<expression::BlockExpression*>(node));
        return;
    case expression::ExpressionKind::CASE_EXPRESSION:
        walk(dynamic_cast<expression::CaseExpression*>(node));
        return;
    case expression::ExpressionKind::CURSOR_ADVANCE_EXPRESSION:
        walk(dynamic_cast<expression::CursorAdvanceExpression*>(node));
        return;
    case expression::ExpressionKind::CURSOR_CREATION_EXPRESSION:
        walk(dynamic_cast<expression::CursorCreationExpression*>(node));
        return;
    case expression::ExpressionKind::CURSOR_ELEMENT_LOAD_EXPRESSION:
        walk(dynamic_cast<expression::CursorElementLoadExpression*>(node));
        return;
    case expression::ExpressionKind::FUNCTION_CALL:
        walk(dynamic_cast<expression::FunctionCall*>(node));
        return;
    case expression::ExpressionKind::IMPLICIT_CAST:
        walk(dynamic_cast<expression::ImplicitCast*>(node));
        return;
    case expression::ExpressionKind::LITERAL:
        walk(dynamic_cast<expression::Literal*>(node));
        return;
    case expression::ExpressionKind::PLACEHOLDER:
        walk(dynamic_cast<expression::Placeholder*>(node));
        return;
    case expression::ExpressionKind::STRING_OPERATOR:
        walk(dynamic_cast<expression::StringOperator*>(node));
        return;
    case expression::ExpressionKind::TUPLE_CREATION_EXPRESSION:
        walk(dynamic_cast<expression::TupleCreationExpression*>(node));
        return;
    case expression::ExpressionKind::TUPLE_ELEMENT_LOAD_EXPRESSION:
        walk(dynamic_cast<expression::TupleElementLoadExpression*>(node));
        return;
    case expression::ExpressionKind::TUPLE_ELEMENT_STORE_EXPRESSION:
        walk(dynamic_cast<expression::TupleElementStoreExpression*>(node));
        return;
    case expression::ExpressionKind::TYPE_OPERATOR:
        walk(dynamic_cast<expression::TypeOperator*>(node));
        return;
    case expression::ExpressionKind::UNARY_OPERATOR:
        walk(dynamic_cast<expression::UnaryOperator*>(node));
        return;
    case expression::ExpressionKind::VARIABLE_REFERENCE:
        walk(dynamic_cast<expression::VariableReference*>(node));
        return;
    case expression::ExpressionKind::VECTOR_CREATION_EXPRESSION:
        walk(dynamic_cast<expression::VectorCreationExpression*>(node));
        return;
    case expression::ExpressionKind::VECTOR_ELEMENT_LOAD_EXPRESSION:
        walk(dynamic_cast<expression::VectorElementLoadExpression*>(node));
        return;
    case expression::ExpressionKind::VECTOR_ELEMENT_STORE_EXPRESSION:
        walk(dynamic_cast<expression::VectorElementStoreExpression*>(node));
        return;
    case expression::ExpressionKind::VECTOR_LENGTH_EXPRESSION:
        walk(dynamic_cast<expression::VectorLengthExpression*>(node));
        return;
    case expression::ExpressionKind::AGGREGATION_EXPRESSION:
        walk(dynamic_cast<expression::relation::AggregationExpression*>(node));
        return;
    case expression::ExpressionKind::DISTINCT_EXPRESSION:
        walk(dynamic_cast<expression::relation::DistinctExpression*>(node));
        return;
    case expression::ExpressionKind::JOIN_EXPRESSION:
        walk(dynamic_cast<expression::relation::JoinExpression*>(node));
        return;
    case expression::ExpressionKind::LIMIT_EXPRESSION:
        walk(dynamic_cast<expression::relation::LimitExpression*>(node));
        return;
    case expression::ExpressionKind::ORDER_EXPRESSION:
        walk(dynamic_cast<expression::relation::OrderExpression*>(node));
        return;
    case expression::ExpressionKind::PROJECTION_EXPRESSION:
        walk(dynamic_cast<expression::relation::ProjectionExpression*>(node));
        return;
    case expression::ExpressionKind::SCAN_EXPRESSION:
        walk(dynamic_cast<expression::relation::ScanExpression*>(node));
        return;
    case expression::ExpressionKind::SELECTION_EXPRESSION:
        walk(dynamic_cast<expression::relation::SelectionExpression*>(node));
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void NodeWalker::walk(expression::ArrayCreationExpression* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(expression::ArrayElementLoadExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    exit(node);
}

void NodeWalker::walk(expression::ArrayElementStoreExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(expression::AssignExpression* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(expression::BinaryOperator* node) {
    if (!enter(node)) return;
    if (node->left()) walk(node->left());
    if (node->right()) walk(node->right());
    exit(node);
}

void NodeWalker::walk(expression::BlockExpression* node) {
    if (!enter(node)) return;
    for (auto child : node->head()) {
        if (child) walk(child);
    }
    if (node->body()) walk(node->body());
    exit(node);
}

void NodeWalker::walk(expression::CaseExpression* node) {
    if (!enter(node)) return;
    for (auto child : node->cases()) {
        if (child->condition()) walk(child->condition());
        if (child->body()) walk(child->body());
    }
    if (node->default_expression()) walk(node->default_expression());
    exit(node);
}

void NodeWalker::walk(expression::CursorAdvanceExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void NodeWalker::walk(expression::CursorCreationExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void NodeWalker::walk(expression::CursorElementLoadExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void NodeWalker::walk(expression::FunctionCall* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    for (auto child : node->arguments()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(expression::ImplicitCast* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void NodeWalker::walk(expression::Literal* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(expression::Placeholder* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(expression::StringOperator* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void NodeWalker::walk(expression::TupleCreationExpression* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(expression::TupleElementLoadExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    exit(node);
}

void NodeWalker::walk(expression::TupleElementStoreExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(expression::TypeOperator* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    if (node->operand()) walk(node->operand());
    exit(node);
}

void NodeWalker::walk(expression::UnaryOperator* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void NodeWalker::walk(expression::VariableReference* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    exit(node);
}

void NodeWalker::walk(expression::VectorCreationExpression* node) {
    if (!enter(node)) return;
    if (node->type()) walk(node->type());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(expression::VectorElementLoadExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    exit(node);
}

void NodeWalker::walk(expression::VectorElementStoreExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(expression::VectorLengthExpression* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    exit(node);
}

void NodeWalker::walk(expression::relation::AggregationExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->keys()) {
        if (child) walk(child);
    }
    for (auto child : node->initialize()) {
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

void NodeWalker::walk(expression::relation::DistinctExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->subsets()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(expression::relation::JoinExpression* node) {
    if (!enter(node)) return;
    if (node->left()) walk(node->left());
    if (node->right()) walk(node->right());
    if (node->condition()) walk(node->condition());
    exit(node);
}

void NodeWalker::walk(expression::relation::LimitExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void NodeWalker::walk(expression::relation::OrderExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->elements()) {
        if (child->column()) walk(child->column());
    }
    exit(node);
}

void NodeWalker::walk(expression::relation::ProjectionExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->initialize()) {
        if (child) walk(child);
    }
    for (auto child : node->columns()) {
        if (child->value()) walk(child->value());
        if (child->alias()) walk(child->alias());
    }
    if (node->alias()) walk(node->alias());
    exit(node);
}

void NodeWalker::walk(expression::relation::ScanExpression* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    if (node->alias()) walk(node->alias());
    if (node->condition()) walk(node->condition());
    exit(node);
}

void NodeWalker::walk(expression::relation::SelectionExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    if (node->condition()) walk(node->condition());
    exit(node);
}

void NodeWalker::walk(statement::Statement* node) {
    switch (node->kind()) {
    case statement::StatementKind::ANCHOR_DECLARATION:
        walk(dynamic_cast<statement::AnchorDeclaration*>(node));
        return;
    case statement::StatementKind::BLOCK_STATEMENT:
        walk(dynamic_cast<statement::BlockStatement*>(node));
        return;
    case statement::StatementKind::BRANCH_STATEMENT:
        walk(dynamic_cast<statement::BranchStatement*>(node));
        return;
    case statement::StatementKind::BREAK_STATEMENT:
        walk(dynamic_cast<statement::BreakStatement*>(node));
        return;
    case statement::StatementKind::CONTINUE_STATEMENT:
        walk(dynamic_cast<statement::ContinueStatement*>(node));
        return;
    case statement::StatementKind::EMPTY_STATEMENT:
        walk(dynamic_cast<statement::EmptyStatement*>(node));
        return;
    case statement::StatementKind::EXPRESSION_STATEMENT:
        walk(dynamic_cast<statement::ExpressionStatement*>(node));
        return;
    case statement::StatementKind::FOR_EACH_STATEMENT:
        walk(dynamic_cast<statement::ForEachStatement*>(node));
        return;
    case statement::StatementKind::FOR_STATEMENT:
        walk(dynamic_cast<statement::ForStatement*>(node));
        return;
    case statement::StatementKind::LOCAL_VARIABLE_DECLARATION:
        walk(dynamic_cast<statement::LocalVariableDeclaration*>(node));
        return;
    case statement::StatementKind::LOG_STATEMENT:
        walk(dynamic_cast<statement::LogStatement*>(node));
        return;
    case statement::StatementKind::RAISE_STATEMENT:
        walk(dynamic_cast<statement::RaiseStatement*>(node));
        return;
    case statement::StatementKind::RETURN_STATEMENT:
        walk(dynamic_cast<statement::ReturnStatement*>(node));
        return;
    case statement::StatementKind::VECTOR_ELEMENT_DELETE_STATEMENT:
        walk(dynamic_cast<statement::VectorElementDeleteStatement*>(node));
        return;
    case statement::StatementKind::VECTOR_ELEMENT_INSERT_STATEMENT:
        walk(dynamic_cast<statement::VectorElementInsertStatement*>(node));
        return;
    case statement::StatementKind::WHILE_STATEMENT:
        walk(dynamic_cast<statement::WhileStatement*>(node));
        return;
    case statement::StatementKind::CREATE_TABLE_STATEMENT:
        walk(dynamic_cast<statement::ddl::CreateTableStatement*>(node));
        return;
    case statement::StatementKind::DROP_TABLE_STATEMENT:
        walk(dynamic_cast<statement::ddl::DropTableStatement*>(node));
        return;
    case statement::StatementKind::DELETE_STATEMENT:
        walk(dynamic_cast<statement::dml::DeleteStatement*>(node));
        return;
    case statement::StatementKind::EMIT_STATEMENT:
        walk(dynamic_cast<statement::dml::EmitStatement*>(node));
        return;
    case statement::StatementKind::INSERT_RELATION_STATEMENT:
        walk(dynamic_cast<statement::dml::InsertRelationStatement*>(node));
        return;
    case statement::StatementKind::INSERT_VALUES_STATEMENT:
        walk(dynamic_cast<statement::dml::InsertValuesStatement*>(node));
        return;
    case statement::StatementKind::UPDATE_STATEMENT:
        walk(dynamic_cast<statement::dml::UpdateStatement*>(node));
        return;
    case statement::StatementKind::TRANSACTION_BLOCK_STATEMENT:
        walk(dynamic_cast<statement::transaction::TransactionBlockStatement*>(node));
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void NodeWalker::walk(statement::AnchorDeclaration* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->body()) walk(node->body());
    exit(node);
}

void NodeWalker::walk(statement::BlockStatement* node) {
    if (!enter(node)) return;
    for (auto child : node->elements()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(statement::BranchStatement* node) {
    if (!enter(node)) return;
    if (node->condition()) walk(node->condition());
    if (node->then_statement()) walk(node->then_statement());
    if (node->else_statement()) walk(node->else_statement());
    exit(node);
}

void NodeWalker::walk(statement::BreakStatement* node) {
    if (!enter(node)) return;
    if (node->anchor()) walk(node->anchor());
    exit(node);
}

void NodeWalker::walk(statement::ContinueStatement* node) {
    if (!enter(node)) return;
    if (node->anchor()) walk(node->anchor());
    exit(node);
}

void NodeWalker::walk(statement::EmptyStatement* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(statement::ExpressionStatement* node) {
    if (!enter(node)) return;
    if (node->body()) walk(node->body());
    exit(node);
}

void NodeWalker::walk(statement::ForEachStatement* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->type()) walk(node->type());
    if (node->enumeration()) walk(node->enumeration());
    if (node->body()) walk(node->body());
    exit(node);
}

void NodeWalker::walk(statement::ForStatement* node) {
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

void NodeWalker::walk(statement::LocalVariableDeclaration* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    if (node->type()) walk(node->type());
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(statement::LogStatement* node) {
    if (!enter(node)) return;
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(statement::RaiseStatement* node) {
    if (!enter(node)) return;
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(statement::ReturnStatement* node) {
    if (!enter(node)) return;
    if (node->value()) walk(node->value());
    exit(node);
}

void NodeWalker::walk(statement::VectorElementDeleteStatement* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->begin()) walk(node->begin());
    if (node->end()) walk(node->end());
    exit(node);
}

void NodeWalker::walk(statement::VectorElementInsertStatement* node) {
    if (!enter(node)) return;
    if (node->data()) walk(node->data());
    if (node->index()) walk(node->index());
    for (auto child : node->values()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(statement::WhileStatement* node) {
    if (!enter(node)) return;
    if (node->condition()) walk(node->condition());
    if (node->body()) walk(node->body());
    exit(node);
}

void NodeWalker::walk(statement::ddl::CreateTableStatement* node) {
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

void NodeWalker::walk(statement::ddl::DropTableStatement* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    exit(node);
}

void NodeWalker::walk(statement::dml::DeleteStatement* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    if (node->condition()) walk(node->condition());
    exit(node);
}

void NodeWalker::walk(statement::dml::EmitStatement* node) {
    if (!enter(node)) return;
    if (node->source()) walk(node->source());
    exit(node);
}

void NodeWalker::walk(statement::dml::InsertRelationStatement* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    for (auto child : node->columns()) {
        if (child) walk(child);
    }
    if (node->source()) walk(node->source());
    exit(node);
}

void NodeWalker::walk(statement::dml::InsertValuesStatement* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    for (auto child : node->columns()) {
        if (child->name()) walk(child->name());
        if (child->value()) walk(child->value());
    }
    exit(node);
}

void NodeWalker::walk(statement::dml::UpdateStatement* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
    for (auto child : node->columns()) {
        if (child->name()) walk(child->name());
        if (child->value()) walk(child->value());
    }
    if (node->condition()) walk(node->condition());
    exit(node);
}

void NodeWalker::walk(statement::transaction::TransactionBlockStatement* node) {
    if (!enter(node)) return;
    if (node->body()) walk(node->body());
    exit(node);
}

void NodeWalker::walk(type::Type* node) {
    switch (node->kind()) {
    case type::TypeKind::ARRAY_TYPE:
        walk(dynamic_cast<type::ArrayType*>(node));
        return;
    case type::TypeKind::BOOLEAN_TYPE:
        walk(dynamic_cast<type::BooleanType*>(node));
        return;
    case type::TypeKind::CHAR_TYPE:
        walk(dynamic_cast<type::CharType*>(node));
        return;
    case type::TypeKind::FLOAT32_TYPE:
        walk(dynamic_cast<type::Float32Type*>(node));
        return;
    case type::TypeKind::FLOAT64_TYPE:
        walk(dynamic_cast<type::Float64Type*>(node));
        return;
    case type::TypeKind::INT32_TYPE:
        walk(dynamic_cast<type::Int32Type*>(node));
        return;
    case type::TypeKind::INT64_TYPE:
        walk(dynamic_cast<type::Int64Type*>(node));
        return;
    case type::TypeKind::NULL_TYPE:
        walk(dynamic_cast<type::NullType*>(node));
        return;
    case type::TypeKind::RELATION_TYPE:
        walk(dynamic_cast<type::RelationType*>(node));
        return;
    case type::TypeKind::STRING_TYPE:
        walk(dynamic_cast<type::StringType*>(node));
        return;
    case type::TypeKind::TUPLE_TYPE:
        walk(dynamic_cast<type::TupleType*>(node));
        return;
    case type::TypeKind::VAR_CHAR_TYPE:
        walk(dynamic_cast<type::VarCharType*>(node));
        return;
    case type::TypeKind::VECTOR_TYPE:
        walk(dynamic_cast<type::VectorType*>(node));
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << node->kind();
    throw std::invalid_argument(ss.str());
}

void NodeWalker::walk(type::ArrayType* node) {
    if (!enter(node)) return;
    if (node->element_type()) walk(node->element_type());
    exit(node);
}

void NodeWalker::walk(type::BooleanType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::CharType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::Float32Type* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::Float64Type* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::Int32Type* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::Int64Type* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::NullType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::RelationType* node) {
    if (!enter(node)) return;
    if (node->element_type()) walk(node->element_type());
    exit(node);
}

void NodeWalker::walk(type::StringType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::TupleType* node) {
    if (!enter(node)) return;
    for (auto child : node->elements()) {
        if (child->name()) walk(child->name());
        if (child->type()) walk(child->type());
    }
    exit(node);
}

void NodeWalker::walk(type::VarCharType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::VectorType* node) {
    if (!enter(node)) return;
    if (node->element_type()) walk(node->element_type());
    exit(node);
}

void NodeWalker::walk(name::Index* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    exit(node);
}

void NodeWalker::walk(program::Program* node) {
    if (!enter(node)) return;
    for (auto child : node->declarations()) {
        if (child) walk(child);
    }
    if (node->main()) walk(node->main());
    exit(node);
}

}  // namespace shakujo::model::util

