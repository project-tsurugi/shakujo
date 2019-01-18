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
#include "ParserTestBase.h"

#include <gtest/gtest.h>
#include <stdexcept>

#include "shakujo/model/IRFactory.h"

namespace shakujo::parser {

using namespace shakujo::model::expression;
using namespace shakujo::model::expression::relation;
using namespace shakujo::model::statement;
using namespace shakujo::model::statement::dml;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

class ParserInsertTest : public ParserTestBase, public ::testing::Test {
public:
    std::unique_ptr<InsertValuesStatement> parse_insert_values(const std::string& text) {
        return parse_program_main<InsertValuesStatement>(text, "must be a select statement");
    }
};

TEST_F(ParserInsertTest, insert_values_simple) {
    auto stmt = parse_insert_values("INSERT INTO TBL VALUES (1)");

    EXPECT_TRUE(eq(f.Name("TBL"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto c = cols[0];
        EXPECT_FALSE(c->name());
        EXPECT_EQ(v::Int(1), *cast_node(c->value()).to<Literal>()->value());
    }
}

TEST_F(ParserInsertTest, insert_values_multiple_values) {
    auto stmt = parse_insert_values("INSERT INTO TBL VALUES (1, 2, 3)");

    auto& cols = stmt->columns();
    ASSERT_EQ(3U, cols.size());
    {
        auto c = cols[0];
        EXPECT_FALSE(c->name());
        EXPECT_EQ(v::Int(1), *cast_node(c->value()).to<Literal>()->value());
    }
    {
        auto c = cols[1];
        EXPECT_FALSE(c->name());
        EXPECT_EQ(v::Int(2), *cast_node(c->value()).to<Literal>()->value());
    }
    {
        auto c = cols[2];
        EXPECT_FALSE(c->name());
        EXPECT_EQ(v::Int(3), *cast_node(c->value()).to<Literal>()->value());
    }
}

TEST_F(ParserInsertTest, insert_values_columns) {
    auto stmt = parse_insert_values("INSERT INTO TBL (C1) VALUES (1)");

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto c = cols[0];
        EXPECT_TRUE(eq(f.SimpleName("C1"), c->name()));
        EXPECT_EQ(v::Int(1), *cast_node(c->value()).to<Literal>()->value());
    }
}

TEST_F(ParserInsertTest, insert_values_multiple_columns) {
    auto stmt = parse_insert_values("INSERT INTO TBL (C1, C2, C3) VALUES (1, 2, 3)");

    auto& cols = stmt->columns();
    ASSERT_EQ(3U, cols.size());
    {
        auto c = cols[0];
        EXPECT_TRUE(eq(f.SimpleName("C1"), c->name()));
        EXPECT_EQ(v::Int(1), *cast_node(c->value()).to<Literal>()->value());
    }
    {
        auto c = cols[1];
        EXPECT_TRUE(eq(f.SimpleName("C2"), c->name()));
        EXPECT_EQ(v::Int(2), *cast_node(c->value()).to<Literal>()->value());
    }
    {
        auto c = cols[2];
        EXPECT_TRUE(eq(f.SimpleName("C3"), c->name()));
        EXPECT_EQ(v::Int(3), *cast_node(c->value()).to<Literal>()->value());
    }
}
}  // namespace shakujo::parser
