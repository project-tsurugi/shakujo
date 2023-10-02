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
#include "analyze/typing.h"

#include <gtest/gtest.h>

#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/type/Numeric.h"
#include "shakujo/common/core/type/Textual.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/type/Float.h"
#include "shakujo/common/core/type/Char.h"
#include "shakujo/common/core/type/String.h"
#include "shakujo/common/core/type/Array.h"
#include "shakujo/common/core/type/Vector.h"
#include "shakujo/common/core/type/Null.h"
#include "shakujo/common/core/type/Tuple.h"
#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/type/Cursor.h"
#include "shakujo/common/core/type/Error.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::analyze {

using IRFactory = model::IRFactory;

namespace t = shakujo::common::core::type;

static const auto NULLABLE = common::core::Type::Nullity::NULLABLE;
static const auto NON_NULL = common::core::Type::Nullity::NEVER_NULL;

class TypingTest : public ::testing::Test {
public:
    bool is_atom(common::core::Type const& t) { return typing::is_atom(&t); }
    bool is_boolean(common::core::Type const& t) { return typing::is_boolean(&t); }
    bool is_numeric(common::core::Type const& t) { return typing::is_numeric(&t); }
    bool is_integral(common::core::Type const& t) { return typing::is_integral(&t); }
    bool is_textual(common::core::Type const& t) { return typing::is_textual(&t); }
    bool is_boolean_convertible(common::core::Type const& t) { return typing::is_boolean_convertible(&t); }

    std::unique_ptr<common::core::Type> binary_promotion(common::core::Type const& a, common::core::Type const& b) {
        return typing::binary_promotion(&a, &b);
    }

    std::unique_ptr<common::core::Type> nullity(common::core::Type const& t, common::core::Type::Nullity n) {
        return typing::nullity(&t, n);
    }

    std::unique_ptr<common::core::Type> convert(model::type::Type const& t) {
        return typing::convert(&t);
    }
};

TEST_F(TypingTest, is_atom) {
    EXPECT_TRUE(is_atom(t::Bool()));
    EXPECT_TRUE(is_atom(t::Int(32U)));
    EXPECT_TRUE(is_atom(t::Float(32U)));
    EXPECT_TRUE(is_atom(t::Char(10U)));
    EXPECT_TRUE(is_atom(t::String()));

    EXPECT_FALSE(is_atom(t::Null()));
}

TEST_F(TypingTest, is_boolean) {
    EXPECT_TRUE(is_boolean(t::Bool(NULLABLE)));
    EXPECT_TRUE(is_boolean(t::Bool(NON_NULL)));

    EXPECT_FALSE(is_boolean(t::Int(32U)));
}

TEST_F(TypingTest, is_numeric) {
    EXPECT_TRUE(is_numeric(t::Int(32U, NON_NULL)));
    EXPECT_TRUE(is_numeric(t::Int(64U, NULLABLE)));
    EXPECT_TRUE(is_numeric(t::Float(64U, NON_NULL)));
    EXPECT_TRUE(is_numeric(t::Float(32U, NULLABLE)));

    EXPECT_FALSE(is_numeric(t::Bool(NON_NULL)));
}

TEST_F(TypingTest, is_integral) {
    EXPECT_TRUE(is_integral(t::Int(32U, NON_NULL)));
    EXPECT_TRUE(is_integral(t::Int(64U, NULLABLE)));

    EXPECT_FALSE(is_integral(t::Float(32U, NON_NULL)));
}

TEST_F(TypingTest, is_textual) {
    EXPECT_TRUE(is_textual(t::String(NULLABLE)));
    EXPECT_TRUE(is_textual(t::String(NON_NULL)));
    EXPECT_TRUE(is_textual(t::Char(10U, NULLABLE)));
    EXPECT_TRUE(is_textual(t::Char(10U, NON_NULL)));

    EXPECT_FALSE(is_textual(t::Int(32U, NON_NULL)));
}

TEST_F(TypingTest, is_boolean_convertible) {
    EXPECT_TRUE(is_boolean_convertible(t::Bool(NULLABLE)));
    EXPECT_TRUE(is_boolean_convertible(t::Bool(NON_NULL)));
    EXPECT_TRUE(is_boolean_convertible(t::Int(32U)));
    EXPECT_TRUE(is_boolean_convertible(t::Float(32U)));

    EXPECT_FALSE(is_boolean_convertible(t::Vector(t::Int(32U))));
}

TEST_F(TypingTest, binary_promotion_ident) {
    EXPECT_EQ(
            t::Int(32U, NON_NULL),
            *binary_promotion(
                    t::Int(32U, NON_NULL),
                    t::Int(32U, NON_NULL)));
    EXPECT_EQ(
            t::Float(32U, NON_NULL),
            *binary_promotion(
                    t::Float(32U, NON_NULL),
                    t::Float(32U, NON_NULL)));
    EXPECT_EQ(
            t::Bool(NON_NULL),
            *binary_promotion(
                    t::Bool(NON_NULL),
                    t::Bool(NON_NULL)));
    EXPECT_EQ(
            t::Char(10U, NON_NULL),
            *binary_promotion(
                    t::Char(10U, NON_NULL),
                    t::Char(10U, NON_NULL)));
    EXPECT_EQ(
            t::String(NON_NULL),
            *binary_promotion(
                    t::String(NON_NULL),
                    t::String(NON_NULL)));
}

TEST_F(TypingTest, binary_promotion_nullity) {
    EXPECT_EQ(
            t::Int(32U, NULLABLE),
            *binary_promotion(
                    t::Int(32U, NULLABLE),
                    t::Int(32U, NON_NULL)));
    EXPECT_EQ(
            t::Int(32U, NULLABLE),
            *binary_promotion(
                    t::Int(32U, NON_NULL),
                    t::Int(32U, NULLABLE)));
    EXPECT_EQ(
            t::Float(32U, NULLABLE),
            *binary_promotion(
                    t::Float(32U, NULLABLE),
                    t::Float(32U, NON_NULL)));
    EXPECT_EQ(
            t::Float(32U, NULLABLE),
            *binary_promotion(
                    t::Float(32U, NON_NULL),
                    t::Float(32U, NULLABLE)));
    EXPECT_EQ(
            t::Bool(NULLABLE),
            *binary_promotion(
                    t::Bool(NULLABLE),
                    t::Bool(NON_NULL)));
    EXPECT_EQ(
            t::Bool(NULLABLE),
            *binary_promotion(
                    t::Bool(NON_NULL),
                    t::Bool(NULLABLE)));
    EXPECT_EQ(
            t::Char(10U, NULLABLE),
            *binary_promotion(
                    t::Char(10U, NULLABLE),
                    t::Char(10U, NON_NULL)));
    EXPECT_EQ(
            t::Char(10U, NULLABLE),
            *binary_promotion(
                    t::Char(10U, NON_NULL),
                    t::Char(10U, NULLABLE)));
    EXPECT_EQ(
            t::String(NULLABLE),
            *binary_promotion(
                    t::String(NULLABLE),
                    t::String(NON_NULL)));
    EXPECT_EQ(
            t::String(NULLABLE),
            *binary_promotion(
                    t::String(NON_NULL),
                    t::String(NULLABLE)));
}

TEST_F(TypingTest, binary_promotion_integer) {
    EXPECT_EQ(
            t::Int(64U, NON_NULL),
            *binary_promotion(
                    t::Int(32U, NON_NULL),
                    t::Int(64U, NON_NULL)));
    EXPECT_EQ(
            t::Int(64U, NON_NULL),
            *binary_promotion(
                    t::Int(64U, NON_NULL),
                    t::Int(32U, NON_NULL)));
}

TEST_F(TypingTest, binary_promotion_float) {
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Float(32U, NON_NULL),
                    t::Float(64U, NON_NULL)));
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Float(64U, NON_NULL),
                    t::Float(32U, NON_NULL)));
}

TEST_F(TypingTest, binary_promotion_numeric) {
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Int(32U, NON_NULL),
                    t::Float(32U, NON_NULL)));
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Float(32U, NON_NULL),
                    t::Int(32U, NON_NULL)));
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Int(64U, NON_NULL),
                    t::Float(32U, NON_NULL)));
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Float(32U, NON_NULL),
                    t::Int(64U, NON_NULL)));
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Int(32U, NON_NULL),
                    t::Float(64U, NON_NULL)));
    EXPECT_EQ(
            t::Float(64U, NON_NULL),
            *binary_promotion(
                    t::Float(64U, NON_NULL),
                    t::Int(32U, NON_NULL)));
}

TEST_F(TypingTest, binary_promotion_textual) {
    EXPECT_EQ(
            t::String(NON_NULL),
            *binary_promotion(
                    t::Char(10U, NON_NULL),
                    t::String(NON_NULL)));
    EXPECT_EQ(
            t::String(NON_NULL),
            *binary_promotion(
                    t::String(NON_NULL),
                    t::Char(10U, NON_NULL)));
    EXPECT_EQ(
            t::String(NON_NULL),
            *binary_promotion(
                    t::Char(10U, NON_NULL),
                    t::Char(20U, NON_NULL)));
}

TEST_F(TypingTest, nullity_bool) {
    EXPECT_EQ(
            t::Bool(NON_NULL),
            *nullity(t::Bool(NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_int) {
    EXPECT_EQ(
            t::Int(32U, NON_NULL),
            *nullity(t::Int(32U, NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_float) {
    EXPECT_EQ(
            t::Float(32U, NON_NULL),
            *nullity(t::Float(32U, NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_char) {
    EXPECT_EQ(
            t::Char(1U, NON_NULL),
            *nullity(t::Char(1U, NULLABLE), NON_NULL));
    EXPECT_EQ(
            t::Char(2U, NON_NULL),
            *nullity(t::Char(2U, NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_string) {
    EXPECT_EQ(
            t::String(NON_NULL),
            *nullity(t::String(NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_tuple) {
    EXPECT_EQ(
            t::Tuple({ t::Int(32U, NULLABLE) }, NON_NULL),
            *nullity(t::Tuple({ t::Int(32U, NULLABLE) }, NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_array) {
    EXPECT_EQ(
            t::Array(t::Int(32U, NULLABLE), 10U, NON_NULL),
            *nullity(t::Array(t::Int(32U, NULLABLE), 10U, NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_vector) {
    EXPECT_EQ(
            t::Vector(t::Int(32U, NULLABLE), NON_NULL),
            *nullity(t::Vector(t::Int(32U, NULLABLE), NULLABLE), NON_NULL));
}

TEST_F(TypingTest, nullity_relation) {
    t::Relation type {
        { t::Int(32U) }
    };
    EXPECT_EQ(type, *nullity(type, NON_NULL));
}

TEST_F(TypingTest, nullity_cursor) {
    t::Cursor type {
        t::Relation {
            { t::Int(32U) }
        }
    };
    EXPECT_EQ(type, *nullity(type, NON_NULL));
}

TEST_F(TypingTest, nullity_null) {
    EXPECT_EQ(
            t::Null(),
            *nullity(t::Null(), NON_NULL));
}

TEST_F(TypingTest, nullity_error) {
    EXPECT_EQ(
            t::Error(),
            *nullity(t::Error(), NON_NULL));
}

TEST_F(TypingTest, convert_bool) {
    IRFactory f;
    EXPECT_EQ(
            t::Bool(NULLABLE),
            *convert(*f.BooleanType()));
}

TEST_F(TypingTest, convert_int) {
    IRFactory f;
    EXPECT_EQ(
            t::Int(32U, NULLABLE),
            *convert(*f.Int32Type()));
    EXPECT_EQ(
            t::Int(64U, NULLABLE),
            *convert(*f.Int64Type()));
}

TEST_F(TypingTest, convert_float) {
    IRFactory f;
    EXPECT_EQ(
            t::Float(32U, NULLABLE),
            *convert(*f.Float32Type()));
    EXPECT_EQ(
            t::Float(64U, NULLABLE),
            *convert(*f.Float64Type()));
}

TEST_F(TypingTest, convert_char) {
    IRFactory f;
    EXPECT_EQ(
            t::Char(10U, NULLABLE),
            *convert(*f.CharType(10U)));
}

TEST_F(TypingTest, convert_string) {
    IRFactory f;
    EXPECT_EQ(
            t::String(NULLABLE),
            *convert(*f.StringType()));
}
}  // namespace shakujo::analyzer::analyze
