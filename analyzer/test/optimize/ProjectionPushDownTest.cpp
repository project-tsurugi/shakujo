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
#include "optimize/ProjectionPushDown.h"

#include <gtest/gtest.h>

#include "../AnalyzerTestBase.h"

#include "optimize/FixRelationInfo.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::optimize {

using JoinKind = model::expression::relation::JoinExpression::Kind;
using Quant = model::expression::FunctionCall::Quantifier;
using BOp = model::expression::BinaryOperator::Kind;
namespace t = common::core::type;
namespace v = common::core::value;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

class ProjectionPushDownTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<class T>
    void do_apply(T&& ptr) {
        do_analyze(ptr.get(), true);
        env.reporter().clear();
        Context context { env.binding_context() };
        ProjectionPushDown { context }(ptr.get());
        FixRelationInfo { context }(ptr.get());
        ensure(ptr.get());
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

    template<class T, class U>
    auto cast(U* ptr) -> decltype(dynamic_pointer_cast<T>(ptr)) {
        if (auto&& r = dynamic_pointer_cast_if<T>(ptr)) {
            return r;
        }
        throw std::domain_error(common::util::to_string("cast to ", typeid(T).name(), " : ", to_string(ptr)));
    }

    std::shared_ptr<binding::VariableBinding> const& find(
            std::string_view name,
            std::vector<std::shared_ptr<binding::VariableBinding>> const& columns) {
        for (auto&& column : columns) {
            if (column->name() == name) {
                return column;
            }
        }
        std::abort();
    }
};

TEST_F(ProjectionPushDownTest, scan) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ScanExpression(f.Name("testing")));
    auto node = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 4U);
    {
        auto&& c = output[0];
        EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
    }
    {
        auto&& c = output[1];
        EXPECT_EQ(*c->type(), t::Int(64U, NULLABLE));
    }
    {
        auto&& c = output[2];
        EXPECT_EQ(*c->type(), t::Int(32U, NON_NULL));
    }
    {
        auto&& c = output[3];
        EXPECT_EQ(*c->type(), t::Int(32U, NULLABLE));
    }
}

TEST_F(ProjectionPushDownTest, selection) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.SelectionExpression(
            f.ScanExpression(f.Name("testing")),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(1))),
        {
            f.ProjectionExpressionColumn(var("C1")),
        }
    ));
    auto node = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto selection = cast<model::expression::relation::SelectionExpression>(node->operand());
    cast<model::expression::relation::ScanExpression>(selection->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 1U);
    {
        auto&& c = output[0];
        EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
    }
}

TEST_F(ProjectionPushDownTest, projection) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.ProjectionExpression(
            f.ScanExpression(f.Name("testing")),
            {
                f.ProjectionExpressionColumn(var("C1"), f.Name("P1")),
                f.ProjectionExpressionColumn(var("C2"), f.Name("P2")),
                f.ProjectionExpressionColumn(var("C3"), f.Name("P3")),
            }),
        {
            f.ProjectionExpressionColumn(var("P2")),
        }));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::ProjectionExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 1U);
    {
        auto&& c = output[0];
        EXPECT_EQ(*c->type(), t::Int(64U, NULLABLE));
    }
}

TEST_F(ProjectionPushDownTest, projection_empty) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.ProjectionExpression(
            f.ScanExpression(f.Name("testing")),
            {
                f.ProjectionExpressionColumn(var("C1"), f.Name("P1")),
                f.ProjectionExpressionColumn(var("C2"), f.Name("P2")),
                f.ProjectionExpressionColumn(var("C3"), f.Name("P3")),
            }),
        {
            f.ProjectionExpressionColumn(literal(1)),
        }));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::ProjectionExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 1U);
    {
        auto&& c = output[0];
        EXPECT_EQ(*c->type(), t::Null());
    }
}

TEST_F(ProjectionPushDownTest, aggregation) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.AggregationExpression(
            f.ScanExpression(f.Name("testing")),
            {
                var("C1"),
                var("C2"),
                var("C3"),
            },
            {
                f.AggregationExpressionColumn(
                    {},
                    Quant::ABSENT,
                    var("C1"),
                    f.Name("A1")),
                f.AggregationExpressionColumn(
                    {},
                    Quant::ABSENT,
                    var("C2"),
                    f.Name("A2")),
                f.AggregationExpressionColumn(
                    {},
                    Quant::ABSENT,
                    var("C3"),
                    f.Name("A3")),
            }),
        {
            f.ProjectionExpressionColumn(var("A2")),
        }
    ));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::AggregationExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 1U);
    {
        auto&& c = output[0];
        EXPECT_EQ(*c->type(), t::Int(64U, NULLABLE));
    }
}

TEST_F(ProjectionPushDownTest, aggregation_empty) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.AggregationExpression(
            f.ScanExpression(f.Name("testing")),
            {
                var("C1"),
                var("C2"),
                var("C3"),
            },
            {
                f.AggregationExpressionColumn(
                    {},
                    Quant::ABSENT,
                    var("C1"),
                    f.Name("A1")),
                f.AggregationExpressionColumn(
                    {},
                    Quant::ABSENT,
                    var("C2"),
                    f.Name("A2")),
                f.AggregationExpressionColumn(
                    {},
                    Quant::ABSENT,
                    var("C3"),
                    f.Name("A3")),
            }),
        {
            f.ProjectionExpressionColumn(literal(1)),
        }
    ));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::AggregationExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 1U);
    {
        auto&& c = output[0];
        EXPECT_EQ(*c->type(), t::Null());
    }
}

TEST_F(ProjectionPushDownTest, distinct) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.DistinctExpression(f.ScanExpression(f.Name("testing"))),
        {
            f.ProjectionExpressionColumn(var("C1")),
        }
    ));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::DistinctExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 4U);
    {
        auto&& c = output[0];
        EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
    }
    {
        auto&& c = output[1];
        EXPECT_EQ(*c->type(), t::Int(64U, NULLABLE));
    }
    {
        auto&& c = output[2];
        EXPECT_EQ(*c->type(), t::Int(32U, NON_NULL));
    }
    {
        auto&& c = output[3];
        EXPECT_EQ(*c->type(), t::Int(32U, NULLABLE));
    }
}

TEST_F(ProjectionPushDownTest, order) {
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.OrderExpression(
            f.ScanExpression(f.Name("testing")),
            {
                f.OrderExpressionElement(var("C3")),
            }),
        {
            f.ProjectionExpressionColumn(var("C1")),
        }
    ));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::OrderExpression>(projection->operand());
    auto slice = cast<model::expression::relation::ProjectionExpression>(node->operand());
    cast<model::expression::relation::ScanExpression>(slice->operand());

    auto relation = extract_relation(node);
    auto&& output = relation->output().columns();
    ASSERT_EQ(output.size(), 2U);
    {
        auto c = find("C1", output);
        EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
    }
    {
        auto c = find("C3", output);
        EXPECT_EQ(*c->type(), t::Int(32U, NON_NULL));
    }
}

TEST_F(ProjectionPushDownTest, join) {
    add(common::schema::TableInfo { "T1", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.JoinExpression(
            JoinKind::INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2")),
            f.BinaryOperator(BOp::EQUAL,
                var("T1", "C3"),
                var("T2", "C4"))
            ),
        {
            f.ProjectionExpressionColumn(var("T1", "C1")),
        }
    ));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::JoinExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->left());
    cast<model::expression::relation::ScanExpression>(node->right());
    {
        auto relation = extract_relation(node);
        auto&& strategy = relation->join_strategy();
        ASSERT_EQ(strategy.columns().size(), 1);
        {
            auto&& c = strategy.columns()[0];
            EXPECT_TRUE(c.left_source());
            EXPECT_FALSE(c.right_source());
            EXPECT_EQ(*c.output()->type(), t::Int(64U, NON_NULL));
        }
        auto&& output = relation->output().columns();
        ASSERT_EQ(output.size(), 1U);
        {
            auto&& c = output[0];
            EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
        }
    }
    {
        auto relation = extract_relation(projection);
        auto&& output = relation->output().columns();
        ASSERT_EQ(output.size(), 1U);
        {
            auto c = output[0];
            EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
        }
    }
}

TEST_F(ProjectionPushDownTest, join_natural) {
    add(common::schema::TableInfo { "T1", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.JoinExpression(
            JoinKind::NATURAL_INNER,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2"))
        ),
        {
            f.ProjectionExpressionColumn(var("T1", "C1")),
        }
    ));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::JoinExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->left());
    cast<model::expression::relation::ScanExpression>(node->right());
    {
        auto relation = extract_relation(node);
        auto&& strategy = relation->join_strategy();
        ASSERT_EQ(strategy.columns().size(), 1);
        {
            auto&& c = strategy.columns()[0];
            EXPECT_TRUE(c.left_source());
            EXPECT_FALSE(c.right_source());
            EXPECT_EQ(*c.output()->type(), t::Int(64U, NON_NULL));
        }
        auto&& output = relation->output().columns();
        ASSERT_EQ(output.size(), 1U);
        {
            auto&& c = output[0];
            EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
        }
    }
    {
        auto relation = extract_relation(projection);
        auto&& output = relation->output().columns();
        ASSERT_EQ(output.size(), 1U);
        {
            auto c = output[0];
            EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
        }
    }
}

TEST_F(ProjectionPushDownTest, join_cross) {
    add(common::schema::TableInfo { "T1", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    add(common::schema::TableInfo { "T2", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NULLABLE), },
        { "C3", t::Int(32U, NON_NULL), },
        { "C4", t::Int(32U, NULLABLE), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.JoinExpression(
            JoinKind::CROSS,
            f.ScanExpression(f.Name("T1")),
            f.ScanExpression(f.Name("T2"))
        ),
        {
            f.ProjectionExpressionColumn(literal(1)),
        }
    ));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto node = cast<model::expression::relation::JoinExpression>(projection->operand());
    cast<model::expression::relation::ScanExpression>(node->left());
    cast<model::expression::relation::ScanExpression>(node->right());
    {
        auto relation = extract_relation(node);
        auto&& strategy = relation->join_strategy();
        ASSERT_EQ(strategy.columns().size(), 1);
        {
            auto&& c = strategy.columns()[0];
            EXPECT_FALSE(c.left_source());
            EXPECT_FALSE(c.right_source());
            EXPECT_EQ(*c.output()->type(), t::Null());
        }
        auto&& output = relation->output().columns();
        ASSERT_EQ(output.size(), 1U);
        {
            auto&& c = output[0];
            EXPECT_EQ(*c->type(), t::Null());
        }
    }
    {
        auto relation = extract_relation(projection);
        auto&& output = relation->output().columns();
        ASSERT_EQ(output.size(), 1U);
        {
            auto c = output[0];
            EXPECT_EQ(*c->type(), t::Int(64U, NON_NULL));
        }
    }
}

TEST_F(ProjectionPushDownTest, rename) {  // test for FixRelationInfo
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
        { "C3", t::Int(64U, NON_NULL), },
        { "C4", t::Int(64U, NON_NULL), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.RenameExpression(
            f.OrderExpression(
                f.ScanExpression(f.Name("testing")),
                {
                    f.OrderExpressionElement(var("C3")),
                }),
            f.Name("A"),
            {}
        ),
        {
            f.ProjectionExpressionColumn(var("C1")),
            f.ProjectionExpressionColumn(var("C3")),
        }));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto rename = cast<model::expression::relation::RenameExpression>(projection->operand());
    auto order = cast<model::expression::relation::OrderExpression>(rename->operand());
    auto optimize = cast<model::expression::relation::ProjectionExpression>(order->operand());
    cast<model::expression::relation::ScanExpression>(optimize->operand());

    ASSERT_EQ(rename->columns().size(), 0);

    auto type = extract_relation_type(rename);
    ASSERT_EQ(type->columns().size(), 2);
    {
        auto&& c = type->columns()[0];
        EXPECT_EQ(c.qualifiers(), names({ "A" }));
        EXPECT_EQ(c.name(), "C1");
    }
    {
        auto&& c = type->columns()[1];
        EXPECT_EQ(c.qualifiers(), names({ "A" }));
        EXPECT_EQ(c.name(), "C3");
    }
}

TEST_F(ProjectionPushDownTest, rename_columns) {  // test for FixRelationInfo
    add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
        { "C2", t::Int(64U, NON_NULL), },
        { "C3", t::Int(64U, NON_NULL), },
        { "C4", t::Int(64U, NON_NULL), },
    }});
    auto expr = apply(f.ProjectionExpression(
        f.RenameExpression(
            f.OrderExpression(
                f.ScanExpression(f.Name("testing")),
                {
                    f.OrderExpressionElement(var("C3")),
                }),
            f.Name("A"),
            {
                f.Name("A1"),
                f.Name("A2"),
                f.Name("A3"),
                f.Name("A4"),
            }
        ),
        {
            f.ProjectionExpressionColumn(var("A1")),
            f.ProjectionExpressionColumn(var("A3")),
        }));
    auto projection = cast<model::expression::relation::ProjectionExpression>(expr.get());
    auto rename = cast<model::expression::relation::RenameExpression>(projection->operand());
    auto order = cast<model::expression::relation::OrderExpression>(rename->operand());
    auto optimize = cast<model::expression::relation::ProjectionExpression>(order->operand());
    cast<model::expression::relation::ScanExpression>(optimize->operand());

    ASSERT_EQ(rename->columns().size(), 2);
    EXPECT_EQ(*f.Name("A1"), *rename->columns()[0]);
    EXPECT_EQ(*f.Name("A3"), *rename->columns()[1]);

    auto type = extract_relation_type(rename);
    ASSERT_EQ(type->columns().size(), 2);
    {
        auto&& c = type->columns()[0];
        EXPECT_EQ(c.qualifiers(), names({ "A" }));
        EXPECT_EQ(c.name(), "A1");
    }
    {
        auto&& c = type->columns()[1];
        EXPECT_EQ(c.qualifiers(), names({ "A" }));
        EXPECT_EQ(c.name(), "A3");
    }
}

}  // namespace shakujo::analyzer::optimize
