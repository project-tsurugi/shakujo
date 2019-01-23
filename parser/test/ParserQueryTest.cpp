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
#include <shakujo/model/expression/relation/JoinExpression.h>

#include "shakujo/model/IRFactory.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::parser {

using namespace shakujo::model::expression;
using namespace shakujo::model::expression::relation;
using namespace shakujo::model::statement;
using namespace shakujo::model::statement::dml;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using common::util::equals;

class ParserQueryTest : public ParserTestBase, public ::testing::Test {
public:
    std::unique_ptr<EmitStatement> parse_select(std::string const& text) {
        return parse_program_main<EmitStatement>(text, "must be a select statement");
    }

    std::string scan_target(Expression const* expr) {
        auto scan = cast_node(expr).to<ScanExpression const>();
        return common::util::to_string(*scan->table());
    }
};

TEST_F(ParserQueryTest, select_simple) {
    auto select = parse_select("SELECT * FROM TBL");
    auto scan = cast_node(select->source()).to<ScanExpression>();
    EXPECT_TRUE(equals(f.Name("TBL"), scan->table()));
    EXPECT_FALSE(scan->alias());
}

TEST_F(ParserQueryTest, select_alias) {
    auto select = parse_select("SELECT * FROM TBL A");
    auto scan = cast_node(select->source()).to<ScanExpression>();
    EXPECT_TRUE(equals(f.Name("TBL"), scan->table()));
    EXPECT_TRUE(equals(f.Name("A"), scan->alias()));
}

TEST_F(ParserQueryTest, select_as_alias) {
    auto select = parse_select("SELECT * FROM TBL AS A");
    auto scan = cast_node(select->source()).to<ScanExpression>();
    EXPECT_TRUE(equals(f.Name("TBL"), scan->table()));
    EXPECT_TRUE(equals(f.Name("A"), scan->alias()));
}

TEST_F(ParserQueryTest, select_condition) {
    auto select = parse_select("SELECT * FROM TBL WHERE TRUE");
    auto selection = cast_node(select->source()).to<SelectionExpression>();
    auto condition = cast_node(selection->condition()).to<Literal>();
    EXPECT_EQ(t::Bool(NON_NULL), *condition->type());
    EXPECT_EQ(v::Bool(true), *condition->value());
}

TEST_F(ParserQueryTest, select_projection) {
    auto select = parse_select("SELECT C1 FROM TBL");
    auto projection = cast_node(select->source()).to<ProjectionExpression>();
    ASSERT_EQ(1U, projection->columns().size());

    auto c1 = projection->columns()[0];
    EXPECT_FALSE(c1->alias());
    auto c1v = cast_node(c1->value()).to<VariableReference>();
    EXPECT_TRUE(equals(f.Name("C1"), c1v->name()));
}

TEST_F(ParserQueryTest, select_projection_alias) {
    auto select = parse_select("SELECT C1 AS x FROM TBL");
    auto projection = cast_node(select->source()).to<ProjectionExpression>();
    ASSERT_EQ(1U, projection->columns().size());

    auto c1 = projection->columns()[0];
    EXPECT_TRUE(equals(f.Name("x"), c1->alias()));
    auto c1v = cast_node(c1->value()).to<VariableReference>();
    EXPECT_TRUE(equals(f.Name("C1"), c1v->name()));
}

TEST_F(ParserQueryTest, select_projection_many) {
    auto select = parse_select("SELECT 1, 2, 3, 4, 5 FROM TBL");
    auto projection = cast_node(select->source()).to<ProjectionExpression>();
    ASSERT_EQ(5U, projection->columns().size());

    auto c1 = projection->columns()[0];
    auto c2 = projection->columns()[1];
    auto c3 = projection->columns()[2];
    auto c4 = projection->columns()[3];
    auto c5 = projection->columns()[4];

    EXPECT_EQ(v::Int(1), *cast_node(c1->value()).to<Literal>()->value());
    EXPECT_EQ(v::Int(2), *cast_node(c2->value()).to<Literal>()->value());
    EXPECT_EQ(v::Int(3), *cast_node(c3->value()).to<Literal>()->value());
    EXPECT_EQ(v::Int(4), *cast_node(c4->value()).to<Literal>()->value());
    EXPECT_EQ(v::Int(5), *cast_node(c5->value()).to<Literal>()->value());
}

TEST_F(ParserQueryTest, join) {
    auto select = parse_select("SELECT * FROM A JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::INNER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_inner) {
    auto select = parse_select("SELECT * FROM A INNER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::INNER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_left) {
    auto select = parse_select("SELECT * FROM A LEFT OUTER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::LEFT_OUTER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_right) {
    auto select = parse_select("SELECT * FROM A RIGHT OUTER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::RIGHT_OUTER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_full) {
    auto select = parse_select("SELECT * FROM A FULL OUTER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::FULL_OUTER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_natural) {
    auto select = parse_select("SELECT * FROM A NATURAL JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::NATURAL_INNER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_natural_inner) {
    auto select = parse_select("SELECT * FROM A NATURAL INNER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::NATURAL_INNER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_natural_left) {
    auto select = parse_select("SELECT * FROM A NATURAL LEFT OUTER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::NATURAL_LEFT_OUTER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_natural_right) {
    auto select = parse_select("SELECT * FROM A NATURAL RIGHT OUTER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::NATURAL_RIGHT_OUTER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_natural_full) {
    auto select = parse_select("SELECT * FROM A NATURAL FULL OUTER JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::NATURAL_FULL_OUTER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_cross) {
    auto select = parse_select("SELECT * FROM A CROSS JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::CROSS, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_union) {
    auto select = parse_select("SELECT * FROM A UNION JOIN B");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::UNION_OUTER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));
    EXPECT_FALSE(join->condition());
}

TEST_F(ParserQueryTest, join_condition) {
    auto select = parse_select("SELECT * FROM A JOIN B ON TRUE");
    auto join = cast_node(select->source()).to<JoinExpression>();

    EXPECT_EQ(JoinExpression::Kind::INNER, join->operator_kind());
    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(join->right()));

    auto condition = cast_node(join->condition()).to<Literal>();
    EXPECT_EQ(v::Bool(true), *condition->value());
}

TEST_F(ParserQueryTest, join_multiple) {
    /*
     * from (A join B) left join C
     */
    auto select = parse_select("SELECT * FROM A JOIN B LEFT JOIN C");
    auto join = cast_node(select->source()).to<JoinExpression>();
    EXPECT_EQ(JoinExpression::Kind::LEFT_OUTER, join->operator_kind());
    EXPECT_FALSE(join->condition());

    auto nest = cast_node(join->left()).to<JoinExpression>();
    EXPECT_EQ(JoinExpression::Kind::INNER, nest->operator_kind());
    EXPECT_FALSE(nest->condition());

    EXPECT_EQ("A", scan_target(nest->left()));
    EXPECT_EQ("B", scan_target(nest->right()));
    EXPECT_EQ("C", scan_target(join->right()));
}

TEST_F(ParserQueryTest, join_parenthesized) {
    /*
     * from (A join B) left join C
     */
    auto select = parse_select("SELECT * FROM A JOIN (B LEFT JOIN C)");
    auto join = cast_node(select->source()).to<JoinExpression>();
    EXPECT_EQ(JoinExpression::Kind::INNER, join->operator_kind());
    EXPECT_FALSE(join->condition());

    auto nest = cast_node(join->right()).to<JoinExpression>();
    EXPECT_EQ(JoinExpression::Kind::LEFT_OUTER, nest->operator_kind());
    EXPECT_FALSE(nest->condition());

    EXPECT_EQ("A", scan_target(join->left()));
    EXPECT_EQ("B", scan_target(nest->left()));
    EXPECT_EQ("C", scan_target(nest->right()));
}
}  // namespace shakujo::parser
