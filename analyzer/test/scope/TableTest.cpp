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
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "shakujo/analyzer/scope/Table.h"
#include "shakujo/analyzer/scope/Result.h"
#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::scope {

shakujo::model::IRFactory f;

class TableTest : public ::testing::Test {};

TEST_F(TableTest, simple) {
    Table<std::string> table;

    EXPECT_FALSE(table.contains("a"));

    table.put("a", std::make_shared<std::string>("A"));
    EXPECT_TRUE(table.contains("a"));

    {
        auto name = f.Name("a");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(name.get(), r.name());
        EXPECT_EQ("A", *r.element());
    }
    {
        auto name = f.Name("a", "b");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(name->qualifier(), r.name());
        EXPECT_EQ("A", *r.element());
    }
    {
        auto name = f.Name("x");
        Result<std::string> r = table.find(name.get());
        EXPECT_FALSE(r);
    }
}

TEST_F(TableTest, simple_vector) {
    Table<std::string> table;

    const std::vector<std::string> key {"a"};
    EXPECT_FALSE(table.contains(key));

    table.put(key, std::make_shared<std::string>("A"));
    EXPECT_TRUE(table.contains(key));

    {
        auto name = f.Name("a");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(name.get(), r.name());
        EXPECT_EQ("A", *r.element());
    }
}

TEST_F(TableTest, qualified) {
    Table<std::string> table;

    const std::vector<std::string> key {"a", "b"};
    EXPECT_FALSE(table.contains(key));

    table.put(key, std::make_shared<std::string>("A"));
    EXPECT_TRUE(table.contains(key));

    {
        auto name = f.Name("a", "b");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(name.get(), r.name());
        EXPECT_EQ("A", *r.element());
    }
    {
        auto name = f.Name("a", "b", "c");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ(name->qualifier(), r.name());
        EXPECT_EQ("A", *r.element());
    }
    {
        auto name = f.Name("a");
        Result<std::string> r = table.find(name.get());
        EXPECT_FALSE(r);
    }
    {
        auto name = f.Name("a", "x");
        Result<std::string> r = table.find(name.get());
        EXPECT_FALSE(r);
    }
}

TEST_F(TableTest, put_conflict_simple) {
    Table<std::string> table;
    table.put("a", std::make_shared<std::string>("A"));
    EXPECT_THROW({
        table.put("a", std::make_shared<std::string>("B"));
    },
    std::runtime_error);
}

TEST_F(TableTest, put_conflict_simple_vector) {
    Table<std::string> table;
    const std::vector<std::string> key {"a"};
    table.put(key, std::make_shared<std::string>("A"));
    EXPECT_THROW({
        table.put(key, std::make_shared<std::string>("B"));
    },
    std::runtime_error);
}

TEST_F(TableTest, put_conflict_qualified) {
    Table<std::string> table;
    const std::vector<std::string> key {"a", "b"};
    table.put(key, std::make_shared<std::string>("A"));
    EXPECT_THROW({
        table.put(key, std::make_shared<std::string>("B"));
    },
    std::runtime_error);
}

TEST_F(TableTest, longest_match) {
    Table<std::string> table;
    const std::vector<std::string> key {"a", "b"};
    table.put(key, std::make_shared<std::string>("long"));
    EXPECT_NO_THROW({
        table.put(key[0], std::make_shared<std::string>("short"), false);
    });
    {
        auto name = f.Name("a");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ("short", *r.element());
    }
    {
        auto name = f.Name("a", "b");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ("long", *r.element());
    }
}

TEST_F(TableTest, put_overwrite_simple) {
    Table<std::string> table;
    table.put("a", std::make_shared<std::string>("A"));
    EXPECT_NO_THROW({
        table.put("a", std::make_shared<std::string>("B"), true);
    });
    {
        auto name = f.Name("a");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ("B", *r.element());
    }
}

TEST_F(TableTest, put_overwrite_qualified) {
    Table<std::string> table;
    const std::vector<std::string> key {"a", "b"};
    table.put(key, std::make_shared<std::string>("A"));
    EXPECT_NO_THROW({
        table.put(key, std::make_shared<std::string>("B"), true);
    });
    {
        auto name = f.Name("a", "b");
        Result<std::string> r = table.find(name.get());
        ASSERT_TRUE(r);
        EXPECT_EQ("B", *r.element());
    }
}
}
