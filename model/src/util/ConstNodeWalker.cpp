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
    case program::GlobalFunctionDeclaration::tag:
        walk(static_cast<program::GlobalFunctionDeclaration const*>(node));  // NOLINT
        return;
    case program::GlobalVariableDeclaration::tag:
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
    case name::QualifiedName::tag:
        walk(static_cast<name::QualifiedName const*>(node));  // NOLINT
        return;
    case name::SimpleName::tag:
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
    case expression::ArrayCreationExpression::tag:
        walk(static_cast<expression::ArrayCreationExpression const*>(node));  // NOLINT
        return;
    case expression::ArrayElementLoadExpression::tag:
        walk(static_cast<expression::ArrayElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::ArrayElementStoreExpression::tag:
        walk(static_cast<expression::ArrayElementStoreExpression const*>(node));  // NOLINT
        return;
    case expression::AssignExpression::tag:
        walk(static_cast<expression::AssignExpression const*>(node));  // NOLINT
        return;
    case expression::BinaryOperator::tag:
        walk(static_cast<expression::BinaryOperator const*>(node));  // NOLINT
        return;
    case expression::BlockExpression::tag:
        walk(static_cast<expression::BlockExpression const*>(node));  // NOLINT
        return;
    case expression::CaseExpression::tag:
        walk(static_cast<expression::CaseExpression const*>(node));  // NOLINT
        return;
    case expression::CursorAdvanceExpression::tag:
        walk(static_cast<expression::CursorAdvanceExpression const*>(node));  // NOLINT
        return;
    case expression::CursorCreationExpression::tag:
        walk(static_cast<expression::CursorCreationExpression const*>(node));  // NOLINT
        return;
    case expression::CursorElementLoadExpression::tag:
        walk(static_cast<expression::CursorElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::FunctionCall::tag:
        walk(static_cast<expression::FunctionCall const*>(node));  // NOLINT
        return;
    case expression::ImplicitCast::tag:
        walk(static_cast<expression::ImplicitCast const*>(node));  // NOLINT
        return;
    case expression::Literal::tag:
        walk(static_cast<expression::Literal const*>(node));  // NOLINT
        return;
    case expression::Placeholder::tag:
        walk(static_cast<expression::Placeholder const*>(node));  // NOLINT
        return;
    case expression::TupleCreationExpression::tag:
        walk(static_cast<expression::TupleCreationExpression const*>(node));  // NOLINT
        return;
    case expression::TupleElementLoadExpression::tag:
        walk(static_cast<expression::TupleElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::TupleElementStoreExpression::tag:
        walk(static_cast<expression::TupleElementStoreExpression const*>(node));  // NOLINT
        return;
    case expression::TypeOperator::tag:
        walk(static_cast<expression::TypeOperator const*>(node));  // NOLINT
        return;
    case expression::UnaryOperator::tag:
        walk(static_cast<expression::UnaryOperator const*>(node));  // NOLINT
        return;
    case expression::VariableReference::tag:
        walk(static_cast<expression::VariableReference const*>(node));  // NOLINT
        return;
    case expression::VectorCreationExpression::tag:
        walk(static_cast<expression::VectorCreationExpression const*>(node));  // NOLINT
        return;
    case expression::VectorElementLoadExpression::tag:
        walk(static_cast<expression::VectorElementLoadExpression const*>(node));  // NOLINT
        return;
    case expression::VectorElementStoreExpression::tag:
        walk(static_cast<expression::VectorElementStoreExpression const*>(node));  // NOLINT
        return;
    case expression::VectorLengthExpression::tag:
        walk(static_cast<expression::VectorLengthExpression const*>(node));  // NOLINT
        return;
    case expression::relation::AggregationExpression::tag:
        walk(static_cast<expression::relation::AggregationExpression const*>(node));  // NOLINT
        return;
    case expression::relation::DistinctExpression::tag:
        walk(static_cast<expression::relation::DistinctExpression const*>(node));  // NOLINT
        return;
    case expression::relation::GroupExpression::tag:
        walk(static_cast<expression::relation::GroupExpression const*>(node));  // NOLINT
        return;
    case expression::relation::JoinExpression::tag:
        walk(static_cast<expression::relation::JoinExpression const*>(node));  // NOLINT
        return;
    case expression::relation::LimitExpression::tag:
        walk(static_cast<expression::relation::LimitExpression const*>(node));  // NOLINT
        return;
    case expression::relation::OrderExpression::tag:
        walk(static_cast<expression::relation::OrderExpression const*>(node));  // NOLINT
        return;
    case expression::relation::ProjectionExpression::tag:
        walk(static_cast<expression::relation::ProjectionExpression const*>(node));  // NOLINT
        return;
    case expression::relation::RenameExpression::tag:
        walk(static_cast<expression::relation::RenameExpression const*>(node));  // NOLINT
        return;
    case expression::relation::ScanExpression::tag:
        walk(static_cast<expression::relation::ScanExpression const*>(node));  // NOLINT
        return;
    case expression::relation::SelectionExpression::tag:
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
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::RenameExpression const* node) {
    if (!enter(node)) return;
    if (node->operand()) walk(node->operand());
    if (node->name()) walk(node->name());
    for (auto child : node->columns()) {
        if (child) walk(child);
    }
    exit(node);
}

void ConstNodeWalker::walk(expression::relation::ScanExpression const* node) {
    if (!enter(node)) return;
    if (node->table()) walk(node->table());
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
    case statement::AnchorDeclaration::tag:
        walk(static_cast<statement::AnchorDeclaration const*>(node));  // NOLINT
        return;
    case statement::BlockStatement::tag:
        walk(static_cast<statement::BlockStatement const*>(node));  // NOLINT
        return;
    case statement::BranchStatement::tag:
        walk(static_cast<statement::BranchStatement const*>(node));  // NOLINT
        return;
    case statement::BreakStatement::tag:
        walk(static_cast<statement::BreakStatement const*>(node));  // NOLINT
        return;
    case statement::ContinueStatement::tag:
        walk(static_cast<statement::ContinueStatement const*>(node));  // NOLINT
        return;
    case statement::EmptyStatement::tag:
        walk(static_cast<statement::EmptyStatement const*>(node));  // NOLINT
        return;
    case statement::ExpressionStatement::tag:
        walk(static_cast<statement::ExpressionStatement const*>(node));  // NOLINT
        return;
    case statement::ForEachStatement::tag:
        walk(static_cast<statement::ForEachStatement const*>(node));  // NOLINT
        return;
    case statement::ForStatement::tag:
        walk(static_cast<statement::ForStatement const*>(node));  // NOLINT
        return;
    case statement::LocalVariableDeclaration::tag:
        walk(static_cast<statement::LocalVariableDeclaration const*>(node));  // NOLINT
        return;
    case statement::LogStatement::tag:
        walk(static_cast<statement::LogStatement const*>(node));  // NOLINT
        return;
    case statement::RaiseStatement::tag:
        walk(static_cast<statement::RaiseStatement const*>(node));  // NOLINT
        return;
    case statement::ReturnStatement::tag:
        walk(static_cast<statement::ReturnStatement const*>(node));  // NOLINT
        return;
    case statement::VectorElementDeleteStatement::tag:
        walk(static_cast<statement::VectorElementDeleteStatement const*>(node));  // NOLINT
        return;
    case statement::VectorElementInsertStatement::tag:
        walk(static_cast<statement::VectorElementInsertStatement const*>(node));  // NOLINT
        return;
    case statement::WhileStatement::tag:
        walk(static_cast<statement::WhileStatement const*>(node));  // NOLINT
        return;
    case statement::ddl::CreateTableStatement::tag:
        walk(static_cast<statement::ddl::CreateTableStatement const*>(node));  // NOLINT
        return;
    case statement::ddl::DropTableStatement::tag:
        walk(static_cast<statement::ddl::DropTableStatement const*>(node));  // NOLINT
        return;
    case statement::dml::DeleteStatement::tag:
        walk(static_cast<statement::dml::DeleteStatement const*>(node));  // NOLINT
        return;
    case statement::dml::EmitStatement::tag:
        walk(static_cast<statement::dml::EmitStatement const*>(node));  // NOLINT
        return;
    case statement::dml::InsertRelationStatement::tag:
        walk(static_cast<statement::dml::InsertRelationStatement const*>(node));  // NOLINT
        return;
    case statement::dml::InsertValuesStatement::tag:
        walk(static_cast<statement::dml::InsertValuesStatement const*>(node));  // NOLINT
        return;
    case statement::dml::UpdateStatement::tag:
        walk(static_cast<statement::dml::UpdateStatement const*>(node));  // NOLINT
        return;
    case statement::transaction::TransactionBlockStatement::tag:
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
    case type::ArrayType::tag:
        walk(static_cast<type::ArrayType const*>(node));  // NOLINT
        return;
    case type::BooleanType::tag:
        walk(static_cast<type::BooleanType const*>(node));  // NOLINT
        return;
    case type::CharType::tag:
        walk(static_cast<type::CharType const*>(node));  // NOLINT
        return;
    case type::Float32Type::tag:
        walk(static_cast<type::Float32Type const*>(node));  // NOLINT
        return;
    case type::Float64Type::tag:
        walk(static_cast<type::Float64Type const*>(node));  // NOLINT
        return;
    case type::Int32Type::tag:
        walk(static_cast<type::Int32Type const*>(node));  // NOLINT
        return;
    case type::Int64Type::tag:
        walk(static_cast<type::Int64Type const*>(node));  // NOLINT
        return;
    case type::NullType::tag:
        walk(static_cast<type::NullType const*>(node));  // NOLINT
        return;
    case type::RelationType::tag:
        walk(static_cast<type::RelationType const*>(node));  // NOLINT
        return;
    case type::StringType::tag:
        walk(static_cast<type::StringType const*>(node));  // NOLINT
        return;
    case type::TupleType::tag:
        walk(static_cast<type::TupleType const*>(node));  // NOLINT
        return;
    case type::VarCharType::tag:
        walk(static_cast<type::VarCharType const*>(node));  // NOLINT
        return;
    case type::VectorType::tag:
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

