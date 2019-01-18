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
using namespace shakujo::model::statement::ddl;
using namespace shakujo::model::expression::relation;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

class AnalyzerDdlTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> analyze(std::unique_ptr<T> node) {
        return analyze_unmanaged<T>(std::move(node));
    }
};

using common::util::equals;

TEST_F(AnalyzerDdlTest, CreateTable) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type()),
        }));
    success();

    EXPECT_TRUE(equals(f.Name("table"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto* c = cols[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_TRUE(equals(f.Int32Type(), c->type()));
        EXPECT_FALSE(c->value());

        auto& attrs = c->attributes();
        EXPECT_EQ(0U, attrs.size());
    }

    auto& attrs = stmt->attributes();
    EXPECT_EQ(0U, attrs.size());

    auto& pks = stmt->primary_keys();
    ASSERT_EQ(0U, pks.size());
}

TEST_F(AnalyzerDdlTest, CreateTable_default_value) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type(), f.Literal(t::Int(64U), v::Int(1024))),
        }));
    success();

    EXPECT_TRUE(equals(f.Name("table"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto* c = cols[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_TRUE(equals(f.Int32Type(), c->type()));
        EXPECT_EQ(1024, get<v::Int>(c->value()));
        EXPECT_EQ(t::Int(32U, NULLABLE), type(c->value()));
    }
}

TEST_F(AnalyzerDdlTest, CreateTable_default_value_not_null) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(
                f.Name("C1"),
                f.Int32Type(),
                f.Literal(t::Int(64U), v::Int(1024)),
                {
                    CreateTableStatement::Column::Attribute::NOT_NULL,
                }),
        }));
    success();

    EXPECT_TRUE(equals(f.Name("table"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto* c = cols[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_TRUE(equals(f.Int32Type(), c->type()));
        EXPECT_EQ(1024, get<v::Int>(c->value()));
        EXPECT_EQ(t::Int(32U, NON_NULL), type(c->value()));
    }
}

TEST_F(AnalyzerDdlTest, CreateTable_default_value_primary_key) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(
                f.Name("C1"),
                f.Int32Type(),
                f.Literal(t::Int(64U), v::Int(1024)),
                {
                    CreateTableStatement::Column::Attribute::PRIMARY_KEY,
                }),
        }));
    success();

    EXPECT_TRUE(equals(f.Name("table"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto* c = cols[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_TRUE(equals(f.Int32Type(), c->type()));
        EXPECT_EQ(1024, get<v::Int>(c->value()));
        EXPECT_EQ(t::Int(32U, NON_NULL), type(c->value()));
    }
}

TEST_F(AnalyzerDdlTest, CreateTable_column_primary_key) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(
                f.Name("C1"),
                f.Int32Type(),
                {},
                {
                    CreateTableStatement::Column::Attribute::PRIMARY_KEY,
                }),
        }));
    success();

    EXPECT_TRUE(equals(f.Name("table"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto* c = cols[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), c->name()));
        EXPECT_TRUE(equals(f.Int32Type(), c->type()));
        EXPECT_FALSE(c->value());

        auto& attrs = c->attributes();
        EXPECT_EQ(1U, attrs.size());
        EXPECT_NE(attrs.end(), attrs.find(CreateTableStatement::Column::Attribute::NOT_NULL));
    }

    auto& pks = stmt->primary_keys();
    ASSERT_EQ(1U, pks.size());
    {
        auto* k = pks[0];
        EXPECT_TRUE(equals(f.SimpleName("C1"), k->name()));
        EXPECT_EQ(CreateTableStatement::PrimaryKey::Direction::DONT_CARE, k->direction());
    }
}

TEST_F(AnalyzerDdlTest, CreateTable_duplicate_columns) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type()),
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type()),
        }));
    success(false);
}

TEST_F(AnalyzerDdlTest, CreateTable_duplicate_primary_key) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(
                f.Name("C1"), f.Int32Type(), {},
                { CreateTableStatement::Column::Attribute::PRIMARY_KEY }),
            f.CreateTableStatementColumn(
                f.Name("C2"), f.Int32Type(), {},
                { CreateTableStatement::Column::Attribute::PRIMARY_KEY }),
        }));
    success(false);
}

TEST_F(AnalyzerDdlTest, CreateTable_unknown_primary_key) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type()),
        },
        {},
        {
            f.CreateTableStatementPrimaryKey(f.Name("MISSING"))
        }));
    success(false);
}

TEST_F(AnalyzerDdlTest, CreateTable_duplicate_primary_key_constraint) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(
                f.Name("C1"), f.Int32Type(), {},
                { CreateTableStatement::Column::Attribute::PRIMARY_KEY }),
        },
        {},
        {
            f.CreateTableStatementPrimaryKey(f.Name("C1")),
        }));
    success(false);
}

TEST_F(AnalyzerDdlTest, CreateTable_inconsistent_default_value) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type(), f.Literal(t::String(), "OK")),
        }));
    success(false);
}
}  // namespace shakujo::analyzer
