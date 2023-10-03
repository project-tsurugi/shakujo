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
#include "shakujo/analyzer/scope/BlockScope.h"

#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "shakujo/analyzer/Reporter.h"
#include "shakujo/analyzer/scope/Result.h"
#include "shakujo/model/IRFactory.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::scope {

using common::util::equals;

class BlockScopeTest : public ::testing::Test {
public:
    shakujo::model::IRFactory f;
    Reporter reporter;
    bool error() { return reporter.saw_error(); }
};

TEST_F(BlockScopeTest, simple) {
    BlockScope<int> scope { &reporter };
    auto n = f.Name("a");

    Result<int> r = scope.find(n.get());
    ASSERT_FALSE(r);
}

TEST_F(BlockScopeTest, insert) {
    BlockScope<int> scope { &reporter };
    auto n = f.Name("a");

    scope.insert(n.get(), std::make_shared<int>(100));
    Result<int> r = scope.find(n.get());
    ASSERT_TRUE(r);
    EXPECT_EQ(n.get(), r.name());
    EXPECT_EQ(100, *r.element());
}

TEST_F(BlockScopeTest, find_qualifier) {
    BlockScope<int> scope { &reporter };
    auto n = f.Name("q", "a");

    scope.insert(n.get()->qualifier(), std::make_shared<int>(100));
    Result<int> r = scope.find(n.get());
    ASSERT_TRUE(r);
    EXPECT_EQ(n.get()->qualifier(), r.name());
    EXPECT_EQ(100, *r.element());
}

TEST_F(BlockScopeTest, insert_conflict) {
    BlockScope<int> scope { &reporter };
    auto n = f.Name("a");

    scope.insert(n.get(), std::make_shared<int>(100));

    ASSERT_FALSE(error());
    scope.insert(n.get(), std::make_shared<int>(200));

    ASSERT_TRUE(error());
    Result<int> r = scope.find(n.get());
    ASSERT_TRUE(r);
    EXPECT_EQ(int {}, *r.element());
}

TEST_F(BlockScopeTest, nested) {
    BlockScope<int> b0 { &reporter };
    BlockScope<int> b1 { &reporter, &b0 };
    BlockScope<int> b2 { &reporter, &b1 };

    auto n0 = f.Name("a");
    auto n1 = f.Name("b");

    b0.insert(n0.get(), std::make_shared<int>(100));
    b1.insert(n1.get(), std::make_shared<int>(200));
    b2.insert(n1.get(), std::make_shared<int>(300)); // hide n1.get()
    ASSERT_FALSE(error());

    {
        auto r = b2.find(n0.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(100, *r.element());
    }
    {
        auto r = b2.find(n1.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(300, *r.element());
    }
    {
        auto r = b1.find(n1.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(200, *r.element());
    }
    {
        auto r = b0.find(n1.get());
        ASSERT_FALSE(r);
    }
}
}
