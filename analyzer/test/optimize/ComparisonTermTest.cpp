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
#include "optimize/ComparisonTerm.h"

#include <gtest/gtest.h>

#include "../AnalyzerTestBase.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::optimize {

using BOp = model::expression::BinaryOperator::Kind;
using UOp = model::expression::UnaryOperator::Kind;
namespace t = common::core::type;
namespace v = common::core::value;

class ComparisonTermTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> analyze(std::unique_ptr<T> expr) {
        return analyze_managed<T, model::expression::Expression>(std::move(expr));
    }
};

TEST_F(ComparisonTermTest, simple) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(BOp::EQUAL, var("v"), literal(1)));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 1U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), expr.get());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::EQ);
        EXPECT_EQ(t.left().variable()->id(), vid);
        EXPECT_EQ(*t.right().constant(), v::Int(1));
    }
}

TEST_F(ComparisonTermTest, commute) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(BOp::EQUAL, literal(1), var("v")));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 1U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), expr.get());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::EQ);
        EXPECT_EQ(t.left().variable()->id(), vid);
        EXPECT_EQ(*t.right().constant(), v::Int(1));
    }
}

TEST_F(ComparisonTermTest, relation) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(BOp::LESS_THAN, var("v"), literal(1)));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 1U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), expr.get());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::LT);
        EXPECT_EQ(t.left().variable()->id(), vid);
        EXPECT_EQ(*t.right().constant(), v::Int(1));
    }
}

TEST_F(ComparisonTermTest, relation_commute) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(BOp::LESS_THAN, literal(1), var("v")));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 1U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), expr.get());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::GT);
        EXPECT_EQ(t.left().variable()->id(), vid);
        EXPECT_EQ(*t.right().constant(), v::Int(1));
    }
}

TEST_F(ComparisonTermTest, cast) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NULLABLE));
    auto expr = analyze(f.BinaryOperator(BOp::EQUAL, var("v"), literal(1)));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 1U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), expr.get());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::EQ);
        EXPECT_EQ(t.left().variable()->id(), vid);
        EXPECT_EQ(*t.right().constant(), v::Int(1));
    }
}

TEST_F(ComparisonTermTest, and) {
    auto& aid = add_variable(f.Name("a"), t::Int(64U, NON_NULL));
    auto& bid = add_variable(f.Name("b"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(BOp::CONDITIONAL_AND,
        f.BinaryOperator(BOp::LESS_THAN, var("a"), literal(1)),
        f.BinaryOperator(BOp::GREATER_THAN, var("b"), literal(2))));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    auto* cand = dynamic_pointer_cast<model::expression::BinaryOperator>(expr.get());
    ASSERT_EQ(results.size(), 2U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), cand->left());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::LT);
        EXPECT_EQ(t.left().variable()->id(), aid);
        EXPECT_EQ(*t.right().constant(), v::Int(1));
    }
    {
        auto& t = results[1];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), cand->right());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::GT);
        EXPECT_EQ(t.left().variable()->id(), bid);
        EXPECT_EQ(*t.right().constant(), v::Int(2));
    }
}

TEST_F(ComparisonTermTest, negative) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(
        BOp::EQUAL, var("v"),
        f.UnaryOperator(UOp::SIGN_INVERSION, literal(1))));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 1U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), expr.get());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::EQ);
        EXPECT_EQ(t.left().variable()->id(), vid);
        EXPECT_EQ(*t.right().constant(), v::Int(-1));
    }
}

TEST_F(ComparisonTermTest, negative_float) {
    auto& vid = add_variable(f.Name("v"), t::Float(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(
        BOp::EQUAL, var("v"),
        f.UnaryOperator(UOp::SIGN_INVERSION, literal(1.0))));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 1U);
    {
        auto& t = results[0];
        ASSERT_TRUE(t.left().is_variable());
        ASSERT_TRUE(t.right().is_constant());

        EXPECT_EQ(t.source(), expr.get());
        EXPECT_EQ(t.op(), ComparisonTerm::Operator::EQ);
        EXPECT_EQ(t.left().variable()->id(), vid);
        EXPECT_EQ(*t.right().constant(), v::Float(-1));
    }
}

TEST_F(ComparisonTermTest, resolve_thin) {
    auto& vid = add_variable(f.Name("v"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(BOp::EQUAL, var("v"), literal(1)));

    auto result = ComparisonTerm::resolve(env.binding_context(), expr.get());
    ASSERT_TRUE(result.left().is_variable());
    ASSERT_TRUE(result.right().is_constant());

    EXPECT_EQ(result.source(), expr.get());
    EXPECT_EQ(result.op(), ComparisonTerm::Operator::EQ);
    EXPECT_EQ(result.left().variable()->id(), vid);
    EXPECT_EQ(*result.right().constant(), v::Int(1));
}

TEST_F(ComparisonTermTest, resolve_thick) {
    add_variable(f.Name("a"), t::Int(64U, NON_NULL));
    add_variable(f.Name("b"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(BOp::CONDITIONAL_AND,
        f.BinaryOperator(BOp::LESS_THAN, var("a"), literal(1)),
        f.BinaryOperator(BOp::GREATER_THAN, var("b"), literal(2))));

    auto result = ComparisonTerm::resolve(env.binding_context(), expr.get());
    ASSERT_FALSE(result);
}

TEST_F(ComparisonTermTest, no_variables) {
    auto expr = analyze(f.BinaryOperator(BOp::EQUAL, literal(1), literal(1)));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 0U);
}

TEST_F(ComparisonTermTest, complex) {
    add_variable(f.Name("v"), t::Int(64U, NON_NULL));
    auto expr = analyze(f.BinaryOperator(
        BOp::EQUAL,
        f.BinaryOperator(BOp::ADD, var("v"), literal(1)),
        literal(1)));

    auto results = ComparisonTerm::collect(env.binding_context(), expr.get());
    ASSERT_EQ(results.size(), 0U);
}

}  // namespace shakujo::analyzer::optimize
