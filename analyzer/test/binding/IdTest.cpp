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
#include "shakujo/analyzer/binding/Id.h"

#include <gtest/gtest.h>
#include <string>
#include <memory>

namespace shakujo::analyzer::binding {

using id = Id<void>;

class IdTest : public ::testing::Test {
public:
    template<typename T>
    void output_test(const T& v) {
        std::ostringstream ss;
        ss << v;
        auto str = ss.str();
        EXPECT_FALSE(str.empty()) << str;
    }

    template<typename T>
    void order_test(const T&) {}

    template<typename T, typename... Args>
    void order_test(const T& first, const T& second, const Args&... args) {
        EXPECT_EQ(first, first);
        EXPECT_NE(first, second);
        EXPECT_LT(first, second);
        EXPECT_GT(second, first);
        order_test(first, args...);
        order_test(second, args...);
    };
};

TEST_F(IdTest, simple) {
    id v { 100U };
    EXPECT_TRUE(v);
    EXPECT_EQ(100U, v.get());
    output_test(v);
}

TEST_F(IdTest, empty) {
    id v;
    EXPECT_FALSE(v);
    output_test(v);
}

TEST_F(IdTest, invalid_id) {
    EXPECT_THROW({
        id v { id::MIN_VALUE - 1U };
    }, std::invalid_argument);
}

TEST_F(IdTest, id_order) {
    id a;
    id b { id::MIN_VALUE };
    id c { id::MIN_VALUE + 1 };
    id d { id::MIN_VALUE + 2 };
    id e { id::MIN_VALUE + 3 };
    order_test(a, b, c, d, e);
}

TEST_F(IdTest, id_hash) {
    id::Hash hash;
    id a;
    id b { id::MIN_VALUE };
    id c { id::MIN_VALUE + 1 };
    EXPECT_EQ(hash(a), hash(a));
    EXPECT_EQ(hash(b), hash(b));
    EXPECT_EQ(hash(c), hash(c));
    EXPECT_NE(hash(b), hash(c));
}
}  // namespace shakujo::analyzer::binding
