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
#include "shakujo/analyzer/binding/BindingContext.h"

#include <gtest/gtest.h>
#include <memory>

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/type/Int.h"

namespace shakujo::analyzer::binding {

using namespace shakujo::model::key;
using namespace shakujo::common::core;

using id = Id<void>;

class BindingContextTest : public ::testing::Test {
public:
    BindingContext context;
};

TEST_F(BindingContextTest, invalid_expr) {
    ExpressionKey key;
    auto binding = context.find(&key);
    ASSERT_FALSE(binding);

    EXPECT_THROW({ context.get(&key); }, std::domain_error);
}

TEST_F(BindingContextTest, invalid_var) {
    VariableKey key;
    auto binding = context.find(&key);
    ASSERT_FALSE(binding);

    EXPECT_THROW({ context.get(&key); }, std::domain_error);
}

TEST_F(BindingContextTest, invalid_func) {
    FunctionKey key;
    auto binding = context.find(&key);
    ASSERT_FALSE(binding);

    EXPECT_THROW({ context.get(&key); }, std::domain_error);
}

TEST_F(BindingContextTest, id_var) {
    auto a = context.next_variable_id();
    auto b = context.next_variable_id();
    auto c = context.next_variable_id();
    EXPECT_TRUE(a);
    EXPECT_TRUE(b);
    EXPECT_TRUE(c);
    EXPECT_NE(a, b);
    EXPECT_NE(b, c);
    EXPECT_NE(c, a);
}

TEST_F(BindingContextTest, id_func) {
    auto a = context.next_function_id();
    auto b = context.next_function_id();
    auto c = context.next_function_id();
    EXPECT_TRUE(a);
    EXPECT_TRUE(b);
    EXPECT_TRUE(c);
    EXPECT_NE(a, b);
    EXPECT_NE(b, c);
    EXPECT_NE(c, a);
}

TEST_F(BindingContextTest, expr) {
    auto binding = std::make_shared<ExpressionBinding>(std::make_unique<type::Int>(32U));
    auto key = context.create_key(binding);
    EXPECT_EQ(binding.get(), context.get(key).get());
    EXPECT_EQ(binding.get(), context.find(key).get());
}

TEST_F(BindingContextTest, var) {
    auto id = context.next_variable_id();
    auto binding = std::make_shared<VariableBinding>(std::move(id), Name("a"), std::make_unique<type::Int>(32U));
    auto key = context.create_key(binding);
    EXPECT_EQ(binding.get(), context.get(key).get());
    EXPECT_EQ(binding.get(), context.find(key).get());
}

TEST_F(BindingContextTest, func) {
    auto id = context.next_function_id();
    auto binding = std::make_shared<FunctionBinding>(std::move(id), Name("a"), std::make_unique<type::Int>(32U));
    auto key = context.create_key(binding);
    EXPECT_EQ(binding.get(), context.get(key).get());
    EXPECT_EQ(binding.get(), context.find(key).get());
}

TEST_F(BindingContextTest, different_instance) {
    auto binding = std::make_shared<ExpressionBinding>(std::make_unique<type::Int>(32U));
    auto key = context.create_key(binding);
    EXPECT_TRUE(context.find(key));

    BindingContext another;
    EXPECT_FALSE(another.find(key));
    EXPECT_THROW({ another.get(key); }, std::domain_error);
}
}  // namespace shakujo::analyzer::binding
