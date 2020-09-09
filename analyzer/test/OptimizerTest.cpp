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
#include "shakujo/analyzer/Optimizer.h"

#include <gtest/gtest.h>

#include "AnalyzerTestBase.h"

namespace shakujo::analyzer {

namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using JoinKind = model::expression::relation::JoinExpression::Kind;
using BOp = model::expression::BinaryOperator::Kind;

using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

class OptimizerTest : public AnalyzerTestBase, public ::testing::Test {
public:

    template<class T>
    auto apply(std::unique_ptr<T> node) {
        do_analyze(node.get(), true);
        env.reporter().clear();

        Optimizer optimizer { env.binding_context() };
        optimizer(node.get());
        return node;
    }

    template<class T, class U>
    auto cast(U* ptr) -> decltype(dynamic_pointer_cast<T>(ptr)) {
        if (auto&& r = dynamic_pointer_cast_if<T>(ptr)) {
            return r;
        }
        throw std::domain_error(common::util::to_string("cast to ", typeid(T).name(), " : ", to_string(ptr)));
    }

    template<class T = model::expression::Expression>
    T* query(model::program::Program* program) {
        auto emit = cast<model::statement::dml::EmitStatement>(program->main());
        auto query = cast<T>(emit->source());
        return query;
    }
};

TEST_F(OptimizerTest, simple) {
    add(common::schema::TableInfo { "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
            {
                { "C1" },
            }
        }
    });
    auto program = apply(f.Program({}, f.EmitStatement(f.ScanExpression(f.Name("T1")))));
    success();

    query<model::expression::relation::ScanExpression>(program.get());
}

TEST_F(OptimizerTest, join_with_seek) {
    add({
        "STOCK", {
            {"s_i_id", t::Int(32U, NON_NULL)},
            {"s_w_id", t::Int(32U, NON_NULL)},
            {"s_quantity", t::Int(32U, NON_NULL)},
            {"s_dist_01", t::Char(24U, NON_NULL)},
            {"s_dist_02", t::Char(24U, NON_NULL)},
            {"s_dist_03", t::Char(24U, NON_NULL)},
            {"s_dist_04", t::Char(24U, NON_NULL)},
            {"s_dist_05", t::Char(24U, NON_NULL)},
            {"s_dist_06", t::Char(24U, NON_NULL)},
            {"s_dist_07", t::Char(24U, NON_NULL)},
            {"s_dist_08", t::Char(24U, NON_NULL)},
            {"s_dist_09", t::Char(24U, NON_NULL)},
            {"s_dist_10", t::Char(24U, NON_NULL)},
            {"s_ytd", t::Int(32U, NON_NULL)},
            {"s_order_cnt", t::Int(32U, NON_NULL)},
            {"s_remote_cnt", t::Int(32U, NON_NULL)},
            {"s_data", t::Char(50U, NON_NULL)}
        },
        { //primary key
            {
                {"s_w_id"},
                {"s_i_id"},
            }
        }
    });
    add({
        "ORDER_LINE", {
            {"ol_d_id", t::Int(32U, NON_NULL)},
            {"ol_w_id", t::Int(32U, NON_NULL)},
            {"ol_o_id", t::Int(32U, NON_NULL)},
            {"ol_number", t::Int(32U, NON_NULL)},
            {"ol_i_id", t::Int(32U, NON_NULL)},
            {"ol_supply_w_id", t::Int(32U, NON_NULL)},
            {"ol_delivery_d", t::Char(25U)},
            {"ol_quantity", t::Int(32U, NON_NULL)},
            {"ol_amount", t::Float(64U, NON_NULL)},
            {"ol_dist_info", t::Char(24U, NON_NULL)}
        },
        { //primary key
            {
                {"ol_d_id"},
                {"ol_w_id"},
                {"ol_o_id"},
                {"ol_number"},
            }
        }
    });
    /*
     * SELECT s_i_id AS id
     *   FROM
     *     ORDER_LINE,
     *     STOCK
     *   WHERE
     *     ol_w_id = 1 AND
     *     ol_d_id = 2 AND
     *     ol_o_id < 31 AND
     *     ol_o_id >= 11 AND
     *     s_w_id = 1 AND
     *     s_i_id = ol_i_id AND
     *     s_quantity < 18.00
     */
    auto program = apply(f.Program({}, f.EmitStatement(
        f.ProjectionExpression(
            f.SelectionExpression(
                f.JoinExpression(
                    JoinKind::CROSS,
                    f.ScanExpression(f.Name("ORDER_LINE")),
                    f.ScanExpression(f.Name("STOCK"))),
                f.BinaryOperator(
                    BOp::CONDITIONAL_AND,
                    f.BinaryOperator(BOp::EQUAL, var("ol_w_id"), literal(1)),
                    f.BinaryOperator(
                        BOp::CONDITIONAL_AND,
                        f.BinaryOperator(BOp::EQUAL, var("ol_d_id"), literal(2)),
                        f.BinaryOperator(
                            BOp::CONDITIONAL_AND,
                            f.BinaryOperator(BOp::LESS_THAN, var("ol_o_id"), literal(31)),
                            f.BinaryOperator(
                                BOp::CONDITIONAL_AND,
                                f.BinaryOperator(BOp::GREATER_THAN_OR_EQUAL, var("ol_o_id"), literal(11)),
                                f.BinaryOperator(
                                    BOp::CONDITIONAL_AND,
                                    f.BinaryOperator(BOp::EQUAL, var("s_w_id"), literal(1)),
                                    f.BinaryOperator(
                                        BOp::CONDITIONAL_AND,
                                        f.BinaryOperator(BOp::EQUAL, var("s_i_id"), var("ol_i_id")),
                                        f.BinaryOperator(BOp::LESS_THAN, var("s_quantity"), literal(18.00))
                                    )
                                )
                            )
                        )
                    )
                )),
            {
                f.ProjectionExpressionColumn(var("s_i_id"), f.Name("id"))
            }
        ))));
    success();
    /*
     * SELECT s_i_id AS id
     *   FROM
     *     ORDER_LINE
     *       WITH RANGE
     *         ol_w_id = 1,
     *         ol_d_id = 2,
     *         ol_o_id < 31 AND ol_o_id >= 11
     *   JOIN
     *     STOCK
     *       WITH RANGE
     *         s_w_id = 1
     *       ON
     *         s_i_id = ol_i_id -- seek
     *       WHERE
     *         s_quantity < 18.00
     */
    auto projection = query<model::expression::relation::ProjectionExpression>(program.get());
    auto join = cast<model::expression::relation::JoinExpression>(projection->operand());
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    {
        std::cout << to_string(join) << std::endl << std::endl;
        auto relation = extract_relation(join);
        auto&& strategy = relation->join_strategy();
        auto&& seek = strategy.seek_columns();
        auto&& equalities = strategy.equalities();
        ASSERT_EQ(seek.size(), 2);
        {
            auto&& c = seek[0];
            ASSERT_TRUE(c.is_resolved());
            EXPECT_EQ(*c.value(), v::Int(1));
        }
        {
            auto&& c = seek[1];
            ASSERT_FALSE(c.is_resolved());
            EXPECT_EQ(c.variable()->name(), "ol_i_id");
        }
        ASSERT_EQ(equalities.size(), 0);
    }
    {
        cast<model::expression::relation::ScanExpression>(join->left());
    }
    {
        auto s = cast<model::expression::relation::SelectionExpression>(join->right());
        cast<model::expression::relation::ScanExpression>(s->operand());
    }
}
}  // namespace shakujo::analyzer
