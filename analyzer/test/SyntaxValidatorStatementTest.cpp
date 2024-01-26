/*
 * Copyright 2018-2024 Project Tsurugi.
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

class SyntaxValidatorStatementTest : public SyntaxValidatorTestBase, public ::testing::Test {
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

TEST_F(SyntaxValidatorStatementTest, EmptyStatement) {
    validate(f.EmptyStatement());
}

TEST_F(SyntaxValidatorStatementTest, BlockStatement) {
    validate(f.BlockStatement());
    validate(f.BlockStatement({
            f.EmptyStatement()
    }));
    validate(f.BlockStatement({
            f.EmptyStatement(),
            f.EmptyStatement(),
            f.EmptyStatement(),
    }));
    should_error(f.BlockStatement({
            std::unique_ptr<model::statement::Statement>(),
    }));
}

TEST_F(SyntaxValidatorStatementTest, LocalVariableDeclaration) {
    using Attr = statement::LocalVariableDeclaration::Attribute;
    validate(f.LocalVariableDeclaration(f.Name("a"), f.Int64Type()));
    validate(f.LocalVariableDeclaration(f.Name("a", "b", "c"), f.Int64Type()));
    validate(f.LocalVariableDeclaration(f.Name("a"), f.Int64Type(), literal(), { Attr::CONST }));
    should_error(f.LocalVariableDeclaration({}, f.Int64Type()));
    should_error(f.LocalVariableDeclaration(f.Name("a"), {}));
}

TEST_F(SyntaxValidatorStatementTest, BranchStatement) {
    validate(f.BranchStatement(literal(), f.EmptyStatement()));
    validate(f.BranchStatement(literal(), f.EmptyStatement(), f.EmptyStatement()));
    should_error(f.BranchStatement({}, f.EmptyStatement()));
    should_error(f.BranchStatement(literal(), {}));
}

TEST_F(SyntaxValidatorStatementTest, ExpressionStatement) {
    validate(f.ExpressionStatement(literal()));
    should_error(f.ExpressionStatement({}));
}

TEST_F(SyntaxValidatorStatementTest, LogStatement) {
    using Level = statement::LogStatement::Level;
    validate(f.LogStatement(Level::INFO, literal()));
    should_error(f.LogStatement(Level::INFO, {}));
}
}  // namespace shakujo::analyzer
