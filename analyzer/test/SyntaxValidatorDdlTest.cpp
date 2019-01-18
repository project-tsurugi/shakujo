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

class SyntaxValidatorDdlTest : public SyntaxValidatorTestBase, public ::testing::Test {
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

TEST_F(SyntaxValidatorDdlTest, CreateTableStatement) {
    validate(f.CreateTableStatement(
        f.Name("db.table"),
        {
            f.CreateTableStatementColumn(f.SimpleName("C1"), f.Int32Type()),
        }));
    should_error(f.CreateTableStatement(
        f.Name("db.table"),
        {}));
    should_error(f.CreateTableStatement(
        f.Name("db.table"),
        {
            f.CreateTableStatementColumn({}, f.Int32Type()),
        }));
    should_error(f.CreateTableStatement(
        f.Name("db.table"),
        {
            f.CreateTableStatementColumn(f.SimpleName("C1"), {}),
        }));
}
}  // namespace shakujo::analyzer
