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

class SyntaxValidatorExpressionTest : public SyntaxValidatorTestBase, public ::testing::Test {
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

TEST_F(SyntaxValidatorExpressionTest, SimpleName) {
    // always OK
    validate(f.VariableReference(f.SimpleName("a")));
}

TEST_F(SyntaxValidatorExpressionTest, QualifiedName) {
    validate(f.VariableReference(f.QualifiedName(f.Name("q"), f.SimpleName("n"))));
    should_error(f.VariableReference(f.QualifiedName({}, f.SimpleName("n"))));
    should_error(f.VariableReference(f.QualifiedName(f.Name("q"), {})));
}

TEST_F(SyntaxValidatorExpressionTest, Literal) {
    validate(f.Literal(t::Int(32U), 100));
    should_error(f.Literal(std::make_unique<t::Int>(32U), {}));
    should_error(f.Literal({}, std::make_unique<v::Int>(100)));
}

TEST_F(SyntaxValidatorExpressionTest, VariableReference) {
    validate(f.VariableReference(f.Name("a")));
    validate(f.VariableReference(f.Name("a", "b", "c")));
    should_error(f.VariableReference({}));
}

TEST_F(SyntaxValidatorExpressionTest, BlockExpression) {
    validate(f.BlockExpression({}, literal()));
    validate(f.BlockExpression({f.EmptyStatement()}, literal()));
    should_error(f.BlockExpression({ std::unique_ptr<model::statement::Statement>() }, literal()));
    should_error(f.BlockExpression({}, {}));
}

TEST_F(SyntaxValidatorExpressionTest, UnaryOperator) {
    using Op = expression::UnaryOperator::Kind;
    validate(f.UnaryOperator(Op::PLUS, literal()));
    should_error(f.UnaryOperator(Op::INVALID, literal()));
    should_error(f.UnaryOperator(Op::PLUS, {}));
}

TEST_F(SyntaxValidatorExpressionTest, BinaryOperator) {
    using Op = expression::BinaryOperator::Kind;
    validate(f.BinaryOperator(Op::ADD, literal(), literal()));
    should_error(f.BinaryOperator(Op::INVALID, literal(), literal()));
    should_error(f.BinaryOperator(Op::ADD, {}, literal()));
    should_error(f.BinaryOperator(Op::ADD, literal(), {}));
}

TEST_F(SyntaxValidatorExpressionTest, AssignExpression) {
    validate(f.AssignExpression(f.Name("a"), literal()));
    validate(f.AssignExpression(f.Name("a", "b", "c"), literal()));
    should_error(f.AssignExpression({}, literal()));
    should_error(f.AssignExpression(f.Name("a"), {}));
}

TEST_F(SyntaxValidatorExpressionTest, TupleElementLoadExpression) {
    validate(f.TupleElementLoadExpression(f.VariableReference(f.Name("a")), f.Index(f.Name("c"), 1U)));
    validate(f.TupleElementLoadExpression(f.VariableReference(f.Name("a")), f.Index(f.Name("c"))));
    validate(f.TupleElementLoadExpression(f.VariableReference(f.Name("a")), f.Index(1U)));
    should_error(f.TupleElementLoadExpression({}, f.Index(1U)));
    should_error(f.TupleElementLoadExpression(f.VariableReference(f.Name("a")), f.Index()));
}

TEST_F(SyntaxValidatorExpressionTest, TupleElementStoreExpression) {
    validate(f.TupleElementStoreExpression(f.VariableReference(f.Name("a")), f.Index(f.Name("c"), 1U), literal()));
    validate(f.TupleElementStoreExpression(f.VariableReference(f.Name("a")), f.Index(f.Name("c")), literal()));
    validate(f.TupleElementStoreExpression(f.VariableReference(f.Name("a")), f.Index(1U), literal()));
    should_error(f.TupleElementStoreExpression({}, f.Index(1U), literal()));
    should_error(f.TupleElementStoreExpression(f.VariableReference(f.Name("a")), f.Index(), literal()));
    should_error(f.TupleElementStoreExpression(f.VariableReference(f.Name("a")), f.Index(1U), {}));
}
}  // namespace shakujo::analyzer
