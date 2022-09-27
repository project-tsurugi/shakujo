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
    if (auto ptr = dynamic_cast<program::Comment*>(node)) {
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
    case program::GlobalFunctionDeclaration::tag:
        walk(static_cast<program::GlobalFunctionDeclaration*>(node));  // NOLINT
        return;
    case program::GlobalVariableDeclaration::tag:
        walk(static_cast<program::GlobalVariableDeclaration*>(node));  // NOLINT
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
    case name::QualifiedName::tag:
        walk(static_cast<name::QualifiedName*>(node));  // NOLINT
        return;
    case name::SimpleName::tag:
        walk(static_cast<name::SimpleName*>(node));  // NOLINT
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
    case expression::ArrayCreationExpression::tag:
        walk(static_cast<expression::ArrayCreationExpression*>(node));  // NOLINT
        return;
    case expression::ArrayElementLoadExpression::tag:
        walk(static_cast<expression::ArrayElementLoadExpression*>(node));  // NOLINT
        return;
    case expression::ArrayElementStoreExpression::tag:
        walk(static_cast<expression::ArrayElementStoreExpression*>(node));  // NOLINT
        return;
    case expression::AssignExpression::tag:
        walk(static_cast<expression::AssignExpression*>(node));  // NOLINT
        return;
    case expression::BinaryOperator::tag:
        walk(static_cast<expression::BinaryOperator*>(node));  // NOLINT
        return;
    case expression::BlockExpression::tag:
        walk(static_cast<expression::BlockExpression*>(node));  // NOLINT
        return;
    case expression::CaseExpression::tag:
        walk(static_cast<expression::CaseExpression*>(node));  // NOLINT
        return;
    case expression::CursorAdvanceExpression::tag:
        walk(static_cast<expression::CursorAdvanceExpression*>(node));  // NOLINT
        return;
    case expression::CursorCreationExpression::tag:
        walk(static_cast<expression::CursorCreationExpression*>(node));  // NOLINT
        return;
    case expression::CursorElementLoadExpression::tag:
        walk(static_cast<expression::CursorElementLoadExpression*>(node));  // NOLINT
        return;
    case expression::FunctionCall::tag:
        walk(static_cast<expression::FunctionCall*>(node));  // NOLINT
        return;
    case expression::ImplicitCast::tag:
        walk(static_cast<expression::ImplicitCast*>(node));  // NOLINT
        return;
    case expression::Literal::tag:
        walk(static_cast<expression::Literal*>(node));  // NOLINT
        return;
    case expression::Placeholder::tag:
        walk(static_cast<expression::Placeholder*>(node));  // NOLINT
        return;
    case expression::TupleCreationExpression::tag:
        walk(static_cast<expression::TupleCreationExpression*>(node));  // NOLINT
        return;
    case expression::TupleElementLoadExpression::tag:
        walk(static_cast<expression::TupleElementLoadExpression*>(node));  // NOLINT
        return;
    case expression::TupleElementStoreExpression::tag:
        walk(static_cast<expression::TupleElementStoreExpression*>(node));  // NOLINT
        return;
    case expression::TypeOperator::tag:
        walk(static_cast<expression::TypeOperator*>(node));  // NOLINT
        return;
    case expression::UnaryOperator::tag:
        walk(static_cast<expression::UnaryOperator*>(node));  // NOLINT
        return;
    case expression::VariableReference::tag:
        walk(static_cast<expression::VariableReference*>(node));  // NOLINT
        return;
    case expression::VectorCreationExpression::tag:
        walk(static_cast<expression::VectorCreationExpression*>(node));  // NOLINT
        return;
    case expression::VectorElementLoadExpression::tag:
        walk(static_cast<expression::VectorElementLoadExpression*>(node));  // NOLINT
        return;
    case expression::VectorElementStoreExpression::tag:
        walk(static_cast<expression::VectorElementStoreExpression*>(node));  // NOLINT
        return;
    case expression::VectorLengthExpression::tag:
        walk(static_cast<expression::VectorLengthExpression*>(node));  // NOLINT
        return;
    case expression::relation::AggregationExpression::tag:
        walk(static_cast<expression::relation::AggregationExpression*>(node));  // NOLINT
        return;
    case expression::relation::DistinctExpression::tag:
        walk(static_cast<expression::relation::DistinctExpression*>(node));  // NOLINT
        return;
    case expression::relation::GroupExpression::tag:
        walk(static_cast<expression::relation::GroupExpression*>(node));  // NOLINT
        return;
    case expression::relation::JoinExpression::tag:
        walk(static_cast<expression::relation::JoinExpression*>(node));  // NOLINT
        return;
    case expression::relation::LimitExpression::tag:
        walk(static_cast<expression::relation::LimitExpression*>(node));  // NOLINT
        return;
    case expression::relation::OrderExpression::tag:
        walk(static_cast<expression::relation::OrderExpression*>(node));  // NOLINT
        return;
    case expression::relation::ProjectionExpression::tag:
        walk(static_cast<expression::relation::ProjectionExpression*>(node));  // NOLINT
        return;
    case expression::relation::RenameExpression::tag:
        walk(static_cast<expression::relation::RenameExpression*>(node));  // NOLINT
        return;
    case expression::relation::ScanExpression::tag:
        walk(static_cast<expression::relation::ScanExpression*>(node));  // NOLINT
        return;
    case expression::relation::SelectionExpression::tag:
        walk(static_cast<expression::relation::SelectionExpression*>(node));  // NOLINT
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
    for (auto child : node->columns()) {
        if (child->function()) walk(child->function());
        if (child->operand()) walk(child->operand());
        if (child->alias()) walk(child->alias());
    }
    exit(node);
}

void NodeWalker::walk(expression::relation::DistinctExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    exit(node);
}

void NodeWalker::walk(expression::relation::GroupExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->keys()) {
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
        if (child->key()) walk(child->key());
    }
    exit(node);
}

void NodeWalker::walk(expression::relation::ProjectionExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    for (auto child : node->columns()) {
        if (child->value()) walk(child->value());
        if (child->alias()) walk(child->alias());
    }
    exit(node);
}

void NodeWalker::walk(expression::relation::RenameExpression* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    if (node->name()) walk(node->name());
    for (auto child : node->columns()) {
        if (child) walk(child);
    }
    exit(node);
}

void NodeWalker::walk(expression::relation::ScanExpression* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
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
    case statement::AnchorDeclaration::tag:
        walk(static_cast<statement::AnchorDeclaration*>(node));  // NOLINT
        return;
    case statement::BlockStatement::tag:
        walk(static_cast<statement::BlockStatement*>(node));  // NOLINT
        return;
    case statement::BranchStatement::tag:
        walk(static_cast<statement::BranchStatement*>(node));  // NOLINT
        return;
    case statement::BreakStatement::tag:
        walk(static_cast<statement::BreakStatement*>(node));  // NOLINT
        return;
    case statement::ContinueStatement::tag:
        walk(static_cast<statement::ContinueStatement*>(node));  // NOLINT
        return;
    case statement::EmptyStatement::tag:
        walk(static_cast<statement::EmptyStatement*>(node));  // NOLINT
        return;
    case statement::ExpressionStatement::tag:
        walk(static_cast<statement::ExpressionStatement*>(node));  // NOLINT
        return;
    case statement::ForEachStatement::tag:
        walk(static_cast<statement::ForEachStatement*>(node));  // NOLINT
        return;
    case statement::ForStatement::tag:
        walk(static_cast<statement::ForStatement*>(node));  // NOLINT
        return;
    case statement::LocalVariableDeclaration::tag:
        walk(static_cast<statement::LocalVariableDeclaration*>(node));  // NOLINT
        return;
    case statement::LogStatement::tag:
        walk(static_cast<statement::LogStatement*>(node));  // NOLINT
        return;
    case statement::RaiseStatement::tag:
        walk(static_cast<statement::RaiseStatement*>(node));  // NOLINT
        return;
    case statement::ReturnStatement::tag:
        walk(static_cast<statement::ReturnStatement*>(node));  // NOLINT
        return;
    case statement::VectorElementDeleteStatement::tag:
        walk(static_cast<statement::VectorElementDeleteStatement*>(node));  // NOLINT
        return;
    case statement::VectorElementInsertStatement::tag:
        walk(static_cast<statement::VectorElementInsertStatement*>(node));  // NOLINT
        return;
    case statement::WhileStatement::tag:
        walk(static_cast<statement::WhileStatement*>(node));  // NOLINT
        return;
    case statement::ddl::CreateTableStatement::tag:
        walk(static_cast<statement::ddl::CreateTableStatement*>(node));  // NOLINT
        return;
    case statement::ddl::DropTableStatement::tag:
        walk(static_cast<statement::ddl::DropTableStatement*>(node));  // NOLINT
        return;
    case statement::dml::DeleteStatement::tag:
        walk(static_cast<statement::dml::DeleteStatement*>(node));  // NOLINT
        return;
    case statement::dml::EmitStatement::tag:
        walk(static_cast<statement::dml::EmitStatement*>(node));  // NOLINT
        return;
    case statement::dml::InsertRelationStatement::tag:
        walk(static_cast<statement::dml::InsertRelationStatement*>(node));  // NOLINT
        return;
    case statement::dml::InsertValuesStatement::tag:
        walk(static_cast<statement::dml::InsertValuesStatement*>(node));  // NOLINT
        return;
    case statement::dml::UpdateStatement::tag:
        walk(static_cast<statement::dml::UpdateStatement*>(node));  // NOLINT
        return;
    case statement::transaction::TransactionBlockStatement::tag:
        walk(static_cast<statement::transaction::TransactionBlockStatement*>(node));  // NOLINT
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
    if (node->source()) walk(node->source());
    if (node->table()) walk(node->table());
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
    for (auto child : node->initialize()) {
        if (child) walk(child);
    }
    for (auto child : node->columns()) {
        if (child->name()) walk(child->name());
        if (child->value()) walk(child->value());
    }
    exit(node);
}

void NodeWalker::walk(statement::dml::UpdateStatement* node) {
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

void NodeWalker::walk(statement::transaction::TransactionBlockStatement* node) {
    if (!enter(node)) return;
    if (node->body()) walk(node->body());
    exit(node);
}

void NodeWalker::walk(type::Type* node) {
    switch (node->kind()) {
    case type::ArrayType::tag:
        walk(static_cast<type::ArrayType*>(node));  // NOLINT
        return;
    case type::BooleanType::tag:
        walk(static_cast<type::BooleanType*>(node));  // NOLINT
        return;
    case type::CharType::tag:
        walk(static_cast<type::CharType*>(node));  // NOLINT
        return;
    case type::Float32Type::tag:
        walk(static_cast<type::Float32Type*>(node));  // NOLINT
        return;
    case type::Float64Type::tag:
        walk(static_cast<type::Float64Type*>(node));  // NOLINT
        return;
    case type::Int32Type::tag:
        walk(static_cast<type::Int32Type*>(node));  // NOLINT
        return;
    case type::Int64Type::tag:
        walk(static_cast<type::Int64Type*>(node));  // NOLINT
        return;
    case type::NullType::tag:
        walk(static_cast<type::NullType*>(node));  // NOLINT
        return;
    case type::RelationType::tag:
        walk(static_cast<type::RelationType*>(node));  // NOLINT
        return;
    case type::StringType::tag:
        walk(static_cast<type::StringType*>(node));  // NOLINT
        return;
    case type::TupleType::tag:
        walk(static_cast<type::TupleType*>(node));  // NOLINT
        return;
    case type::VarCharType::tag:
        walk(static_cast<type::VarCharType*>(node));  // NOLINT
        return;
    case type::VectorType::tag:
        walk(static_cast<type::VectorType*>(node));  // NOLINT
        return;
    case type::DecimalType::tag:
        walk(static_cast<type::DecimalType*>(node)); // NOLINT
        return;
    case type::BinaryType::tag:
        walk(static_cast<type::BinaryType*>(node)); // NOLINT
        return;
    case type::VarBinaryType::tag:
        walk(static_cast<type::VarBinaryType*>(node)); // NOLINT
        return;
    case type::DateType::tag:
        walk(static_cast<type::DateType*>(node)); // NOLINT
        return;
    case type::TimeType::tag:
        walk(static_cast<type::TimeType*>(node)); // NOLINT
        return;
    case type::TimestampType::tag:
        walk(static_cast<type::TimestampType*>(node)); // NOLINT
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

void NodeWalker::walk(type::DecimalType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::BinaryType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::VarBinaryType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::DateType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::TimeType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(type::TimestampType* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(name::Index* node) {
    if (!enter(node)) return;
    if (node->name()) walk(node->name());
    exit(node);
}

void NodeWalker::walk(program::Comment* node) {
    if (!enter(node)) return;
    exit(node);
}

void NodeWalker::walk(program::Program* node) {
    if (!enter(node)) return;
    for (auto child : node->declarations()) {
        if (child) walk(child);
    }
    if (node->main()) walk(node->main());
    for (auto child : node->comments()) {
        if (child) walk(child);
    }
    exit(node);
}

}  // namespace shakujo::model::util

