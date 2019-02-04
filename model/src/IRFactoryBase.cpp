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
#include "shakujo/model/IRFactoryBase.h"

#include <cstddef>
#include <utility>
#include <memory>
#include <string>
#include <initializer_list>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/util/MoveInitializerList.h"
#include "shakujo/model/expression/BinaryOperator.h"
#include "shakujo/model/expression/CaseExpression.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/FunctionCall.h"
#include "shakujo/model/expression/StringOperator.h"
#include "shakujo/model/expression/TypeOperator.h"
#include "shakujo/model/expression/UnaryOperator.h"
#include "shakujo/model/expression/relation/AggregationExpression.h"
#include "shakujo/model/expression/relation/JoinExpression.h"
#include "shakujo/model/expression/relation/OrderExpression.h"
#include "shakujo/model/expression/relation/ProjectionExpression.h"
#include "shakujo/model/name/Index.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/program/GlobalDeclaration.h"
#include "shakujo/model/program/GlobalFunctionDeclaration.h"
#include "shakujo/model/program/GlobalVariableDeclaration.h"
#include "shakujo/model/statement/LocalVariableDeclaration.h"
#include "shakujo/model/statement/LogStatement.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/ddl/CreateTableStatement.h"
#include "shakujo/model/statement/ddl/DropTableStatement.h"
#include "shakujo/model/statement/dml/InsertValuesStatement.h"
#include "shakujo/model/statement/dml/UpdateStatement.h"
#include "shakujo/model/type/TupleType.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model {
std::unique_ptr<expression::ArrayCreationExpression> IRFactoryBase::ArrayCreationExpression() {
    return std::make_unique<expression::ArrayCreationExpression>();
}

std::unique_ptr<expression::ArrayCreationExpression> IRFactoryBase::ArrayCreationExpression(
        std::unique_ptr<type::Type> type,
        common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values) {
    auto ret = ArrayCreationExpression();
    ret->type(std::move(type));
    ret->values() = std::move(values).build();
    return ret;
}

std::unique_ptr<expression::ArrayElementLoadExpression> IRFactoryBase::ArrayElementLoadExpression() {
    return std::make_unique<expression::ArrayElementLoadExpression>();
}

std::unique_ptr<expression::ArrayElementLoadExpression> IRFactoryBase::ArrayElementLoadExpression(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<expression::Expression> index) {
    auto ret = ArrayElementLoadExpression();
    ret->data(std::move(data));
    ret->index(std::move(index));
    return ret;
}

std::unique_ptr<expression::ArrayElementStoreExpression> IRFactoryBase::ArrayElementStoreExpression() {
    return std::make_unique<expression::ArrayElementStoreExpression>();
}

std::unique_ptr<expression::ArrayElementStoreExpression> IRFactoryBase::ArrayElementStoreExpression(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<expression::Expression> index,
        std::unique_ptr<expression::Expression> value) {
    auto ret = ArrayElementStoreExpression();
    ret->data(std::move(data));
    ret->index(std::move(index));
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<expression::AssignExpression> IRFactoryBase::AssignExpression() {
    return std::make_unique<expression::AssignExpression>();
}

std::unique_ptr<expression::AssignExpression> IRFactoryBase::AssignExpression(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<expression::Expression> value) {
    auto ret = AssignExpression();
    ret->name(std::move(name));
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<expression::BinaryOperator> IRFactoryBase::BinaryOperator() {
    return std::make_unique<expression::BinaryOperator>();
}

std::unique_ptr<expression::BinaryOperator> IRFactoryBase::BinaryOperator(
        expression::BinaryOperator::Kind operator_kind,
        std::unique_ptr<expression::Expression> left,
        std::unique_ptr<expression::Expression> right) {
    auto ret = BinaryOperator();
    ret->operator_kind(operator_kind);
    ret->left(std::move(left));
    ret->right(std::move(right));
    return ret;
}

std::unique_ptr<expression::BlockExpression> IRFactoryBase::BlockExpression() {
    return std::make_unique<expression::BlockExpression>();
}

std::unique_ptr<expression::BlockExpression> IRFactoryBase::BlockExpression(
        common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> head,
        std::unique_ptr<expression::Expression> body) {
    auto ret = BlockExpression();
    ret->head() = std::move(head).build();
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<expression::CaseExpression> IRFactoryBase::CaseExpression() {
    return std::make_unique<expression::CaseExpression>();
}

std::unique_ptr<expression::CaseExpression> IRFactoryBase::CaseExpression(
        common::util::MoveInitializerList<std::unique_ptr<expression::CaseExpression::Clause>> cases,
        std::unique_ptr<expression::Expression> default_expression) {
    auto ret = CaseExpression();
    ret->cases() = std::move(cases).build();
    ret->default_expression(std::move(default_expression));
    return ret;
}

std::unique_ptr<expression::CaseExpression::Clause> IRFactoryBase::CaseExpressionClause() {
    return std::make_unique<expression::CaseExpression::Clause>();
}

std::unique_ptr<expression::CaseExpression::Clause> IRFactoryBase::CaseExpressionClause(
        std::unique_ptr<expression::Expression> condition,
        std::unique_ptr<expression::Expression> body) {
    auto ret = CaseExpressionClause();
    ret->condition(std::move(condition));
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<expression::CursorAdvanceExpression> IRFactoryBase::CursorAdvanceExpression() {
    return std::make_unique<expression::CursorAdvanceExpression>();
}

std::unique_ptr<expression::CursorAdvanceExpression> IRFactoryBase::CursorAdvanceExpression(
        std::unique_ptr<expression::Expression> data) {
    auto ret = CursorAdvanceExpression();
    ret->data(std::move(data));
    return ret;
}

std::unique_ptr<expression::CursorCreationExpression> IRFactoryBase::CursorCreationExpression() {
    return std::make_unique<expression::CursorCreationExpression>();
}

std::unique_ptr<expression::CursorCreationExpression> IRFactoryBase::CursorCreationExpression(
        std::unique_ptr<expression::Expression> data) {
    auto ret = CursorCreationExpression();
    ret->data(std::move(data));
    return ret;
}

std::unique_ptr<expression::CursorElementLoadExpression> IRFactoryBase::CursorElementLoadExpression() {
    return std::make_unique<expression::CursorElementLoadExpression>();
}

std::unique_ptr<expression::CursorElementLoadExpression> IRFactoryBase::CursorElementLoadExpression(
        std::unique_ptr<expression::Expression> data) {
    auto ret = CursorElementLoadExpression();
    ret->data(std::move(data));
    return ret;
}

std::unique_ptr<expression::FunctionCall> IRFactoryBase::FunctionCall() {
    return std::make_unique<expression::FunctionCall>();
}

std::unique_ptr<expression::FunctionCall> IRFactoryBase::FunctionCall(
        std::unique_ptr<name::Name> name,
        common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> arguments,
        expression::FunctionCall::Quantifier quantifier) {
    auto ret = FunctionCall();
    ret->name(std::move(name));
    ret->arguments() = std::move(arguments).build();
    ret->quantifier(quantifier);
    return ret;
}

std::unique_ptr<expression::ImplicitCast> IRFactoryBase::ImplicitCast() {
    return std::make_unique<expression::ImplicitCast>();
}

std::unique_ptr<expression::ImplicitCast> IRFactoryBase::ImplicitCast(
        std::unique_ptr<common::core::Type> type,
        std::unique_ptr<expression::Expression> operand) {
    auto ret = ImplicitCast();
    ret->type(std::move(type));
    ret->operand(std::move(operand));
    return ret;
}

std::unique_ptr<expression::Literal> IRFactoryBase::Literal() {
    return std::make_unique<expression::Literal>();
}

std::unique_ptr<expression::Literal> IRFactoryBase::Literal(
        std::unique_ptr<common::core::Type> type,
        std::unique_ptr<common::core::Value> value) {
    auto ret = Literal();
    ret->type(std::move(type));
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<expression::Placeholder> IRFactoryBase::Placeholder() {
    return std::make_unique<expression::Placeholder>();
}

std::unique_ptr<expression::Placeholder> IRFactoryBase::Placeholder(
        std::string name) {
    auto ret = Placeholder();
    ret->name(std::move(name));
    return ret;
}

std::unique_ptr<expression::StringOperator> IRFactoryBase::StringOperator() {
    return std::make_unique<expression::StringOperator>();
}

std::unique_ptr<expression::StringOperator> IRFactoryBase::StringOperator(
        expression::StringOperator::Kind operator_kind,
        std::string string,
        std::unique_ptr<expression::Expression> operand) {
    auto ret = StringOperator();
    ret->operator_kind(operator_kind);
    ret->string(std::move(string));
    ret->operand(std::move(operand));
    return ret;
}

std::unique_ptr<expression::TupleCreationExpression> IRFactoryBase::TupleCreationExpression() {
    return std::make_unique<expression::TupleCreationExpression>();
}

std::unique_ptr<expression::TupleCreationExpression> IRFactoryBase::TupleCreationExpression(
        std::unique_ptr<type::Type> type,
        common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values) {
    auto ret = TupleCreationExpression();
    ret->type(std::move(type));
    ret->values() = std::move(values).build();
    return ret;
}

std::unique_ptr<expression::TupleElementLoadExpression> IRFactoryBase::TupleElementLoadExpression() {
    return std::make_unique<expression::TupleElementLoadExpression>();
}

std::unique_ptr<expression::TupleElementLoadExpression> IRFactoryBase::TupleElementLoadExpression(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<name::Index> index) {
    auto ret = TupleElementLoadExpression();
    ret->data(std::move(data));
    ret->index(std::move(index));
    return ret;
}

std::unique_ptr<expression::TupleElementStoreExpression> IRFactoryBase::TupleElementStoreExpression() {
    return std::make_unique<expression::TupleElementStoreExpression>();
}

std::unique_ptr<expression::TupleElementStoreExpression> IRFactoryBase::TupleElementStoreExpression(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<name::Index> index,
        std::unique_ptr<expression::Expression> value) {
    auto ret = TupleElementStoreExpression();
    ret->data(std::move(data));
    ret->index(std::move(index));
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<expression::TypeOperator> IRFactoryBase::TypeOperator() {
    return std::make_unique<expression::TypeOperator>();
}

std::unique_ptr<expression::TypeOperator> IRFactoryBase::TypeOperator(
        expression::TypeOperator::Kind operator_kind,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> operand) {
    auto ret = TypeOperator();
    ret->operator_kind(operator_kind);
    ret->type(std::move(type));
    ret->operand(std::move(operand));
    return ret;
}

std::unique_ptr<expression::UnaryOperator> IRFactoryBase::UnaryOperator() {
    return std::make_unique<expression::UnaryOperator>();
}

std::unique_ptr<expression::UnaryOperator> IRFactoryBase::UnaryOperator(
        expression::UnaryOperator::Kind operator_kind,
        std::unique_ptr<expression::Expression> operand) {
    auto ret = UnaryOperator();
    ret->operator_kind(operator_kind);
    ret->operand(std::move(operand));
    return ret;
}

std::unique_ptr<expression::VariableReference> IRFactoryBase::VariableReference() {
    return std::make_unique<expression::VariableReference>();
}

std::unique_ptr<expression::VariableReference> IRFactoryBase::VariableReference(
        std::unique_ptr<name::Name> name) {
    auto ret = VariableReference();
    ret->name(std::move(name));
    return ret;
}

std::unique_ptr<expression::VectorCreationExpression> IRFactoryBase::VectorCreationExpression() {
    return std::make_unique<expression::VectorCreationExpression>();
}

std::unique_ptr<expression::VectorCreationExpression> IRFactoryBase::VectorCreationExpression(
        std::unique_ptr<type::Type> type,
        common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values) {
    auto ret = VectorCreationExpression();
    ret->type(std::move(type));
    ret->values() = std::move(values).build();
    return ret;
}

std::unique_ptr<expression::VectorElementLoadExpression> IRFactoryBase::VectorElementLoadExpression() {
    return std::make_unique<expression::VectorElementLoadExpression>();
}

std::unique_ptr<expression::VectorElementLoadExpression> IRFactoryBase::VectorElementLoadExpression(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<expression::Expression> index) {
    auto ret = VectorElementLoadExpression();
    ret->data(std::move(data));
    ret->index(std::move(index));
    return ret;
}

std::unique_ptr<expression::VectorElementStoreExpression> IRFactoryBase::VectorElementStoreExpression() {
    return std::make_unique<expression::VectorElementStoreExpression>();
}

std::unique_ptr<expression::VectorElementStoreExpression> IRFactoryBase::VectorElementStoreExpression(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<expression::Expression> index,
        std::unique_ptr<expression::Expression> value) {
    auto ret = VectorElementStoreExpression();
    ret->data(std::move(data));
    ret->index(std::move(index));
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<expression::VectorLengthExpression> IRFactoryBase::VectorLengthExpression() {
    return std::make_unique<expression::VectorLengthExpression>();
}

std::unique_ptr<expression::VectorLengthExpression> IRFactoryBase::VectorLengthExpression(
        std::unique_ptr<expression::Expression> data) {
    auto ret = VectorLengthExpression();
    ret->data(std::move(data));
    return ret;
}

std::unique_ptr<expression::relation::AggregationExpression> IRFactoryBase::AggregationExpression() {
    return std::make_unique<expression::relation::AggregationExpression>();
}

std::unique_ptr<expression::relation::AggregationExpression> IRFactoryBase::AggregationExpression(
        std::unique_ptr<expression::Expression> operand,
        common::util::MoveInitializerList<std::unique_ptr<name::Index>> keys,
        common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> initialize,
        common::util::MoveInitializerList<std::unique_ptr<expression::relation::AggregationExpression::Column>> columns,
        std::unique_ptr<name::SimpleName> alias) {
    auto ret = AggregationExpression();
    ret->operand(std::move(operand));
    ret->keys() = std::move(keys).build();
    ret->initialize() = std::move(initialize).build();
    ret->columns() = std::move(columns).build();
    ret->alias(std::move(alias));
    return ret;
}

std::unique_ptr<expression::relation::AggregationExpression::Column> IRFactoryBase::AggregationExpressionColumn() {
    return std::make_unique<expression::relation::AggregationExpression::Column>();
}

std::unique_ptr<expression::relation::AggregationExpression::Column> IRFactoryBase::AggregationExpressionColumn(
        std::unique_ptr<name::Name> function,
        expression::FunctionCall::Quantifier quantifier,
        std::unique_ptr<expression::Expression> operand,
        std::unique_ptr<name::SimpleName> alias) {
    auto ret = AggregationExpressionColumn();
    ret->function(std::move(function));
    ret->quantifier(quantifier);
    ret->operand(std::move(operand));
    ret->alias(std::move(alias));
    return ret;
}

std::unique_ptr<expression::relation::DistinctExpression> IRFactoryBase::DistinctExpression() {
    return std::make_unique<expression::relation::DistinctExpression>();
}

std::unique_ptr<expression::relation::DistinctExpression> IRFactoryBase::DistinctExpression(
        std::unique_ptr<expression::Expression> operand,
        common::util::MoveInitializerList<std::unique_ptr<name::Index>> subsets) {
    auto ret = DistinctExpression();
    ret->operand(std::move(operand));
    ret->subsets() = std::move(subsets).build();
    return ret;
}

std::unique_ptr<expression::relation::JoinExpression> IRFactoryBase::JoinExpression() {
    return std::make_unique<expression::relation::JoinExpression>();
}

std::unique_ptr<expression::relation::JoinExpression> IRFactoryBase::JoinExpression(
        expression::relation::JoinExpression::Kind operator_kind,
        std::unique_ptr<expression::Expression> left,
        std::unique_ptr<expression::Expression> right,
        std::unique_ptr<expression::Expression> condition) {
    auto ret = JoinExpression();
    ret->operator_kind(operator_kind);
    ret->left(std::move(left));
    ret->right(std::move(right));
    ret->condition(std::move(condition));
    return ret;
}

std::unique_ptr<expression::relation::LimitExpression> IRFactoryBase::LimitExpression() {
    return std::make_unique<expression::relation::LimitExpression>();
}

std::unique_ptr<expression::relation::LimitExpression> IRFactoryBase::LimitExpression(
        std::unique_ptr<expression::Expression> operand,
        std::size_t count) {
    auto ret = LimitExpression();
    ret->operand(std::move(operand));
    ret->count(count);
    return ret;
}

std::unique_ptr<expression::relation::OrderExpression> IRFactoryBase::OrderExpression() {
    return std::make_unique<expression::relation::OrderExpression>();
}

std::unique_ptr<expression::relation::OrderExpression> IRFactoryBase::OrderExpression(
        std::unique_ptr<expression::Expression> operand,
        common::util::MoveInitializerList<std::unique_ptr<expression::relation::OrderExpression::Element>> elements) {
    auto ret = OrderExpression();
    ret->operand(std::move(operand));
    ret->elements() = std::move(elements).build();
    return ret;
}

std::unique_ptr<expression::relation::OrderExpression::Element> IRFactoryBase::OrderExpressionElement() {
    return std::make_unique<expression::relation::OrderExpression::Element>();
}

std::unique_ptr<expression::relation::OrderExpression::Element> IRFactoryBase::OrderExpressionElement(
        std::unique_ptr<name::Index> column,
        expression::relation::OrderExpression::Direction direction) {
    auto ret = OrderExpressionElement();
    ret->column(std::move(column));
    ret->direction(direction);
    return ret;
}

std::unique_ptr<expression::relation::ProjectionExpression> IRFactoryBase::ProjectionExpression() {
    return std::make_unique<expression::relation::ProjectionExpression>();
}

std::unique_ptr<expression::relation::ProjectionExpression> IRFactoryBase::ProjectionExpression(
        std::unique_ptr<expression::Expression> operand,
        common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> initialize,
        common::util::MoveInitializerList<std::unique_ptr<expression::relation::ProjectionExpression::Column>> columns,
        std::unique_ptr<name::SimpleName> alias) {
    auto ret = ProjectionExpression();
    ret->operand(std::move(operand));
    ret->initialize() = std::move(initialize).build();
    ret->columns() = std::move(columns).build();
    ret->alias(std::move(alias));
    return ret;
}

std::unique_ptr<expression::relation::ProjectionExpression::Column> IRFactoryBase::ProjectionExpressionColumn() {
    return std::make_unique<expression::relation::ProjectionExpression::Column>();
}

std::unique_ptr<expression::relation::ProjectionExpression::Column> IRFactoryBase::ProjectionExpressionColumn(
        std::unique_ptr<expression::Expression> value,
        std::unique_ptr<name::SimpleName> alias) {
    auto ret = ProjectionExpressionColumn();
    ret->value(std::move(value));
    ret->alias(std::move(alias));
    return ret;
}

std::unique_ptr<expression::relation::ScanExpression> IRFactoryBase::ScanExpression() {
    return std::make_unique<expression::relation::ScanExpression>();
}

std::unique_ptr<expression::relation::ScanExpression> IRFactoryBase::ScanExpression(
        std::unique_ptr<name::Name> table,
        std::unique_ptr<name::SimpleName> alias,
        std::unique_ptr<expression::Expression> condition) {
    auto ret = ScanExpression();
    ret->table(std::move(table));
    ret->alias(std::move(alias));
    ret->condition(std::move(condition));
    return ret;
}

std::unique_ptr<expression::relation::SelectionExpression> IRFactoryBase::SelectionExpression() {
    return std::make_unique<expression::relation::SelectionExpression>();
}

std::unique_ptr<expression::relation::SelectionExpression> IRFactoryBase::SelectionExpression(
        std::unique_ptr<expression::Expression> operand,
        std::unique_ptr<expression::Expression> condition) {
    auto ret = SelectionExpression();
    ret->operand(std::move(operand));
    ret->condition(std::move(condition));
    return ret;
}

std::unique_ptr<name::Index> IRFactoryBase::Index() {
    return std::make_unique<name::Index>();
}

std::unique_ptr<name::Index> IRFactoryBase::Index(
        std::unique_ptr<name::SimpleName> name,
        std::size_t position) {
    auto ret = Index();
    ret->name(std::move(name));
    ret->position(position);
    return ret;
}

std::unique_ptr<name::QualifiedName> IRFactoryBase::QualifiedName() {
    return std::make_unique<name::QualifiedName>();
}

std::unique_ptr<name::QualifiedName> IRFactoryBase::QualifiedName(
        std::unique_ptr<name::Name> qualifier,
        std::unique_ptr<name::SimpleName> name) {
    auto ret = QualifiedName();
    ret->qualifier(std::move(qualifier));
    ret->name(std::move(name));
    return ret;
}

std::unique_ptr<name::SimpleName> IRFactoryBase::SimpleName() {
    return std::make_unique<name::SimpleName>();
}

std::unique_ptr<name::SimpleName> IRFactoryBase::SimpleName(
        std::string token) {
    auto ret = SimpleName();
    ret->token(std::move(token));
    return ret;
}

std::unique_ptr<program::GlobalFunctionDeclaration> IRFactoryBase::GlobalFunctionDeclaration() {
    return std::make_unique<program::GlobalFunctionDeclaration>();
}

std::unique_ptr<program::GlobalFunctionDeclaration> IRFactoryBase::GlobalFunctionDeclaration(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<type::Type> type,
        common::util::MoveInitializerList<std::unique_ptr<program::GlobalFunctionDeclaration::Parameter>> parameters,
        std::unique_ptr<statement::Statement> body,
        std::initializer_list<program::GlobalFunctionDeclaration::Attribute> attributes) {
    auto ret = GlobalFunctionDeclaration();
    ret->name(std::move(name));
    ret->type(std::move(type));
    ret->parameters() = std::move(parameters).build();
    ret->body(std::move(body));
    ret->attributes() = attributes;
    return ret;
}

std::unique_ptr<program::GlobalFunctionDeclaration::Parameter> IRFactoryBase::GlobalFunctionDeclarationParameter() {
    return std::make_unique<program::GlobalFunctionDeclaration::Parameter>();
}

std::unique_ptr<program::GlobalFunctionDeclaration::Parameter> IRFactoryBase::GlobalFunctionDeclarationParameter(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<type::Type> type,
        std::initializer_list<program::GlobalFunctionDeclaration::Parameter::Attribute> attributes) {
    auto ret = GlobalFunctionDeclarationParameter();
    ret->name(std::move(name));
    ret->type(std::move(type));
    ret->attributes() = attributes;
    return ret;
}

std::unique_ptr<program::GlobalVariableDeclaration> IRFactoryBase::GlobalVariableDeclaration() {
    return std::make_unique<program::GlobalVariableDeclaration>();
}

std::unique_ptr<program::GlobalVariableDeclaration> IRFactoryBase::GlobalVariableDeclaration(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> value,
        std::initializer_list<program::GlobalVariableDeclaration::Attribute> attributes) {
    auto ret = GlobalVariableDeclaration();
    ret->name(std::move(name));
    ret->type(std::move(type));
    ret->value(std::move(value));
    ret->attributes() = attributes;
    return ret;
}

std::unique_ptr<program::Program> IRFactoryBase::Program() {
    return std::make_unique<program::Program>();
}

std::unique_ptr<program::Program> IRFactoryBase::Program(
        common::util::MoveInitializerList<std::unique_ptr<program::GlobalDeclaration>> declarations,
        std::unique_ptr<statement::Statement> main) {
    auto ret = Program();
    ret->declarations() = std::move(declarations).build();
    ret->main(std::move(main));
    return ret;
}

std::unique_ptr<statement::AnchorDeclaration> IRFactoryBase::AnchorDeclaration() {
    return std::make_unique<statement::AnchorDeclaration>();
}

std::unique_ptr<statement::AnchorDeclaration> IRFactoryBase::AnchorDeclaration(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<statement::Statement> body) {
    auto ret = AnchorDeclaration();
    ret->name(std::move(name));
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<statement::BlockStatement> IRFactoryBase::BlockStatement() {
    return std::make_unique<statement::BlockStatement>();
}

std::unique_ptr<statement::BlockStatement> IRFactoryBase::BlockStatement(
        common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> elements) {
    auto ret = BlockStatement();
    ret->elements() = std::move(elements).build();
    return ret;
}

std::unique_ptr<statement::BranchStatement> IRFactoryBase::BranchStatement() {
    return std::make_unique<statement::BranchStatement>();
}

std::unique_ptr<statement::BranchStatement> IRFactoryBase::BranchStatement(
        std::unique_ptr<expression::Expression> condition,
        std::unique_ptr<statement::Statement> then_statement,
        std::unique_ptr<statement::Statement> else_statement) {
    auto ret = BranchStatement();
    ret->condition(std::move(condition));
    ret->then_statement(std::move(then_statement));
    ret->else_statement(std::move(else_statement));
    return ret;
}

std::unique_ptr<statement::BreakStatement> IRFactoryBase::BreakStatement() {
    return std::make_unique<statement::BreakStatement>();
}

std::unique_ptr<statement::BreakStatement> IRFactoryBase::BreakStatement(
        std::unique_ptr<name::Name> anchor) {
    auto ret = BreakStatement();
    ret->anchor(std::move(anchor));
    return ret;
}

std::unique_ptr<statement::ContinueStatement> IRFactoryBase::ContinueStatement() {
    return std::make_unique<statement::ContinueStatement>();
}

std::unique_ptr<statement::ContinueStatement> IRFactoryBase::ContinueStatement(
        std::unique_ptr<name::Name> anchor) {
    auto ret = ContinueStatement();
    ret->anchor(std::move(anchor));
    return ret;
}

std::unique_ptr<statement::EmptyStatement> IRFactoryBase::EmptyStatement() {
    return std::make_unique<statement::EmptyStatement>();
}

std::unique_ptr<statement::ExpressionStatement> IRFactoryBase::ExpressionStatement() {
    return std::make_unique<statement::ExpressionStatement>();
}

std::unique_ptr<statement::ExpressionStatement> IRFactoryBase::ExpressionStatement(
        std::unique_ptr<expression::Expression> body) {
    auto ret = ExpressionStatement();
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<statement::ForEachStatement> IRFactoryBase::ForEachStatement() {
    return std::make_unique<statement::ForEachStatement>();
}

std::unique_ptr<statement::ForEachStatement> IRFactoryBase::ForEachStatement(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> enumeration,
        std::unique_ptr<statement::Statement> body) {
    auto ret = ForEachStatement();
    ret->name(std::move(name));
    ret->type(std::move(type));
    ret->enumeration(std::move(enumeration));
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<statement::ForStatement> IRFactoryBase::ForStatement() {
    return std::make_unique<statement::ForStatement>();
}

std::unique_ptr<statement::ForStatement> IRFactoryBase::ForStatement(
        common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> initialize,
        std::unique_ptr<expression::Expression> condition,
        common::util::MoveInitializerList<std::unique_ptr<statement::Statement>> update,
        std::unique_ptr<statement::Statement> body) {
    auto ret = ForStatement();
    ret->initialize() = std::move(initialize).build();
    ret->condition(std::move(condition));
    ret->update() = std::move(update).build();
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<statement::LocalVariableDeclaration> IRFactoryBase::LocalVariableDeclaration() {
    return std::make_unique<statement::LocalVariableDeclaration>();
}

std::unique_ptr<statement::LocalVariableDeclaration> IRFactoryBase::LocalVariableDeclaration(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> value,
        std::initializer_list<statement::LocalVariableDeclaration::Attribute> attributes) {
    auto ret = LocalVariableDeclaration();
    ret->name(std::move(name));
    ret->type(std::move(type));
    ret->value(std::move(value));
    ret->attributes() = attributes;
    return ret;
}

std::unique_ptr<statement::LogStatement> IRFactoryBase::LogStatement() {
    return std::make_unique<statement::LogStatement>();
}

std::unique_ptr<statement::LogStatement> IRFactoryBase::LogStatement(
        statement::LogStatement::Level level,
        std::unique_ptr<expression::Expression> value) {
    auto ret = LogStatement();
    ret->level(level);
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<statement::RaiseStatement> IRFactoryBase::RaiseStatement() {
    return std::make_unique<statement::RaiseStatement>();
}

std::unique_ptr<statement::RaiseStatement> IRFactoryBase::RaiseStatement(
        std::unique_ptr<expression::Expression> value) {
    auto ret = RaiseStatement();
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<statement::ReturnStatement> IRFactoryBase::ReturnStatement() {
    return std::make_unique<statement::ReturnStatement>();
}

std::unique_ptr<statement::ReturnStatement> IRFactoryBase::ReturnStatement(
        std::unique_ptr<expression::Expression> value) {
    auto ret = ReturnStatement();
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<statement::VectorElementDeleteStatement> IRFactoryBase::VectorElementDeleteStatement() {
    return std::make_unique<statement::VectorElementDeleteStatement>();
}

std::unique_ptr<statement::VectorElementDeleteStatement> IRFactoryBase::VectorElementDeleteStatement(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<expression::Expression> begin,
        std::unique_ptr<expression::Expression> end) {
    auto ret = VectorElementDeleteStatement();
    ret->data(std::move(data));
    ret->begin(std::move(begin));
    ret->end(std::move(end));
    return ret;
}

std::unique_ptr<statement::VectorElementInsertStatement> IRFactoryBase::VectorElementInsertStatement() {
    return std::make_unique<statement::VectorElementInsertStatement>();
}

std::unique_ptr<statement::VectorElementInsertStatement> IRFactoryBase::VectorElementInsertStatement(
        std::unique_ptr<expression::Expression> data,
        std::unique_ptr<expression::Expression> index,
        common::util::MoveInitializerList<std::unique_ptr<expression::Expression>> values) {
    auto ret = VectorElementInsertStatement();
    ret->data(std::move(data));
    ret->index(std::move(index));
    ret->values() = std::move(values).build();
    return ret;
}

std::unique_ptr<statement::WhileStatement> IRFactoryBase::WhileStatement() {
    return std::make_unique<statement::WhileStatement>();
}

std::unique_ptr<statement::WhileStatement> IRFactoryBase::WhileStatement(
        std::unique_ptr<expression::Expression> condition,
        std::unique_ptr<statement::Statement> body) {
    auto ret = WhileStatement();
    ret->condition(std::move(condition));
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<statement::ddl::CreateTableStatement> IRFactoryBase::CreateTableStatement() {
    return std::make_unique<statement::ddl::CreateTableStatement>();
}

std::unique_ptr<statement::ddl::CreateTableStatement> IRFactoryBase::CreateTableStatement(
        std::unique_ptr<name::Name> table,
        common::util::MoveInitializerList<std::unique_ptr<statement::ddl::CreateTableStatement::Column>> columns,
        std::initializer_list<statement::ddl::CreateTableStatement::Attribute> attributes,
        common::util::MoveInitializerList<std::unique_ptr<statement::ddl::CreateTableStatement::PrimaryKey>> primary_keys) {
    auto ret = CreateTableStatement();
    ret->table(std::move(table));
    ret->columns() = std::move(columns).build();
    ret->attributes() = attributes;
    ret->primary_keys() = std::move(primary_keys).build();
    return ret;
}

std::unique_ptr<statement::ddl::CreateTableStatement::Column> IRFactoryBase::CreateTableStatementColumn() {
    return std::make_unique<statement::ddl::CreateTableStatement::Column>();
}

std::unique_ptr<statement::ddl::CreateTableStatement::Column> IRFactoryBase::CreateTableStatementColumn(
        std::unique_ptr<name::SimpleName> name,
        std::unique_ptr<type::Type> type,
        std::unique_ptr<expression::Expression> value,
        std::initializer_list<statement::ddl::CreateTableStatement::Column::Attribute> attributes) {
    auto ret = CreateTableStatementColumn();
    ret->name(std::move(name));
    ret->type(std::move(type));
    ret->value(std::move(value));
    ret->attributes() = attributes;
    return ret;
}

std::unique_ptr<statement::ddl::CreateTableStatement::PrimaryKey> IRFactoryBase::CreateTableStatementPrimaryKey() {
    return std::make_unique<statement::ddl::CreateTableStatement::PrimaryKey>();
}

std::unique_ptr<statement::ddl::CreateTableStatement::PrimaryKey> IRFactoryBase::CreateTableStatementPrimaryKey(
        std::unique_ptr<name::SimpleName> name,
        statement::ddl::CreateTableStatement::PrimaryKey::Direction direction) {
    auto ret = CreateTableStatementPrimaryKey();
    ret->name(std::move(name));
    ret->direction(direction);
    return ret;
}

std::unique_ptr<statement::ddl::DropTableStatement> IRFactoryBase::DropTableStatement() {
    return std::make_unique<statement::ddl::DropTableStatement>();
}

std::unique_ptr<statement::ddl::DropTableStatement> IRFactoryBase::DropTableStatement(
        std::unique_ptr<name::Name> table,
        std::initializer_list<statement::ddl::DropTableStatement::Attribute> attributes) {
    auto ret = DropTableStatement();
    ret->table(std::move(table));
    ret->attributes() = attributes;
    return ret;
}

std::unique_ptr<statement::dml::DeleteStatement> IRFactoryBase::DeleteStatement() {
    return std::make_unique<statement::dml::DeleteStatement>();
}

std::unique_ptr<statement::dml::DeleteStatement> IRFactoryBase::DeleteStatement(
        std::unique_ptr<name::Name> table,
        std::unique_ptr<expression::Expression> condition) {
    auto ret = DeleteStatement();
    ret->table(std::move(table));
    ret->condition(std::move(condition));
    return ret;
}

std::unique_ptr<statement::dml::EmitStatement> IRFactoryBase::EmitStatement() {
    return std::make_unique<statement::dml::EmitStatement>();
}

std::unique_ptr<statement::dml::EmitStatement> IRFactoryBase::EmitStatement(
        std::unique_ptr<expression::Expression> source) {
    auto ret = EmitStatement();
    ret->source(std::move(source));
    return ret;
}

std::unique_ptr<statement::dml::InsertRelationStatement> IRFactoryBase::InsertRelationStatement() {
    return std::make_unique<statement::dml::InsertRelationStatement>();
}

std::unique_ptr<statement::dml::InsertRelationStatement> IRFactoryBase::InsertRelationStatement(
        std::unique_ptr<name::Name> table,
        common::util::MoveInitializerList<std::unique_ptr<name::SimpleName>> columns,
        std::unique_ptr<expression::Expression> source) {
    auto ret = InsertRelationStatement();
    ret->table(std::move(table));
    ret->columns() = std::move(columns).build();
    ret->source(std::move(source));
    return ret;
}

std::unique_ptr<statement::dml::InsertValuesStatement> IRFactoryBase::InsertValuesStatement() {
    return std::make_unique<statement::dml::InsertValuesStatement>();
}

std::unique_ptr<statement::dml::InsertValuesStatement> IRFactoryBase::InsertValuesStatement(
        std::unique_ptr<name::Name> table,
        common::util::MoveInitializerList<std::unique_ptr<statement::dml::InsertValuesStatement::Column>> columns) {
    auto ret = InsertValuesStatement();
    ret->table(std::move(table));
    ret->columns() = std::move(columns).build();
    return ret;
}

std::unique_ptr<statement::dml::InsertValuesStatement::Column> IRFactoryBase::InsertValuesStatementColumn() {
    return std::make_unique<statement::dml::InsertValuesStatement::Column>();
}

std::unique_ptr<statement::dml::InsertValuesStatement::Column> IRFactoryBase::InsertValuesStatementColumn(
        std::unique_ptr<name::SimpleName> name,
        std::unique_ptr<expression::Expression> value) {
    auto ret = InsertValuesStatementColumn();
    ret->name(std::move(name));
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<statement::dml::UpdateStatement> IRFactoryBase::UpdateStatement() {
    return std::make_unique<statement::dml::UpdateStatement>();
}

std::unique_ptr<statement::dml::UpdateStatement> IRFactoryBase::UpdateStatement(
        std::unique_ptr<name::Name> table,
        common::util::MoveInitializerList<std::unique_ptr<statement::dml::UpdateStatement::Column>> columns,
        std::unique_ptr<expression::Expression> condition) {
    auto ret = UpdateStatement();
    ret->table(std::move(table));
    ret->columns() = std::move(columns).build();
    ret->condition(std::move(condition));
    return ret;
}

std::unique_ptr<statement::dml::UpdateStatement::Column> IRFactoryBase::UpdateStatementColumn() {
    return std::make_unique<statement::dml::UpdateStatement::Column>();
}

std::unique_ptr<statement::dml::UpdateStatement::Column> IRFactoryBase::UpdateStatementColumn(
        std::unique_ptr<name::Name> name,
        std::unique_ptr<expression::Expression> value) {
    auto ret = UpdateStatementColumn();
    ret->name(std::move(name));
    ret->value(std::move(value));
    return ret;
}

std::unique_ptr<statement::transaction::TransactionBlockStatement> IRFactoryBase::TransactionBlockStatement() {
    return std::make_unique<statement::transaction::TransactionBlockStatement>();
}

std::unique_ptr<statement::transaction::TransactionBlockStatement> IRFactoryBase::TransactionBlockStatement(
        std::unique_ptr<statement::Statement> body) {
    auto ret = TransactionBlockStatement();
    ret->body(std::move(body));
    return ret;
}

std::unique_ptr<type::ArrayType> IRFactoryBase::ArrayType() {
    return std::make_unique<type::ArrayType>();
}

std::unique_ptr<type::ArrayType> IRFactoryBase::ArrayType(
        std::unique_ptr<type::Type> element_type,
        std::size_t size) {
    auto ret = ArrayType();
    ret->element_type(std::move(element_type));
    ret->size(size);
    return ret;
}

std::unique_ptr<type::BooleanType> IRFactoryBase::BooleanType() {
    return std::make_unique<type::BooleanType>();
}

std::unique_ptr<type::CharType> IRFactoryBase::CharType() {
    return std::make_unique<type::CharType>();
}

std::unique_ptr<type::CharType> IRFactoryBase::CharType(
        std::size_t size) {
    auto ret = CharType();
    ret->size(size);
    return ret;
}

std::unique_ptr<type::Float32Type> IRFactoryBase::Float32Type() {
    return std::make_unique<type::Float32Type>();
}

std::unique_ptr<type::Float64Type> IRFactoryBase::Float64Type() {
    return std::make_unique<type::Float64Type>();
}

std::unique_ptr<type::Int32Type> IRFactoryBase::Int32Type() {
    return std::make_unique<type::Int32Type>();
}

std::unique_ptr<type::Int64Type> IRFactoryBase::Int64Type() {
    return std::make_unique<type::Int64Type>();
}

std::unique_ptr<type::NullType> IRFactoryBase::NullType() {
    return std::make_unique<type::NullType>();
}

std::unique_ptr<type::RelationType> IRFactoryBase::RelationType() {
    return std::make_unique<type::RelationType>();
}

std::unique_ptr<type::RelationType> IRFactoryBase::RelationType(
        std::unique_ptr<type::TupleType> element_type) {
    auto ret = RelationType();
    ret->element_type(std::move(element_type));
    return ret;
}

std::unique_ptr<type::StringType> IRFactoryBase::StringType() {
    return std::make_unique<type::StringType>();
}

std::unique_ptr<type::TupleType> IRFactoryBase::TupleType() {
    return std::make_unique<type::TupleType>();
}

std::unique_ptr<type::TupleType> IRFactoryBase::TupleType(
        common::util::MoveInitializerList<std::unique_ptr<type::TupleType::Element>> elements) {
    auto ret = TupleType();
    ret->elements() = std::move(elements).build();
    return ret;
}

std::unique_ptr<type::TupleType::Element> IRFactoryBase::TupleTypeElement() {
    return std::make_unique<type::TupleType::Element>();
}

std::unique_ptr<type::TupleType::Element> IRFactoryBase::TupleTypeElement(
        std::unique_ptr<name::SimpleName> name,
        std::unique_ptr<type::Type> type) {
    auto ret = TupleTypeElement();
    ret->name(std::move(name));
    ret->type(std::move(type));
    return ret;
}

std::unique_ptr<type::VarCharType> IRFactoryBase::VarCharType() {
    return std::make_unique<type::VarCharType>();
}

std::unique_ptr<type::VarCharType> IRFactoryBase::VarCharType(
        std::size_t size) {
    auto ret = VarCharType();
    ret->size(size);
    return ret;
}

std::unique_ptr<type::VectorType> IRFactoryBase::VectorType() {
    return std::make_unique<type::VectorType>();
}

std::unique_ptr<type::VectorType> IRFactoryBase::VectorType(
        std::unique_ptr<type::Type> element_type) {
    auto ret = VectorType();
    ret->element_type(std::move(element_type));
    return ret;
}

}  // namespace shakujo::model

