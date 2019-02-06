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

#include "AnalyzerTestBase.h"

namespace shakujo::analyzer {

using namespace shakujo::model;
using namespace shakujo::common;
using namespace shakujo::model::statement::dml;
using namespace shakujo::model::expression::relation;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using common::util::equals;
using common::util::is_valid;
using common::util::dynamic_pointer_cast;

class AnalyzerDmlTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> analyze(std::unique_ptr<T> node) {
        return analyze_unmanaged<T>(std::move(node));
    }
};

TEST_F(AnalyzerDmlTest, emit) {
    add(schema::TableInfo { "testing", {
            { "C1", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.EmitStatement(f.ScanExpression(f.Name("testing"))));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->output().source_table().name(), "testing");
    ASSERT_EQ(rbinding->output().columns().size(), 1U);

    auto* relation = extract_relation_type(stmt->source());
    auto& cols = relation->columns();
    ASSERT_EQ(1U, cols.size());

    EXPECT_EQ("C1", cols[0].name());
    EXPECT_EQ(t::Int(32, NON_NULL), *cols[0].type());
    EXPECT_EQ(names({"testing"}), cols[0].qualifiers());
}

TEST_F(AnalyzerDmlTest, emit_inconsistent) {
    auto stmt = analyze(f.EmitStatement(literal(1)));
    success(false);
}

TEST_F(AnalyzerDmlTest, insert_values) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C1"), literal(1, 32U)),
        }));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->output().columns().size(), 1U);

    auto& columns = stmt->columns();
    ASSERT_EQ(1U, columns.size());
    {
        auto* c = columns[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_EQ(1, get<v::Int>(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C1");
        EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
    }
}

TEST_F(AnalyzerDmlTest, insert_values_wo_colum_names) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn({}, literal(1, 32U)),
        }));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->output().columns().size(), 1U);

    auto& columns = stmt->columns();
    ASSERT_EQ(1U, columns.size());
    {
        auto* c = columns[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_EQ(1, get<v::Int>(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C1");
        EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
    }
}

TEST_F(AnalyzerDmlTest, insert_values_reorder_columns) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C3"), literal(3, 32U)),
            f.InsertValuesStatementColumn(f.SimpleName("C1"), literal(1, 32U)),
            f.InsertValuesStatementColumn(f.SimpleName("C2"), literal(2, 32U)),
        }));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->output().columns().size(), 3U);

    auto& columns = stmt->columns();
    ASSERT_EQ(3U, columns.size());
    {
        auto* c = columns[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_EQ(1, get<v::Int>(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C1");
        EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
    }
    {
        auto* c = columns[1];
        EXPECT_TRUE(equals(f.SimpleName("C2"), c->name()));
        EXPECT_EQ(2, get<v::Int>(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C2");
        EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
    }
    {
        auto* c = columns[2];
        EXPECT_TRUE(equals(f.SimpleName("C3"), c->name()));
        EXPECT_EQ(3, get<v::Int>(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C3");
        EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
    }
}

TEST_F(AnalyzerDmlTest, insert_values_omit_columns) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), v::Int(-1) },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NULLABLE), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C2"), literal(2, 32U)),
        }));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->output().columns().size(), 3U);

    auto& columns = stmt->columns();
    ASSERT_EQ(3U, columns.size());
    {
        auto* c = columns[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_EQ(-1, get<v::Int>(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C1");
        EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
    }
    {
        auto* c = columns[1];
        EXPECT_TRUE(equals(f.SimpleName("C2"), c->name()));
        EXPECT_EQ(2, get<v::Int>(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C2");
        EXPECT_EQ(*var->type(), t::Int(32U, NON_NULL));
    }
    {
        auto* c = columns[2];
        EXPECT_TRUE(equals(f.SimpleName("C3"), c->name()));
        auto* cast = as<expression::ImplicitCast>(c->value());
        EXPECT_EQ(t::Int(32U, NULLABLE), *cast->type());
        EXPECT_EQ(v::Null(), *as<expression::Literal>(cast->operand())->value());

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C3");
        EXPECT_EQ(*var->type(), t::Int(32U, NULLABLE));
    }
}

TEST_F(AnalyzerDmlTest, insert_values_promote) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C1"), literal(1, 32U)),
        }));
    success();

    auto& columns = stmt->columns();
    ASSERT_EQ(1U, columns.size());
    {
        auto* c = columns[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_EQ(t::Int(64U, NON_NULL), cast_type(c->value()));

        auto var = extract_var(c);
        EXPECT_EQ(var->name(), "C1");
        EXPECT_EQ(*var->type(), t::Int(64U, NON_NULL));
    }
}

TEST_F(AnalyzerDmlTest, insert_values_promote_fail) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C1"), literal(true)),
        }));
    success(false);

    auto& columns = stmt->columns();
    ASSERT_EQ(1U, columns.size());
    EXPECT_FALSE(is_valid(extract_var(columns[0], true)));
}

TEST_F(AnalyzerDmlTest, insert_values_promote_fail_null) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C1"), literal(1)),
        }));
    success(false);

    auto& columns = stmt->columns();
    ASSERT_EQ(2U, columns.size());
    EXPECT_TRUE(is_valid(extract_var(columns[0], true)));
    EXPECT_FALSE(is_valid(extract_var(columns[1], true)));
}

TEST_F(AnalyzerDmlTest, insert_values_unknown_column) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.InsertValuesStatement(
        f.Name("testing"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C2"), literal(1, 32U)),
        }));
    success(false);

    auto& columns = stmt->columns();
    ASSERT_EQ(1U, columns.size());
    EXPECT_FALSE(is_valid(extract_var(columns[0], true)));
}

TEST_F(AnalyzerDmlTest, update) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("testing")),
        f.Name("testing"),
        {
            f.UpdateStatementColumn(f.Name("C2"), literal(1, 32U)),
        }));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->process().columns().size(), 3U);
    ASSERT_EQ(rbinding->output().columns().size(), 1U);

    {
        auto&& c = stmt->columns()[0];
        EXPECT_EQ(get<v::Int>(c->value()), 1);

        auto v = extract_var(c);
        EXPECT_EQ(v->name(), "C2");
        EXPECT_EQ(*v->type(), t::Int(32U, NON_NULL));
    }
}

TEST_F(AnalyzerDmlTest, update_multiple_columns) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("testing")),
        f.Name("testing"),
        {
            f.UpdateStatementColumn(f.Name("C2"), literal(2, 32U)),
            f.UpdateStatementColumn(f.Name("C1"), literal(1, 32U)),
            f.UpdateStatementColumn(f.Name("C3"), literal(3, 32U)),
        }));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->process().columns().size(), 3U);
    ASSERT_EQ(rbinding->output().columns().size(), 3U);

    {
        auto&& c = stmt->columns()[0];
        auto v = extract_var(c);
        EXPECT_EQ(v->name(), "C2");
        EXPECT_EQ(*v->type(), t::Int(32U, NON_NULL));
        EXPECT_EQ(get<v::Int>(c->value()), 2);
    }
    {
        auto&& c = stmt->columns()[1];
        auto v = extract_var(c);
        EXPECT_EQ(v->name(), "C1");
        EXPECT_EQ(*v->type(), t::Int(32U, NON_NULL));
        EXPECT_EQ(get<v::Int>(c->value()), 1);
    }
    {
        auto&& c = stmt->columns()[2];
        auto v = extract_var(c);
        EXPECT_EQ(v->name(), "C3");
        EXPECT_EQ(*v->type(), t::Int(32U, NON_NULL));
        EXPECT_EQ(get<v::Int>(c->value()), 3);
    }
}

TEST_F(AnalyzerDmlTest, update_conversion) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("testing")),
        f.Name("testing"),
        {
            f.UpdateStatementColumn(f.Name("C2"), literal(1, 32U)),
        }));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->process().columns().size(), 3U);
    ASSERT_EQ(rbinding->output().columns().size(), 1U);

    {
        auto&& c = stmt->columns()[0];
        EXPECT_EQ(cast_type(c->value()), t::Int(64U, NON_NULL));

        auto v = extract_var(c);
        EXPECT_EQ(v->name(), "C2");
        EXPECT_EQ(*v->type(), t::Int(64U, NON_NULL));
    }
}

TEST_F(AnalyzerDmlTest, update_not_a_relation) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        literal(false),
        f.Name("testing"),
        { f.UpdateStatementColumn(f.Name("C2"), literal(1, 32U)), }));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE));
}

TEST_F(AnalyzerDmlTest, update_table_not_found) {
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("testing")),
        f.Name("testing"),
        { f.UpdateStatementColumn(f.Name("C2"), literal(1, 32U)), }));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::TABLE_NOT_FOUND));
}

TEST_F(AnalyzerDmlTest, update_not_a_table_expression) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "another", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("another")),
        f.Name("testing"),
        { f.UpdateStatementColumn(f.Name("C2"), literal(1, 32U)), }));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::INVALID_TABLE_EXPRESSION));
}

TEST_F(AnalyzerDmlTest, update_column_not_found) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("testing")),
        f.Name("testing"),
        { f.UpdateStatementColumn(f.Name("INVALID"), literal(1, 32U)), }));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::COLUMN_NOT_FOUND));
}

TEST_F(AnalyzerDmlTest, update_duplicate_column) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("testing")),
        f.Name("testing"),
        {
            f.UpdateStatementColumn(f.Name("C2"), literal(1, 32U)),
            f.UpdateStatementColumn(f.Name("C2"), literal(2, 32U)),
        }));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::DUPLICATE_COLUMN));
}

TEST_F(AnalyzerDmlTest, update_incompatible_type) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.UpdateStatement(
        f.ScanExpression(f.Name("testing")),
        f.Name("testing"),
        {
            f.UpdateStatementColumn(f.Name("C2"), literal("1")),
        }));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE));
}

TEST_F(AnalyzerDmlTest, delete) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.DeleteStatement(f.ScanExpression(f.Name("testing")), f.Name("testing")));
    success();

    auto rbinding = extract_relation(stmt.get());
    EXPECT_EQ(rbinding->destination_table().name(), "testing");
    ASSERT_EQ(rbinding->output().columns().size(), 3U);
}

TEST_F(AnalyzerDmlTest, delete_not_a_relation) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.DeleteStatement(literal(1), f.Name("testing")));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE));
}

TEST_F(AnalyzerDmlTest, delete_table_not_found) {
    auto stmt = analyze(f.DeleteStatement(f.ScanExpression(f.Name("testing")), f.Name("testing")));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::TABLE_NOT_FOUND));
}

TEST_F(AnalyzerDmlTest, delete_not_a_table_expression) {
    add(schema::TableInfo { "testing", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    add(schema::TableInfo { "another", {
        { "C1", t::Int(32U, NON_NULL), },
        { "C2", t::Int(32U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto stmt = analyze(f.DeleteStatement(f.ScanExpression(f.Name("another")), f.Name("testing")));
    success(false);
    EXPECT_TRUE(contains(Diagnostic::Code::INVALID_TABLE_EXPRESSION));
}

}  // namespace shakujo::analyzer
