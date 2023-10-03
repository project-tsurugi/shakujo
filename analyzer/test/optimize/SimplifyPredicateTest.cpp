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
#include "optimize/SimplifyPredicate.h"

#include <gtest/gtest.h>

#include <tuple>

#include "../AnalyzerTestBase.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::optimize {

using Ternary = common::util::Ternary;
using BOp = model::expression::BinaryOperator::Kind;
using UOp = model::expression::UnaryOperator::Kind;

namespace t = common::core::type;
namespace v = common::core::value;
using common::util::dynamic_pointer_cast;

class SimplifyPredicateTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<class T = model::expression::Expression>
    std::tuple<std::unique_ptr<T>, Ternary> apply(std::unique_ptr<model::expression::Expression> expr) {
        auto manager = manage<model::expression::Expression>(std::move(expr));
        do_analyze(manager.get());

        Context context { env.binding_context() };
        SimplifyPredicate optimizer { context };
        auto result = optimizer(manager.get());
        ensure(manager.get());

        return std::make_tuple(dynamic_pointer_cast<T>(manager.release()), result);
    }

    binding::Id<binding::VariableBinding> const& get_id(model::expression::Expression const* node) {
        auto var = extract_var(dynamic_pointer_cast<model::expression::VariableReference>(node));
        return var->id();
    }
};

TEST_F(SimplifyPredicateTest, trivial_true) {
    auto [expr, result] = apply(literal(true));
    ASSERT_EQ(result, Ternary::TRUE);
    EXPECT_EQ(get<v::Bool>(expr.get()), true);
}

TEST_F(SimplifyPredicateTest, trivial_false) {
    auto [expr, result] = apply(literal(false));
    ASSERT_EQ(result, Ternary::FALSE);
    EXPECT_EQ(get<v::Bool>(expr.get()), false);
}

TEST_F(SimplifyPredicateTest, trivial_unknown) {
    auto [expr, result] = apply(literal(100));
    EXPECT_EQ(result, Ternary::UNKNOWN);
    EXPECT_EQ(get<v::Int>(expr.get()), 100);
}

TEST_F(SimplifyPredicateTest, var) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(var("v"));
    EXPECT_EQ(result, Ternary::UNKNOWN);
    EXPECT_TRUE(is_instance<model::expression::VariableReference>(expr.get()));
}

TEST_F(SimplifyPredicateTest, not_t) {
    auto [expr, result] = apply(f.UnaryOperator(UOp::CONDITIONAL_NOT, literal(true)));
    ASSERT_EQ(result, Ternary::FALSE);
    EXPECT_TRUE(is_instance<model::expression::Literal>(expr.get()));
}

TEST_F(SimplifyPredicateTest, not_f) {
    auto [expr, result] = apply(f.UnaryOperator(UOp::CONDITIONAL_NOT, literal(false)));
    ASSERT_EQ(result, Ternary::TRUE);
    EXPECT_TRUE(is_instance<model::expression::Literal>(expr.get()));
}

TEST_F(SimplifyPredicateTest, not_x) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(f.UnaryOperator(UOp::CONDITIONAL_NOT, var("v")));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_TRUE(is_instance<model::expression::UnaryOperator>(expr.get()));
}

TEST_F(SimplifyPredicateTest, and_tt) {
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_AND, literal(true), literal(true)));
    ASSERT_EQ(result, Ternary::TRUE);
    EXPECT_EQ(get<v::Bool>(expr.get()), true);
}

TEST_F(SimplifyPredicateTest, and_fx) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_AND, literal(false), var("v")));
    ASSERT_EQ(result, Ternary::FALSE);
    EXPECT_EQ(get<v::Bool>(expr.get()), false);
}

TEST_F(SimplifyPredicateTest, and_xf) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_AND, var("v"), literal(false)));
    ASSERT_EQ(result, Ternary::FALSE);
    EXPECT_EQ(get<v::Bool>(expr.get()), false);
}

TEST_F(SimplifyPredicateTest, and_uu) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_AND, var("v"), var("v")));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_TRUE(is_instance<model::expression::BinaryOperator>(expr.get()));
}

TEST_F(SimplifyPredicateTest, and_tx) {
    auto&& vid = add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_AND, literal(true), var("v")));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_EQ(get_id(expr.get()), vid);
}

TEST_F(SimplifyPredicateTest, and_xt) {
    auto&& vid = add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_AND, var("v"), literal(true)));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_EQ(get_id(expr.get()), vid);
}

TEST_F(SimplifyPredicateTest, and_nest) {
    auto&& vid = add_variable("v", t::Bool());
    auto [expr, result] = apply(
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            literal(true),
            f.BinaryOperator(BOp::CONDITIONAL_AND,
                var("v"),
                literal(true))));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_EQ(get_id(expr.get()), vid);
}

TEST_F(SimplifyPredicateTest, or_ff) {
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_OR, literal(false), literal(false)));
    ASSERT_EQ(result, Ternary::FALSE);
    EXPECT_EQ(get<v::Bool>(expr.get()), false);
}

TEST_F(SimplifyPredicateTest, or_tx) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_OR, literal(true), var("v")));
    ASSERT_EQ(result, Ternary::TRUE);
    EXPECT_EQ(get<v::Bool>(expr.get()), true);
}

TEST_F(SimplifyPredicateTest, or_xt) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_OR, var("v"), literal(true)));
    ASSERT_EQ(result, Ternary::TRUE);
    EXPECT_EQ(get<v::Bool>(expr.get()), true);
}

TEST_F(SimplifyPredicateTest, or_uu) {
    add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_OR, var("v"), var("v")));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_TRUE(is_instance<model::expression::BinaryOperator>(expr.get()));
}

TEST_F(SimplifyPredicateTest, or_fx) {
    auto&& vid = add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_OR, literal(false), var("v")));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_EQ(get_id(expr.get()), vid);
}

TEST_F(SimplifyPredicateTest, or_xf) {
    auto&& vid = add_variable("v", t::Bool());
    auto [expr, result] = apply(f.BinaryOperator(BOp::CONDITIONAL_OR, var("v"), literal(false)));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_EQ(get_id(expr.get()), vid);
}

TEST_F(SimplifyPredicateTest, or_nest) {
    auto&& vid = add_variable("v", t::Bool());
    auto [expr, result] = apply(
        f.BinaryOperator(BOp::CONDITIONAL_OR,
            literal(false),
            f.BinaryOperator(BOp::CONDITIONAL_OR,
                var("v"),
                literal(false))));
    ASSERT_EQ(result, Ternary::UNKNOWN);
    EXPECT_EQ(get_id(expr.get()), vid);
}

}  // namespace shakujo::analyzer::optimize
