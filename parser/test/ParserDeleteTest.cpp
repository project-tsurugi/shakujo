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
#include "ParserTestBase.h"

#include <gtest/gtest.h>
#include <stdexcept>

#include "shakujo/model/IRFactory.h"

namespace shakujo::parser {

using namespace shakujo::model::expression;
using namespace shakujo::model::expression::relation;
using namespace shakujo::model::statement;
using namespace shakujo::model::statement::dml;
namespace t = shakujo::common::core::type;
namespace v = shakujo::common::core::value;

using common::util::equals;

class ParserDeleteTest : public ParserTestBase, public ::testing::Test {
public:
    std::unique_ptr<DeleteStatement> parse(std::string_view text) {
        std::string s { text };
        return parse_program_main<DeleteStatement>(s);
    }
};

TEST_F(ParserDeleteTest, simple) {
    auto stmt = parse("DELETE FROM TBL");

    EXPECT_TRUE(equals(f.Name("TBL"), stmt->table()));

    auto* source = dynamic_pointer_cast<ScanExpression>(stmt->source());
    EXPECT_TRUE(equals(stmt->table(), source->table()));
}

TEST_F(ParserDeleteTest, conditional) {
    auto stmt = parse("DELETE FROM TBL WHERE 1");

    EXPECT_TRUE(equals(f.Name("TBL"), stmt->table()));

    auto* source = dynamic_pointer_cast<SelectionExpression>(stmt->source());
    EXPECT_EQ(1, value_of<v::Int>(source->condition()));

    auto* scan = dynamic_pointer_cast<ScanExpression>(source->operand());
    EXPECT_TRUE(equals(stmt->table(), scan->table()));
}
}  // namespace shakujo::parser
