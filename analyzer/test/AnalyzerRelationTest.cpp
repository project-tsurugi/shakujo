/*
 * Copyright 2018 shakujo project.
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
#include "shakujo/analyzer/Analyzer.h"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "AnalyzerTestBase.h"

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/type/Float.h"
#include "shakujo/common/core/type/Char.h"
#include "shakujo/common/core/type/String.h"
#include "shakujo/common/core/type/Null.h"
#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/core/value/String.h"
#include "shakujo/common/core/value/Null.h"

namespace shakujo::analyzer {

using namespace shakujo::model;
using namespace shakujo::common;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using common::util::dynamic_pointer_cast;
using common::util::equals;
using common::util::is_defined;
using common::util::is_valid;


class AnalyzerRelationTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> analyze(std::unique_ptr<T> expr) {
        return analyze_managed<T, expression::Expression>(std::move(expr));
    }
};

TEST_F(AnalyzerRelationTest, scan) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.ScanExpression(f.Name("testing")));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(1U, cols.size());

    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());
    EXPECT_EQ(names({"testing"}), cols[0].qualifiers());

    auto binding = extract_relation(expr.get());
    ASSERT_TRUE(is_valid(binding));
    EXPECT_TRUE(binding->output().source_table());
    EXPECT_EQ(binding->output().order().size(), 0U);
    EXPECT_FALSE(binding->output().distinct());

    auto&& strategy = binding->scan_strategy();
    ASSERT_TRUE(strategy.is_valid());
    EXPECT_EQ(strategy.table().name(), "testing");
    EXPECT_FALSE(strategy.index());
    EXPECT_EQ(strategy.kind(), binding::ScanStrategy::Kind::FULL);
}

TEST_F(AnalyzerRelationTest, scan_alias) {
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.ScanExpression(f.Name("testing"), f.Name("TT")));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(1U, cols.size());

    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());
    EXPECT_EQ(names({"TT"}), cols[0].qualifiers());

    auto binding = extract_relation(expr.get());
    auto&& strategy = binding->scan_strategy();
    ASSERT_TRUE(strategy.is_valid());
    EXPECT_EQ(strategy.table().name(), "testing");
    EXPECT_FALSE(strategy.index());
    EXPECT_EQ(strategy.kind(), binding::ScanStrategy::Kind::FULL);
}

TEST_F(AnalyzerRelationTest, scan_not_found) {
    auto expr = analyze(f.ScanExpression(f.Name("testing")));
    success(false);
}

TEST_F(AnalyzerRelationTest, select) {
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            literal(true, NON_NULL)));
    success();

    ASSERT_EQ(t::Bool(NON_NULL), type(expr->condition()));
}

TEST_F(AnalyzerRelationTest, select_invalid_relation) {
    auto expr = analyze(f.SelectionExpression(
            literal(1, 32U, NON_NULL),
            literal(true, NON_NULL)));
    success(false);
    EXPECT_FALSE(is_propagated_error(expr.get()));
    EXPECT_FALSE(is_valid(extract_relation(expr.get(), true)));
}

TEST_F(AnalyzerRelationTest, select_propagate_error) {
    auto expr = analyze(f.SelectionExpression(
            f.ScanExpression(f.Name("missing")),
            literal(true, NON_NULL)));
    success(false);
    EXPECT_TRUE(is_propagated_error(expr.get()));
    EXPECT_FALSE(is_valid(extract_relation(expr.get(), true)));
}

TEST_F(AnalyzerRelationTest, select_invalid_condition) {
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            literal(1, 32U, NON_NULL)));
    success(false);
}

TEST_F(AnalyzerRelationTest, select_ref_simple) {
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(
                    expression::BinaryOperator::Kind::EQUAL,
                    var("C1"),
                    literal(1, 32U, NON_NULL))));
    success();
    auto relation_decl = extract_relation(expr.get());
    ASSERT_EQ(relation_decl->process().columns().size(), 1U);

    auto&& c0_ref = relation_decl->process().columns()[0];

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(1U, cols.size());

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    auto* left = as<expression::VariableReference>(cond->left());
    auto var_ref = extract_var(left);
    EXPECT_EQ(c0_ref->id(), var_ref->id());
}

TEST_F(AnalyzerRelationTest, select_ref_qualified) {
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(
                    expression::BinaryOperator::Kind::EQUAL,
                    f.VariableReference(f.Name("testing", "C1")),
                    literal(1, 32U, NON_NULL))));
    success();
    auto relation_decl = extract_relation(expr.get());
    ASSERT_EQ(relation_decl->process().columns().size(), 1U);

    auto&& c0_ref = relation_decl->process().columns()[0];

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(1U, cols.size());

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    auto* left = as<expression::VariableReference>(cond->left());
    EXPECT_TRUE(equals(f.Name("testing", "C1"), left->name()));

    auto var_ref = extract_var(left);
    EXPECT_EQ(c0_ref->id(), var_ref->id());
}

TEST_F(AnalyzerRelationTest, select_ref_qualified_column_missing) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(
                    expression::BinaryOperator::Kind::EQUAL,
                    f.VariableReference(f.Name("testing", "MISSING")),
                    literal(1, 32U, NON_NULL))));
    success(false);
}

TEST_F(AnalyzerRelationTest, projection) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
        { "C3", t::Bool(NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("testing")),
        {
            f.ProjectionExpressionColumn(var("C1")),
            f.ProjectionExpressionColumn(var("C3")),
        }
    ));
    success();
    {
        auto* relation = extract_relation_type(expr.get());
        auto& cols = relation->columns();
        ASSERT_EQ(2U, cols.size());
        EXPECT_EQ("", cols[0].name());
        EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());
        EXPECT_EQ(names({}), cols[0].qualifiers());

        EXPECT_EQ("", cols[1].name());
        EXPECT_EQ(t::Bool(NON_NULL), *cols[1].type());
        EXPECT_EQ(names({}), cols[1].qualifiers());
    }
    {
        auto& cols = expr->columns();
        ASSERT_EQ(2U, cols.size());
        {
            auto var = extract_var(cols[0]);
            EXPECT_EQ(var->name(), common::core::Name());
            EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
        }
        {
            auto var = extract_var(cols[1]);
            EXPECT_EQ(var->name(), common::core::Name());
            EXPECT_EQ(*var->type(), t::Bool(NON_NULL));
        }
    }
}

TEST_F(AnalyzerRelationTest, projection_invalid_relation) {
    auto expr = analyze(f.ProjectionExpression(
        literal(1),
        {
            f.ProjectionExpressionColumn(var("C1")),
        }
    ));
    success(false);
    EXPECT_FALSE(is_propagated_error(expr.get()));
    EXPECT_FALSE(is_valid(extract_relation(expr.get(), true)));

    auto& cols = expr->columns();
    ASSERT_EQ(1U, cols.size());
    EXPECT_FALSE(is_valid(extract_var(cols[0], true)));
}

TEST_F(AnalyzerRelationTest, projection_propagate_error) {
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("MISSING")),
        {
            f.ProjectionExpressionColumn(var("C1")),
        }
    ));
    success(false);
    EXPECT_TRUE(is_propagated_error(expr.get()));
    EXPECT_FALSE(is_valid(extract_relation(expr.get(), true)));
}

TEST_F(AnalyzerRelationTest, projection_named_relation) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
        { "C3", t::Bool(NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("testing")),
        {
            f.ProjectionExpressionColumn(var("C1")),
            f.ProjectionExpressionColumn(var("C3")),
        },
        f.SimpleName("X")
    ));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    EXPECT_EQ("", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());
    EXPECT_EQ(names({"X"}), cols[0].qualifiers());

    EXPECT_EQ("", cols[1].name());
    EXPECT_EQ(t::Bool(NON_NULL), *cols[1].type());
    EXPECT_EQ(names({"X"}), cols[1].qualifiers());
    {
        auto& cols = expr->columns();
        ASSERT_EQ(2U, cols.size());
        {
            auto var = extract_var(cols[0]);
            EXPECT_EQ(var->name(), common::core::Name());
            EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
        }
        {
            auto var = extract_var(cols[1]);
            EXPECT_EQ(var->name(), common::core::Name());
            EXPECT_EQ(*var->type(), t::Bool(NON_NULL));
        }
    }
}

TEST_F(AnalyzerRelationTest, projection_named_columns) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
        { "C3", t::Bool(NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("testing")),
        {
            f.ProjectionExpressionColumn(var("C1"), f.SimpleName("D1")),
            f.ProjectionExpressionColumn(var("C3"), f.SimpleName("D2")),
        }
    ));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    EXPECT_EQ("D1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());
    EXPECT_EQ(names({}), cols[0].qualifiers());

    EXPECT_EQ("D2", cols[1].name());
    EXPECT_EQ(t::Bool(NON_NULL), *cols[1].type());
    EXPECT_EQ(names({}), cols[1].qualifiers());
    {
        auto& cols = expr->columns();
        ASSERT_EQ(2U, cols.size());
        {
            auto var = extract_var(cols[0]);
            EXPECT_EQ(var->name(), "D1");
            EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
        }
        {
            auto var = extract_var(cols[1]);
            EXPECT_EQ(var->name(), "D2");
            EXPECT_EQ(*var->type(), t::Bool(NON_NULL));
        }
    }
}

TEST_F(AnalyzerRelationTest, projection_free) {
    add_variable(f.Name("free"), t::String(NON_NULL));
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("testing")),
        {
            f.ProjectionExpressionColumn(var("C1"), f.SimpleName("D1")),
            f.ProjectionExpressionColumn(var("free"), f.SimpleName("D2")),
        }
    ));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    EXPECT_EQ("D1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());

    EXPECT_EQ("D2", cols[1].name());
    EXPECT_EQ(t::String(NON_NULL), *cols[1].type());
}

TEST_F(AnalyzerRelationTest, join) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::CROSS,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        {}
    ));
    success();

    auto relation = extract_relation(expr.get());
    auto&& left = extract_relation(expr->left())->output();
    auto&& right = extract_relation(expr->right())->output();
    auto&& output = relation->output();
    auto&& join = relation->join_strategy();

    EXPECT_EQ(join.kind(), binding::JoinStrategy::Kind::NESTED_LOOP);
    EXPECT_TRUE(join.equalities().empty());
    EXPECT_FALSE(join.left_outer());
    EXPECT_FALSE(join.right_outer());
    EXPECT_FALSE(join.left_semi());
    EXPECT_FALSE(join.right_semi());

    auto&& columns = join.columns();
    ASSERT_EQ(columns.size(), 2U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.output(), output.columns()[0]);
        EXPECT_EQ(c.left_source(), left.columns()[0]);
        EXPECT_EQ(c.right_source(), nullptr);
        EXPECT_FALSE(c.nullify_left_source());
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.output(), output.columns()[1]);
        EXPECT_EQ(c.left_source(), nullptr);
        EXPECT_EQ(c.right_source(), right.columns()[0]);
        EXPECT_FALSE(c.nullify_right_source());
    }
}

TEST_F(AnalyzerRelationTest, join_inner) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(
            model::expression::BinaryOperator::Kind::EQUAL,
            var("C1"),
            var("C2"))
    ));
    success();

    auto relation = extract_relation(expr.get());
    auto&& left = extract_relation(expr->left())->output();
    auto&& right = extract_relation(expr->right())->output();
    auto&& output = relation->output();
    auto&& join = relation->join_strategy();

    EXPECT_EQ(join.kind(), binding::JoinStrategy::Kind::NESTED_LOOP);
    EXPECT_TRUE(join.equalities().empty());
    EXPECT_FALSE(join.left_outer());
    EXPECT_FALSE(join.right_outer());
    EXPECT_FALSE(join.left_semi());
    EXPECT_FALSE(join.right_semi());

    auto&& columns = join.columns();
    ASSERT_EQ(columns.size(), 2U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.output(), output.columns()[0]);
        EXPECT_EQ(c.left_source(), left.columns()[0]);
        EXPECT_EQ(c.right_source(), nullptr);
        EXPECT_FALSE(c.nullify_left_source());
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.output(), output.columns()[1]);
        EXPECT_EQ(c.left_source(), nullptr);
        EXPECT_EQ(c.right_source(), right.columns()[0]);
        EXPECT_FALSE(c.nullify_right_source());
    }

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    EXPECT_EQ(left.columns()[0], extract_var(cond->left()));
    EXPECT_EQ(right.columns()[0], extract_var(cond->right()));
}

TEST_F(AnalyzerRelationTest, join_left_outer) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::LEFT_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(
            model::expression::BinaryOperator::Kind::EQUAL,
            var("C1"),
            var("C2"))
    ));
    success();

    auto relation = extract_relation(expr.get());
    auto&& left = extract_relation(expr->left())->output();
    auto&& right = extract_relation(expr->right())->output();
    auto&& output = relation->output();
    auto&& join = relation->join_strategy();

    EXPECT_EQ(join.kind(), binding::JoinStrategy::Kind::NESTED_LOOP);
    EXPECT_TRUE(join.equalities().empty());
    EXPECT_TRUE(join.left_outer());
    EXPECT_FALSE(join.right_outer());
    EXPECT_FALSE(join.left_semi());
    EXPECT_FALSE(join.right_semi());

    auto&& columns = join.columns();
    ASSERT_EQ(columns.size(), 2U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.output(), output.columns()[0]);
        EXPECT_EQ(c.left_source(), left.columns()[0]);
        EXPECT_EQ(c.right_source(), nullptr);
        EXPECT_FALSE(c.nullify_left_source());
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.output(), output.columns()[1]);
        EXPECT_EQ(c.left_source(), nullptr);
        EXPECT_EQ(c.right_source(), right.columns()[0]);
        EXPECT_TRUE(c.nullify_right_source());
    }

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    EXPECT_EQ(left.columns()[0], extract_var(cond->left()));
    EXPECT_EQ(right.columns()[0], extract_var(cond->right()));
}

TEST_F(AnalyzerRelationTest, join_right_outer) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::RIGHT_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(
            model::expression::BinaryOperator::Kind::EQUAL,
            var("C1"),
            var("C2"))
    ));
    success();

    auto relation = extract_relation(expr.get());
    auto&& left = extract_relation(expr->left())->output();
    auto&& right = extract_relation(expr->right())->output();
    auto&& output = relation->output();
    auto&& join = relation->join_strategy();

    EXPECT_EQ(join.kind(), binding::JoinStrategy::Kind::NESTED_LOOP);
    EXPECT_TRUE(join.equalities().empty());
    EXPECT_FALSE(join.left_outer());
    EXPECT_TRUE(join.right_outer());
    EXPECT_FALSE(join.left_semi());
    EXPECT_FALSE(join.right_semi());

    auto&& columns = join.columns();
    ASSERT_EQ(columns.size(), 2U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.output(), output.columns()[0]);
        EXPECT_EQ(c.left_source(), left.columns()[0]);
        EXPECT_EQ(c.right_source(), nullptr);
        EXPECT_TRUE(c.nullify_left_source());
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.output(), output.columns()[1]);
        EXPECT_EQ(c.left_source(), nullptr);
        EXPECT_EQ(c.right_source(), right.columns()[0]);
        EXPECT_FALSE(c.nullify_right_source());
    }

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    EXPECT_EQ(left.columns()[0], extract_var(cond->left()));
    EXPECT_EQ(right.columns()[0], extract_var(cond->right()));
}

TEST_F(AnalyzerRelationTest, join_full_outer) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::FULL_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(
            model::expression::BinaryOperator::Kind::EQUAL,
            var("C1"),
            var("C2"))
    ));
    success();

    auto relation = extract_relation(expr.get());
    auto&& left = extract_relation(expr->left())->output();
    auto&& right = extract_relation(expr->right())->output();
    auto&& output = relation->output();
    auto&& join = relation->join_strategy();

    EXPECT_EQ(join.kind(), binding::JoinStrategy::Kind::NESTED_LOOP);
    EXPECT_TRUE(join.equalities().empty());
    EXPECT_TRUE(join.left_outer());
    EXPECT_TRUE(join.right_outer());
    EXPECT_FALSE(join.left_semi());
    EXPECT_FALSE(join.right_semi());

    auto&& columns = join.columns();
    ASSERT_EQ(columns.size(), 2U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.output(), output.columns()[0]);
        EXPECT_EQ(c.left_source(), left.columns()[0]);
        EXPECT_EQ(c.right_source(), nullptr);
        EXPECT_TRUE(c.nullify_left_source());
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.output(), output.columns()[1]);
        EXPECT_EQ(c.left_source(), nullptr);
        EXPECT_EQ(c.right_source(), right.columns()[0]);
        EXPECT_TRUE(c.nullify_right_source());
    }

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    EXPECT_EQ(left.columns()[0], extract_var(cond->left()));
    EXPECT_EQ(right.columns()[0], extract_var(cond->right()));
}

TEST_F(AnalyzerRelationTest, join_natural) {
    add(schema::TableInfo { "T1", {
        { "K", t::Int(32U, NON_NULL), },
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "K", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::NATURAL_INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        {}
    ));
    success();

    auto relation = extract_relation(expr.get());
    ASSERT_EQ(relation->process().columns().size(), 4);

    auto&& left = extract_relation(expr->left())->output();
    auto&& right = extract_relation(expr->right())->output();
    auto&& output = relation->output();
    auto&& join = relation->join_strategy();

    EXPECT_EQ(join.kind(), binding::JoinStrategy::Kind::NESTED_LOOP);
    EXPECT_FALSE(join.equalities().empty());
    EXPECT_FALSE(join.left_outer());
    EXPECT_FALSE(join.right_outer());
    EXPECT_FALSE(join.left_semi());
    EXPECT_FALSE(join.right_semi());

    auto&& columns = join.columns();
    ASSERT_EQ(columns.size(), 3U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.output(), output.columns()[0]);
        EXPECT_EQ(c.left_source(), left.columns()[0]);
        EXPECT_EQ(c.right_source(), right.columns()[0]);
        EXPECT_FALSE(c.nullify_left_source());
        EXPECT_FALSE(c.nullify_right_source());
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.output(), output.columns()[1]);
        EXPECT_EQ(c.left_source(), left.columns()[1]);
        EXPECT_EQ(c.right_source(), nullptr);
        EXPECT_FALSE(c.nullify_left_source());
    }
    {
        auto&& c = columns[2];
        EXPECT_EQ(c.output(), output.columns()[2]);
        EXPECT_EQ(c.left_source(), nullptr);
        EXPECT_EQ(c.right_source(), right.columns()[1]);
        EXPECT_FALSE(c.nullify_right_source());
    }

    auto&& equalities = join.equalities();
    ASSERT_EQ(equalities.size(), 1);
    {
        auto&& [a, b] = *equalities.begin();
        EXPECT_EQ(a, left.columns()[0]);
        EXPECT_EQ(b, right.columns()[0]);
    }
}

TEST_F(AnalyzerRelationTest, join_natural_missing) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::NATURAL_INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        {}
    ));
    success(false);
}

TEST_F(AnalyzerRelationTest, join_natural_inconsistent) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Char(10U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::NATURAL_INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        {}
    ));
    success(false);
}

TEST_F(AnalyzerRelationTest, join_natural_ambiguous) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T3", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::NATURAL_INNER,
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2"))
        ),
        f.ScanExpression(f.Name("T3"))
    ));
    success(false);
    contains(Diagnostic::Code::DUPLICATE_COLUMN);
}

TEST_F(AnalyzerRelationTest, join_not_relation_left) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::CROSS,
        literal(1, 32U, NON_NULL),
        f.ScanExpression(f.Name("T2")),
        {}
    ));
    success(false);
}

TEST_F(AnalyzerRelationTest, join_not_relation_right) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::CROSS,
        f.ScanExpression(f.Name("T1")),
        literal(1, 32U, NON_NULL),
        {}
    ));
    success(false);
}

TEST_F(AnalyzerRelationTest, join_not_boolean_condition) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.ScanExpression(f.Name("T2"))
    ));
    success(false);
}

TEST_F(AnalyzerRelationTest, join_ambiguous_column) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(
            model::expression::BinaryOperator::Kind::EQUAL,
            var("C1"),
            var("C1"))
    ));
    success(false);
}

TEST_F(AnalyzerRelationTest, join_qualified_column) {
    add(schema::TableInfo { "T1", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "T2", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.JoinExpression(
        model::expression::relation::JoinExpression::Kind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(
            model::expression::BinaryOperator::Kind::EQUAL,
            f.VariableReference(f.Name(std::vector<std::string> { "T1", "C1" })),
            f.VariableReference(f.Name(std::vector<std::string> { "T2", "C1" })))
    ));
    success();

    auto relation = extract_relation(expr.get());
    auto&& left = extract_relation(expr->left())->output();
    auto&& right = extract_relation(expr->right())->output();
    auto&& output = relation->output();
    auto&& join = relation->join_strategy();

    EXPECT_EQ(join.kind(), binding::JoinStrategy::Kind::NESTED_LOOP);
    EXPECT_TRUE(join.equalities().empty());
    EXPECT_FALSE(join.left_outer());
    EXPECT_FALSE(join.right_outer());
    EXPECT_FALSE(join.left_semi());
    EXPECT_FALSE(join.right_semi());

    auto&& columns = join.columns();
    ASSERT_EQ(columns.size(), 2U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.output(), output.columns()[0]);
        EXPECT_EQ(c.left_source(), left.columns()[0]);
        EXPECT_EQ(c.right_source(), nullptr);
        EXPECT_FALSE(c.nullify_left_source());
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.output(), output.columns()[1]);
        EXPECT_EQ(c.left_source(), nullptr);
        EXPECT_EQ(c.right_source(), right.columns()[0]);
        EXPECT_FALSE(c.nullify_right_source());
    }

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    EXPECT_EQ(left.columns()[0], extract_var(cond->left()));
    EXPECT_EQ(right.columns()[0], extract_var(cond->right()));
}

TEST_F(AnalyzerRelationTest, aggregation) {
    using Quantifier = binding::FunctionBinding::Quantifier;
    auto fid = [&] { return env.binding_context().next_function_id(); };
    auto count_asterisk = std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::make_unique<t::Int>(32U, NON_NULL),
        Quantifier::ASTERISK);
    auto count_int32 = std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::make_unique<t::Int>(32U, NON_NULL),
        Quantifier::ALL,
        std::vector<binding::FunctionBinding::Parameter> {
            { "count", std::make_unique<t::Int>(32U, NON_NULL), }
        });
    auto count_int32n = std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::make_unique<t::Int>(32U, NON_NULL),
        Quantifier::ALL,
        std::vector<binding::FunctionBinding::Parameter> {
            { "count", std::make_unique<t::Int>(32U, NULLABLE), }
        });
    env.register_builtin(std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::vector {
            count_asterisk,
            count_int32,
            count_int32n,
        }));
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("testing")),
        {
            f.ProjectionExpressionColumn(f.FunctionCall(
                f.Name("COUNT"),
                {
                    var("C1")
                })),
        }
    ));
    success();

    auto agg = dynamic_pointer_cast<expression::relation::AggregationExpression>(expr->operand());
    auto scan = dynamic_pointer_cast<expression::relation::ScanExpression>(agg->operand());

    auto&& scan_out = extract_relation(scan)->output();
    auto&& agg_in = extract_relation(agg)->process();
    auto&& agg_out = extract_relation(agg)->output();
    auto&& proj_in = extract_relation(expr.get())->process();

    ASSERT_EQ(scan_out.columns().size(), 1U);
    ASSERT_EQ(agg_in.columns().size(), 1U);
    ASSERT_EQ(agg_out.columns().size(), 1U);
    ASSERT_EQ(proj_in.columns().size(), 1U);

    ASSERT_EQ(scan_out.columns()[0], agg_in.columns()[0]);
    ASSERT_EQ(agg_out.columns()[0], proj_in.columns()[0]);

    ASSERT_EQ(agg->keys().size(), 0U);
    ASSERT_EQ(agg->columns().size(), 1U);
    {
        auto c = agg->columns()[0];
        EXPECT_EQ(agg_out.columns()[0], extract_var(c));

        auto f = extract_func(c);
        EXPECT_EQ(f, count_int32);

        auto v = extract_var(c->operand());
        EXPECT_EQ(v, agg_in.columns()[0]);
    }

    ASSERT_EQ(expr->columns().size(), 1U);
    {
        auto c = expr->columns()[0];
        auto v = extract_var(c->value());
        EXPECT_EQ(v, proj_in.columns()[0]);
    }
}

TEST_F(AnalyzerRelationTest, aggregation_direct_table) {
    using Quantifier = binding::FunctionBinding::Quantifier;
    auto fid = [&] { return env.binding_context().next_function_id(); };
    auto count_asterisk = std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::make_unique<t::Int>(32U, NON_NULL),
        Quantifier::ASTERISK);
    auto count_int32 = std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::make_unique<t::Int>(32U, NON_NULL),
        Quantifier::ALL,
        std::vector<binding::FunctionBinding::Parameter> {
            { "count", std::make_unique<t::Int>(32U, NON_NULL), }
        });
    auto count_int32n = std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::make_unique<t::Int>(32U, NON_NULL),
        Quantifier::ALL,
        std::vector<binding::FunctionBinding::Parameter> {
            { "count", std::make_unique<t::Int>(32U, NULLABLE), }
        });
    env.register_builtin(std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::vector {
            count_asterisk,
            count_int32,
            count_int32n,
        }));
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.AggregationExpression(
        f.ScanExpression(f.Name("testing")),
        {},
        {
            f.AggregationExpressionColumn(
                f.Name("COUNT"),
                model::expression::FunctionCall::Quantifier::ABSENT,
                var("C1")),
        }
    ));
    success();

    auto agg = dynamic_pointer_cast<expression::relation::AggregationExpression>(expr.get());
    auto scan = dynamic_pointer_cast<expression::relation::ScanExpression>(agg->operand());

    auto&& scan_out = extract_relation(scan)->output();
    auto&& agg_in = extract_relation(agg)->process();
    auto&& agg_out = extract_relation(agg)->output();

    ASSERT_EQ(scan_out.columns().size(), 1U);
    ASSERT_EQ(agg_in.columns().size(), 1U);
    ASSERT_EQ(agg_out.columns().size(), 1U);

    EXPECT_EQ(scan_out.columns()[0], agg_in.columns()[0]);

    ASSERT_EQ(agg->keys().size(), 0U);
    ASSERT_EQ(agg->columns().size(), 1U);
    {
        auto c = agg->columns()[0];
        EXPECT_EQ(agg_out.columns()[0], extract_var(c));

        auto f = extract_func(c);
        EXPECT_EQ(f, count_int32);

        auto v = extract_var(c->operand());
        EXPECT_EQ(v, agg_in.columns()[0]);
    }
}

TEST_F(AnalyzerRelationTest, aggregation_group_by) {
    using Quantifier = binding::FunctionBinding::Quantifier;
    auto fid = [&] { return env.binding_context().next_function_id(); };
    auto count = std::make_shared<binding::FunctionBinding>(
        fid(),
        common::core::Name { "count" },
        std::make_unique<t::Int>(32U, NON_NULL),
        Quantifier::ALL,
        std::vector<binding::FunctionBinding::Parameter> {
            { "count", std::make_unique<t::Int>(32U, NON_NULL), }
        });
    env.register_builtin(count);
    add(schema::TableInfo { "testing", {
        { "K", t::Int(32U, NON_NULL), },
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.GroupExpression(
            f.ScanExpression(f.Name("testing")),
            {
                var("K")
            }),
        {
            f.ProjectionExpressionColumn(var("K")),
            f.ProjectionExpressionColumn(f.FunctionCall(
                f.Name("COUNT"),
                {
                    var("C1")
                })),
        }
    ));
    success();

    auto agg = dynamic_pointer_cast<expression::relation::AggregationExpression>(expr->operand());
    auto scan = dynamic_pointer_cast<expression::relation::ScanExpression>(agg->operand());

    auto&& scan_out = extract_relation(scan)->output();
    auto&& agg_in = extract_relation(agg)->process();
    auto&& agg_out = extract_relation(agg)->output();
    auto&& proj_in = extract_relation(expr.get())->process();

    ASSERT_EQ(scan_out.columns().size(), 2);
    ASSERT_EQ(agg_in.columns().size(), 2);
    ASSERT_EQ(agg_out.columns().size(), 2);
    ASSERT_EQ(proj_in.columns().size(), 2);

    ASSERT_EQ(scan_out.columns()[0], agg_in.columns()[0]);
    ASSERT_EQ(scan_out.columns()[1], agg_in.columns()[1]);
    ASSERT_EQ(agg_out.columns()[0], proj_in.columns()[0]);
    ASSERT_EQ(agg_out.columns()[1], proj_in.columns()[1]);

    ASSERT_EQ(agg->keys().size(), 1U);
    {
        auto k = agg->keys()[0];
        auto v = extract_var(k);
        EXPECT_EQ(v, agg_in.columns()[0]);
    }
    ASSERT_EQ(agg->columns().size(), 2U);
    {
        auto c = agg->columns()[0];
        EXPECT_EQ(agg_out.columns()[0], extract_var(c));

        EXPECT_EQ(c->function(), nullptr);

        auto v = extract_var(c->operand());
        EXPECT_EQ(v, agg_in.columns()[0]);
    }
    {
        auto c = agg->columns()[1];
        EXPECT_EQ(agg_out.columns()[1], extract_var(c));

        auto f = extract_func(c);
        EXPECT_EQ(f, count);

        auto v = extract_var(c->operand());
        EXPECT_EQ(v, agg_in.columns()[1]);
    }

    ASSERT_EQ(expr->columns().size(), 2);
    {
        auto c = expr->columns()[0];
        auto v = extract_var(c->value());
        EXPECT_EQ(v, proj_in.columns()[0]);
    }
    {
        auto c = expr->columns()[1];
        auto v = extract_var(c->value());
        EXPECT_EQ(v, proj_in.columns()[1]);
    }
}

TEST_F(AnalyzerRelationTest, order) {
    // using IDir = model::expression::relation::OrderExpression::Direction;

    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.OrderExpression(
        f.ScanExpression(f.Name("testing")),
        {
            f.OrderExpressionElement(var("C2")),
        }));
    success();

    auto relation = extract_relation(expr.get());
    auto& cols = relation->process().columns();
    ASSERT_EQ(3U, cols.size());

    ASSERT_EQ(expr->elements().size(), 1);
    {
        auto* c = expr->elements()[0];
        auto var = extract_var(c);
        EXPECT_EQ(var, cols[1]);
    }
}

TEST_F(AnalyzerRelationTest, order_complex) {
    using IDir = model::expression::relation::OrderExpression::Direction;

    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.OrderExpression(
        f.ScanExpression(f.Name("testing")),
        {
            f.OrderExpressionElement(var("C1"), IDir::DESCENDANT),
            f.OrderExpressionElement(literal(100)),
            f.OrderExpressionElement(f.UnaryOperator(expression::UnaryOperator::Kind::SIGN_INVERSION, var("C3"))),
        }));
    success();

    auto relation = extract_relation(expr.get());
    auto& cols = relation->process().columns();
    ASSERT_EQ(3U, cols.size());

    ASSERT_EQ(expr->elements().size(), 3);
    {
        auto* c = expr->elements()[0];
        auto var = extract_var(c);
        EXPECT_EQ(var, cols[0]);
    }
    {
        auto* c = expr->elements()[1];
        auto var = extract_var(c);
        EXPECT_EQ(*var->value(), v::Int(100));
        EXPECT_FALSE(relation->process().index_of(*var).has_value());
    }
    {
        auto* c = expr->elements()[2];
        auto var = extract_var(c);
        EXPECT_FALSE(var->value());
        EXPECT_FALSE(relation->process().index_of(*var).has_value());
    }
}

TEST_F(AnalyzerRelationTest, distinct) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.DistinctExpression(f.ScanExpression(f.Name("testing"))));
    success();

    auto relation = extract_relation(expr.get());
    auto& cols = relation->process().columns();
    ASSERT_EQ(1U, cols.size());
}
}  // namespace shakujo::analyzer
