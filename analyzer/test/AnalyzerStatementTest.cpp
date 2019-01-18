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
#include "shakujo/analyzer/Analyzer.h"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

#include "AnalyzerTestBase.h"

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/type/Float.h"
#include "shakujo/common/core/type/Char.h"
#include "shakujo/common/core/type/String.h"
#include "shakujo/common/core/type/Tuple.h"
#include "shakujo/common/core/type/Null.h"
#include "shakujo/common/core/type/Error.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/core/value/String.h"
#include "shakujo/common/core/value/Null.h"

namespace shakujo::analyzer {

using namespace shakujo::model;
using namespace shakujo::common;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using common::util::equals;
using common::util::is_defined;
using common::util::is_valid;

class AnalyzerStatementTest : public AnalyzerTestBase, public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> analyze(std::unique_ptr<T> node) {
        return analyze_unmanaged<T>(std::move(node));
    }

    using AnalyzerTestBase::get;

    template<typename T>
    T* get(std::unique_ptr<statement::BlockStatement>& block, std::size_t location) {
        return as<T>(block->elements()[location]);
    }
};

TEST_F(AnalyzerStatementTest, empty) {
    analyze(f.EmptyStatement());
    success();
}

TEST_F(AnalyzerStatementTest, expression) {
    auto stmt = analyze(f.ExpressionStatement(literal(1, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(stmt->body()));
}

TEST_F(AnalyzerStatementTest, block) {
    auto stmt = analyze(f.BlockStatement({
        f.ExpressionStatement(literal(1, 32U, NON_NULL)),
        f.ExpressionStatement(literal(2, 32U, NON_NULL)),
        f.ExpressionStatement(literal(3, 32U, NON_NULL)),
    }));
    success();

    auto* s0 = get<statement::ExpressionStatement>(stmt, 0);
    auto* s1 = get<statement::ExpressionStatement>(stmt, 1);
    auto* s2 = get<statement::ExpressionStatement>(stmt, 2);

    EXPECT_EQ(t::Int(32U, NON_NULL), type(s0->body()));
    EXPECT_EQ(t::Int(32U, NON_NULL), type(s1->body()));
    EXPECT_EQ(t::Int(32U, NON_NULL), type(s2->body()));

    EXPECT_EQ(1, get<v::Int>(s0->body()));
    EXPECT_EQ(2, get<v::Int>(s1->body()));
    EXPECT_EQ(3, get<v::Int>(s2->body()));
}

TEST_F(AnalyzerStatementTest, local_variable) {
    auto block = analyze(f.BlockStatement({
            f.LocalVariableDeclaration(f.Name("v"), f.Int32Type()),
            f.ExpressionStatement(f.AssignExpression(f.Name("v"), literal(1, 32U, NULLABLE))),
    }));
    success();

    auto* decl = get<statement::LocalVariableDeclaration>(block, 0);
    auto var = extract_var(decl);
    EXPECT_EQ(core::Name("v"), var->name());
    EXPECT_EQ(t::Int(32U, NULLABLE), *var->type());
    EXPECT_FALSE(is_defined(var->value()));

    auto* assign = get<expression::AssignExpression>(block, 1);
    auto ref = extract_var(assign);
    EXPECT_EQ(var->id(), ref->id());
}

TEST_F(AnalyzerStatementTest, local_variable_init) {
    auto block = analyze(f.BlockStatement({
            f.LocalVariableDeclaration(f.Name("v"), {}, literal(1, 32U, NON_NULL)),
            f.ExpressionStatement(f.AssignExpression(f.Name("v"), literal(2, 32U, NON_NULL))),
    }));
    success();

    auto* decl = get<statement::LocalVariableDeclaration>(block, 0);
    auto var = extract_var(decl);
    EXPECT_EQ(core::Name("v"), var->name());
    EXPECT_EQ(t::Int(32U, NON_NULL), *var->type());
    EXPECT_FALSE(var->value());

    auto* assign = get<expression::AssignExpression>(block, 1);
    auto ref = extract_var(assign);
    EXPECT_EQ(var->id(), ref->id());
}

TEST_F(AnalyzerStatementTest, local_variable_invalid) {
    auto block = analyze(f.BlockStatement({
            f.LocalVariableDeclaration(f.Name("v"), {}, {}),
            f.ExpressionStatement(f.AssignExpression(f.Name("v"), literal(1, 32U, NULLABLE))),
    }));
    success(false);
    // syntactic error
}

TEST_F(AnalyzerStatementTest, local_variable_inconsistent) {
    auto block = analyze(f.BlockStatement({
            f.LocalVariableDeclaration(f.Name("v"), f.Int32Type(), literal("x", NULLABLE)),
    }));
    success(false);

    auto* decl = get<statement::LocalVariableDeclaration>(block, 0);
    auto var = extract_var(decl, true);
    EXPECT_EQ(core::Name("v"), var->name());
    EXPECT_FALSE(is_valid(var->type()));
    EXPECT_FALSE(is_valid(var->value()));
}

TEST_F(AnalyzerStatementTest, local_variable_conflict) {
    auto block = analyze(f.BlockStatement({
            f.LocalVariableDeclaration(f.Name("v"), f.Int32Type()),
            f.LocalVariableDeclaration(f.Name("v"), f.Int32Type()),
            f.ExpressionStatement(f.AssignExpression(f.Name("v"), literal(2, 32U, NON_NULL))),
    }));
    success(false);

    auto* d1 = get<statement::LocalVariableDeclaration>(block, 0);
    auto v1 = extract_var(d1, true);
    EXPECT_TRUE(is_valid(v1));

    auto* d2 = get<statement::LocalVariableDeclaration>(block, 1);
    auto v2 = extract_var(d2, true);
    EXPECT_TRUE(is_valid(v2));

    auto* assign = get<expression::AssignExpression>(block, 2);
    auto ref = extract_var(assign, true);
    EXPECT_FALSE(ref->id());
}

TEST_F(AnalyzerStatementTest, branch) {
    auto stmt = analyze(f.BranchStatement(
            literal(true, NON_NULL),
            f.ExpressionStatement(literal(10, 32U, NON_NULL))));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(stmt->condition()));
    EXPECT_EQ(t::Int(32U, NON_NULL), type(as<expression::Expression>(stmt->then_statement())));
}

TEST_F(AnalyzerStatementTest, branch_else) {
    auto stmt = analyze(f.BranchStatement(
            literal(true, NON_NULL),
            f.ExpressionStatement(literal(10, 32U, NON_NULL)),
            f.ExpressionStatement(literal(20, 64U, NON_NULL))));
    success();

    EXPECT_EQ(t::Bool(NON_NULL), type(stmt->condition()));
    EXPECT_EQ(t::Int(32U, NON_NULL), type(as<expression::Expression>(stmt->then_statement())));
    EXPECT_EQ(t::Int(64U, NON_NULL), type(as<expression::Expression>(stmt->else_statement())));
}

TEST_F(AnalyzerStatementTest, branch_inconsistent) {
    add_variable("v", t::Vector(t::Bool()));
    auto stmt = analyze(f.BranchStatement(
            var("v"),
            f.ExpressionStatement(literal(10, 32U, NON_NULL))));
    success(false);
}

TEST_F(AnalyzerStatementTest, log) {
    using Level = statement::LogStatement::Level;
    auto stmt = analyze(f.LogStatement(Level::INFO, literal(1, 32U, NON_NULL)));
    success();

    EXPECT_EQ(t::Int(32U, NON_NULL), type(stmt->value()));
}
}  // namespace shakujo::analyzer
