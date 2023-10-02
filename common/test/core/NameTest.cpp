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
#include "shakujo/common/core/Name.h"

#include <gtest/gtest.h>

namespace shakujo::common::core {

class CoreNameTest : public ::testing::Test {};

TEST_F(CoreNameTest, empty) {
    Name name {};
    EXPECT_FALSE(name);
}

TEST_F(CoreNameTest, simple) {
    Name name { "a" };
    EXPECT_TRUE(name);
}

TEST_F(CoreNameTest, segments) {
    Name name {"a", "b", "c"};
    auto& segments = name.segments();
    std::vector<std::string> vec {"a", "b", "c"};
    EXPECT_EQ(vec, segments);
    EXPECT_EQ(&segments, &name.segments());
}

TEST_F(CoreNameTest, segments_rvalue) {
    Name name {"a", "b", "c"};
    auto segments = std::move(name).segments();
    std::vector<std::string> vec {"a", "b", "c"};
    EXPECT_EQ(vec, segments);
}

TEST_F(CoreNameTest, segments_compare) {
    Name a {"a"};
    Name a2 {"a"};
    Name b {"b"};
    Name ab {"a", "b"};
    Name ba {"b", "a"};

    EXPECT_EQ(a, a2);
    EXPECT_GE(a, a2);
    EXPECT_LE(a, a2);

    EXPECT_NE(a, b);
    EXPECT_LT(a, b);
    EXPECT_LE(a, b);

    EXPECT_NE(a, ab);
    EXPECT_LT(a, ab);
    EXPECT_LE(a, ab);

    EXPECT_NE(b, ab);
    EXPECT_GT(b, ab);
    EXPECT_GE(b, ab);

    EXPECT_NE(a, ba);
    EXPECT_LT(a, ba);
    EXPECT_LE(a, ba);
}

TEST_F(CoreNameTest, hash) {
    Name a;
    Name b { "a" };
    Name c { "b" };
    Name d { "a", "b", "c" };

    Name::Hash hash;
    EXPECT_EQ(hash(a), hash(a));
    EXPECT_EQ(hash(b), hash(b));
    EXPECT_EQ(hash(c), hash(c));
    EXPECT_EQ(hash(d), hash(d));

    EXPECT_NE(hash(a), hash(b));
    EXPECT_NE(hash(b), hash(c));
}
}  // namespace shakujo::common::core
