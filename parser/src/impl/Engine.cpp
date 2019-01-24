/*
 * Copyright 2018-2018 shakujo project.
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
#include "Engine.h"

#include <algorithm>
#include <cassert>

#include "shakujo/common/util/utility.h"

namespace shakujo::parser::impl {

using common::util::is_defined;

inline static common::core::DocumentRegion::Position position(antlr4::Token* token, bool head) {
    if (is_defined(token) && head) {
        return common::core::DocumentRegion::Position {
                token->getLine(),
                token->getCharPositionInLine() + 1,
        };
    }
    // FIXME handle stop position in ANTLR
    return {};
}

common::core::DocumentRegion Engine::region(antlr4::ParserRuleContext *first, antlr4::ParserRuleContext *last) {
    assert(is_defined(first));  // NOLINT
    auto begin = position(first->getStart(), true);
    auto end = position((is_defined(last) ? last : first)->getStop(), false);
    return common::core::DocumentRegion(location_, begin, end);
}

[[noreturn]] static void rule_error(antlr4::ParserRuleContext *) {
    // FIXME: more info
    throw std::domain_error("unknown rule");
}

std::unique_ptr<model::program::Program> Engine::visit(Grammar::ProgramEntryContext *c) {
    if (auto s = c->dmlStatement(); is_defined(s)) {
        auto main = visit(s);
        return f.Program({}, std::move(main)) << region(c);
    }
    if (auto s = c->ddlStatement(); is_defined(s)) {
        auto main = visit(s);
        return f.Program({}, std::move(main)) << region(c);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(Grammar::StatementEntryContext *c) {
    if (auto s = c->statement(); is_defined(s)) {
        return visit(s);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::ExpressionEntryContext *c) {
    if (auto e = c->expression(); is_defined(e)) {
        return visit(e);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(Grammar::DmlStatementContext *c) {
    if (auto s = c->selectStatement(); is_defined(s)) {
        return visit(s);
    }
    if (auto s = c->insertStatement(); is_defined(s)) {
        return visit(s);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(Grammar::DdlStatementContext *c) {
    if (auto s = c->createTableStatement(); is_defined(s)) {
        return visit(s);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(Grammar::InsertStatementContext *c) {
    auto t = c->insertionTarget();
    auto s = c->insertColumnsAndSources();
    if (is_defined(t) && is_defined(s)) {
        auto target = visit(t);
        return visit(s, std::move(target));
    }
    rule_error(c);
}

std::unique_ptr<model::name::Name> Engine::visit(Grammar::InsertionTargetContext *c) {
    if (auto n = c->name(); is_defined(n)) {
        return visit(n);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(
        Grammar::InsertColumnsAndSourcesContext *c,
        std::unique_ptr<model::name::Name> target) {
    if (auto ctor = c->insertValuesConstructor(); is_defined(ctor)) {
        // INSERT INTO ... VALUES ...
        std::vector<std::unique_ptr<model::name::SimpleName>> column_names;
        if (auto cs = c->insertColumnList(); is_defined(cs)) {
            column_names = visit(cs);
        }
        auto values = visit(ctor);

        std::vector<std::unique_ptr<model::statement::dml::InsertValuesStatement::Column>> columns;
        columns.reserve(std::max(column_names.size(), values.size()));
        for (std::size_t i = 0, n = std::max(column_names.size(), values.size()); i < n; i++) {
            columns.emplace_back(f.InsertValuesStatementColumn());
        }
        for (std::size_t i = 0, n = column_names.size(); i < n; i++) {
            columns[i]->name(std::move(column_names[i]));
        }
        for (std::size_t i = 0, n = values.size(); i < n; i++) {
            columns[i]->value(std::move(values[i]));
        }
        return f.InsertValuesStatement(
            std::move(target),
            std::move(columns)) << region(c);
    }
    rule_error(c);
}

std::vector<std::unique_ptr<model::name::SimpleName>> Engine::visit(Grammar::InsertColumnListContext *c) {
    std::vector<std::unique_ptr<model::name::SimpleName>> results;
    for (auto element : c->simpleName()) {
        auto e = visit(element);
        results.emplace_back(std::move(e));
    }
    return results;
}

std::vector<std::unique_ptr<model::expression::Expression>> Engine::visit(
        Grammar::InsertValuesConstructorContext *c) {
    if (auto es = c->insertValuesExpressionList(); is_defined(es)) {
        // VALUES ...
        return visit(es);
    }
    rule_error(c);
}

std::vector<std::unique_ptr<model::expression::Expression>> Engine::visit(
        Grammar::InsertValuesExpressionListContext *c) {
    if (!c->insertValuesExpression().empty()) {
        std::vector<std::unique_ptr<model::expression::Expression>> results;
        for (auto element : c->insertValuesExpression()) {
            auto e = visit(element);
            results.emplace_back(std::move(e));
        }
        return results;
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::InsertValuesExpressionContext *c) {
    if (auto e = c->expression(); is_defined(e)) {
        return visit(e);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(Grammar::SelectStatementContext *c) {
    if (auto q = c->query(); is_defined(q)) {
        auto query = visit(q);
        return f.EmitStatement(std::move(query)) << region(c);
    }
    rule_error(c);
}

// query
//     : K_SELECT projectionSpec fromClause whereClause?
//     ;
std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::QueryContext *c) {
    if (is_defined(c->K_SELECT())) {
        if (auto from = c->fromClause(); is_defined(from)) {
            auto result = visit(from);
            if (auto cond = c->whereClause(); is_defined(cond)) {
                result = visit(cond, std::move(result));
            }
            if (auto proj = c->projectionSpec(); is_defined(proj)) {
                return visit(proj, std::move(result));
            }
        }
    }
    rule_error(c);
}

// fromClause
//     : K_FROM tableReference (',' tableReference)*
//     ;
std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::FromClauseContext *c) {
    if (is_defined(c->K_FROM()) && !c->tableReference().empty()) {
        std::unique_ptr<model::expression::Expression> result;
        for (auto ref : c->tableReference()) {
            auto term = visit(ref);
            if (!is_defined(result)) {
                result = std::move(term);
            } else {
                result = f.JoinExpression(
                    model::expression::relation::JoinExpression::Kind::CROSS,
                    std::move(result),
                    std::move(term),
                    {} // cross join should not have any conditions
                    ) << region(c, ref);
            }
        }
        return result;
    }
    rule_error(c);
}

// tableReference
//     : tablePrimary
//     | tableReference joinedTableRest
//     ;
std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::TableReferenceContext *c) {
    if (auto p = c->tablePrimary(); is_defined(p)) {
        return visit(p);
    }
    auto ref = c->tableReference();
    auto join = c->joinedTableRest();
    if (is_defined(ref) && is_defined(join)) {
        return visit(ref, join);
    }
    rule_error(c);
}

// tablePrimary
//     : tableName
//     | '(' joinedTable ')'
//     ;
std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::TablePrimaryContext *c) {
    if (auto name = c->tableName(); is_defined(name)) {
        return visit(name);
    }
    if (auto join = c->joinedTable(); is_defined(join)) {
        return visit(join);
    }
    rule_error(c);
}

// tableName
//     : name (K_AS? simpleName)?
//     ;
std::unique_ptr<model::expression::relation::ScanExpression> Engine::visit(Grammar::TableNameContext *c) {
    if (auto q = c->name(); is_defined(q)) {
        auto name = visit(q);
        if (auto n = c->simpleName(); is_defined(n)) {
            auto alias = visit(n);
            return f.ScanExpression(std::move(name), std::move(alias)) << region(c);
        }
        return f.ScanExpression(std::move(name)) << region(c);
    }
    rule_error(c);
}

// joinedTable
//     : tableReference joinedTableRest
//     ;
std::unique_ptr<model::expression::relation::JoinExpression> Engine::visit(Grammar::JoinedTableContext *c) {
    auto table = c->tableReference();
    auto join = c->joinedTableRest();
    if (is_defined(table) && is_defined(join)) {
        return visit(table, join);
    }
    rule_error(c);
}

// joinedTableRest
//     : joinType? K_JOIN tablePrimary joinSpecification?
//     | K_CROSS K_JOIN tablePrimary
//     | K_NATURAL joinType? K_JOIN tablePrimary
//     | K_UNION K_JOIN  tablePrimary
//     ;
std::unique_ptr<model::expression::relation::JoinExpression>
Engine::visit(Grammar::TableReferenceContext *leading, Grammar::JoinedTableRestContext *c) {
    assert(leading);  // NOLINT
    if (!is_defined(c->K_JOIN())) {
        rule_error(c);
    }
    if (!is_defined(c->tablePrimary())) {
        rule_error(c);
    }
    using Kind = model::expression::relation::JoinExpression::Kind;
    Kind kind { Kind::INVALID };
    std::unique_ptr<model::expression::Expression> spec {};
    if (is_defined(c->K_CROSS())) {
        kind = Kind::CROSS;
    } else if (is_defined(c->K_NATURAL())) {
        auto k = visit(c->joinType());
        switch (k) {
        case Kind::INNER:
            kind = Kind::NATURAL_INNER;
            break;
        case Kind::LEFT_OUTER:
            kind = Kind::NATURAL_LEFT_OUTER;
            break;
        case Kind::RIGHT_OUTER:
            kind = Kind::NATURAL_RIGHT_OUTER;
            break;
        case Kind::FULL_OUTER:
            kind = Kind::NATURAL_FULL_OUTER;
            break;
        default:
            abort();
        }
    } else if (is_defined(c->K_UNION())) {
        kind = Kind::UNION_OUTER;
    } else {
        kind = visit(c->joinType());
        if (auto s = c->joinSpecification(); is_defined(s)) {
            spec = visit(s);
        }
    }
    assert(kind != Kind::INVALID);  // NOLINT

    auto left = visit(leading);
    auto right = visit(c->tablePrimary());

    return f.JoinExpression(
        kind,
        std::move(left),
        std::move(right),
        std::move(spec)) << region(leading, c);
}

// joinType
//     : K_INNER
//     | K_LEFT (K_OUTER)?
//     | K_RIGHT (K_OUTER)?
//     | K_FULL (K_OUTER)?
//     ;
model::expression::relation::JoinExpression::Kind Engine::visit(Grammar::JoinTypeContext *c) {
    using Kind = model::expression::relation::JoinExpression::Kind;
    if (!is_defined(c)) {
        return Kind::INNER;
    }
    if (is_defined(c->K_INNER())) {
        return Kind::INNER;
    }
    if (is_defined(c->K_LEFT())) {
        return Kind::LEFT_OUTER;
    }
    if (is_defined(c->K_RIGHT())) {
        return Kind::RIGHT_OUTER;
    }
    if (is_defined(c->K_FULL())) {
        return Kind::FULL_OUTER;
    }
    rule_error(c);
}

// joinSpecification
//     : K_ON searchCondition
//     ;
std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::JoinSpecificationContext *c) {
    if (is_defined(c->K_ON())) {
        if (auto s = c->searchCondition(); is_defined(s)) {
            return visit(s);
        }
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(
        Grammar::ProjectionSpecContext *c,
        std::unique_ptr<model::expression::Expression> source) {
    if (is_defined(c->any)) {
        return source;
    }
    if (auto l = c->projectionColumnList(); is_defined(l)) {
        return visit(l, std::move(source));
    }
    rule_error(c);
}

std::unique_ptr<model::expression::relation::ProjectionExpression> Engine::visit(
        Grammar::ProjectionColumnListContext *c,
        std::unique_ptr<model::expression::Expression> source) {
    std::vector<std::unique_ptr<model::expression::relation::ProjectionExpression::Column>> columns;
    for (auto column : c->projectionColumn()) {
        columns.emplace_back(visit(column));
    }
    return f.ProjectionExpression(std::move(source), {}, std::move(columns)) << region(c);
}

std::unique_ptr<model::expression::relation::ProjectionExpression::Column> Engine::visit(
        Grammar::ProjectionColumnContext *c) {
    if (auto e = c->expression(); is_defined(e)) {
        auto expr = visit(e);
        std::unique_ptr<model::name::SimpleName> alias;
        if (auto a = c->simpleName(); is_defined(a)) {
            alias = visit(a);
        }
        return f.ProjectionExpressionColumn(std::move(expr), std::move(alias));
    }
    rule_error(c);
}

std::unique_ptr<model::expression::relation::SelectionExpression> Engine::visit(
        Grammar::WhereClauseContext *c,
        std::unique_ptr<model::expression::Expression> source) {
    if (auto e = c->searchCondition(); is_defined(e)) {
        auto expr = visit(e);
        return f.SelectionExpression(std::move(source), std::move(expr)) << region(c);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::SearchConditionContext *c) {
    if (auto e = c->booleanValueExpression(); is_defined(e)) {
        return visit(e);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::BooleanValueExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->booleanValueExpression();
    auto right_context = c->booleanTerm();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.BinaryOperator(
                model::expression::BinaryOperator::Kind::CONDITIONAL_OR,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::BooleanTermContext *c) {
    auto operator_token = c->op;
    auto left_context = c->booleanTerm();
    auto right_context = c->booleanFactor();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.BinaryOperator(
                model::expression::BinaryOperator::Kind::CONDITIONAL_AND,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::BooleanFactorContext *c) {
    auto operator_token = c->op;
    auto operand_context = c->booleanTest();
    if (is_defined(operator_token) && is_defined(operand_context)) {
        auto operand = visit(operand_context);
        return f.UnaryOperator(
                model::expression::UnaryOperator::Kind::CONDITIONAL_NOT,
                std::move(operand)) << region(c);
    }
    if (is_defined(operand_context)) {
        return visit(operand_context);
    }
    rule_error(c);
}

static model::expression::UnaryOperator::Kind boolean_test_operator(Grammar::BooleanTestContext *c) {
    if (is_defined(c->K_TRUE())) {
        if (is_defined(c->K_NOT())) {
            return model::expression::UnaryOperator::Kind::IS_NOT_TRUE;
        }
        return model::expression::UnaryOperator::Kind::IS_TRUE;
    }
    if (is_defined(c->K_FALSE())) {
        if (is_defined(c->K_NOT())) {
            return model::expression::UnaryOperator::Kind::IS_NOT_FALSE;
        }
        return model::expression::UnaryOperator::Kind::IS_FALSE;
    }
    return model::expression::UnaryOperator::Kind::INVALID;
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::BooleanTestContext *c) {
    auto operand_context = c->booleanPrimary();
    if (is_defined(operand_context) && is_defined(c->K_IS())) {
        auto kind = boolean_test_operator(c);
        if (kind != model::expression::UnaryOperator::Kind::INVALID) {
            auto operand = visit(operand_context);
            return f.UnaryOperator(
                    kind,
                    std::move(operand)) << region(c);
        }
    } else if (is_defined(operand_context)) {
        return visit(operand_context);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::BooleanPrimaryContext *c) {
    if (auto e = c->parenthesizedBooleanValueExpression(); is_defined(e)) {
        return visit(e);
    }
    if (auto e = c->expression(); is_defined(e)) {
        return visit(e);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::ParenthesizedBooleanValueExpressionContext *c) {
    if (auto expression = c->booleanValueExpression(); is_defined(expression)) {
        return visit(expression);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(Grammar::CreateTableStatementContext *c) {
    if (auto s = c->tableDefinition(); is_defined(s)) {
        return visit(s);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::ddl::CreateTableStatement> Engine::visit(Grammar::TableDefinitionContext *c) {
    auto n = c->name();
    auto es = c->tableElementList();
    if (is_defined(c->K_CREATE()) && is_defined(c->K_TABLE()) && is_defined(n) && is_defined(es)) {
        auto result = f.CreateTableStatement() << region(c);
        auto name = visit(n);
        result->table(std::move(name));
        for (auto a : c->tableDefinitionOption()) {
            visit(a, result.get());
        }
        visit(es, result.get());
        return result;
    }
    rule_error(c);
}

void Engine::visit(Grammar::TableDefinitionOptionContext *c, model::statement::ddl::CreateTableStatement *r) {
    using Attribute = model::statement::ddl::CreateTableStatement::Attribute;
    if (is_defined(c->K_IF()) && is_defined(c->K_NOT()) && is_defined(c->K_EXISTS())) {
        r->attributes().emplace(Attribute::IF_NOT_EXISTS);
        return;
    }
    rule_error(c);
}

void Engine::visit(Grammar::TableElementListContext *c, model::statement::ddl::CreateTableStatement *r) {
    if (!c->tableElement().empty()) {
        for (auto element : c->tableElement()) {
            visit(element, r);
        }
        return;
    }
    rule_error(c);
}

void Engine::visit(Grammar::TableElementContext *c, model::statement::ddl::CreateTableStatement *r) {
    if (auto d = c->columnDefinition(); is_defined(d)) {
        visit(d, r);
        return;
    }
    if (auto d = c->tableConstraintDefinition(); is_defined(d)) {
        visit(d, r);
        return;
    }
    rule_error(c);
}

void Engine::visit(Grammar::ColumnDefinitionContext *c, model::statement::ddl::CreateTableStatement *r) {
    auto n = c->columnName();
    auto t = c->dataType();
    if (is_defined(n) && is_defined(t)) {
        auto result = f.CreateTableStatementColumn();
        result->name(visit(n));
        result->type(visit(t));
        if (auto v = c->defaultClause(); is_defined(v)) {
            result->value(visit(v));
        }
        for (auto a : c->columnConstraintDefinition()) {
            visit(a, result.get());
        }
        r->columns().push_back(std::move(result));
        return;
    }
    rule_error(c);
}

std::vector<std::unique_ptr<model::statement::ddl::CreateTableStatement::PrimaryKey>> Engine::visit(
        Grammar::ColumnOrderListContext *c) {
    if (!c->columnOrder().empty()) {
        std::vector<std::unique_ptr<model::statement::ddl::CreateTableStatement::PrimaryKey>> results;
        results.reserve(c->columnOrder().size());
        for (auto e : c->columnOrder()) {
            results.emplace_back(visit(e));
        }
        return results;
    }
    rule_error(c);
}

std::unique_ptr<model::statement::ddl::CreateTableStatement::PrimaryKey> Engine::visit(
        Grammar::ColumnOrderContext *c) {
    auto n = c->columnName();
    if (is_defined(n)) {
        using Direction = model::statement::ddl::CreateTableStatement::PrimaryKey::Direction;
        auto name = visit(n);
        if (is_defined(c->K_ASC())) {
            return f.CreateTableStatementPrimaryKey(std::move(name), Direction::ASCENDANT);
        }
        if (is_defined(c->K_DESC())) {
            return f.CreateTableStatementPrimaryKey(std::move(name), Direction::DESCENDANT);
        }
        return f.CreateTableStatementPrimaryKey(std::move(name), Direction::DONT_CARE);
    }
    rule_error(c);
}

std::unique_ptr<model::name::SimpleName> Engine::visit(Grammar::ColumnNameContext *c) {
    if (auto n = c->simpleName(); is_defined(n)) {
        return visit(n);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::DefaultClauseContext *c) {
    if (is_defined(c->K_DEFAULT())) {
        if (auto v = c->literal(); is_defined(v)) {
            return visit(v);
        }
    }
    rule_error(c);
}

void Engine::visit(
        Grammar::ColumnConstraintDefinitionContext *c,
        model::statement::ddl::CreateTableStatement::Column *rc) {
    using Attribute = model::statement::ddl::CreateTableStatement::Column::Attribute;
    if (is_defined(c->K_NOT()) && is_defined(c->K_NULL())) {
        rc->attributes().emplace(Attribute::NOT_NULL);
        return;
    }
    if (is_defined(c->K_NULL())) {
        // already NULL
        return;
    }
    if (is_defined(c->K_PRIMARY())) {
        rc->attributes().emplace(Attribute::PRIMARY_KEY);
        return;
    }
    rule_error(c);
}

void Engine::visit(Grammar::TableConstraintDefinitionContext *c, model::statement::ddl::CreateTableStatement *r) {
    if (auto pk = c->primaryKeyDefinition(); is_defined(pk)) {
        visit(pk, r);
        return;
    }
    rule_error(c);
}

void Engine::visit(Grammar::PrimaryKeyDefinitionContext *c, model::statement::ddl::CreateTableStatement *r) {
    if (is_defined(c->K_PRIMARY()) && is_defined(c->K_KEY())) {
        // FIXME: only once
        if (auto cs = c->columnOrderList(); is_defined(cs)) {
            auto columns = visit(cs);
            for (auto& p : columns) {
                r->primary_keys().push_back(std::move(p));
            }
        }
        return;
    }
    rule_error(c);
}

std::unique_ptr<model::statement::Statement> Engine::visit(Grammar::StatementContext *c) {
    if (auto s = c->expressionStatement(); is_defined(s)) {
        return visit(s);
    }
    rule_error(c);
}

std::unique_ptr<model::statement::ExpressionStatement> Engine::visit(Grammar::ExpressionStatementContext *c) {
    if (auto e = c->expression(); is_defined(e)) {
        auto expression = visit(e);
        return f.ExpressionStatement(std::move(expression)) << region(c);
    }
    rule_error(c);
}

std::vector<std::unique_ptr<model::expression::Expression>> Engine::visit(Grammar::ExpressionListContext *c) {
    if (!c->expression().empty()) {
        std::vector<std::unique_ptr<model::expression::Expression>> results;
        for (auto element : c->expression()) {
            auto e = visit(element);
            results.push_back(std::move(e));
        }
        return results;
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::ExpressionContext *c) {
    if (auto e = c->assignExpression(); is_defined(e)) {
        return visit(e);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::AssignExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->name();
    auto right_context = c->assignExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        // FIXME: compound assignment
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.AssignExpression(
                std::move(left),
                std::move(right)) << region(c);
    }
    if (auto e = c->signConditionalOrExpression(); is_defined(e)) {
        return visit(e);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::SignConditionalOrExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->signConditionalOrExpression();
    auto right_context = c->signConditionalAndExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.BinaryOperator(
                model::expression::BinaryOperator::Kind::CONDITIONAL_OR,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::SignConditionalAndExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->signConditionalAndExpression();
    auto right_context = c->inclusiveOrExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.BinaryOperator(
                model::expression::BinaryOperator::Kind::CONDITIONAL_AND,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::InclusiveOrExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->inclusiveOrExpression();
    auto right_context = c->exclusiveOrExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.BinaryOperator(
                model::expression::BinaryOperator::Kind::LOGICAL_OR,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::ExclusiveOrExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->exclusiveOrExpression();
    auto right_context = c->andExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.BinaryOperator(
                model::expression::BinaryOperator::Kind::LOGICAL_XOR,
                std::move(left),
                std::move(right));
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::AndExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->andExpression();
    auto right_context = c->equalityExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto right = visit(right_context);
        return f.BinaryOperator(
                model::expression::BinaryOperator::Kind::LOGICAL_AND,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

static model::expression::BinaryOperator::Kind equality_operator(antlr4::Token *token) {
    switch (token->getType()) {
        case Grammar::EQUAL: return model::expression::BinaryOperator::Kind::EQUAL;
        case Grammar::LEFT_ANGLE_RIGHT_ANGLE: return model::expression::BinaryOperator::Kind::NOT_EQUAL;
        case Grammar::EXCLAMATION_MARK_EQUAL: return model::expression::BinaryOperator::Kind::NOT_EQUAL;
        default: return model::expression::BinaryOperator::Kind::INVALID;
    }
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::EqualityExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->equalityExpression();
    auto right_context = c->relationalExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto kind = equality_operator(operator_token);
        auto right = visit(right_context);
        return f.BinaryOperator(
                kind,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

static model::expression::BinaryOperator::Kind relational_operator(antlr4::Token *token) {
    switch (token->getType()) {
        case Grammar::LEFT_ANGLE: return model::expression::BinaryOperator::Kind::LESS_THAN;
        case Grammar::RIGHT_ANGLE: return model::expression::BinaryOperator::Kind::GREATER_THAN;
        case Grammar::LEFT_ANGLE_EQUAL: return model::expression::BinaryOperator::Kind::LESS_THAN_OR_EQUAL;
        case Grammar::RIGHT_ANGLE_EQUAL: return model::expression::BinaryOperator::Kind::GREATER_THAN_OR_EQUAL;
        default: return model::expression::BinaryOperator::Kind::INVALID;
    }
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::RelationalExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->relationalExpression();
    auto right_context = c->shiftExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto kind = relational_operator(operator_token);
        auto right = visit(right_context);
        return f.BinaryOperator(
                kind,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

static model::expression::BinaryOperator::Kind shift_operator(antlr4::Token *token) {
    switch (token->getType()) {
        case Grammar::LEFT_ANGLE_2: return model::expression::BinaryOperator::Kind::SHIFT_LEFT;
        case Grammar::RIGHT_ANGLE_2: return model::expression::BinaryOperator::Kind::SHIFT_RIGHT_ARITHMETIC;
        case Grammar::RIGHT_ANGLE_3: return model::expression::BinaryOperator::Kind::SHIFT_RIGHT_LOGICAL;
        default: return model::expression::BinaryOperator::Kind::INVALID;
    }
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::ShiftExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->shiftExpression();
    auto right_context = c->additiveExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto kind = shift_operator(operator_token);
        auto right = visit(right_context);
        return f.BinaryOperator(
                kind,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

static model::expression::BinaryOperator::Kind additive_operator(antlr4::Token *token) {
    switch (token->getType()) {
        case Grammar::PLUS: return model::expression::BinaryOperator::Kind::ADD;
        case Grammar::MINUS: return model::expression::BinaryOperator::Kind::SUBTRACT;
        default: return model::expression::BinaryOperator::Kind::INVALID;
    }
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::AdditiveExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->additiveExpression();
    auto right_context = c->multiplicativeExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto kind = additive_operator(operator_token);
        auto right = visit(right_context);
        return f.BinaryOperator(
                kind,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

static model::expression::BinaryOperator::Kind multiplicative_operator(antlr4::Token *token) {
    switch (token->getType()) {
        case Grammar::ASTERISK: return model::expression::BinaryOperator::Kind::MULTIPLY;
        case Grammar::SLASH: return model::expression::BinaryOperator::Kind::DIVIDE;
        case Grammar::PERCENT: return model::expression::BinaryOperator::Kind::REMAINDER;
        default: return model::expression::BinaryOperator::Kind::INVALID;
    }
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::MultiplicativeExpressionContext *c) {
    auto operator_token = c->op;
    auto left_context = c->multiplicativeExpression();
    auto right_context = c->unaryExpression();
    if (is_defined(left_context) && is_defined(operator_token) && is_defined(right_context)) {
        auto left = visit(left_context);
        auto kind = multiplicative_operator(operator_token);
        auto right = visit(right_context);
        return f.BinaryOperator(
                kind,
                std::move(left),
                std::move(right)) << region(c);
    }
    if (is_defined(right_context)) {
        return visit(right_context);
    }
    rule_error(c);
}

static model::expression::UnaryOperator::Kind unary_operator(antlr4::Token *token) {
    switch (token->getType()) {
        case Grammar::PLUS: return model::expression::UnaryOperator::Kind::PLUS;
        case Grammar::MINUS: return model::expression::UnaryOperator::Kind::SIGN_INVERSION;
        case Grammar::TILDE: return model::expression::UnaryOperator::Kind::BITWISE_COMPLEMENT;
        case Grammar::EXCLAMATION_MARK_EQUAL: return model::expression::UnaryOperator::Kind::LOGICAL_NOT;
        default: return model::expression::UnaryOperator::Kind::INVALID;
    }
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::UnaryExpressionContext *c) {
    auto operator_token = c->op;
    auto operand_context = c->unaryExpression();
    if (is_defined(operator_token) && is_defined(operand_context)) {
        auto kind = unary_operator(operator_token);
        auto operand = visit(operand_context);
        return f.UnaryOperator(
                kind,
                std::move(operand)) << region(c);
    }
    if (auto next = c->postfixExpression(); is_defined(next)) {
        return visit(next);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::PostfixExpressionContext *c) {
    if (auto e = c->primaryExpression(); is_defined(e)) {
        return visit(e);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::PrimaryExpressionContext *c) {
    if (auto e = c->functionCall(); is_defined(e)) {
        return visit(e);
    }
    if (auto n = c->name(); is_defined(n)) {
        auto name = visit(n);
        return f.VariableReference(std::move(name)) << region(c);
    }
    if (auto t = c->literal(); is_defined(t)) {
        return visit(t);
    }
    rule_error(c);
}

std::unique_ptr<model::expression::Expression> Engine::visit(Grammar::FunctionCallContext *c) {
    if (auto n = c->name(); is_defined(n)) {
        auto name = visit(n);
        if (auto el = c->expressionList(); is_defined(el)) {
            auto arguments = visit(el);
            return f.FunctionCall(std::move(name), std::move(arguments)) << region(c);
        }
        return f.FunctionCall(std::move(name)) << region(c);
    }
    rule_error(c);
}

std::unique_ptr<model::name::Name> Engine::visit(Grammar::NameContext *c) {
    auto q = c->name();
    auto n = c->simpleName();
    if (is_defined(q) && is_defined(n)) {
        auto qualifier = visit(q);
        auto name = visit(n);
        return f.QualifiedName(std::move(qualifier), std::move(name)) << region(c);
    }
    if (is_defined(n)) {
        return visit(n);
    }
    rule_error(c);
}

std::unique_ptr<model::name::SimpleName> Engine::visit(Grammar::SimpleNameContext *c) {
    if (is_defined(c->IDENTIFIER())) {
        auto token = c->IDENTIFIER()->getSymbol();
        return f.SimpleName(token->getText()) << region(c);
    }
    rule_error(c);
}

static common::core::value::Int::type parse_int(antlr4::Token *token) {
    return std::stoll(token->getText());
}

static common::core::value::Float::type parse_float(antlr4::Token *token) {
    return std::stod(token->getText());
}

static common::core::value::String::type parse_string(antlr4::Token *token) {
    auto str = token->getText();
    assert(str.length() >= 2);  // NOLINT
    std::string result;
    result.reserve(str.length() - 2);
    bool saw_escape = false;
    for (std::size_t i = 1, n = str.length() - 1; i < n; i++) {
        auto c = str[i];
        if (saw_escape) {
            switch (c) {
                case 't': result.push_back('\t'); break;
                case 'r': result.push_back('\r'); break;
                case 'n': result.push_back('\n'); break;
                default: result.push_back(c); break;
            }
            saw_escape = false;
        } else {
            if (c == '\\') {
                saw_escape = true;
            } else {
                result.push_back(c);
            }
        }
    }
    assert(!saw_escape);  // NOLINT
    return result;
}

std::unique_ptr<model::expression::Literal> Engine::visit(Grammar::LiteralContext *c) {
    if (auto n = c->INTEGRAL_NUMBER(); is_defined(n)) {
        try {
            auto value = parse_int(n->getSymbol());
            return f.Literal(common::core::type::Int(64U, common::core::Type::Nullity::NEVER_NULL), value) << region(c);
        } catch (std::out_of_range&) {
            // FIXME
            rule_error(c);
        }
    }
    if (auto n = c->FLOATING_POINT_NUMBER(); is_defined(n)) {
        try {
            auto value = parse_float(n->getSymbol());
            return f.Literal(common::core::type::Float(64U, common::core::Type::Nullity::NEVER_NULL), value) << region(c);
        } catch (std::out_of_range&) {
            // FIXME
            rule_error(c);
        }
    }
    if (is_defined(c->K_TRUE())) {
        return f.Literal(common::core::type::Bool(common::core::Type::Nullity::NEVER_NULL), true) << region(c);
    }
    if (is_defined(c->K_FALSE())) {
        return f.Literal(common::core::type::Bool(common::core::Type::Nullity::NEVER_NULL), false) << region(c);
    }
    if (is_defined(c->K_NULL())) {
        return f.Literal(common::core::type::Null(), nullptr);
    }
    if (auto n = c->STRING(); is_defined(n)) {
        auto value = parse_string(n->getSymbol());
        return f.Literal(common::core::type::String(common::core::Type::Nullity::NEVER_NULL), value) << region(c);
    }
    rule_error(c);
}

std::unique_ptr<model::type::Type> Engine::visit(Grammar::DataTypeContext *c) {
    if (is_defined(c->K_INT())) {
        if (auto p = c->dataSize(); is_defined(p)) {
            auto sz = visit(p);
            if (sz == 32U) {
                return f.Int32Type() << region(c);
            }
            if (sz == 64U) {
                return f.Int64Type() << region(c);
            }
        } else {
            return f.Int32Type() << region(c);
        }
    }
    if (is_defined(c->K_BIGINT())) {
        return f.Int64Type() << region(c);
    }
    if (is_defined(c->K_FLOAT())) {
        if (auto p = c->dataSize(); is_defined(p)) {
            auto sz = visit(p);
            if (sz == 32U) {
                return f.Float32Type() << region(c);
            }
            if (sz == 64U) {
                return f.Float64Type() << region(c);
            }
        } else {
            return f.Float32Type() << region(c);
        }
    }
    if (is_defined(c->K_DOUBLE())) {
        return f.Float64Type() << region(c);
    }
    if (is_defined(c->K_CHAR())) {
        if (auto p = c->dataSize(); is_defined(p)) {
            auto sz = visit(p);
            return f.CharType(sz) << region(c);
        }
    }
    if (is_defined(c->K_VARCHAR())) {
        if (auto p = c->dataSize(); is_defined(p)) {
            auto sz = visit(p);
            return f.VarCharType(sz) << region(c);
        }
    }
    if (is_defined(c->K_STRING())) {
        return f.StringType() << region(c);
    }
    rule_error(c);
}

std::size_t Engine::visit(Grammar::DataSizeContext *c) {
    if (auto t = c->INTEGRAL_NUMBER(); is_defined(t)) {
        return std::stoull(t->getSymbol()->getText());
    }
    return 0;
}

}  // namespace shakujo::parser::impl
