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
#include "ParserTestBase.h"

#include <gtest/gtest.h>
#include <stdexcept>

#include "shakujo/model/IRFactory.h"

namespace shakujo::parser {

using namespace shakujo::model::expression;
using namespace shakujo::model::expression::relation;
using namespace shakujo::model::statement;
using namespace shakujo::model::statement::ddl;
namespace v = shakujo::common::core::value;

using common::util::equals;

class ParserDropIndexTest : public ParserTestBase, public ::testing::Test {
public:
    std::unique_ptr<DropIndexStatement> parse(const std::string& text) {
        return parse_program_main<DropIndexStatement>(text);
    }
};


TEST_F(ParserDropIndexTest, simple) {
    auto stmt = parse("DROP INDEX t");

    EXPECT_TRUE(equals(f.Name("t"), stmt->index()));
}

}  // namespace shakujo::parser
