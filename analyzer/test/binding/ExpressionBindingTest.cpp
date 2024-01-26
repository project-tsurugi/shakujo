/*
 * Copyright 2018-2024 Project Tsurugi.
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
#include "shakujo/analyzer/binding/ExpressionBinding.h"

#include <gtest/gtest.h>

namespace shakujo::analyzer::binding {

class ExpressionBindingTest : public ::testing::Test {};

TEST_F(ExpressionBindingTest, attributes) {
    ExpressionBinding binding;

    auto&& s0 = binding.find_attribute("testing");
    EXPECT_FALSE(s0.has_value());

    binding.put_attribute("testing", std::make_any<int>(100));
    auto&& s1 = binding.find_attribute("testing");
    ASSERT_TRUE(s1.has_value());
    EXPECT_EQ(100, std::any_cast<int>(s1));
}

TEST_F(ExpressionBindingTest, attributes_overwrite) {
    ExpressionBinding binding;
    binding.put_attribute("testing", std::make_any<int>(100));
    auto&& v0 = binding.get_attribute<int>("testing");
    EXPECT_EQ(100, v0);

    auto&& v1 = binding.get_attribute<int>("testing");
    v0 = 200;
    EXPECT_EQ(200, v1);
}
}  // namespace shakujo::analyzer::binding
