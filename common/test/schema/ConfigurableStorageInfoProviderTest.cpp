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
#include "shakujo/common/schema/ConfigurableStorageInfoProvider.h"

#include <gtest/gtest.h>

#include <functional>
#include <future>
#include <string>
#include <memory>
#include <thread>

#include "shakujo/common/core/type/Int.h"

namespace shakujo::common::schema {

namespace t = core::type;

class ConfigurableStorageInfoProviderTest : public ::testing::Test {};

TEST_F(ConfigurableStorageInfoProviderTest, simple) {
    ConfigurableStorageInfoProvider provider {};
    provider.add({
        "T",
        {
            { "C1", t::Int(32U) },
        }
    });
    {
        auto&& table = provider.find_table(common::core::Name("T"));
        EXPECT_EQ(table.name(), "T");
    }
    {
        auto&& table = provider.find_table(common::core::Name("t"));
        EXPECT_FALSE(table.is_valid());
    }
}

TEST_F(ConfigurableStorageInfoProviderTest, case_insensitive) {
    ConfigurableStorageInfoProvider provider { false };
    provider.add({
        "T",
        {
            { "C1", t::Int(32U) },
        }
    });
    {
        auto&& table = provider.find_table(common::core::Name("T"));
        EXPECT_EQ(table.name(), "T");
    }
    {
        auto&& table = provider.find_table(common::core::Name("t"));
        EXPECT_EQ(table.name(), "T");
    }
}

TEST_F(ConfigurableStorageInfoProviderTest, overwrite) {
    ConfigurableStorageInfoProvider provider {};
    provider.add({
        "T",
        {
            { "C1", t::Int(32U) },
        }
    });
    provider.add({
        "T",
        {
            { "C1", t::Int(32U) },
            { "C2", t::Int(32U) },
        }
    });
    try {
        provider.add({
            "T",
            {
                { "C1", t::Int(32U) },
                { "C2", t::Int(32U) },
                { "C3", t::Int(32U) },
            }
        }, false);
        FAIL();
    } catch (std::invalid_argument&) {
        // ok
    }
    auto&& table = provider.find_table(common::core::Name("T"));
    EXPECT_EQ(table.name(), "T");
    EXPECT_EQ(table.columns().size(), 2);
}


TEST_F(ConfigurableStorageInfoProviderTest, concurrent) {
    constexpr std::size_t count = 100;
    ConfigurableStorageInfoProvider provider {};
    std::function<void()> adder = [&]() {
        for (std::size_t i = 0; i < count; ++i) {
            provider.add({
                std::to_string(i),
                {
                    { "C1", t::Int(32U) },
                }
            });
        }
    };
    auto r1 = std::async(std::launch::async, adder);
    auto r2 = std::async(std::launch::async, adder);
    auto r3 = std::async(std::launch::async, adder);
    r1.wait();
    r2.wait();
    r3.wait();
    ASSERT_TRUE(r1.valid());
    ASSERT_TRUE(r2.valid());
    ASSERT_TRUE(r3.valid());
    for (std::size_t i = 0; i < count; ++i) {
        auto&& table = provider.find_table(common::core::Name(std::to_string(i)));
        EXPECT_TRUE(table.is_valid());
    }
}

TEST_F(ConfigurableStorageInfoProviderTest, remove) {
    ConfigurableStorageInfoProvider provider {};
    provider.add({
                         "T",
                         {
                                 { "C1", t::Int(32U) },
                         }
                 });
    {
        auto&& table = provider.find_table(common::core::Name("T"));
        EXPECT_EQ(table.name(), "T");
    }
    {
        EXPECT_TRUE(provider.remove(common::core::Name("T")));
    }
    {
        auto&& table = provider.find_table(common::core::Name("T"));
        EXPECT_FALSE(table.is_valid());
    }
    {
        EXPECT_FALSE(provider.remove(common::core::Name("T")));
    }
}

TEST_F(ConfigurableStorageInfoProviderTest, each) {
    ConfigurableStorageInfoProvider provider {};
    provider.add({
            "T1",
            {
                    { "C1", t::Int(32U) },
            }
    });
    provider.add({
            "T2",
            {
                    { "C1", t::Int(32U) },
            }
    });
    std::vector<TableInfo const*> vec{};
    provider.each_table([&](auto& info){
        vec.emplace_back(&info);
    });
    ASSERT_EQ(vec.size(), 2);
    std::sort(vec.begin(), vec.end(), [](TableInfo const* a, TableInfo const* b){
        return a->name() < b->name();
    });
    EXPECT_EQ(vec[0]->name(), "T1");
    EXPECT_EQ(vec[1]->name(), "T2");
}


}  // namespace shakujo::common::schema
