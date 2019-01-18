/*
 * Copyright 2018-2018 shakujo project.
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
#include "shakujo/common/core/Value.h"

#include <gtest/gtest.h>
#include <memory>
#include <utility>
#include <sstream>

#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/core/value/String.h"
#include "shakujo/common/core/value/Tuple.h"
#include "shakujo/common/core/value/Null.h"
#include "shakujo/common/core/value/Visitor.h"

namespace shakujo::common::core {

using namespace shakujo::common::core::value;

class CoreValueTest : public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> clone(const T &v) {
        return std::unique_ptr<T>(v.clone());
    }
    template<typename T>
    std::unique_ptr<T> clone(std::unique_ptr<T> p) {
        return std::unique_ptr<T>(std::move(*p).clone());
    }
    template<typename T>
    void output_test(const T& v) {
        std::ostringstream ss;
        ss << v.kind() << ": " << v;
        auto str = ss.str();
        EXPECT_FALSE(str.empty()) << str;
    }
    template<typename T>
    void clone_test(const T& v) {
        auto p = clone(v);
        EXPECT_EQ(v, *p);
        EXPECT_EQ(v, *clone(std::move(p)));
    }

    int as_int(Value const* v) {
        return static_cast<int>(dynamic_cast<Int const*>(v)->get());
    }
};

TEST_F(CoreValueTest, Bool) {
    Bool v { true };
    EXPECT_EQ(Value::Kind::BOOL, v.kind());
    EXPECT_EQ(true, *v);

    EXPECT_TRUE(v.is_valid());
    output_test(v);
    clone_test(v);
}

TEST_F(CoreValueTest, Int) {
    Int v { 100 };
    EXPECT_EQ(Value::Kind::INT, v.kind());
    EXPECT_EQ(100, *v);

    EXPECT_TRUE(v.is_valid());
    output_test(v);
    clone_test(v);
}

TEST_F(CoreValueTest, Float) {
    Float v { 1.25 };
    EXPECT_EQ(Value::Kind::FLOAT, v.kind());
    EXPECT_EQ(1.25, *v);

    EXPECT_TRUE(v.is_valid());
    output_test(v);
    clone_test(v);
}

TEST_F(CoreValueTest, String) {
    String v { "a" };
    EXPECT_EQ(Value::Kind::STRING, v.kind());
    EXPECT_EQ("a", *v);

    EXPECT_TRUE(v.is_valid());
    output_test(v);
    clone_test(v);
}

TEST_F(CoreValueTest, Tuple) {
    Tuple v { Int(10) };
    EXPECT_EQ(Value::Kind::TUPLE, v.kind());
    ASSERT_EQ(1U, v.size());

    ASSERT_EQ(10, as_int(v[0]));

    EXPECT_TRUE(v.is_valid());
    output_test(v);
    clone_test(v);
}

TEST_F(CoreValueTest, Null) {
    Null v;
    EXPECT_EQ(Value::Kind::NULL_, v.kind());

    EXPECT_TRUE(v.is_valid());
    output_test(v);
    clone_test(v);
}

TEST_F(CoreValueTest, Error) {
    Error v;
    EXPECT_EQ(Value::Kind::ERROR, v.kind());

    EXPECT_FALSE(v.is_valid());
    output_test(v);
    clone_test(v);
}

TEST_F(CoreValueTest, Bool_eq) {
    Bool a { true };
    Bool b { false };
    Int c { 1 };

    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
}

TEST_F(CoreValueTest, Int_eq) {
    Int a { 1 };
    Int b { 2 };
    Float c { 1 };

    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
}

TEST_F(CoreValueTest, Float_eq) {
    Float a { 1 };
    Float b { 2 };
    Int c { 1 };

    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
}

TEST_F(CoreValueTest, String_eq) {
    String a { "a" };
    String b { "b" };
    Int c { 1 };

    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
}

TEST_F(CoreValueTest, Tuple_eq) {
    Tuple a {
        Int(1),
        Int(2),
        Int(3),
    };
    Tuple b {
        Int(1),
        Int(2),
    };
    Tuple c {
        Int(1),
        Int(2),
        Int(3),
        Int(4),
    };
    Tuple d {
        Int(1),
        Int(2),
        Int(1),
    };
    Tuple e {
        Int(3),
        Int(2),
        Int(1),
    };
    Tuple f;
    Null g;

    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
    EXPECT_NE(a, e);
    EXPECT_NE(a, f);
    EXPECT_NE(a, g);
}
}  // namespace shakujo::common::core
