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
#include "optimize/FixRelationInfo.h"

#include <gtest/gtest.h>

#include "../AnalyzerTestBase.h"

#include "optimize/SelectScan.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::optimize {

using JoinKind = model::expression::relation::JoinExpression::Kind;
using Dir = common::core::Direction;
using BOp = model::expression::BinaryOperator::Kind;
namespace t = common::core::type;
namespace v = common::core::value;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

class FixRelationInfoTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<class T>
    void do_apply(T&& ptr) {
        do_analyze(ptr.get(), true);
        env.reporter() = {};
        Context context { env.binding_context() };
        SelectScan { context }(ptr.get());
        FixRelationInfo { context }(ptr.get());
    }

    std::unique_ptr<model::expression::Expression> apply(std::unique_ptr<model::expression::Expression> expr) {
        auto manager = manage<model::expression::Expression>(std::move(expr));
        do_apply(manager);
        return manager.release();
    }

    template<class T>
    std::enable_if_t<!std::is_base_of_v<model::expression::Expression, T>, std::unique_ptr<T>>
    apply(std::unique_ptr<T> node) {
        do_apply(node);
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

    template<class T>
    typename T::type val_of(common::core::Value const* value) {
        return cast<T>(value)->get();
    }

    template<class T, class U>
    auto cast(U* ptr) -> decltype(dynamic_pointer_cast<T>(ptr)) {
        if (auto&& r = dynamic_pointer_cast_if<T>(ptr)) {
            return r;
        }
        throw std::domain_error(common::util::to_string("cast to ", typeid(T).name(), " : ", to_string(ptr)));
    }
};

TEST_F(FixRelationInfoTest, trivial) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    auto expr = apply(f.ScanExpression(f.Name("testing")));
    auto node = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 0U);
    ASSERT_EQ(output.unique_keys().size(), 0U);
}

TEST_F(FixRelationInfoTest, scan_key) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))));
    auto node = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 1U);
    {
        auto&& o = output.order()[0];
        EXPECT_EQ(o.column(), output.columns()[0]);
        EXPECT_EQ(o.direction(), Dir::ASCENDANT);
    }

    ASSERT_EQ(output.unique_keys().size(), 1U);
    {
        std::set<std::shared_ptr<binding::VariableBinding>> key {};
        key.emplace(output.columns()[0]);
        EXPECT_NE(output.unique_keys().find(key), output.unique_keys().end());
    }
}

TEST_F(FixRelationInfoTest, scan_key_complex) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C3", Dir::DESCENDANT },
                { "C1", Dir::ASCENDANT },
            }
        }});
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C3"), literal(1))));
    auto node = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 2U);
    {
        auto&& o = output.order()[0];
        EXPECT_EQ(o.column(), output.columns()[2]);
        EXPECT_EQ(o.direction(), Dir::DESCENDANT);
    }
    {
        auto&& o = output.order()[1];
        EXPECT_EQ(o.column(), output.columns()[0]);
        EXPECT_EQ(o.direction(), Dir::ASCENDANT);
    }

    ASSERT_EQ(output.unique_keys().size(), 1U);
    {
        std::set<std::shared_ptr<binding::VariableBinding>> key {};
        key.emplace(output.columns()[0]);
        key.emplace(output.columns()[2]);
        EXPECT_NE(output.unique_keys().find(key), output.unique_keys().end());
    }
}

TEST_F(FixRelationInfoTest, scan_secondary) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {},
        {
            {
                "S",
                {
                    { "C1" },
                }
            }
        }});
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))));
    auto node = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 1U);
    {
        auto&& o = output.order()[0];
        EXPECT_EQ(o.column(), output.columns()[0]);
        EXPECT_EQ(o.direction(), Dir::ASCENDANT);
    }

    ASSERT_EQ(output.unique_keys().size(), 0U);
}

TEST_F(FixRelationInfoTest, select) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(1)))
    ));
    auto node = cast<model::expression::relation::SelectionExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 1U);
    ASSERT_EQ(output.unique_keys().size(), 1U);
}

TEST_F(FixRelationInfoTest, join) {
    add({
        "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
        }});
    add({
        "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
        }});
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
        ));
    auto node = cast<model::expression::relation::JoinExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->left());
    cast<model::expression::relation::ScanExpression>(node->right());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 0U);
    ASSERT_EQ(output.unique_keys().size(), 0U);
}

TEST_F(FixRelationInfoTest, join_pk) {
    add({
        "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    add({
        "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.SelectionExpression(
            f.ScanExpression(f.Name("T1")),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(1))),
        f.SelectionExpression(
            f.ScanExpression(f.Name("T2")),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(1))),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto node = cast<model::expression::relation::JoinExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->left());
    cast<model::expression::relation::ScanExpression>(node->right());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 0U);
    ASSERT_EQ(output.unique_keys().size(), 1U);
    {
        std::set<std::shared_ptr<binding::VariableBinding>> key {};
        key.emplace(output.columns()[0]);
        key.emplace(output.columns()[2]);
        EXPECT_NE(output.unique_keys().find(key), output.unique_keys().end());
    }
}

TEST_F(FixRelationInfoTest, join_left_pk) {
    add({
        "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    add({
        "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
        }});
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.SelectionExpression(
            f.ScanExpression(f.Name("T1")),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(1))),
        f.SelectionExpression(
            f.ScanExpression(f.Name("T2")),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(1))),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto node = cast<model::expression::relation::JoinExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->left());
    cast<model::expression::relation::SelectionExpression>(node->right());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 0U);
    ASSERT_EQ(output.unique_keys().size(), 0U);
}

TEST_F(FixRelationInfoTest, join_right_pk) {
    add({
        "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
        }});
    add({
        "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.SelectionExpression(
            f.ScanExpression(f.Name("T1")),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(1))),
        f.SelectionExpression(
            f.ScanExpression(f.Name("T2")),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(1))),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto node = cast<model::expression::relation::JoinExpression>(expr.get());
    cast<model::expression::relation::SelectionExpression>(node->left());
    cast<model::expression::relation::ScanExpression>(node->right());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 0U);
    ASSERT_EQ(output.unique_keys().size(), 0U);
}

TEST_F(FixRelationInfoTest, order) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    auto expr = apply(f.OrderExpression(
        f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))),
        {
            f.OrderExpressionElement(var("C2")),
        }
        ));
    auto node = cast<model::expression::relation::OrderExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 1U);
    {
        auto&& o = output.order()[0];
        EXPECT_EQ(o.column(), output.columns()[1]);
        EXPECT_EQ(o.direction(), Dir::ASCENDANT);
    }

    ASSERT_EQ(output.unique_keys().size(), 1U);
    {
        std::set<std::shared_ptr<binding::VariableBinding>> key {};
        key.emplace(output.columns()[0]);
        EXPECT_NE(output.unique_keys().find(key), output.unique_keys().end());
    }
}

TEST_F(FixRelationInfoTest, order_desc) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" }
            }
        }});
    auto expr = apply(f.OrderExpression(
        f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))),
        {
            f.OrderExpressionElement(var("C2"), model::expression::relation::OrderExpression::Direction::DESCENDANT),
        }
    ));
    auto node = cast<model::expression::relation::OrderExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 1U);
    {
        auto&& o = output.order()[0];
        EXPECT_EQ(o.column(), output.columns()[1]);
        EXPECT_EQ(o.direction(), Dir::DESCENDANT);
    }

    ASSERT_EQ(output.unique_keys().size(), 1U);
    {
        std::set<std::shared_ptr<binding::VariableBinding>> key {};
        key.emplace(output.columns()[0]);
        EXPECT_NE(output.unique_keys().find(key), output.unique_keys().end());
    }
}

TEST_F(FixRelationInfoTest, projection) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" },
            }
        }});
    auto expr = apply(f.ProjectionExpression(
        f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))),
        {
            f.ProjectionExpressionColumn(var("C1")),
            f.ProjectionExpressionColumn(literal(1)),
        }
    ));
    auto node = cast<model::expression::relation::ProjectionExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 1U);
    {
        auto&& o = output.order()[0];
        EXPECT_EQ(o.column(), output.columns()[0]);
        EXPECT_EQ(o.direction(), Dir::ASCENDANT);
    }

    ASSERT_EQ(output.unique_keys().size(), 1U);
    {
        std::set<std::shared_ptr<binding::VariableBinding>> key {};
        key.emplace(output.columns()[0]);
        EXPECT_NE(output.unique_keys().find(key), output.unique_keys().end());
    }
}

TEST_F(FixRelationInfoTest, projection_partial) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" },
                { "C2" },
            }
        }});
    auto expr = apply(f.ProjectionExpression(
        f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))),
        {
            f.ProjectionExpressionColumn(var("C1")),
            f.ProjectionExpressionColumn(literal(1)),
        }
    ));
    auto node = cast<model::expression::relation::ProjectionExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 1U);
    {
        auto&& o = output.order()[0];
        EXPECT_EQ(o.column(), output.columns()[0]);
        EXPECT_EQ(o.direction(), Dir::ASCENDANT);
    }
    ASSERT_EQ(output.unique_keys().size(), 0U);
}

TEST_F(FixRelationInfoTest, projection_unknown) {
    add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {
            {
                { "C1" },
            }
        }});
    auto expr = apply(f.ProjectionExpression(
        f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))),
        {
            f.ProjectionExpressionColumn(literal(1)),
        }
    ));
    auto node = cast<model::expression::relation::ProjectionExpression>(expr.get());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output();
    ASSERT_EQ(output.order().size(), 0U);
    ASSERT_EQ(output.unique_keys().size(), 0U);
}
}  // namespace shakujo::analyzer::optimize
