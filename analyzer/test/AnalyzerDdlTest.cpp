/*
 * Copyright 2018-2024 Project Tsurugi.
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

    auto&& table = extract_relation(stmt.get())->destination_table();
    ASSERT_TRUE(table);
    EXPECT_EQ(table.name(), "table");

    auto&& columns = table.columns();
    ASSERT_EQ(columns.size(), 1U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(*c.type(), t::Int(32U, NULLABLE));
        EXPECT_EQ(c.default_value(), nullptr);
    }

    EXPECT_FALSE(table.primary_index().is_valid());

    auto&& indices = table.secondary_indices();
    ASSERT_EQ(indices.size(), 0U);
}

TEST_F(AnalyzerDdlTest, CreateTable_default_value) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type(), f.Literal(t::Int(64U), v::Int(1024))),
        }));
    success();


    auto&& table = extract_relation(stmt.get())->destination_table();
    ASSERT_TRUE(table);
    EXPECT_EQ(table.name(), "table");

    auto&& columns = table.columns();
    ASSERT_EQ(columns.size(), 1U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(*c.type(), t::Int(32U, NULLABLE));
        EXPECT_EQ(*c.default_value(), v::Int(1024));
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


    auto&& table = extract_relation(stmt.get())->destination_table();
    ASSERT_TRUE(table);
    EXPECT_EQ(table.name(), "table");

    auto&& columns = table.columns();
    ASSERT_EQ(columns.size(), 1U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(*c.type(), t::Int(32U, NON_NULL));
        EXPECT_EQ(*c.default_value(), v::Int(1024));
    }

    EXPECT_FALSE(table.primary_index().is_valid());

    auto&& indices = table.secondary_indices();
    ASSERT_EQ(indices.size(), 0U);
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

    auto&& table = extract_relation(stmt.get())->destination_table();
    ASSERT_TRUE(table);
    EXPECT_EQ(table.name(), "table");

    auto&& columns = table.columns();
    ASSERT_EQ(columns.size(), 1U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(*c.type(), t::Int(32U, NON_NULL));
        EXPECT_EQ(*c.default_value(), v::Int(1024));
    }

    auto&& pk = table.primary_index().columns();
    EXPECT_TRUE(table.primary_index().is_primary());
    ASSERT_EQ(pk.size(), 1U);
    {
        auto&& c = pk[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(c.direction(), common::core::Direction::ASCENDANT);
    }

    auto&& indices = table.secondary_indices();
    ASSERT_EQ(indices.size(), 0U);
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
                    CreateTableStatement::Column::Attribute::NOT_NULL,
                }),
        },
        {},
        {
            f.CreateTableStatementPrimaryKey(f.Name("C1")),
        }));
    success();

    auto&& table = extract_relation(stmt.get())->destination_table();
    ASSERT_TRUE(table);
    EXPECT_EQ(table.name(), "table");

    auto&& columns = table.columns();
    ASSERT_EQ(columns.size(), 1U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(*c.type(), t::Int(32U, NON_NULL));
        EXPECT_EQ(c.default_value(), nullptr);
    }

    auto&& pk = table.primary_index().columns();
    EXPECT_TRUE(table.primary_index().is_primary());
    ASSERT_EQ(pk.size(), 1U);
    {
        auto&& c = pk[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(c.direction(), common::core::Direction::ASCENDANT);
    }

    auto&& indices = table.secondary_indices();
    ASSERT_EQ(indices.size(), 0U);
}

TEST_F(AnalyzerDdlTest, CreateTable_column_primary_key_multiple) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type()),
            f.CreateTableStatementColumn(f.Name("C2"), f.Int32Type()),
            f.CreateTableStatementColumn(f.Name("C3"), f.Int32Type()),
        },
        {},
        {
            f.CreateTableStatementPrimaryKey(f.Name("C3"), CreateTableStatement::PrimaryKey::Direction::ASCENDANT),
            f.CreateTableStatementPrimaryKey(f.Name("C2"), CreateTableStatement::PrimaryKey::Direction::DESCENDANT),
        }));
    success();

    auto&& table = extract_relation(stmt.get())->destination_table();
    ASSERT_TRUE(table);
    EXPECT_EQ(table.name(), "table");

    auto&& columns = table.columns();
    ASSERT_EQ(columns.size(), 3U);
    {
        auto&& c = columns[0];
        EXPECT_EQ(c.name(), "C1");
        EXPECT_EQ(*c.type(), t::Int(32U, NULLABLE));
    }
    {
        auto&& c = columns[1];
        EXPECT_EQ(c.name(), "C2");
        EXPECT_EQ(*c.type(), t::Int(32U, NULLABLE));
    }
    {
        auto&& c = columns[2];
        EXPECT_EQ(c.name(), "C3");
        EXPECT_EQ(*c.type(), t::Int(32U, NULLABLE));
    }

    auto&& pk = table.primary_index().columns();
    EXPECT_TRUE(table.primary_index().is_primary());
    ASSERT_EQ(pk.size(), 2U);
    {
        auto&& c = pk[0];
        EXPECT_EQ(c.name(), "C3");
        EXPECT_EQ(c.direction(), common::core::Direction::ASCENDANT);
    }
    {
        auto&& c = pk[1];
        EXPECT_EQ(c.name(), "C2");
        EXPECT_EQ(c.direction(), common::core::Direction::DESCENDANT);
    }

    auto&& indices = table.secondary_indices();
    ASSERT_EQ(indices.size(), 0U);
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

TEST_F(AnalyzerDdlTest, CreateTable_duplicate_primary_key_multiple) {
    auto stmt = analyze(f.CreateTableStatement(
        f.Name("table"),
        {
            f.CreateTableStatementColumn(f.Name("C1"), f.Int32Type()),
        },
        {},
        {
            f.CreateTableStatementPrimaryKey(f.Name("C1")),
            f.CreateTableStatementPrimaryKey(f.Name("C1")),
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
