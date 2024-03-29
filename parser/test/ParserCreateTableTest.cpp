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

class ParserCreateTableTest : public ParserTestBase, public ::testing::Test {
public:
    static constexpr std::size_t dont_care = model::type::VarCharType::dont_care;

    std::unique_ptr<CreateTableStatement> parse(const std::string& text) {
        return parse_program_main<CreateTableStatement>(text);
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

TEST_F(ParserCreateTableTest, simple) {
    auto stmt = parse("CREATE TABLE t (C1 INT)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_TRUE(equals(f.Int32Type(), col->type()));
        EXPECT_FALSE(col->value());
        EXPECT_EQ(0U, col->attributes().size());
    }

    auto& attrs = stmt->attributes();
    ASSERT_EQ(0U, attrs.size());

    auto& pks = stmt->primary_keys();
    ASSERT_EQ(0U, pks.size());
}

TEST_F(ParserCreateTableTest, multiple_columns) {
    auto stmt = parse("CREATE TABLE t (C1 INT, C2 CHAR(10), C3 FLOAT)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(3U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_TRUE(equals(f.Int32Type(), col->type()));
    }
    {
        auto col = cols[1];
        EXPECT_TRUE(equals(f.Name("C2"), col->name()));
        EXPECT_TRUE(equals(f.CharType(10), col->type()));
    }
    {
        auto col = cols[2];
        EXPECT_TRUE(equals(f.Name("C3"), col->name()));
        EXPECT_TRUE(equals(f.Float32Type(), col->type()));
    }
}

TEST_F(ParserCreateTableTest, option_if_not_exists) {
    auto stmt = parse("CREATE TABLE IF NOT EXISTS t (C1 INT)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& attrs = stmt->attributes();
    ASSERT_EQ(1U, attrs.size());
    EXPECT_TRUE(contains(attrs, model::statement::ddl::CreateTableStatement::Attribute::IF_NOT_EXISTS));
}

TEST_F(ParserCreateTableTest, column_default) {
    auto stmt = parse("CREATE TABLE t (C1 INT DEFAULT 1024)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_TRUE(equals(f.Int32Type(), col->type()));
        EXPECT_EQ(1024, get<v::Int>(col->value()));
    }
}

TEST_F(ParserCreateTableTest, column_not_null) {
    auto stmt = parse("CREATE TABLE t (C1 INT NOT NULL)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_TRUE(equals(f.Int32Type(), col->type()));
        EXPECT_FALSE(col->value());
        auto& as = col->attributes();
        EXPECT_TRUE(contains(as, CreateTableStatement::Column::Attribute::NOT_NULL));
    }
}

TEST_F(ParserCreateTableTest, column_null) {
    auto stmt = parse("CREATE TABLE t (C1 INT NULL)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_TRUE(equals(f.Int32Type(), col->type()));
        EXPECT_FALSE(col->value());
        auto& as = col->attributes();
        EXPECT_FALSE(contains(as, CreateTableStatement::Column::Attribute::NOT_NULL));
    }
}

TEST_F(ParserCreateTableTest, column_primary_key) {
    auto stmt = parse("CREATE TABLE t (C1 INT PRIMARY KEY)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    {
        auto col = cols[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_TRUE(equals(f.Int32Type(), col->type()));
        EXPECT_FALSE(col->value());
        auto& as = col->attributes();
        EXPECT_TRUE(contains(as, CreateTableStatement::Column::Attribute::PRIMARY_KEY));
    }
}

TEST_F(ParserCreateTableTest, table_primary_key) {
    auto stmt = parse("CREATE TABLE t (C1 INT, PRIMARY KEY(C1))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& pks = stmt->primary_keys();
    ASSERT_EQ(1U, pks.size());
    {
        auto col = pks[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_EQ(CreateTableStatement::PrimaryKey::Direction::DONT_CARE, col->direction());
    }
}

TEST_F(ParserCreateTableTest, table_primary_key_asc) {
    auto stmt = parse("CREATE TABLE t (C1 INT, PRIMARY KEY(C1 ASC))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& pks = stmt->primary_keys();
    ASSERT_EQ(1U, pks.size());
    {
        auto col = pks[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_EQ(CreateTableStatement::PrimaryKey::Direction::ASCENDANT, col->direction());
    }
}

TEST_F(ParserCreateTableTest, table_primary_key_desc) {
    auto stmt = parse("CREATE TABLE t (C1 INT, PRIMARY KEY(C1 DESC))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& pks = stmt->primary_keys();
    ASSERT_EQ(1U, pks.size());
    {
        auto col = pks[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
        EXPECT_EQ(CreateTableStatement::PrimaryKey::Direction::DESCENDANT, col->direction());
    }
}

TEST_F(ParserCreateTableTest, table_primary_key_multiple) {
    auto stmt = parse("CREATE TABLE t (C1 INT, C2 BIGINT, C3 STRING, PRIMARY KEY(C1, C2, C3))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& pks = stmt->primary_keys();
    ASSERT_EQ(3U, pks.size());
    {
        auto col = pks[0];
        EXPECT_TRUE(equals(f.Name("C1"), col->name()));
    }
    {
        auto col = pks[1];
        EXPECT_TRUE(equals(f.Name("C2"), col->name()));
    }
    {
        auto col = pks[2];
        EXPECT_TRUE(equals(f.Name("C3"), col->name()));
    }
}

TEST_F(ParserCreateTableTest, type_int) {
    auto stmt = parse("CREATE TABLE t (C1 INT)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Int32Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_int_32) {
    auto stmt = parse("CREATE TABLE t (C1 INT(32))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Int32Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_int_64) {
    auto stmt = parse("CREATE TABLE t (C1 INT(64))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Int64Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_bigint) {
    auto stmt = parse("CREATE TABLE t (C1 BIGINT)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Int64Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_real) {
    auto stmt = parse("CREATE TABLE t (C1 REAL)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Float32Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_float) {
    auto stmt = parse("CREATE TABLE t (C1 FLOAT)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Float32Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_float32) {
    auto stmt = parse("CREATE TABLE t (C1 FLOAT(32))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Float32Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_float64) {
    auto stmt = parse("CREATE TABLE t (C1 FLOAT(64))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Float64Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_double) {
    auto stmt = parse("CREATE TABLE t (C1 DOUBLE)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Float64Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_double_precision) {
    auto stmt = parse("CREATE TABLE t (C1 DOUBLE PRECISION)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.Float64Type(), col->type()));
}

TEST_F(ParserCreateTableTest, type_decimal) {
    auto stmt = parse("CREATE TABLE t (C1 DECIMAL)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.DecimalType(), col->type()));
}

TEST_F(ParserCreateTableTest, type_decimal_precision) {
    auto stmt = parse("CREATE TABLE t (C1 DECIMAL(10))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.DecimalType(10), col->type()));
}

TEST_F(ParserCreateTableTest, type_decimal_scale) {
    auto stmt = parse("CREATE TABLE t (C1 DECIMAL(10, 2))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.DecimalType(10, 2), col->type())) << *col->type();
}

TEST_F(ParserCreateTableTest, type_decimal_asterisk) {
    auto stmt = parse("CREATE TABLE t (C1 DECIMAL(*, *))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.DecimalType(dont_care, dont_care), col->type())) << *col->type();
}

TEST_F(ParserCreateTableTest, type_char) {
    auto stmt = parse("CREATE TABLE t (C1 CHAR)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.CharType(1), col->type()));
}

TEST_F(ParserCreateTableTest, type_char_size) {
    auto stmt = parse("CREATE TABLE t (C1 CHAR(10))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.CharType(10), col->type()));
}

TEST_F(ParserCreateTableTest, type_varchar) {
    auto stmt = parse("CREATE TABLE t (C1 VARCHAR(10))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.VarCharType(10), col->type()));
}

TEST_F(ParserCreateTableTest, type_varchar_asterisk) {
    auto stmt = parse("CREATE TABLE t (C1 VARCHAR(*))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.VarCharType(dont_care), col->type()));
}

TEST_F(ParserCreateTableTest, type_string) {
    auto stmt = parse("CREATE TABLE t (C1 STRING)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.StringType(), col->type()));
}

TEST_F(ParserCreateTableTest, type_binary) {
    auto stmt = parse("CREATE TABLE t (C1 BINARY)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.BinaryType(1), col->type()));
}

TEST_F(ParserCreateTableTest, type_binary_size) {
    auto stmt = parse("CREATE TABLE t (C1 BINARY(10))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.BinaryType(10), col->type()));
}

TEST_F(ParserCreateTableTest, type_varbinary) {
    auto stmt = parse("CREATE TABLE t (C1 VARBINARY(10))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.VarBinaryType(10), col->type()));
}

TEST_F(ParserCreateTableTest, type_varbinary_asterisk) {
    auto stmt = parse("CREATE TABLE t (C1 VARBINARY(*))");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.VarBinaryType(dont_care), col->type()));
}

TEST_F(ParserCreateTableTest, type_date) {
    auto stmt = parse("CREATE TABLE t (C1 DATE)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.DateType(), col->type()));
}

TEST_F(ParserCreateTableTest, type_time) {
    auto stmt = parse("CREATE TABLE t (C1 TIME)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.TimeType(), col->type()));
}

TEST_F(ParserCreateTableTest, type_time_with_time_zone) {
    auto stmt = parse("CREATE TABLE t (C1 TIME WITH TIME ZONE)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.TimeType(true), col->type()));
}

TEST_F(ParserCreateTableTest, type_timestamp) {
    auto stmt = parse("CREATE TABLE t (C1 TIMESTAMP)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.TimestampType(), col->type()));
}

TEST_F(ParserCreateTableTest, type_timestamp_with_time_zone) {
    auto stmt = parse("CREATE TABLE t (C1 TIMESTAMP WITH TIME ZONE)");

    EXPECT_TRUE(equals(f.Name("t"), stmt->table()));

    auto& cols = stmt->columns();
    ASSERT_EQ(1U, cols.size());
    auto col = cols[0];
    EXPECT_TRUE(equals(f.TimestampType(true), col->type()));
}
}  // namespace shakujo::parser
