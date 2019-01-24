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

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(1U, cols.size());

    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());

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
    ASSERT_EQ(relation_decl->columns().size(), 1U);

    auto&& c0_ref = relation_decl->columns()[0];

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
    ASSERT_EQ(relation_decl->columns().size(), 1U);

    auto&& c0_ref = relation_decl->columns()[0];

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
        {},
        {
            f.ProjectionExpressionColumn(var("C1")),
            f.ProjectionExpressionColumn(var("C3")),
        }
    ));
    success();

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

TEST_F(AnalyzerRelationTest, projection_invalid_relation) {
    auto expr = analyze(f.ProjectionExpression(
        literal(1),
        {},
        {
            f.ProjectionExpressionColumn(var("C1")),
        }
    ));
    success(false);
    EXPECT_FALSE(is_propagated_error(expr.get()));
    EXPECT_FALSE(is_valid(extract_relation(expr.get(), true)));
}

TEST_F(AnalyzerRelationTest, projection_propagate_error) {
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("MISSING")),
        {},
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
        {},
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
}

TEST_F(AnalyzerRelationTest, projection_named_columns) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
        { "C3", t::Bool(NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("testing")),
        {},
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
}

TEST_F(AnalyzerRelationTest, projection_free) {
    add_variable(f.Name("free"), t::String(NON_NULL));
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto expr = analyze(f.ProjectionExpression(
        f.ScanExpression(f.Name("testing")),
        {},
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

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    EXPECT_EQ(names({"T1"}), cols[0].qualifiers());
    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());

    EXPECT_EQ(names({"T2"}), cols[1].qualifiers());
    EXPECT_EQ("C2", cols[1].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[1].type());
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
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    auto* left = as<expression::VariableReference>(cond->left());
    EXPECT_EQ(cols[0], extract_var(left));

    auto* right = as<expression::VariableReference>(cond->right());
    EXPECT_EQ(cols[1], extract_var(right));
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
        {}
    ));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());

    EXPECT_EQ("C2", cols[1].name());
    EXPECT_EQ(t::Int(32, NULLABLE), *cols[1].type());
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
        {}
    ));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NULLABLE), *cols[0].type());

    EXPECT_EQ("C2", cols[1].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[1].type());
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
        {}
    ));
    success();

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NULLABLE), *cols[0].type());

    EXPECT_EQ("C2", cols[1].name());
    EXPECT_EQ(t::Int(32, NULLABLE), *cols[1].type());
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

    auto* relation = extract_relation_type(expr.get());
    auto& cols = relation->columns();
    ASSERT_EQ(3U, cols.size());

    EXPECT_EQ(names({"T1", "T2"}), cols[0].qualifiers());
    EXPECT_EQ("K", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());

    EXPECT_EQ(names({"T1"}), cols[1].qualifiers());
    EXPECT_EQ("C1", cols[1].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[1].type());

    EXPECT_EQ(names({"T2"}), cols[2].qualifiers());
    EXPECT_EQ("C2", cols[2].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[2].type());
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
    auto& cols = relation->columns();
    ASSERT_EQ(2U, cols.size());

    auto* cond = as<expression::BinaryOperator>(expr->condition());
    auto* left = as<expression::VariableReference>(cond->left());
    EXPECT_EQ(cols[0], extract_var(left));

    auto* right = as<expression::VariableReference>(cond->right());
    EXPECT_EQ(cols[1], extract_var(right));
}

}  // namespace shakujo::analyzer
