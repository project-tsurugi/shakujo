/*
 * Copyright 2018-2023 tsurugi project..
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
#include "shakujo/analyzer/Analyzer.h"

#include <gtest/gtest.h>

#include "AnalyzerTestBase.h"

namespace shakujo::analyzer {

using namespace shakujo::model;
using namespace shakujo::common;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using common::util::equals;
using common::util::is_valid;

class AnalyzerProgramTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> analyze(std::unique_ptr<T> node) {
        return analyze_unmanaged<T>(std::move(node));
    }
};

TEST_F(AnalyzerProgramTest, simple) {
    auto prog = analyze(f.Program(
            {},
            f.EmptyStatement()));
    success();
}

TEST_F(AnalyzerProgramTest, body) {
    auto prog = analyze(f.Program(
            {},
            f.ExpressionStatement(literal(1, 32U, NON_NULL))));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(as<expression::Expression>(prog->main())));
}

TEST_F(AnalyzerProgramTest, global_variable) {
    auto prog = analyze(f.Program(
            {
                    f.GlobalVariableDeclaration(f.Name("v"), f.Int32Type()),
            },
            f.ExpressionStatement(f.AssignExpression(f.Name("v"), literal(1, 32U, NULLABLE)))
    ));
    success();

    auto& decls = prog->declarations();

    ASSERT_EQ(1U, decls.size());
    auto decl = as<program::GlobalVariableDeclaration>(decls[0]);
    auto var = extract_var(decl);
    EXPECT_EQ(core::Name("v"), var->name());
    EXPECT_EQ(t::Int(32U, NULLABLE), *var->type());
    EXPECT_FALSE(is_valid(var->value()));

    auto assign = as<expression::AssignExpression>(prog->main());
    auto ref = extract_var(assign);
    EXPECT_EQ(var->id(), ref->id());
}

TEST_F(AnalyzerProgramTest, global_variable_init) {
    auto prog = analyze(f.Program(
            {
                    f.GlobalVariableDeclaration(f.Name("v"), {}, literal(1, 32U, NON_NULL)),
            },
            f.ExpressionStatement(f.AssignExpression(f.Name("v"), literal(2, 32U, NON_NULL)))
    ));
    success();

    auto& decls = prog->declarations();
    ASSERT_EQ(1U, decls.size());
    auto decl = as<program::GlobalVariableDeclaration>(decls[0]);
    auto var = extract_var(decl);
    EXPECT_EQ(core::Name("v"), var->name());
    EXPECT_EQ(t::Int(32U, NON_NULL), *var->type());
    EXPECT_FALSE(is_valid(var->value()));

    auto assign = as<expression::AssignExpression>(prog->main());
    auto ref = extract_var(assign);
    EXPECT_EQ(var->id(), ref->id());
}

TEST_F(AnalyzerProgramTest, global_variable_invalid) {
    auto prog = analyze(f.Program(
            {
                    f.GlobalVariableDeclaration(f.Name("v"), {}, {}),
            },
            f.EmptyStatement()
    ));
    success(false);
    // syntactic error
}

TEST_F(AnalyzerProgramTest, global_variable_invalid_inconsistent) {
    auto prog = analyze(f.Program(
            {
                    f.GlobalVariableDeclaration(f.Name("v"), f.Int32Type(), literal("x")),
            },
            f.EmptyStatement()
    ));
    success(false);

    auto& decls = prog->declarations();
    ASSERT_EQ(1U, decls.size());
    auto decl = as<program::GlobalVariableDeclaration>(decls[0]);
    auto var = extract_var(decl, true);
    EXPECT_EQ(core::Name("v"), var->name());
    EXPECT_FALSE(is_valid(var->type()));
    EXPECT_FALSE(is_valid(var->value()));
}

TEST_F(AnalyzerProgramTest, global_variable_invalid_conflict) {
    auto prog = analyze(f.Program(
            {
                    f.GlobalVariableDeclaration(f.Name("v"), f.Int32Type()),
                    f.GlobalVariableDeclaration(f.Name("v"), f.Int32Type()),
            },
            f.ExpressionStatement(f.AssignExpression(f.Name("v"), literal(1, 32U, NON_NULL)))
    ));
    success(false);

    auto& decls = prog->declarations();
    ASSERT_EQ(2U, decls.size());
    auto d1 = as<program::GlobalVariableDeclaration>(decls[0]);
    auto v1 = extract_var(d1, true);
    EXPECT_TRUE(is_valid(v1));

    auto d2 = as<program::GlobalVariableDeclaration>(decls[1]);
    auto v2 = extract_var(d2, true);
    EXPECT_TRUE(is_valid(v2));

    auto assign = as<expression::AssignExpression>(prog->main());
    auto ref = extract_var(assign, true);
    EXPECT_FALSE(ref->id());
}
}  // namespace shakujo::analyzer
