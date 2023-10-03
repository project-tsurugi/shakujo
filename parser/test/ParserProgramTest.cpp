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

#include <initializer_list>

#include <gtest/gtest.h>

#include "shakujo/model/IRFactory.h"

namespace shakujo::parser {

class ParserProgramTest : public ParserTestBase, public ::testing::Test {
public:
    std::unique_ptr<model::program::Program> parse(std::initializer_list<std::string_view> lines) {
        std::string text;
        for (auto line : lines) {
            text.append(line);
            text.append(1, '\n');
        }
        return parse_program(text);
    }
};

TEST_F(ParserProgramTest, simple) {
    auto program = parse({
        "SELECT * FROM T",
    });
    EXPECT_EQ(program->main()->kind(), model::statement::dml::EmitStatement::tag);
}

TEST_F(ParserProgramTest, single_line_comment) {
    auto program = parse({
        "-- begin",
        "SELECT *",
        "-- middle",
        "FROM T",
        "-- end",
    });
    ASSERT_EQ(program->comments().size(), 3);
    EXPECT_EQ(program->comments()[0]->contents(), "-- begin");
    EXPECT_EQ(program->comments()[1]->contents(), "-- middle");
    EXPECT_EQ(program->comments()[2]->contents(), "-- end");
}

TEST_F(ParserProgramTest, multi_line_comment) {
    auto program = parse({
        "/* begin */",
        "SELECT * /* middle */ FROM T",
        "/* end */",
    });
    ASSERT_EQ(program->comments().size(), 3);
    EXPECT_EQ(program->comments()[0]->contents(), "/* begin */");
    EXPECT_EQ(program->comments()[1]->contents(), "/* middle */");
    EXPECT_EQ(program->comments()[2]->contents(), "/* end */");
}

}  // namespace shakujo::parser
