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
#include "shakujo/common/core/type/Tuple.h"
#include "shakujo/common/core/type/Null.h"
#include "shakujo/common/core/type/Error.h"
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
using common::util::is_valid;

class AnalyzerExpressionTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> analyze(std::unique_ptr<T> expr) {
        return analyze_managed<T, expression::Expression>(std::move(expr));
    }
};

TEST_F(AnalyzerExpressionTest, literal_bool) {
    auto expr = analyze(f.Literal(t::Bool(), true));
    success();

    auto binding = extract(expr.get());
    EXPECT_EQ(t::Bool(), *binding->type());
    EXPECT_EQ(v::Bool(true), *binding->value());
}

TEST_F(AnalyzerExpressionTest, literal_bool_inconsistent) {
    auto expr = analyze(f.Literal(t::Bool(), v::String("TRUE")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_int) {
    auto expr = analyze(f.Literal(t::Int(32U), 100));
    success();

    auto binding = extract(expr.get());
    EXPECT_EQ(t::Int(32U), *binding->type());
    EXPECT_EQ(v::Int(100), *binding->value());
}

TEST_F(AnalyzerExpressionTest, literal_int_too_large) {
    auto expr = analyze(f.Literal(t::Int(32U), 0x100000000LL));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_int_too_small) {
    auto expr = analyze(f.Literal(t::Int(32U), -0x100000000LL));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_int_inconsistent) {
    auto expr = analyze(f.Literal(t::Int(32U), v::Bool(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_float) {
    auto expr = analyze(f.Literal(t::Float(64U), 1.5));
    success();

    auto binding = extract(expr.get());
    EXPECT_EQ(t::Float(64U), *binding->type());
    EXPECT_EQ(v::Float(1.5), *binding->value());
}

TEST_F(AnalyzerExpressionTest, literal_float_inconsistent) {
    auto expr = analyze(f.Literal(t::Float(64U), v::Bool(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_char) {
    auto expr = analyze(f.Literal(t::Char(5U), "HELLO"));
    success();

    auto binding = extract(expr.get());
    EXPECT_EQ(t::Char(5U), *binding->type());
    EXPECT_EQ(v::String("HELLO"), *binding->value());
}

TEST_F(AnalyzerExpressionTest, literal_char_too_long) {
    auto expr = analyze(f.Literal(t::Char(5U), "HELLO!"));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_char_inconsistent) {
    auto expr = analyze(f.Literal(t::Char(5U), v::Bool(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_varchar) {
    auto expr = analyze(f.Literal(t::Char(true, 5U), "HELLO"));
    success();

    auto binding = extract(expr.get());
    EXPECT_EQ(t::Char(true, 5U), *binding->type());
    EXPECT_EQ(v::String("HELLO"), *binding->value());
}

TEST_F(AnalyzerExpressionTest, literal_varchar_too_long) {
    auto expr = analyze(f.Literal(t::Char(true, 5U), "HELLO!"));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_varchar_inconsistent) {
    auto expr = analyze(f.Literal(t::Char(true, 5U), v::Bool(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_string) {
    auto expr = analyze(f.Literal(t::String(), "Hello, world!"));
    success();

    auto binding = extract(expr.get());
    EXPECT_EQ(t::String(), *binding->type());
    EXPECT_EQ(v::String("Hello, world!"), *binding->value());
}

TEST_F(AnalyzerExpressionTest, literal_string_inconsistent) {
    auto expr = analyze(f.Literal(t::String(), v::Bool(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_null) {
    auto expr = analyze(f.Literal(t::Null()));
    success();

    auto binding = extract(expr.get());
    EXPECT_EQ(t::Null(), *binding->type());
    EXPECT_EQ(v::Null(), *binding->value());
}

TEST_F(AnalyzerExpressionTest, literal_null_inconsistent) {
    auto expr = analyze(f.Literal(t::Null(), v::Bool(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, literal_nullable) {
    auto expr = analyze(f.Literal(t::Bool(), v::Null()));
    success(false);
}

TEST_F(AnalyzerExpressionTest, variable_ref_simple) {
    auto& vid = add_variable("a", t::Int(32U, NON_NULL));
    auto expr = analyze(f.VariableReference(f.Name("a")));
    success();

    auto binding = extract(expr.get());
    ASSERT_EQ(t::Int(32U, NON_NULL), *binding->type());

    auto var = extract_var(expr.get());
    ASSERT_EQ(vid, var->id());
}

TEST_F(AnalyzerExpressionTest, variable_ref_qualified) {
    auto& vid = add_variable(f.Name("q", "a"), t::Int(32U, NON_NULL));
    auto expr = analyze(f.VariableReference(f.Name("q", "a")));
    success();

    auto binding = extract(expr.get());
    ASSERT_EQ(t::Int(32U, NON_NULL), *binding->type());

    auto var = extract_var(expr.get());
    ASSERT_EQ(vid, var->id());
}

TEST_F(AnalyzerExpressionTest, variable_ref_partial) {
    t::Tuple tuple_type({
        { "C1", t::Int(32U, NON_NULL) },
    }, NON_NULL);
    auto& vid = add_variable(f.Name("t"), tuple_type);
    auto expr = analyze<expression::Expression>(f.VariableReference(f.Name("t", "C1")));
    success();

    ASSERT_EQ(expression::ExpressionKind::TUPLE_ELEMENT_LOAD_EXPRESSION, expr->kind());
    auto load = dynamic_cast<expression::TupleElementLoadExpression*>(expr.get());
    auto load_binding = extract(load);
    EXPECT_EQ(t::Int(32U, NON_NULL), *load_binding->type());
    EXPECT_EQ(0U, load->index()->position());

    ASSERT_EQ(expression::ExpressionKind::VARIABLE_REFERENCE, load->data()->kind());
    auto* ref = dynamic_cast<expression::VariableReference*>(load->data());
    auto ref_binding = extract(ref);
    auto ref_var = extract_var(ref);
    EXPECT_EQ(tuple_type, *ref_binding->type()) << ref_binding->type();
    EXPECT_EQ(vid, ref_var->id());
}

TEST_F(AnalyzerExpressionTest, variable_ref_ambiguous) {
    auto element = std::make_shared<binding::VariableBinding>();
    auto name = f.Name("a");
    env.variable_scope().insert(name.get(), element);
    auto expr = analyze(f.VariableReference(std::move(name)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, variable_ref_partial_not_tuple) {
    add_variable(f.Name("t"), t::Int(32U, NON_NULL));
    auto expr = analyze<expression::Expression>(f.VariableReference(f.Name("t", "MISSING")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, variable_ref_partial_missing) {
    t::Tuple tuple_type({
            { "C1", t::Int(32U, NON_NULL) },
    }, NON_NULL);
    auto expr = analyze<expression::Expression>(f.VariableReference(f.Name("t", "MISSING")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, variable_ref_missing) {
    auto expr = analyze(f.VariableReference(f.Name("__MISSING__")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, variable_ref_invalid) {
    add_variable(f.Name("invalid"), t::Error());
    auto expr = analyze(f.VariableReference(f.Name("invalid")));
    success();

    auto binding = env.binding_context().get(expr->expression_key());
    ASSERT_FALSE(binding->is_valid());
}

TEST_F(AnalyzerExpressionTest, assign_simple) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NULLABLE));
    auto expr = analyze(f.AssignExpression(f.Name("v"), literal(1, 64U, NULLABLE)));
    success();

    ASSERT_EQ(t::Int(64U, NULLABLE), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->value()));

    auto var = extract_var(expr.get());
    EXPECT_EQ(vid, var->id());
}

TEST_F(AnalyzerExpressionTest, assign_promote) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NULLABLE));
    auto expr = analyze(f.AssignExpression(f.Name("v"), literal(1, 32U, NULLABLE)));
    success();

    ASSERT_EQ(t::Int(64U, NULLABLE), type(expr.get()));
    EXPECT_EQ(t::Int(64U, NULLABLE), cast_type(expr->value()));

    auto var = extract_var(expr.get());
    EXPECT_EQ(vid, var->id());
}

TEST_F(AnalyzerExpressionTest, assign_partial_not_implemented) {
    t::Tuple tuple_type({
            { "C1", t::Int(64U, NON_NULL) },
    }, NON_NULL);
    add_variable(f.Name("t"), tuple_type);
    auto expr = analyze(f.AssignExpression(f.Name("t", "C1"), literal(1, 64U, NON_NULL)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, assign_missing) {
    auto expr = analyze(f.AssignExpression(f.Name("missing"), literal(1)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, assign_ambiguous) {
    auto element = std::make_shared<binding::VariableBinding>();
    env.variable_scope().insert(f.Name("a"), element);
    auto expr = analyze(f.AssignExpression(f.Name("a"), literal(1)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, assign_invalid) {
    add_variable(f.Name("v"), t::Error());
    auto expr = analyze(f.AssignExpression(f.Name("v"), literal(1)));
    // only propagate error
    EXPECT_TRUE(is_propagated_error(expr.get()));
}

TEST_F(AnalyzerExpressionTest, assign_propagate) {
    add_variable(f.Name("v"), t::Int(64U, NULLABLE));
    auto expr = analyze(f.AssignExpression(f.Name("v"), var("missing")));
    success(false);

    EXPECT_TRUE(is_propagated_error(expr.get()));
}

TEST_F(AnalyzerExpressionTest, assign_inconsistent) {
    add_variable(f.Name("v"), t::Int(64U, NULLABLE));
    auto expr = analyze(f.AssignExpression(f.Name("v"), literal("x", NULLABLE)));
    success(false);

    EXPECT_FALSE(is_propagated_error(expr.get()));
}

TEST_F(AnalyzerExpressionTest, unary_op_plus) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::PLUS, literal(0, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
}

TEST_F(AnalyzerExpressionTest, unary_op_plus_inconsistent) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::PLUS, literal(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, unary_op_minus) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::SIGN_INVERSION, literal(0.0, 64U, NON_NULL)));
    success();

    EXPECT_EQ(t::Float(64U, NON_NULL), type(expr.get()));
}

TEST_F(AnalyzerExpressionTest, unary_op_bitwise_not) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::BITWISE_COMPLEMENT, literal(0, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
}

TEST_F(AnalyzerExpressionTest, unary_op_bitwise_not_inconsistent) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::BITWISE_COMPLEMENT, literal(true)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, unary_op_logical_not) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::LOGICAL_NOT, literal(true, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
}

TEST_F(AnalyzerExpressionTest, unary_op_logical_not_inconsistent) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::LOGICAL_NOT, literal("1", NON_NULL)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, unary_op_is_bool) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::IS_TRUE, literal(true, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_TRUE(is<expression::Literal>(expr->operand()));
}

TEST_F(AnalyzerExpressionTest, unary_op_is_bool_nullable) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::IS_TRUE, literal(true, NULLABLE)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_TRUE(is<expression::Literal>(expr->operand()));
}

TEST_F(AnalyzerExpressionTest, unary_op_is_bool_inconsistent) {
    using Op = expression::UnaryOperator::Kind;
    add_variable(f.Name("a"), t::Vector(t::Int(32U)));
    auto expr = analyze(f.UnaryOperator(Op::IS_TRUE, var("a")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, unary_op_is_null) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::IS_NULL, literal(1, NULLABLE)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_TRUE(is<expression::Literal>(expr->operand()));
}

TEST_F(AnalyzerExpressionTest, unary_op_is_null_inconsistent) {
    using Op = expression::UnaryOperator::Kind;
    add_variable(f.Name("a"), t::Vector(t::Int(32U)));
    auto expr = analyze(f.UnaryOperator(Op::IS_NULL, var("a")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, unary_op_check_null) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::CHECK_NULL, literal(1, 32U, NULLABLE)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
    EXPECT_TRUE(is<expression::Literal>(expr->operand()));
}

TEST_F(AnalyzerExpressionTest, unary_op_check_null_inconsistent) {
    using Op = expression::UnaryOperator::Kind;
    add_variable(f.Name("a"), t::Vector(t::Int(32U)));
    auto expr = analyze(f.UnaryOperator(Op::CHECK_NULL, var("a")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, unary_op_relational_not_impl) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::RELATION_EXISTS, literal(1)));
    ASSERT_TRUE(contains(Diagnostic::Code::NOT_IMPLEMENTED));
}

TEST_F(AnalyzerExpressionTest, unary_op_propagate) {
    using Op = expression::UnaryOperator::Kind;
    auto expr = analyze(f.UnaryOperator(Op::PLUS, var("missing")));
    success(false);

    EXPECT_TRUE(is_propagated_error(expr.get()));
}

TEST_F(AnalyzerExpressionTest, binary_op_arithmetic) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::ADD,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_arithmetic_numeric_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::ADD,
            literal(1, 32U, NON_NULL),
            literal(2, 64U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(64U, NON_NULL), type(expr.get()));
    EXPECT_EQ(t::Int(64U, NON_NULL), cast_type(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_arithmetic_nullable_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::ADD,
            literal(1, 32U, NULLABLE),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NULLABLE), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(t::Int(32U, NULLABLE), cast_type(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_arithmetic_inconsistent) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::ADD,
            literal(1, 32U, NON_NULL),
            literal("2")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_bitwise) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::BITWISE_AND,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_bitwise_integral_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::BITWISE_AND,
            literal(1, 32U, NON_NULL),
            literal(2, 64U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(64U, NON_NULL), type(expr.get()));
    EXPECT_EQ(t::Int(64U, NON_NULL), cast_type(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_bitwise_nullable_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::BITWISE_AND,
            literal(1, 32U, NULLABLE),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NULLABLE), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(t::Int(32U, NULLABLE), cast_type(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_bitwise_from_logical_and) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LOGICAL_AND,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(Op::BITWISE_AND, expr->operator_kind());
}

TEST_F(AnalyzerExpressionTest, binary_op_bitwise_from_logical_or) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LOGICAL_OR,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(Op::BITWISE_OR, expr->operator_kind());
}

TEST_F(AnalyzerExpressionTest, binary_op_bitwise_from_logical_xor) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LOGICAL_XOR,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(Op::BITWISE_XOR, expr->operator_kind());
}

TEST_F(AnalyzerExpressionTest, binary_op_shift) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::SHIFT_LEFT,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_shift_diff_prec) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::SHIFT_LEFT,
            literal(1, 64U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(64U, NON_NULL), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_shift_nullable_left) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::SHIFT_LEFT,
            literal(1, 32U, NULLABLE),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NULLABLE), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_shift_nullable_right) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::SHIFT_LEFT,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NULLABLE)));
    success();

    EXPECT_EQ(t::Int(32U, NULLABLE), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_shift_inconsistent_left) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::SHIFT_LEFT,
            literal(1, 64U, NON_NULL),
            literal(2., 64U, NON_NULL)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_shift_inconsistent_right) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::SHIFT_LEFT,
            literal(1., 64U, NON_NULL),
            literal(2, 64U, NON_NULL)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_eq) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::EQUAL,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_eq_null_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::EQUAL,
            literal(1, 32U, NULLABLE),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(t::Int(32U, NULLABLE), cast_type(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_eq_numeric_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::EQUAL,
            literal(0, 32U, NON_NULL),
            literal(.0, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_EQ(t::Float(64U, NON_NULL), cast_type(expr->left()));
    EXPECT_EQ(t::Float(64U, NON_NULL), cast_type(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_eq_promotion_failed) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::EQUAL,
            literal(1),
            literal("1")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_eq_inconsistent) {
    using Op = expression::BinaryOperator::Kind;
    add_variable(f.Name("a"), t::Vector(t::Int(32U)));
    auto expr = analyze(f.BinaryOperator(
            Op::EQUAL,
            literal(1),
            var("a")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_cmp) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LESS_THAN,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_EQ(1, get<v::Int>(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_cmp_numeric_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LESS_THAN,
            literal(1, 32U, NON_NULL),
            literal(2, 64U, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_EQ(t::Int(64U, NON_NULL), cast_type(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_cmp_textual_promotion) {
    using Op = expression::BinaryOperator::Kind;
    add_variable(f.Name("a"), t::Char(10U, NON_NULL));
    add_variable(f.Name("b"), t::Char(20U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(
            Op::LESS_THAN,
            var("a"),
            var("b")));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_EQ(t::String(NON_NULL), cast_type(expr->left()));
    EXPECT_EQ(t::String(NON_NULL), cast_type(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_cmp_nullable_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LESS_THAN,
            literal(1, 32U, NON_NULL),
            literal(2, 32U, NULLABLE)));
    success();

    EXPECT_EQ(t::Bool(NULLABLE), type(expr.get()));
    EXPECT_EQ(t::Int(32U, NULLABLE), cast_type(expr->left()));
    EXPECT_EQ(2, get<v::Int>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_cmp_promotion_failed) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LESS_THAN,
            literal(1, 32U, NON_NULL),
            literal("2", NON_NULL)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_cmp_inconsistent) {
    using Op = expression::BinaryOperator::Kind;
    add_variable(f.Name("a"), t::Vector(t::Int(32U)));
    auto expr = analyze(f.BinaryOperator(
            Op::LESS_THAN,
            literal(1),
            var("a")));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_logical) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LOGICAL_AND,
            literal(true, NON_NULL),
            literal(false, NON_NULL)));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(expr.get()));
    EXPECT_EQ(true, get<v::Bool>(expr->left()));
    EXPECT_EQ(false, get<v::Bool>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_logical_nullable_promotion) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LOGICAL_AND,
            literal(true, NON_NULL),
            literal(false, NULLABLE)));
    success();

    EXPECT_EQ(t::Bool(NULLABLE), type(expr.get()));
    EXPECT_EQ(t::Bool(NULLABLE), cast_type(expr->left()));
    EXPECT_EQ(false, get<v::Bool>(expr->right()));
}

TEST_F(AnalyzerExpressionTest, binary_op_logical_inconsistent_left) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LOGICAL_AND,
            literal(1.),
            literal(false, NON_NULL)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_logical_inconsistent_right) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::LOGICAL_AND,
            literal(true, NON_NULL),
            literal(2.)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, binary_op_in_not_impl) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(Op::IN, literal(1), literal(2)));
    ASSERT_TRUE(contains(Diagnostic::Code::NOT_IMPLEMENTED));
}

TEST_F(AnalyzerExpressionTest, binary_op_relational_not_impl) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(Op::RELATION_UNION, literal(1), literal(2)));
    ASSERT_TRUE(contains(Diagnostic::Code::NOT_IMPLEMENTED));
}

TEST_F(AnalyzerExpressionTest, binary_op_propagate_error_left) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::ADD,
            var("missing"),
            literal("X", NON_NULL)));
    success(false);

    EXPECT_TRUE(is_propagated_error(expr.get()));
}

TEST_F(AnalyzerExpressionTest, binary_op_propagate_error_right) {
    using Op = expression::BinaryOperator::Kind;
    auto expr = analyze(f.BinaryOperator(
            Op::ADD,
            literal("X", NON_NULL),
            var("missing")));
    success(false);

    EXPECT_TRUE(is_propagated_error(expr.get()));
}

TEST_F(AnalyzerExpressionTest, tuple_element_load) {
    add_variable("a", t::Tuple {
        { "c", t::Int(32U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(f.Name("c"), 0U)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
    EXPECT_EQ("c", expr->index()->name()->token());
    EXPECT_EQ(0U, expr->index()->position());
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_name) {
    add_variable("a", t::Tuple {
        { "c", t::Int(32U, NON_NULL) },
        { "d", t::Int(64U, NON_NULL) },
        { "e", t::Int(32U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(f.Name("d"))));
    success();

    EXPECT_EQ(t::Int(64U, NON_NULL), type(expr.get()));
    EXPECT_EQ("d", expr->index()->name()->token());
    EXPECT_EQ(1U, expr->index()->position());
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_index) {
    add_variable("a", t::Tuple {
        { "c", t::Int(32U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(f.Name("c"))));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(expr.get()));
    EXPECT_EQ("c", expr->index()->name()->token());
    EXPECT_EQ(0U, expr->index()->position());
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_position_out) {
    add_variable("a", t::Tuple {
        { "c", t::Int(32U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(2U)));
    success(false);
    EXPECT_FALSE(is_valid(extract(expr.get(), true)));
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_name_missing) {
    add_variable("a", t::Tuple {
        { "c", t::Int(32U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(f.Name("INVALID"))));
    success(false);
    EXPECT_FALSE(is_valid(extract(expr.get(), true)));
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_name_ambiguous) {
    add_variable("a", t::Tuple {
        { "c", t::Int(32U, NON_NULL) },
        { "c", t::Int(64U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(f.Name("d"))));
    success(false);
    EXPECT_FALSE(is_valid(extract(expr.get(), true)));
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_name_inconsistent) {
    add_variable("a", t::Tuple {
        { "c", t::Int(32U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(f.Name("INVALID"), 0U)));
    success(false);
    EXPECT_FALSE(is_valid(extract(expr.get(), true)));
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_name_extra) {
    add_variable("a", t::Tuple {
        { t::Int(32U, NON_NULL) },
    });
    auto expr = analyze(f.TupleElementLoadExpression(
            var("a"),
            f.Index(f.Name("INVALID"), 0U)));
    success(false);
    EXPECT_FALSE(is_valid(extract(expr.get(), true)));
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_not_tuple) {
    auto expr = analyze(f.TupleElementLoadExpression(literal(1), f.Index(0U)));
    success(false);
}

TEST_F(AnalyzerExpressionTest, tuple_element_load_propagate) {
    add_variable("a", t::Error());
    auto expr = analyze(f.TupleElementLoadExpression(var("a"), f.Index(0U)));
    success(); // should no extra errors
    EXPECT_TRUE(is_propagated_error(expr.get()));
}
}  // namespace shakujo::analyzer
