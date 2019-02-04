/*
 * Copyright 2018-2018 shakujo project.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http:// www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SHAKUJO_PARSER_IMPL_ENGINE_H_
#define SHAKUJO_PARSER_IMPL_ENGINE_H_

#include <cstring>
#include <string>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "antlr4-runtime.h"
#include "ShakujoParser.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::parser::impl {

using Grammar = shakujo_lang::ShakujoParser;

class Engine {
private:
    std::shared_ptr<std::string> location_;
    model::IRFactory f;

public:
    explicit Engine(std::string location)
        : location_(std::make_shared<std::string>(std::move(location)))
    {}

    // programEntry
    //     : dmlStatement EOF
    //     | ddlStatement EOF
    //     ;
    std::unique_ptr<model::program::Program> visit(Grammar::ProgramEntryContext *);

    // statementEntry
    //     : statement EOF
    //     ;
    std::unique_ptr<model::statement::Statement> visit(Grammar::StatementEntryContext *);

    // expressionEntry
    //     : expression EOF
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::ExpressionEntryContext *);

    // dmlStatement
    //     : selectStatement
    //     | insertStatement
    //     ;
    std::unique_ptr<model::statement::Statement> visit(Grammar::DmlStatementContext *);

    // ddlStatement
    //     : createTableStatement
    //     ;
    std::unique_ptr<model::statement::Statement> visit(Grammar::DdlStatementContext *);

    // -- INSERT
    // insertStatement
    //     : K_INSERT K_INTO insertionTarget insertColumnsAndSources
    //     ;
    std::unique_ptr<model::statement::Statement> visit(Grammar::InsertStatementContext *);

    // insertionTarget
    //     : name
    //     ;
    std::unique_ptr<model::name::Name> visit(Grammar::InsertionTargetContext *);

    // insertColumnsAndSources
    //     : ( '(' insertColumnList ')' )? insertValuesConstructor
    //     ;
    std::unique_ptr<model::statement::Statement> visit(
            Grammar::InsertColumnsAndSourcesContext *,
            std::unique_ptr<model::name::Name> target);

    // insertColumnList
    //     : simpleName (',' simpleName)*
    //     ;
    std::vector<std::unique_ptr<model::name::SimpleName>> visit(Grammar::InsertColumnListContext *);

    // insertValuesConstructor
    //     : K_VALUES '(' insertValuesExpressionList ')'
    //     ;
    std::vector<std::unique_ptr<model::expression::Expression>> visit(Grammar::InsertValuesConstructorContext *);

    // insertValuesExpressionList
    //     : insertValuesExpression (',' insertValuesExpression)*
    //     ;
    std::vector<std::unique_ptr<model::expression::Expression>> visit(Grammar::InsertValuesExpressionListContext *);

    // insertValuesExpression
    //     : expression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::InsertValuesExpressionContext *);

    // -- SELECT
    // selectStatement
    //     : query
    //     ;
    std::unique_ptr<model::statement::Statement> visit(Grammar::SelectStatementContext *);

    // query
    //     : K_SELECT projectionSpec fromClause whereClause?
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::QueryContext *);

    // projectionSpec
    //     : any='*'
    //     | projectionColumnList
    //     ;
    std::unique_ptr<model::expression::Expression> visit(
            Grammar::ProjectionSpecContext *,
            std::unique_ptr<model::expression::Expression>);

    // projectionColumnList
    //     : projectionColumn (',' projectionColumn)*
    //     ;
    std::unique_ptr<model::expression::relation::ProjectionExpression> visit(
            Grammar::ProjectionColumnListContext *,
            std::unique_ptr<model::expression::Expression>);

    // projectionColumn
    //     : expression (K_AS simpleName)?
    //     ;
    std::unique_ptr<model::expression::relation::ProjectionExpression::Column> visit(Grammar::ProjectionColumnContext *);

    // fromClause
    //     : K_FROM tableReference (',' tableReference)*
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::FromClauseContext *);

    // tableReference
    //     : tablePrimary
    //     | tableReference joinedTableRest
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::TableReferenceContext *);

    // tablePrimary
    //     : tableName
    //     | '(' joinedTable ')'
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::TablePrimaryContext *);

    // tableName
    //     : name (K_AS simpleName)? // FIXME: correlation name wit derived columns
    //     ;
    std::unique_ptr<model::expression::relation::ScanExpression> visit(Grammar::TableNameContext *);

    // joinedTable
    //     : tableReference joinedTableRest
    //     ;
    std::unique_ptr<model::expression::relation::JoinExpression> visit(Grammar::JoinedTableContext *);

    // joinedTableRest
    //     : joinType? K_JOIN tablePrimary joinSpecification?
    //     | K_CROSS K_JOIN tablePrimary
    //     | K_NATURAL joinType? K_JOIN tablePrimary
    //     | K_UNION K_JOIN  tablePrimary
    //     ;
    std::unique_ptr<model::expression::relation::JoinExpression>
    visit(Grammar::TableReferenceContext *, Grammar::JoinedTableRestContext *);

    // joinType
    //     : K_INNER
    //     | K_LEFT (K_OUTER)?
    //     | K_RIGHT (K_OUTER)?
    //     | K_FULL (K_OUTER)?
    //     ;
    model::expression::relation::JoinExpression::Kind visit(Grammar::JoinTypeContext *);

    // joinSpecification
    //     : K_ON searchCondition
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::JoinSpecificationContext *);

    // whereClause
    //     : K_WHERE searchCondition
    //     ;
    std::unique_ptr<model::expression::relation::SelectionExpression> visit(
            Grammar::WhereClauseContext *,
            std::unique_ptr<model::expression::Expression>);

    // searchCondition
    //     : booleanValueExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::SearchConditionContext *);

    // booleanValueExpression
    //     : booleanValueExpression op=K_OR booleanTerm
    //     | booleanTerm
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::BooleanValueExpressionContext *);

    // booleanTerm
    //     : booleanTerm op=K_AND booleanFactor
    //     | booleanFactor
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::BooleanTermContext *);

    // booleanFactor
    //     : op=K_NOT booleanTest
    //     | booleanTest
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::BooleanFactorContext *);

    // booleanTest
    //     : booleanPrimary K_IS (K_TRUE|K_FALSE)
    //     | booleanPrimary K_IS K_NOT (K_TRUE|K_FALSE)
    //     | booleanPrimary
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::BooleanTestContext *);

    // booleanPrimary
    //     : parenthesizedBooleanValueExpression
    //     | expression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::BooleanPrimaryContext *);

    // parenthesizedBooleanValueExpression
    //     : '(' booleanValueExpression ')'
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::ParenthesizedBooleanValueExpressionContext *);

    // -- CREATE TABLE
    // createTableStatement
    //     : tableDefinition
    //     ;
    std::unique_ptr<model::statement::Statement> visit(Grammar::CreateTableStatementContext *);

    // tableDefinition
    //     : K_CREATE K_TABLE ( tableDefinitionOption )* name tableElementList
    //     ;
    std::unique_ptr<model::statement::ddl::CreateTableStatement> visit(Grammar::TableDefinitionContext *);

    // tableDefinitionOption
    //     : K_IF K_NOT K_EXISTS
    //     ;
    void visit(Grammar::TableDefinitionOptionContext *, model::statement::ddl::CreateTableStatement *);

    // tableElementList
    //     : '(' tableElement ( ',' tableElement )* ')'
    //     ;
    void visit(Grammar::TableElementListContext *, model::statement::ddl::CreateTableStatement *);

    // tableElement
    //     : columnDefinition
    //     | tableConstraintDefinition
    //     ;
    void visit(Grammar::TableElementContext *, model::statement::ddl::CreateTableStatement *);

    // columnDefinition
    //     : columnName dataType ( defaultClause )? ( columnConstraintDefinition )*
    //     ;
    void visit(Grammar::ColumnDefinitionContext *, model::statement::ddl::CreateTableStatement *);

    // columnName
    //     : simpleName
    //     ;
    std::unique_ptr<model::name::SimpleName> visit(Grammar::ColumnNameContext *);

    // defaultClause
    //     : K_DEFAULT literal
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::DefaultClauseContext *);

    // columnConstraintDefinition
    //     : K_NOT K_NULL
    //     | K_NULL
    //     | K_PRIMARY K_KEY
    //     ;
    void visit(Grammar::ColumnConstraintDefinitionContext *, model::statement::ddl::CreateTableStatement::Column *);

    // tableConstraintDefinition
    //     : primaryKeyDefinition
    //     ;
    void visit(Grammar::TableConstraintDefinitionContext *, model::statement::ddl::CreateTableStatement *);

    // primaryKeyDefinition
    //     : K_PRIMARY K_KEY '(' columnOrderList ')'
    //     ;
    void visit(Grammar::PrimaryKeyDefinitionContext *, model::statement::ddl::CreateTableStatement *);

    // columnOrderList
    //     : columnOrder ( ',' columnOrder )*
    //     ;
    std::vector<std::unique_ptr<model::statement::ddl::CreateTableStatement::PrimaryKey>> visit(
            Grammar::ColumnOrderListContext *);

    // columnOrder
    //     : columnName K_ASC
    //     | columnName K_DESC
    //     | columnName
    //     ;
    std::unique_ptr<model::statement::ddl::CreateTableStatement::PrimaryKey> visit(Grammar::ColumnOrderContext *);


    // -- generic statements
    // statement
    //     : expressionStatement
    //     ;
    std::unique_ptr<model::statement::Statement> visit(Grammar::StatementContext *);

    // expressionStatement
    //     : expression ';'
    //     ;
    std::unique_ptr<model::statement::ExpressionStatement> visit(Grammar::ExpressionStatementContext *);

    // expressionList
    //     : expression (',' expression)* ','?
    //     ;
    std::vector<std::unique_ptr<model::expression::Expression>> visit(Grammar::ExpressionListContext *);

    // expression
    //     : assignExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::ExpressionContext *);

    // assignExpression
    //     : name op=':=' assignExpression {false}?
    //     | signConditionalOrExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::AssignExpressionContext *);

    // signConditionalOrExpression
    //     : signConditionalOrExpression op='||' signConditionalAndExpression {false}?
    //     | signConditionalAndExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::SignConditionalOrExpressionContext *);

    // signConditionalAndExpression
    //     : signConditionalAndExpression op='&&' inclusiveOrExpression {false}?
    //     | inclusiveOrExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::SignConditionalAndExpressionContext *);

    // inclusiveOrExpression
    //     : inclusiveOrExpression '|' exclusiveOrExpression
    //     | exclusiveOrExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::InclusiveOrExpressionContext *);

    // exclusiveOrExpression
    //     : exclusiveOrExpression '^' andExpression
    //     | andExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::ExclusiveOrExpressionContext *);

    // andExpression
    //     : andExpression '&' equalityExpression
    //     | equalityExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::AndExpressionContext *);

    // equalityExpression
    //     : equalityExpression op='=' relationalExpression
    //     | equalityExpression op='<>' relationalExpression
    //     | relationalExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::EqualityExpressionContext *);

    // relationalExpression
    //     : relationalExpression '<' shiftExpression
    //     | relationalExpression '>' shiftExpression
    //     | relationalExpression '<=' shiftExpression
    //     | relationalExpression '>=' shiftExpression
    //     | shiftExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::RelationalExpressionContext *);

    // shiftExpression
    //     : shiftExpression '<<' additiveExpression
    //     | shiftExpression '>>' additiveExpression
    //     | shiftExpression '>>>' additiveExpression
    //     | additiveExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::ShiftExpressionContext *);

    // additiveExpression
    //     : additiveExpression '+' multiplicativeExpression
    //     | additiveExpression '-' multiplicativeExpression
    //     | multiplicativeExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::AdditiveExpressionContext *);

    // multiplicativeExpression
    //     : multiplicativeExpression '*' unaryExpression
    //     | multiplicativeExpression '/' unaryExpression
    //     | multiplicativeExpression '%' unaryExpression
    //     | unaryExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::MultiplicativeExpressionContext *);

    // unaryExpression
    //     : '+' unaryExpression
    //     | '-' unaryExpression
    //     | '~' unaryExpression
    //     | '!' unaryExpression {false}?
    //     | postfixExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::UnaryExpressionContext *);

    // postfixExpression
    //     : primaryExpression
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::PostfixExpressionContext *);

    // primaryExpression
    //     : functionCall
    //     | name
    //     | literal
    //     | placeholder
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::PrimaryExpressionContext *);

    // functionCall
    //     : name '(' expressionList? ')'
    //     | name '(' '*' ')'
    //     | name '(' K_ALL expression ')'
    //     | name '(' K_DISTINCT expression ')'
    //     ;
    std::unique_ptr<model::expression::Expression> visit(Grammar::FunctionCallContext *);

    // name
    //     : name '.' simpleName
    //     | simpleName
    //     ;
    std::unique_ptr<model::name::Name> visit(Grammar::NameContext *);

    // simpleName
    //     : IDENTIFIER
    //     ;
    std::unique_ptr<model::name::SimpleName> visit(Grammar::SimpleNameContext *);

    // literal
    //     : INTEGRAL_NUMBER
    //     | FLOATING_POINT_NUMBER
    //     | K_TRUE
    //     | K_FALSE
    //     | K_NULL
    //     | STRING
    //     ;
    std::unique_ptr<model::expression::Literal> visit(Grammar::LiteralContext *);

    // placeholder
    //     : NAMED_PLACEHOLDER
    //     | QUESTION
    //     ;
    std::unique_ptr<model::expression::Placeholder> visit(Grammar::PlaceholderContext *);

    // dataType
    //     : K_INT ( dataSize )?
    //     | K_BIGINT
    //     | K_FLOAT ( dataSize )?
    //     | K_DOUBLE ( K_PRECISION )?
    //     | K_CHAR dataSize
    //     | K_STRING
    //     ;
    std::unique_ptr<model::type::Type> visit(Grammar::DataTypeContext *);

    // dataSize
    //     : '(' INTEGRAL_NUMBER ')'
    //     ;
    std::size_t visit(Grammar::DataSizeContext *);

private:
    common::core::DocumentRegion region(antlr4::ParserRuleContext *, antlr4::ParserRuleContext * = nullptr);
};

}  //  namespace shakujo::parser::impl

#endif  //  SHAKUJO_PARSER_IMPL_ENGINE_H_
