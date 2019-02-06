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

class SyntaxValidatorDmlTest : public SyntaxValidatorTestBase, public ::testing::Test {
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

TEST_F(SyntaxValidatorDmlTest, EmitStatement) {
    validate(f.EmitStatement(literal()));
    should_error(f.EmitStatement({}));
}

TEST_F(SyntaxValidatorDmlTest, InsertValuesStatement) {
    validate(f.InsertValuesStatement(
        f.Name("TBL"),
        {
            f.InsertValuesStatementColumn({}, literal()),
        }));
    validate(f.InsertValuesStatement(
        f.Name("TBL"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C1"), literal()),
        }));
    validate(f.InsertValuesStatement(
        f.Name("TBL"),
        {
            f.InsertValuesStatementColumn(f.SimpleName("C1"), literal(1)),
            f.InsertValuesStatementColumn(f.SimpleName("C2"), literal(2)),
            f.InsertValuesStatementColumn(f.SimpleName("C3"), literal(3)),
        }));
    validate(f.InsertValuesStatement(
        f.Name("TBL"),
        {
            f.InsertValuesStatementColumn({}, literal(1)),
            f.InsertValuesStatementColumn({}, literal(2)),
            f.InsertValuesStatementColumn({}, literal(3)),
        }));
    should_error(f.InsertValuesStatement(
        {},
        {
            f.InsertValuesStatementColumn({}, literal(1)),
        }));
    should_error(f.InsertValuesStatement(
        f.Name("TBL"),
        {}));
    should_error(f.InsertValuesStatement(
        f.Name("TBL"),
        {
            f.InsertValuesStatementColumn({}, {}),
        }));
    should_error(f.InsertValuesStatement(
        f.Name("TBL"),
        {
            f.InsertValuesStatementColumn({}, literal(1)),
            f.InsertValuesStatementColumn(f.SimpleName("C2"), literal(2)),
            f.InsertValuesStatementColumn({}, literal(3)),
        }));
}

TEST_F(SyntaxValidatorDmlTest, UpdateStatement) {
    validate(f.UpdateStatement(
        literal(1),
        f.Name("T"),
        {},
        {
            f.UpdateStatementColumn(f.Name("a"), literal(2)),
        }));
    validate(f.UpdateStatement(
        literal(1),
        f.Name("T"),
        {},
        {
            f.UpdateStatementColumn(f.Name("a"), literal(2)),
            f.UpdateStatementColumn(f.Name("b"), literal(3)),
            f.UpdateStatementColumn(f.Name("c"), literal(4)),
        }));
    should_error(f.UpdateStatement(
        {},
        f.Name("T"),
        {},
        {
            f.UpdateStatementColumn(f.Name("a"), literal(2)),
        }));
    should_error(f.UpdateStatement(
        literal(1),
        {},
        {},
        {
            f.UpdateStatementColumn(f.Name("a"), literal(2)),
        }));
    should_error(f.UpdateStatement(
        literal(1),
        f.Name("T"),
        {},
        {}));
    should_error(f.UpdateStatement(
        literal(1),
        f.Name("T"),
        {},
        {
            f.UpdateStatementColumn({}, literal(2)),
        }));
    should_error(f.UpdateStatement(
        literal(1),
        f.Name("T"),
        {},
        {
            f.UpdateStatementColumn(f.Name("a"), {}),
        }));
}

TEST_F(SyntaxValidatorDmlTest, DeleteStatement) {
    validate(f.DeleteStatement(literal(1), f.Name("T")));
    should_error(f.DeleteStatement({}, f.Name("T")));
    should_error(f.DeleteStatement(literal(1), {}));
}
}  // namespace shakujo::analyzer
