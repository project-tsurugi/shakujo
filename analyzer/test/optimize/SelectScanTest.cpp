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
#include "optimize/SelectScan.h"

#include <gtest/gtest.h>

#include "../AnalyzerTestBase.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::optimize {

using ScanOp = binding::ScanStrategy::Kind;
using Dir = common::core::Direction;
using BOp = model::expression::BinaryOperator::Kind;
using UOp = model::expression::UnaryOperator::Kind;

namespace t = common::core::type;
namespace v = common::core::value;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

class SelectScanTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<class T>
    void do_apply(T&& ptr) {
        do_analyze(ptr.get(), true);
        env.reporter().clear();

        Context context { env.binding_context() };
        SelectScan { context }(ptr.get());
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
};

TEST_F(SelectScanTest, trivial) {
    auto&& table = add(common::schema::TableInfo { "testing", {
        { "C1", t::Int(64U, NON_NULL), },
    }});
    auto expr = apply(f.ScanExpression(f.Name("testing")));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);

    auto&& strategy = relation->scan_strategy();
    ASSERT_EQ(strategy.kind(), ScanOp::FULL);
    EXPECT_EQ(&strategy.table(), &table);
}

TEST_F(SelectScanTest, pk_eq) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))
        ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::PREFIX);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 1U);
    EXPECT_EQ(*strategy.prefix()[0], v::Int(1));
}

TEST_F(SelectScanTest, pk_multiple) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
                { "C2" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::EQUAL, var("C2"), literal(2)))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::PREFIX);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 2U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);
    EXPECT_EQ(strategy.key_columns()[1], relation->output().columns()[1]);

    ASSERT_EQ(strategy.prefix().size(), 2U);
    EXPECT_EQ(*strategy.prefix()[0], v::Int(1));
    EXPECT_EQ(*strategy.prefix()[1], v::Int(2));
}

TEST_F(SelectScanTest, pk_lower) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::GREATER_THAN, var("C1"), literal(1))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_TRUE(strategy.lower_suffix());
    EXPECT_EQ(*strategy.lower_suffix().value(), v::Int(1));
    EXPECT_EQ(strategy.lower_suffix().inclusive(), false);

    ASSERT_FALSE(strategy.upper_suffix());
}

TEST_F(SelectScanTest, pk_lower_inclusive) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::GREATER_THAN_OR_EQUAL, var("C1"), literal(1))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_TRUE(strategy.lower_suffix());
    EXPECT_EQ(*strategy.lower_suffix().value(), v::Int(1));
    EXPECT_EQ(strategy.lower_suffix().inclusive(), true);

    ASSERT_FALSE(strategy.upper_suffix());
}

TEST_F(SelectScanTest, pk_upper) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(1))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_FALSE(strategy.lower_suffix());

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Int(1));
    EXPECT_EQ(strategy.upper_suffix().inclusive(), false);
}

TEST_F(SelectScanTest, pk_upper_inclusive) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::LESS_THAN_OR_EQUAL, var("C1"), literal(1))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_FALSE(strategy.lower_suffix());

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Int(1));
    EXPECT_EQ(strategy.upper_suffix().inclusive(), true);
}

TEST_F(SelectScanTest, pk_between) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::GREATER_THAN_OR_EQUAL, var("C1"), literal(0)),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(10)))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_TRUE(strategy.lower_suffix());
    EXPECT_EQ(*strategy.lower_suffix().value(), v::Int(0));
    EXPECT_EQ(strategy.lower_suffix().inclusive(), true);

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Int(10));
    EXPECT_EQ(strategy.upper_suffix().inclusive(), false);
}

TEST_F(SelectScanTest, pk_between_desc) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1", Dir::DESCENDANT },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::GREATER_THAN_OR_EQUAL, var("C1"), literal(0)),
            f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(10)))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_TRUE(strategy.lower_suffix());
    EXPECT_EQ(*strategy.lower_suffix().value(), v::Int(10));
    EXPECT_EQ(strategy.lower_suffix().inclusive(), false);

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Int(0));
    EXPECT_EQ(strategy.upper_suffix().inclusive(), true);
}

TEST_F(SelectScanTest, pk_partial) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
                { "C3" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::PREFIX);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 2U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);
    EXPECT_EQ(strategy.key_columns()[1], relation->output().columns()[2]);

    ASSERT_EQ(strategy.prefix().size(), 1U);
    EXPECT_EQ(*strategy.prefix()[0], v::Int(1));

    ASSERT_FALSE(strategy.lower_suffix());
    ASSERT_FALSE(strategy.upper_suffix());
}

TEST_F(SelectScanTest, pk_range) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
                { "C3" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::CONDITIONAL_AND,
                f.BinaryOperator(BOp::GREATER_THAN_OR_EQUAL, var("C3"), literal(0)),
                f.BinaryOperator(BOp::LESS_THAN, var("C3"), literal(10))))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 2U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);
    EXPECT_EQ(strategy.key_columns()[1], relation->output().columns()[2]);

    ASSERT_EQ(strategy.prefix().size(), 1U);
    EXPECT_EQ(*strategy.prefix()[0], v::Int(1));

    ASSERT_TRUE(strategy.lower_suffix());
    EXPECT_EQ(*strategy.lower_suffix().value(), v::Int(0));
    EXPECT_EQ(strategy.lower_suffix().inclusive(), true);

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Int(10));
    EXPECT_EQ(strategy.upper_suffix().inclusive(), false);
}

TEST_F(SelectScanTest, miss_prefix) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
                { "C3" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C3"), literal(1))
    ));
    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    auto scan = cast<model::expression::relation::ScanExpression>(select->operand());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::FULL);
    EXPECT_EQ(&strategy.table(), &table);

    auto cond = select->condition();
    EXPECT_EQ(bop(cond), BOp::EQUAL);
    EXPECT_EQ(var_of(left(cond)), relation->output().columns()[2]);
    EXPECT_EQ(val_of<v::Int>(right(cond)), 1);
}

TEST_F(SelectScanTest, miss_partial) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
                { "C3" },
            },
        }
    });
    // WHERE C1 = 1 AND C2 > 0 AND C3 < 10
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::CONDITIONAL_AND,
                f.BinaryOperator(BOp::GREATER_THAN, var("C2"), literal(0)),
                f.BinaryOperator(BOp::LESS_THAN, var("C3"), literal(10))))
    ));
    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    auto scan = cast<model::expression::relation::ScanExpression>(select->operand());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 2U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);
    EXPECT_EQ(strategy.key_columns()[1], relation->output().columns()[2]);

    ASSERT_EQ(strategy.prefix().size(), 1U);
    EXPECT_EQ(*strategy.prefix()[0], v::Int(1));

    ASSERT_FALSE(strategy.lower_suffix());

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Int(10));
    EXPECT_EQ(strategy.upper_suffix().inclusive(), false);

    auto cond = select->condition();
    EXPECT_EQ(bop(cond), BOp::GREATER_THAN);
    EXPECT_EQ(var_of(left(cond)), relation->output().columns()[1]);
    EXPECT_EQ(val_of<v::Int>(right(cond)), 0);
}

TEST_F(SelectScanTest, sk_eq) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        { // PK
            {
                { "C1" },
                { "C2" },
            },
        },
        { // SKs
            {
                "S",
                {
                    { "C2" },
                }
            }
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::EQUAL, var("C2"), literal(1))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::PREFIX);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.find_secondary_index("S"));
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[1]);

    ASSERT_EQ(strategy.prefix().size(), 1U);
    EXPECT_EQ(*strategy.prefix()[0], v::Int(1));
}

TEST_F(SelectScanTest, select) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NON_NULL), },
            { "C2", t::Int(64U, NON_NULL), },
            { "C3", t::Int(64U, NON_NULL), },
        },
        { // PK
        },
        { // SKs
            {
                "S0",
                {
                    { "C1" },
                    { "C2" },
                }
            },
            {
                "S1",
                {
                    { "C1" },
                    { "C3" },
                }
            },
            {
                "S2",
                {
                    { "C2" },
                }
            },
        }
    });
    // C1 = 1 AND C2 > 0 AND C2 < 10 AND C3 = 100
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::CONDITIONAL_AND,
            f.BinaryOperator(BOp::EQUAL, var("C1"), literal(1)),
            f.BinaryOperator(BOp::CONDITIONAL_AND,
                f.BinaryOperator(BOp::GREATER_THAN, var("C2"), literal(0)),
                f.BinaryOperator(BOp::CONDITIONAL_AND,
                    f.BinaryOperator(BOp::LESS_THAN, var("C2"), literal(10)),
                    f.BinaryOperator(BOp::EQUAL, var("C3"), literal(100)))))
    ));
    auto select = cast<model::expression::relation::SelectionExpression>(expr.get());
    auto scan = cast<model::expression::relation::ScanExpression>(select->operand());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::PREFIX);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.find_secondary_index("S1"));
    ASSERT_EQ(strategy.key_columns().size(), 2U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);
    EXPECT_EQ(strategy.key_columns()[1], relation->output().columns()[2]);

    ASSERT_EQ(strategy.prefix().size(), 2U);
    EXPECT_EQ(*strategy.prefix()[0], v::Int(1));
    EXPECT_EQ(*strategy.prefix()[1], v::Int(100));
}

TEST_F(SelectScanTest, nulls_asc) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NULLABLE), },
        },
        { // PK
            {
                { "C1" },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(0))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.primary_index());
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_TRUE(strategy.lower_suffix());
    EXPECT_EQ(*strategy.lower_suffix().value(), v::Null());
    EXPECT_EQ(strategy.lower_suffix().inclusive(), false);

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Int(0));
    EXPECT_EQ(strategy.upper_suffix().inclusive(), false);
}

TEST_F(SelectScanTest, nulls_desc) {
    auto&& table = add({
        "testing",
        {
            { "C1", t::Int(64U, NULLABLE), },
        },
        { // PK
        },
        { // SKs
            {
                "S",
                {
                    { "C1", Dir::DESCENDANT },
                },
            },
        }
    });
    auto expr = apply(f.SelectionExpression(
        f.ScanExpression(f.Name("testing")),
        f.BinaryOperator(BOp::LESS_THAN, var("C1"), literal(0))
    ));
    auto scan = cast<model::expression::relation::ScanExpression>(expr.get());

    auto relation = extract_relation(scan);
    auto&& strategy = relation->scan_strategy();

    ASSERT_EQ(strategy.kind(), ScanOp::RANGE);
    EXPECT_EQ(&strategy.table(), &table);
    EXPECT_EQ(&strategy.index(), &table.find_secondary_index("S"));
    ASSERT_EQ(strategy.key_columns().size(), 1U);
    EXPECT_EQ(strategy.key_columns()[0], relation->output().columns()[0]);

    ASSERT_EQ(strategy.prefix().size(), 0U);

    ASSERT_TRUE(strategy.lower_suffix());
    EXPECT_EQ(*strategy.lower_suffix().value(), v::Int(0));
    EXPECT_EQ(strategy.lower_suffix().inclusive(), false);

    ASSERT_TRUE(strategy.upper_suffix());
    EXPECT_EQ(*strategy.upper_suffix().value(), v::Null());
    EXPECT_EQ(strategy.upper_suffix().inclusive(), false);
}

}  // namespace shakujo::analyzer::optimize
