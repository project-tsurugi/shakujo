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
#include "optimize/SelectJoin.h"

#include <gtest/gtest.h>

#include "optimize/PredicatePushDown.h"
#include "optimize/SelectScan.h"
#include "optimize/SimplifyCast.h"
#include "optimize/FixRelationInfo.h"

#include "../AnalyzerTestBase.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::optimize {

using JoinKind = model::expression::relation::JoinExpression::Kind;
using BOp = model::expression::BinaryOperator::Kind;

namespace t = common::core::type;
namespace v = common::core::value;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

class SelectJoinTest : public AnalyzerTestBase, public ::testing::Test {
public:
    std::unique_ptr<model::expression::Expression> apply(std::unique_ptr<model::expression::Expression> expr) {
        auto manager = manage<model::expression::Expression>(std::move(expr));
        do_analyze(manager.get(), true);
        env.reporter().clear();

        Context context { env.binding_context() };
        PredicatePushDown { context }(manager.get());
        SimplifyCast { context}(manager.get());
        SelectScan { context }(manager.get());
        SimplifyCast { context}(manager.get());
        SelectJoin { context }(manager.get());
        FixRelationInfo { context }(manager.get());

        ensure(manager.get());

        return manager.release();
    }

    std::shared_ptr<binding::VariableBinding> var_of(model::expression::Expression* node) {
        model::expression::Expression* current = node;
        while (auto cast = dynamic_pointer_cast_if<model::expression::ImplicitCast>(current)) {
            current = cast->operand();
        }
        return extract_var(cast<model::expression::VariableReference>(current));
    }

    template<class T, class U>
    auto cast(U* ptr) -> decltype(dynamic_pointer_cast<T>(ptr)) {
        if (auto&& r = dynamic_pointer_cast_if<T>(ptr)) {
            return r;
        }
        throw std::domain_error(common::util::to_string("cast to ", typeid(T).name(), " : ", to_string(ptr)));
    }

    template<class C, class T>
    static bool contains(C const& container, T const& value) {
        return container.find(value) != container.end();
    }
};

TEST_F(SelectJoinTest, cross) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::CROSS,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        {}
        ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto&& strategy = relation->join_strategy();

    ASSERT_EQ(strategy.seek_columns().size(), 0);
    ASSERT_EQ(strategy.equalities().size(), 0);
}

TEST_F(SelectJoinTest, simple) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, equalities) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1")),
            f.BinaryOperator(BOp::EQUAL, var("T1", "C2"), var("T2", "C2")))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 1);
    EXPECT_TRUE(contains(equalities, std::make_pair(left->output().columns()[1], right->output().columns()[1])));
}

TEST_F(SelectJoinTest, equalities_swap) {
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
    add(common::schema::TableInfo { "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
        }
    });
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1")),
            f.BinaryOperator(BOp::EQUAL, var("T1", "C2"), var("T2", "C2")))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T2");
    ASSERT_EQ(right->scan_strategy().table().name(), "T1");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 1);
    EXPECT_TRUE(contains(equalities, std::make_pair(left->output().columns()[1], right->output().columns()[1])));
}

TEST_F(SelectJoinTest, right_pk) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C2"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[1]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, left_pk) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C2"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T2");
    ASSERT_EQ(right->scan_strategy().table().name(), "T1");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[1]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, seek_partial) {
    add(common::schema::TableInfo { "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
            {
                { "C1" },
                { "C2" },
                { "C3" },
            }
        }
    });
    add(common::schema::TableInfo { "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
            {
                { "C1" },
                { "C2" },
                { "C3" },
            }
        }
    });
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.SelectionExpression(
            f.ScanExpression(f.Name("T2")),
            f.BinaryOperator(BOp::EQUAL, var("T2", "C1"), literal(100))),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C2"), var("T2", "C2"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 2);
    {
        auto&& c = seek[0];
        ASSERT_TRUE(c.is_resolved());
        EXPECT_EQ(*c.value(), v::Int(100));
    }
    {
        auto&& c = seek[1];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[1]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, left_outer) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::LEFT_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, left_outer_miss) {
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
    add(common::schema::TableInfo { "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
        }
    });
    auto expr = apply(f.JoinExpression(
        JoinKind::LEFT_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 0);
    ASSERT_EQ(equalities.size(), 1);
    EXPECT_TRUE(contains(equalities, std::make_pair(left->output().columns()[0], right->output().columns()[0])));
}

TEST_F(SelectJoinTest, right_outer) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::RIGHT_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T2");
    ASSERT_EQ(right->scan_strategy().table().name(), "T1");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, right_outer_miss) {
    add(common::schema::TableInfo { "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
        }
    });
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::RIGHT_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 0);
    ASSERT_EQ(equalities.size(), 1);
    EXPECT_TRUE(contains(equalities, std::make_pair(left->output().columns()[0], right->output().columns()[0])));
}

TEST_F(SelectJoinTest, full_outer_miss) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::FULL_OUTER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 0);
    ASSERT_EQ(equalities.size(), 1);
    EXPECT_TRUE(contains(equalities, std::make_pair(left->output().columns()[0], right->output().columns()[0])));
}

TEST_F(SelectJoinTest, via_selection) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.SelectionExpression(
            f.ScanExpression(f.Name("T2")),
            f.BinaryOperator(BOp::EQUAL, var("T2", "C2"), literal(0))),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, via_projection) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ProjectionExpression(
            f.ScanExpression(f.Name("T2")),
            {
                f.ProjectionExpressionColumn(var("C1"), f.Name("P1")),
                f.ProjectionExpressionColumn(var("C2"), f.Name("P2")),
            }),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("P1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, via_projection_removed) {
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
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ProjectionExpression(
            f.ScanExpression(f.Name("T2")),
            {
                f.ProjectionExpressionColumn(var("C3"), f.Name("P1")),
                f.ProjectionExpressionColumn(var("C2"), f.Name("P2")),
            }),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("P1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(right->scan_strategy().table().name(), "T1");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, via_order) {
    add(common::schema::TableInfo { "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
        }
    });
    add(common::schema::TableInfo { "T2",
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
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.OrderExpression(
            f.ScanExpression(f.Name("T2")),
            {
                f.OrderExpressionElement(var("C1")),
                f.OrderExpressionElement(var("C2")),
            }
            ),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto through = cast<model::expression::relation::OrderExpression>(join->right());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(through->operand());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, via_distinct) {
    add(common::schema::TableInfo { "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
        }
    });
    add(common::schema::TableInfo { "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
            {
                { "C1" },
                { "C2" },
            }
        }
    });
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.DistinctExpression(
            f.ProjectionExpression(
                f.ScanExpression(f.Name("T2")),
                {
                    f.ProjectionExpressionColumn(var("C1"), f.Name("P1")),
                    f.ProjectionExpressionColumn(var("C3"), f.Name("P2")),
                })),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("P1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto distinct = cast<model::expression::relation::DistinctExpression>(join->right());
    auto projection = cast<model::expression::relation::ProjectionExpression>(distinct->operand());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(projection->operand());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

TEST_F(SelectJoinTest, compete) {
    add(common::schema::TableInfo { "T1",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
            {
                { "C1" },
                { "C2" },
            }
        }
    });
    add(common::schema::TableInfo { "T2",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        {  // pk
            {
                { "C1" },
                { "C2" },
            }
        }
    });
    auto expr = apply(f.JoinExpression(
        JoinKind::INNER,
        f.ScanExpression(f.Name("T1")),
        f.ScanExpression(f.Name("T2")),
        f.BinaryOperator(BOp::EQUAL, var("T1", "C1"), var("T2", "C1"))
    ));
    auto join = cast<model::expression::relation::JoinExpression>(expr.get());
    auto relation = extract_relation(join);
    auto left = extract_relation(join->left());
    auto right = extract_relation(join->right());
    ASSERT_EQ(left->scan_strategy().table().name(), "T1");
    ASSERT_EQ(right->scan_strategy().table().name(), "T2");

    auto&& strategy = relation->join_strategy();

    auto&& seek = strategy.seek_columns();
    auto&& equalities = strategy.equalities();
    ASSERT_EQ(seek.size(), 1);
    {
        auto&& c = seek[0];
        ASSERT_FALSE(c.is_resolved());
        EXPECT_EQ(c.variable(), left->output().columns()[0]);
    }

    ASSERT_EQ(equalities.size(), 0);
}

}  // namespace shakujo::analyzer::optimize
