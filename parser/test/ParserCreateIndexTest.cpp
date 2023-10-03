/*
 * Copyright 2018-2023 Project Tsurugi.
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
using namespace shakujo::model::statement::ddl;
namespace v = shakujo::common::core::value;

using common::util::equals;

class ParserCreateIndexTest : public ParserTestBase, public ::testing::Test {
public:
    static constexpr std::size_t dont_care = model::type::VarCharType::dont_care;

    std::unique_ptr<CreateIndexStatement> parse(const std::string& text) {
        return parse_program_main<CreateIndexStatement>(text);
    }

    template<typename T>
    typename T::type get(Expression const* expr) {
        return value_of<T>(expr);
    }
};

template<typename C, typename E>
static bool contains(const C& container, const E& element) {
    return container.find(element) != container.end();
}

TEST_F(ParserCreateIndexTest, simple) {
    auto stmt = parse("CREATE INDEX ON t (C1)");

    EXPECT_FALSE(stmt->index());
    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_EQ(CreateIndexStatement::Column::Direction::DONT_CARE, col->direction());
    }

    auto& attrs = stmt->attributes();
    ASSERT_EQ(0U, attrs.size());
}

TEST_F(ParserCreateIndexTest, with_name) {
    auto stmt = parse("CREATE INDEX i ON t (C1)");

    EXPECT_TRUE(equals(f.Name("i"), stmt->index()));
    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));
}

TEST_F(ParserCreateIndexTest, columns) {
    auto stmt = parse("CREATE INDEX ON t (C1, C2, C3)");

    EXPECT_FALSE(stmt->index());
    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(3U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_EQ(CreateIndexStatement::Column::Direction::DONT_CARE, col->direction());
    }
    {
        auto col = cols[1];
        EXPECT_TRUE(equals(f.Name("C2"), col->name()));
        EXPECT_EQ(CreateIndexStatement::Column::Direction::DONT_CARE, col->direction());
    }
    {
        auto col = cols[2];
        EXPECT_TRUE(equals(f.Name("C3"), col->name()));
        EXPECT_EQ(CreateIndexStatement::Column::Direction::DONT_CARE, col->direction());
    }
}

TEST_F(ParserCreateIndexTest, columns_direction) {
    auto stmt = parse("CREATE INDEX ON t (C1 ASC, C2 DESC)");

    EXPECT_FALSE(stmt->index());
    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(2U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_EQ(CreateIndexStatement::Column::Direction::ASCENDANT, col->direction());
    }
    {
        auto col = cols[1];
        EXPECT_TRUE(equals(f.Name("C2"), col->name()));
        EXPECT_EQ(CreateIndexStatement::Column::Direction::DESCENDANT, col->direction());
    }
}

TEST_F(ParserCreateIndexTest, if_not_exists) {
    auto stmt = parse("CREATE INDEX IF NOT EXISTS i ON t (C1)");

    EXPECT_TRUE(equals(f.Name("i"), stmt->index()));
    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& attrs = stmt->attributes();
    ASSERT_EQ(1U, attrs.size());
    EXPECT_TRUE(contains(attrs, CreateIndexStatement::Attribute::IF_NOT_EXISTS));
}

}  // namespace shakujo::parser
