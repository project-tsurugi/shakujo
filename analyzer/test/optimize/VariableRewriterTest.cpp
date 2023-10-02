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
#include "optimize/VariableRewriter.h"

#include <gtest/gtest.h>

#include "shakujo/common/core/type/Int.h"

#include "shakujo/model/IRFactory.h"

#include "shakujo/analyzer/binding/BindingContext.h"

namespace shakujo::analyzer::optimize {

using common::util::is_valid;

class VariableRewriterTest : public ::testing::Test {
public:
    std::shared_ptr<binding::VariableBinding> var(std::string_view name = "") {
        common::core::Name qname;
        if (!name.empty()) {
            qname = common::core::Name { name };
        }

        return std::make_shared<binding::VariableBinding>(
            bindings.next_variable_id(),
            std::move(qname),
            std::make_unique<common::core::type::Int>(32U));
    }

    std::unique_ptr<model::expression::VariableReference> ref(std::shared_ptr<binding::VariableBinding> var) {
        auto expr = f.VariableReference(f.Name(var->name()));
        expr->variable_key(bindings.create_key(std::move(var)));
        return expr;
    }

    std::shared_ptr<binding::VariableBinding> extract(model::key::VariableKey::Provider* ref) {
        return bindings.get(ref->variable_key());
    }

    model::IRFactory f;
    binding::BindingContext bindings;
};

TEST_F(VariableRewriterTest, simple) {
    VariableRewriter rewriter;

    auto a = var();
    auto b = var();
    rewriter.add_rule(a, b);
    auto result = rewriter.apply(a);
    EXPECT_EQ(result, b);
}

TEST_F(VariableRewriterTest, through) {
    VariableRewriter rewriter;

    auto a = var();
    auto result = rewriter.apply(a);
    EXPECT_EQ(result, a);
}

TEST_F(VariableRewriterTest, merge) {
    VariableRewriter r0;
    VariableRewriter r1;

    auto a = var();
    auto b = var();
    auto c = var();
    r0.add_rule(a, b);
    r1.add_rule(b, c);
    r0.merge(r1);

    EXPECT_EQ(r0.apply(a), c);
    EXPECT_EQ(r0.apply(b), c);
}

TEST_F(VariableRewriterTest, merge_missing) {
    VariableRewriter r0;
    VariableRewriter r1;

    auto a = var();
    auto b = var();
    auto c = var();
    auto d = var();
    r0.add_rule(a, b);
    r1.add_rule(c, d);
    r0.merge(r1);

    EXPECT_FALSE(is_valid(r0.apply(a)));
}

TEST_F(VariableRewriterTest, merge_join) {
    VariableRewriter r0;
    VariableRewriter r1;

    auto a0 = var("a0");
    auto a1 = var("a1");
    auto b0 = var("b0");
    auto b1 = var("b1");
    auto c0 = var("c0");

    r0.add_rule(a0, b0);  // a0 -> b0
    r0.add_rule(a1, b1);  // a1 -> b1

    r1.add_rule(b0, c0);  // b0 -> c0
    r0.merge(r1);  // a0 -> c0

    ASSERT_TRUE(r0.apply(a0)->is_valid());
    EXPECT_EQ(r0.apply(a0), c0);

    ASSERT_FALSE(r0.apply(a1)->is_valid());
}

}  // namespace shakujo::analyzer::optimize
