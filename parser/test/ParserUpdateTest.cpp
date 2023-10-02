/*
 * Copyright 2018-2023 tsurugi project..
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

using common::util::dynamic_pointer_cast;
using common::util::equals;

class ParserUpdateTest : public ParserTestBase, public ::testing::Test {
public:
    std::unique_ptr<UpdateStatement> parse(std::string_view text) {
        std::string s { text };
        return parse_program_main<UpdateStatement>(s);
    }
};

TEST_F(ParserUpdateTest, simple) {
    auto stmt = parse("UPDATE TBL SET a = 1");

    EXPECT_TRUE(equals(f.Name("TBL"), stmt->table()));

    auto&& columns = stmt->columns();
    ASSERT_EQ(columns.size(), 1U);
    {
        auto&& c = columns[0];
        EXPECT_TRUE(equals(c->name(), f.Name("a")));
        EXPECT_EQ(value_of<v::Int>(c->value()), 1);
    }

    auto* source = dynamic_pointer_cast<ScanExpression>(stmt->source());
    EXPECT_TRUE(equals(stmt->table(), source->table()));
}

TEST_F(ParserUpdateTest, multiple_set) {
    auto stmt = parse("UPDATE TBL SET a = 1, b = 2, c = 3");

    EXPECT_TRUE(equals(f.Name("TBL"), stmt->table()));

    auto&& columns = stmt->columns();
    ASSERT_EQ(columns.size(), 3U);
    {
        auto&& c = columns[0];
        EXPECT_TRUE(equals(c->name(), f.Name("a")));
        EXPECT_EQ(value_of<v::Int>(c->value()), 1);
    }
    {
        auto&& c = columns[1];
        EXPECT_TRUE(equals(c->name(), f.Name("b")));
        EXPECT_EQ(value_of<v::Int>(c->value()), 2);
    }
    {
        auto&& c = columns[2];
        EXPECT_TRUE(equals(c->name(), f.Name("c")));
        EXPECT_EQ(value_of<v::Int>(c->value()), 3);
    }

    auto* source = dynamic_pointer_cast<ScanExpression>(stmt->source());
    EXPECT_TRUE(equals(stmt->table(), source->table()));
}

TEST_F(ParserUpdateTest, conditional) {
    auto stmt = parse("UPDATE TBL SET a = 1 WHERE 1");

    EXPECT_TRUE(equals(f.Name("TBL"), stmt->table()));

    auto&& columns = stmt->columns();
    ASSERT_EQ(columns.size(), 1U);
    {
        auto&& c = columns[0];
        EXPECT_TRUE(equals(c->name(), f.Name("a")));
        EXPECT_EQ(value_of<v::Int>(c->value()), 1);
    }

    auto* source = dynamic_pointer_cast<SelectionExpression>(stmt->source());
    auto* condition = dynamic_pointer_cast<Literal>(source->condition());
    EXPECT_EQ(v::Int(1), *condition->value());

    auto* scan = dynamic_pointer_cast<ScanExpression>(source->operand());
    EXPECT_TRUE(equals(stmt->table(), scan->table()));
}
}  // namespace shakujo::parser
