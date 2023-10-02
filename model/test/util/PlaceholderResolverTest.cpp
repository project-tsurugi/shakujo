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
#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "shakujo/model/util/PlaceholderResolver.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::model::util {

class PlaceholderResolverTest : public ::testing::Test {
public:
    IRFactory f;
    std::unique_ptr<expression::Literal> literal(int v) {
        return f.Literal(common::core::type::Int(32U, common::core::Type::Nullity::NEVER_NULL), v);
    }

    template<class T>
    typename T::type literal(expression::Expression const* node) {
        if (auto literal = dynamic_cast<expression::Literal const*>(node)) {
            if (auto value = dynamic_cast<T const*>(literal->value())) {
                return value->get();
            }
        }
        throw std::bad_cast();
    }

    std::string const& placeholder(expression::Expression const* node) {
        if (auto placeholder = dynamic_cast<expression::Placeholder const*>(node)) {
            return placeholder->name();
        }
        throw std::bad_cast();
    }
};

using UnaryOp = expression::UnaryOperator::Kind;

TEST_F(PlaceholderResolverTest, simple) {
    auto expr = f.UnaryOperator(
            UnaryOp::PLUS,
            f.Placeholder("testing"));

    std::map<std::string, std::unique_ptr<expression::Expression>> replacements;
    replacements.emplace("testing", literal(100));

    PlaceholderResolver resolver { std::move(replacements) };
    auto results = resolver(expr.get());
    EXPECT_TRUE(results);
    ASSERT_EQ(results.replaced().size(), 1U);
    EXPECT_EQ(placeholder(results.replaced()[0].get()), "testing");

    ASSERT_EQ(expr->operand()->kind(), expression::ExpressionKind::LITERAL);
    EXPECT_EQ(literal<common::core::value::Int>(expr->operand()), 100);
}

TEST_F(PlaceholderResolverTest, miss) {
    auto expr = f.UnaryOperator(
            UnaryOp::PLUS,
            f.Placeholder("testing"));

    std::map<std::string, std::unique_ptr<expression::Expression>> replacements;
    replacements.emplace("MISS", literal(100));

    PlaceholderResolver resolver { std::move(replacements) };
    auto results = resolver(expr.get());
    EXPECT_FALSE(results);
    EXPECT_TRUE(results.replaced().empty());
    ASSERT_EQ(results.left().size(), 1U);
    EXPECT_EQ(placeholder(results.left()[0]), "testing");

    ASSERT_EQ(expr->operand()->kind(), expression::ExpressionKind::PLACEHOLDER);
    EXPECT_EQ(expr->operand(), results.left()[0]);
}

}  // namespace shakujo::model::util
