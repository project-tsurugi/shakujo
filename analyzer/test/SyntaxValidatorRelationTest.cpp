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
#include "SyntaxValidatorTestBase.h"

#include <gtest/gtest.h>

namespace shakujo::analyzer {

using namespace shakujo::model;
using namespace shakujo::common;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

class SyntaxValidatorRelationTest : public SyntaxValidatorTestBase, public ::testing::Test {
public:
    template<typename T>
    void validate(std::unique_ptr<T> node) {
        Reporter reporter;
        analyze(reporter, node.get());
        ASSERT_FALSE(reporter.saw_error()) << diagnostics(reporter);
    }
    template<typename T>
    void should_error(std::unique_ptr<T> node) {
        Reporter reporter;
        analyze(reporter, node.get());
        ASSERT_TRUE(reporter.saw_error()) << diagnostics(reporter);
    }

    std::unique_ptr<expression::Expression> literal(int value = 0) {
        return f.Literal(t::Int(64U, NON_NULL), value);
    }
};

TEST_F(SyntaxValidatorRelationTest, ScanExpression) {
    validate(f.ScanExpression(f.Name("a")));
    validate(f.ScanExpression(f.Name("a", "b", "c")));
    validate(f.ScanExpression(f.Name("a"), f.SimpleName("alias")));
    should_error(f.ScanExpression({}));
}

TEST_F(SyntaxValidatorRelationTest, ProjectionExpression) {
    validate(f.ProjectionExpression(
        literal(),
        {},
        {
            f.ProjectionExpressionColumn(literal()),
        }
    ));
    validate(f.ProjectionExpression(
        literal(),
        {},
        {
            f.ProjectionExpressionColumn(literal(1), f.SimpleName("a")),
            f.ProjectionExpressionColumn(literal(2), f.SimpleName("b")),
            f.ProjectionExpressionColumn(literal(3), f.SimpleName("c")),
        }
    ));
    validate(f.ProjectionExpression(
        literal(),
        {f.EmptyStatement()},
        {
            f.ProjectionExpressionColumn(literal(), f.SimpleName("c")),
        },
        f.SimpleName("p")
    ));
    should_error(f.ProjectionExpression(
        literal(),
        {},
        {} // no columns
    ));
    should_error(f.ProjectionExpression(
        {},
        {},
        {
            f.ProjectionExpressionColumn(literal()),
        }
    ));
    should_error(f.ProjectionExpression(
        literal(),
        { std::unique_ptr<model::statement::Statement>() },
        {
            f.ProjectionExpressionColumn(literal()),
        }
    ));
    should_error(f.ProjectionExpression(
        literal(),
        {},
        {
            f.ProjectionExpressionColumn({}),
        }
    ));
}
TEST_F(SyntaxValidatorRelationTest, JoinExpression) {
    using Kind = model::expression::relation::JoinExpression::Kind;
    validate(f.JoinExpression(
        Kind::INNER,
        literal(),
        literal(),
        {}
    ));
    validate(f.JoinExpression(
        Kind::INNER,
        literal(),
        literal(),
        literal()
    ));
    should_error(f.JoinExpression(
        Kind::INNER,
        {},
        literal(),
        literal()
    ));
    should_error(f.JoinExpression(
        Kind::INNER,
        literal(),
        {},
        literal()
    ));
    should_error(f.JoinExpression(
        Kind::NATURAL_INNER,
        literal(),
        literal(),
        literal()
    ));
}

TEST_F(SyntaxValidatorRelationTest, SelectionExpression) {
    validate(f.SelectionExpression(literal(), literal()));
    should_error(f.SelectionExpression({}, literal()));
    should_error(f.SelectionExpression(literal(), {}));
}
}  // namespace shakujo::analyzer
