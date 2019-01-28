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

namespace shakujo::parser {

using namespace shakujo::model::expression;
using namespace shakujo::model::statement;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using common::util::equals;

class ParserExpressionTest : public ParserTestBase, public ::testing::Test {
public:
    int as_int(Expression const* expr) {
        auto literal = dynamic_cast<Literal const*>(expr);
        auto value = dynamic_cast<v::Int const*>(literal->value());
        return static_cast<int>(value->get());
    }
};

TEST_F(ParserExpressionTest, literal_int) {
    auto v = parse_expression<Literal>("100");
    EXPECT_EQ(t::Int(64U, NON_NULL), *v->type());
    EXPECT_EQ(v::Int(100), *v->value());
}

TEST_F(ParserExpressionTest, literal_float) {
    auto v = parse_expression<Literal>("1.25");
    EXPECT_EQ(t::Float(64U, NON_NULL), *v->type());
    EXPECT_EQ(v::Float(1.25), *v->value());
}

TEST_F(ParserExpressionTest, literal_true) {
    auto v = parse_expression<Literal>("TRUE");
    EXPECT_EQ(t::Bool(NON_NULL), *v->type());
    EXPECT_EQ(v::Bool(true), *v->value());
}

TEST_F(ParserExpressionTest, literal_false) {
    auto v = parse_expression<Literal>("FALSE");
    EXPECT_EQ(t::Bool(NON_NULL), *v->type());
    EXPECT_EQ(v::Bool(false), *v->value());
}

TEST_F(ParserExpressionTest, literal_null) {
    auto v = parse_expression<Literal>("NULL");
    EXPECT_EQ(t::Null(), *v->type());
    EXPECT_EQ(v::Null(), *v->value());
}

TEST_F(ParserExpressionTest, literal_string) {
    auto v = parse_expression<Literal>("'Hello, world!'");
    EXPECT_EQ(t::String(NON_NULL), *v->type());
    EXPECT_EQ(v::String("Hello, world!"), *v->value());
}

TEST_F(ParserExpressionTest, literal_string_empty) {
    auto v = parse_expression<Literal>("''");
    EXPECT_EQ(t::String(NON_NULL), *v->type());
    EXPECT_EQ(v::String(""), *v->value());
}

TEST_F(ParserExpressionTest, literal_string_escape) {
    auto v = parse_expression<Literal>("'\\'-\\\\-\\\r-\\\n-\\\t'");
    EXPECT_EQ(t::String(NON_NULL), *v->type());
    EXPECT_EQ(v::String("'-\\-\r-\n-\t"), *v->value());
}

TEST_F(ParserExpressionTest, simple_name) {
    auto v = parse_expression<VariableReference>("a");
    EXPECT_TRUE(equals(f.Name("a"), v->name()));
}

TEST_F(ParserExpressionTest, qualified_name) {
    auto v = parse_expression<VariableReference>("a.b.c");
    EXPECT_TRUE(equals(f.Name("a", "b", "c"), v->name()));
}

TEST_F(ParserExpressionTest, function_call) {
    auto v = parse_expression<FunctionCall>("f(100)");
    EXPECT_TRUE(equals(f.Name("f"), v->name()));

    auto& args = v->arguments();
    ASSERT_EQ(1U, args.size());
    EXPECT_EQ(100, as_int(args[0]));
}

TEST_F(ParserExpressionTest, function_call_empty_args) {
    auto v = parse_expression<FunctionCall>("f()");
    EXPECT_TRUE(equals(f.Name("f"), v->name()));

    auto& args = v->arguments();
    ASSERT_EQ(0U, args.size());
}

TEST_F(ParserExpressionTest, function_call_many_args) {
    auto v = parse_expression<FunctionCall>("f(1, 2, 3)");
    EXPECT_TRUE(equals(f.Name("f"), v->name()));

    auto& args = v->arguments();
    ASSERT_EQ(3U, args.size());
    EXPECT_EQ(1, as_int(args[0]));
    EXPECT_EQ(2, as_int(args[1]));
    EXPECT_EQ(3, as_int(args[2]));
}

TEST_F(ParserExpressionTest, unary_plus) {
    auto v = parse_expression<UnaryOperator>("+1");
    EXPECT_EQ(UnaryOperator::Kind::PLUS, v->operator_kind());
    EXPECT_EQ(1, as_int(v->operand()));
}

TEST_F(ParserExpressionTest, unary_minus) {
    auto v = parse_expression<UnaryOperator>("-1");
    EXPECT_EQ(UnaryOperator::Kind::SIGN_INVERSION, v->operator_kind());
    EXPECT_EQ(1, as_int(v->operand()));
}

TEST_F(ParserExpressionTest, unary_complement) {
    auto v = parse_expression<UnaryOperator>("~1");
    EXPECT_EQ(UnaryOperator::Kind::BITWISE_COMPLEMENT, v->operator_kind());
    EXPECT_EQ(1, as_int(v->operand()));
}

TEST_F(ParserExpressionTest, multiplicative_multiply) {
    auto v = parse_expression<BinaryOperator>("1 * 2");
    EXPECT_EQ(BinaryOperator::Kind::MULTIPLY, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, multiplicative_divide) {
    auto v = parse_expression<BinaryOperator>("1 / 2");
    EXPECT_EQ(BinaryOperator::Kind::DIVIDE, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, multiplicative_remainder) {
    auto v = parse_expression<BinaryOperator>("1 % 2");
    EXPECT_EQ(BinaryOperator::Kind::REMAINDER, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, additive_add) {
    auto v = parse_expression<BinaryOperator>("1 + 2");
    EXPECT_EQ(BinaryOperator::Kind::ADD, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, additive_subtract) {
    auto v = parse_expression<BinaryOperator>("1 - 2");
    EXPECT_EQ(BinaryOperator::Kind::SUBTRACT, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, shift_left) {
    auto v = parse_expression<BinaryOperator>("1 << 2");
    EXPECT_EQ(BinaryOperator::Kind::SHIFT_LEFT, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, shift_right_arithmetic) {
    auto v = parse_expression<BinaryOperator>("1 >> 2");
    EXPECT_EQ(BinaryOperator::Kind::SHIFT_RIGHT_ARITHMETIC, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, shift_right_logical) {
    auto v = parse_expression<BinaryOperator>("1 >>> 2");
    EXPECT_EQ(BinaryOperator::Kind::SHIFT_RIGHT_LOGICAL, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, relational_less) {
    auto v = parse_expression<BinaryOperator>("1 < 2");
    EXPECT_EQ(BinaryOperator::Kind::LESS_THAN, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, relational_greater) {
    auto v = parse_expression<BinaryOperator>("1 > 2");
    EXPECT_EQ(BinaryOperator::Kind::GREATER_THAN, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, relational_less_eq) {
    auto v = parse_expression<BinaryOperator>("1 <= 2");
    EXPECT_EQ(BinaryOperator::Kind::LESS_THAN_OR_EQUAL, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, relational_greater_eq) {
    auto v = parse_expression<BinaryOperator>("1 >= 2");
    EXPECT_EQ(BinaryOperator::Kind::GREATER_THAN_OR_EQUAL, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, equality_equal) {
    auto v = parse_expression<BinaryOperator>("1 = 2");
    EXPECT_EQ(BinaryOperator::Kind::EQUAL, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, equality_not_equal) {
    auto v = parse_expression<BinaryOperator>("1 <> 2");
    EXPECT_EQ(BinaryOperator::Kind::NOT_EQUAL, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, logical_and) {
    auto v = parse_expression<BinaryOperator>("1 & 2");
    EXPECT_EQ(BinaryOperator::Kind::LOGICAL_AND, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, logical_or) {
    auto v = parse_expression<BinaryOperator>("1 | 2");
    EXPECT_EQ(BinaryOperator::Kind::LOGICAL_OR, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}

TEST_F(ParserExpressionTest, logical_xor) {
    auto v = parse_expression<BinaryOperator>("1 ^ 2");
    EXPECT_EQ(BinaryOperator::Kind::LOGICAL_XOR, v->operator_kind());
    EXPECT_EQ(1, as_int(v->left()));
    EXPECT_EQ(2, as_int(v->right()));
}
}  // namespace shakujo::parser
