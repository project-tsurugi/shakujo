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
#include "optimize/PredicatePushDown.h"

#include <gtest/gtest.h>

#include "../AnalyzerTestBase.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::optimize {

using BOp = model::expression::BinaryOperator::Kind;
using UOp = model::expression::UnaryOperator::Kind;

namespace t = common::core::type;
namespace v = common::core::value;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

class PredicatePushDownTest : public AnalyzerTestBase, public ::testing::Test {
public:
    std::unique_ptr<model::expression::Expression> apply(std::unique_ptr<model::expression::Expression> expr) {
        auto manager = manage<model::expression::Expression>(std::move(expr));
        do_analyze(manager.get(), true);
        env.reporter().clear();

        Context context { env.binding_context() };
        PredicatePushDown optimizer { context };
        optimizer(manager.get());
        return manager.release();
    }

    template<class T>
    std::enable_if_t<!std::is_base_of_v<model::expression::Expression, T>, std::unique_ptr<T>>
    apply(std::unique_ptr<T> node) {
        do_analyze(node.get());
        Context context { env.binding_context() };
        PredicatePushDown optimizer { context };
        optimizer(node.get());
        return node;
    }

    std::shared_ptr<binding::VariableBinding> var_of(model::expression::Expression* node) {
        model::expression::Expression* current = node;
        while (auto cast = dynamic_pointer_cast_if<model::expression::ImplicitCast>(current)) {
            current = cast->operand();
        }
        return extract_var(cast<model::expression::VariableReference>(current));
    }

    template<class T>
    typename T::type val_of(model::expression::Expression* node) {
        model::expression::Expression* current = node;
        while (auto cast = dynamic_pointer_cast_if<model::expression::ImplicitCast>(current)) {
            current = cast->operand();
        }
        return get<T>(cast<model::expression::Literal>(current));
    }

    BOp bop(model::expression::Expression* node) {
        return cast<model::expression::BinaryOperator>(node)->operator_kind();
    }

    model::expression::Expression* left(model::expression::Expression* node) {
        return cast<model::expression::BinaryOperator>(node)->left();
    }

    model::expression::Expression* right(model::expression::Expression* node) {
        return cast<model::expression::BinaryOperator>(node)->right();
    }

    template<class T, class U>
    auto cast(U* ptr) -> decltype(dynamic_pointer_cast<T>(ptr)) {
        if (auto&& r = dynamic_pointer_cast_if<T>(ptr)) {
            return r;
        }
        throw std::domain_error(common::util::to_string("cast to ", typeid(T).name(), " : ", to_string(ptr)));
    }

    template<class T>
    binding::Id<binding::VariableBinding> const& get_id(T const* node) {
        auto var = extract_var(cast<model::expression::VariableReference>(node));
        return var->id();
    }

    template<class E>
    static bool contains(std::set<E> const& set, E const& element) {
        return set.find(element) != set.end();
    }
};

TEST_F(PredicatePushDownTest, scan) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    auto expr = apply(f.ScanExpression(f.Name("testing")));
    ASSERT_EQ(expr->kind(), model::expression::relation::ScanExpression::tag);
}

TEST_F(PredicatePushDownTest, scan_select) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))));

    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(select->operand());
    auto relation = extract_relation(select);

    auto cond = select->condition();
    ASSERT_TRUE(cond);
    EXPECT_EQ(var_of(left(cond)), relation->process().columns()[0]);
    EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
}

TEST_F(PredicatePushDownTest, scan_sort_select) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    // select - order - scan
    auto expr = apply(f.SelectionExpression(
        f.OrderExpression(
            f.ScanExpression(f.Name("testing")),
            {
                f.OrderExpressionElement(var("C1")),
            }),
        f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))));

    // order - select - scan
    auto order = cast<model::expression::relation::OrderExpression>(expr.get());
    auto select = cast<model::expression::relation::SelectionExpression>(order->operand());
    cast<model::expression::relation::ScanExpression>(select->operand());
    auto relation = extract_relation(select);

    auto cond = select->condition();
    ASSERT_TRUE(cond);
    EXPECT_EQ(var_of(left(cond)), relation->process().columns()[0]);
    EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
}

TEST_F(PredicatePushDownTest, scan_distinct_select) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    // select - distinct - scan
    auto expr = apply(f.SelectionExpression(
        f.DistinctExpression(f.ScanExpression(f.Name("testing"))),
        f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))));

    // distinct - select - scan
    auto node = cast<model::expression::relation::DistinctExpression>(expr.get());
    auto select = cast<model::expression::relation::SelectionExpression>(node->operand());
    cast<model::expression::relation::ScanExpression>(select->operand());
    auto relation = extract_relation(select);

    auto cond = select->condition();
    ASSERT_TRUE(cond);
    EXPECT_EQ(var_of(left(cond)), relation->process().columns()[0]);
    EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
}

TEST_F(PredicatePushDownTest, complex_predicate) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    // select - order - scan
    auto expr = apply(f.SelectionExpression(
        f.OrderExpression(
            f.ScanExpression(f.Name("testing")),
            {
                f.OrderExpressionElement(var("C1")),
            }),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::GREATER_THAN, var("C1"), literal(0)),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(100)))));

    // order - select - scan
    auto order = cast<model::expression::relation::OrderExpression>(expr.get());
    auto select = cast<model::expression::relation::SelectionExpression>(order->operand());
    cast<model::expression::relation::ScanExpression>(select->operand());
    auto relation = extract_relation(select);

    auto cond = select->condition();
    ASSERT_TRUE(cond);

    auto t0 = left(cond);
    auto t1 = right(cond);

    EXPECT_EQ(var_of(left(t0)), relation->process().columns()[0]);
    EXPECT_EQ(var_of(left(t1)), relation->process().columns()[0]);
    EXPECT_NE(bop(t0), bop(t1));

    if (bop(t0) == BOp::LESS_THAN) {
        EXPECT_EQ(bop(t1), BOp::GREATER_THAN);
        EXPECT_EQ(val_of<v::Int>(right(t0)), 100);
        EXPECT_EQ(val_of<v::Int>(right(t1)), 0);
    } else {
        EXPECT_EQ(bop(t1), BOp::LESS_THAN);
        EXPECT_EQ(val_of<v::Int>(right(t0)), 0);
        EXPECT_EQ(val_of<v::Int>(right(t1)), 100);
    }
}

TEST_F(PredicatePushDownTest, scan_select_select) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    auto expr = apply(f.SelectionExpression(
        f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(100))),
        f.BinaryOperator(BOp::GREATER_THAN, var("C1"), literal(0))));

    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(select->operand());
    auto relation = extract_relation(select);

    auto cond = select->condition();
    ASSERT_TRUE(cond);

    auto t0 = left(cond);
    auto t1 = right(cond);

    EXPECT_EQ(var_of(left(t0)), relation->process().columns()[0]);
    EXPECT_EQ(var_of(left(t1)), relation->process().columns()[0]);
    EXPECT_NE(bop(t0), bop(t1));

    if (bop(t0) == BOp::LESS_THAN) {
        EXPECT_EQ(bop(t1), BOp::GREATER_THAN);
        EXPECT_EQ(val_of<v::Int>(right(t0)), 100);
        EXPECT_EQ(val_of<v::Int>(right(t1)), 0);
    } else {
        EXPECT_EQ(bop(t1), BOp::LESS_THAN);
        EXPECT_EQ(val_of<v::Int>(right(t0)), 0);
        EXPECT_EQ(val_of<v::Int>(right(t1)), 100);
    }
}

TEST_F(PredicatePushDownTest, scan_join_select) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            {
                f.BinaryOperator(BOp::EQUAL, var("T1", "K"), var("T2", "K")),
            }),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(2)))));

    // join - select - scan
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& left = extract_relation(join->left())->output().columns();
        auto&& right = extract_relation(join->right())->output().columns();
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 1U);
        EXPECT_TRUE(contains(eq, std::make_pair(left[0], right[0])));
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->left());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[1]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->right());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[1]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 2);
    }
}

TEST_F(PredicatePushDownTest, scan_join_select_complex) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            {
                f.BinaryOperator(BOp::EQUAL, var("T1", "K"), var("T2", "K")),
            }),
        f.BinaryOperator(BOp::LESS_THAN, var("C1"), var("C2"))));

    // select - join - scan
    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    auto join = cast<model::expression::relation::JoinExpression>(select->operand());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& left = extract_relation(join->left())->output().columns();
        auto&& right = extract_relation(join->right())->output().columns();
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 1U);
        EXPECT_TRUE(contains(eq, std::make_pair(left[0], right[0])));
    }
    cast<model::expression::relation::ScanExpression>(join->left());
    cast<model::expression::relation::ScanExpression>(join->right());
}

TEST_F(PredicatePushDownTest, scan_joinleft_select) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::LEFT_OUTER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            {
                f.BinaryOperator(BOp::EQUAL, var("T1", "K"), var("T2", "K")),
            }),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(2)))));

    // select - join(select - scan, scan)
    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    {
        auto relation = extract_relation(select);
        auto cond = select->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[3]); // T2.C2
        EXPECT_EQ(val_of<v::Int>(right(cond)), 2);
    }
    auto join = cast<model::expression::relation::JoinExpression>(select->operand());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& left = extract_relation(join->left())->output().columns();
        auto&& right = extract_relation(join->right())->output().columns();
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 1U);
        EXPECT_TRUE(contains(eq, std::make_pair(left[0], right[0])));
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->left());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[1]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
    cast<model::expression::relation::ScanExpression>(join->right());
}

TEST_F(PredicatePushDownTest, scan_joinright_select) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::RIGHT_OUTER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            {
                f.BinaryOperator(BOp::EQUAL, var("T1", "K"), var("T2", "K")),
            }),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(2)))));

    // select - join(scan, select - scan)
    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    {
        auto relation = extract_relation(select);
        auto cond = select->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[1]); // T1.C1
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
    auto join = cast<model::expression::relation::JoinExpression>(select->operand());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& left = extract_relation(join->left())->output().columns();
        auto&& right = extract_relation(join->right())->output().columns();
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 1U);
        EXPECT_TRUE(contains(eq, std::make_pair(left[0], right[0])));
    }
    cast<model::expression::relation::ScanExpression>(join->left());
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->right());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[1]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 2);
    }
}

TEST_F(PredicatePushDownTest, scan_joinfull_select) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::FULL_OUTER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            {
                f.BinaryOperator(BOp::EQUAL, var("T1", "K"), var("T2", "K")),
            }),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(2)))));

    // select - join(scan, select - scan)
    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    auto join = cast<model::expression::relation::JoinExpression>(select->operand());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& left = extract_relation(join->left())->output().columns();
        auto&& right = extract_relation(join->right())->output().columns();
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 1U);
        EXPECT_TRUE(contains(eq, std::make_pair(left[0], right[0])));
    }
    cast<model::expression::relation::ScanExpression>(join->left());
    cast<model::expression::relation::ScanExpression>(join->right());
}

TEST_F(PredicatePushDownTest, scan_joinnatural_select) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::NATURAL_INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2"))),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(2)))));

    // join - select - scan
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& left = extract_relation(join->left())->output().columns();
        auto&& right = extract_relation(join->right())->output().columns();
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 1U);
        EXPECT_TRUE(contains(eq, std::make_pair(left[0], right[0])));
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->left());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[1]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->right());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[1]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 2);
    }
}

TEST_F(PredicatePushDownTest, scan_join_select_propagate) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            {
                f.BinaryOperator(BOp::EQUAL, var("T1", "K"), var("T2", "K")),
            }),
        f.BinaryOperator(BOp::EQUAL, var("T1", "K"), literal(1))));

    // join - select - scan
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 0U);
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->left());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[0]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->right());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[0]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
}

TEST_F(PredicatePushDownTest, scan_joinnatural_select_propagate) {
    add(common::schema::TableInfo { "T1", {
        { "K", t::Int(64U, NON_NULL), },
        { "C1", t::Int(64U, NON_NULL), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "K", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
    }});
    // select - join - scan
    auto expr = apply(f.SelectionExpression(
        f.JoinExpression(
            model::expression::relation::JoinExpression::Kind::NATURAL_INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            {}),
        f.BinaryOperator(BOp::EQUAL, var("T1", "K"), literal(1))));

    // join - select - scan
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    {
        auto cond = join->condition();
        ASSERT_FALSE(cond);

        auto relation = extract_relation(join);
        auto&& strategy = relation->join_strategy();
        auto&& eq = strategy.equalities();
        EXPECT_EQ(eq.size(), 0);
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->left());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[0]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
    {
        auto selection = cast<model::expression::relation::SelectionExpression>(join->right());
        cast<model::expression::relation::ScanExpression>(selection->operand());
        auto relation = extract_relation(selection);
        auto cond = selection->condition();
        ASSERT_TRUE(cond);
        EXPECT_EQ(var_of(left(cond)), relation->process().columns()[0]);
        EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
    }
}

}  // namespace shakujo::analyzer::optimize
