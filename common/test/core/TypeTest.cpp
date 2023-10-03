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
#include "shakujo/common/core/Type.h"

#include <gtest/gtest.h>
#include <memory>
#include <utility>
#include <sstream>

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/type/Float.h"
#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/type/Char.h"
#include "shakujo/common/core/type/String.h"
#include "shakujo/common/core/type/Null.h"
#include "shakujo/common/core/type/Array.h"
#include "shakujo/common/core/type/Vector.h"
#include "shakujo/common/core/type/Tuple.h"
#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/type/Cursor.h"
#include "shakujo/common/core/type/Error.h"

namespace shakujo::common::core {

using namespace shakujo::common::core::type;

class CoreTypeTest : public ::testing::Test {
public:
    template<typename T>
    std::unique_ptr<T> clone(const T& v) {
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
};

TEST_F(CoreTypeTest, Bool) {
    Bool t {};
    EXPECT_EQ(Type::Kind::BOOL, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Int) {
    Int t { 32U };
    EXPECT_EQ(Type::Kind::INT, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_EQ(32U, t.size());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Float) {
    Float t { 32U };
    EXPECT_EQ(Type::Kind::FLOAT, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_EQ(32U, t.size());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Char) {
    Char t { 10U };
    EXPECT_EQ(Type::Kind::CHAR, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_EQ(10U, t.size());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, String) {
    String t {};
    EXPECT_EQ(Type::Kind::STRING, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Null) {
    Null t {};
    EXPECT_EQ(Type::Kind::NULL_, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Array) {
    Array t {
        Int { 32U },
        100U,
    };
    EXPECT_EQ(Type::Kind::ARRAY, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_EQ(Type::Kind::INT, t.element_type()->kind());
    EXPECT_EQ(100U, t.size());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Vector) {
    Vector t {
        Int { 32U },
    };
    EXPECT_EQ(Type::Kind::VECTOR, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_EQ(Type::Kind::INT, t.element_type()->kind());
    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Tuple) {
    Tuple t {
        Tuple::Element { "a", Int {32} },
        Tuple::Element { "b", Int {64} },
        Tuple::Element { Float {32} },
    };
    auto &es = t.elements();
    ASSERT_EQ(3U, es.size());
    EXPECT_EQ(Type::Kind::INT, es[0].type()->kind());
    EXPECT_EQ(Type::Kind::INT, es[1].type()->kind());
    EXPECT_EQ(Type::Kind::FLOAT, es[2].type()->kind());

    EXPECT_EQ("a", es[0].name());
    EXPECT_EQ("b", es[1].name());
    EXPECT_TRUE(es[2].name().empty());

    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Relation) {
    Relation t {
        {
            Relation::Column { "a", Int {32} },
            Relation::Column { "b", Int {64} },
            Relation::Column { Float {32} },
        }
    };
    auto& es = t.columns();
    ASSERT_EQ(3U, es.size());
    EXPECT_EQ(Type::Kind::INT, es[0].type()->kind());
    EXPECT_EQ(Type::Kind::INT, es[1].type()->kind());
    EXPECT_EQ(Type::Kind::FLOAT, es[2].type()->kind());
    EXPECT_EQ("a", es[0].name());
    EXPECT_EQ("b", es[1].name());
    EXPECT_TRUE(es[2].name().empty());

    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Cursor) {
    Cursor t {
        Relation {
            {
                Relation::Column { "a", Int {32} },
                Relation::Column { "b", Int {64} },
                Relation::Column { Float {32} },
            }
        }
    };
    auto r = t.element_type();
    auto& es = r->columns();
    ASSERT_EQ(3U, es.size());
    EXPECT_EQ(Type::Kind::INT, es[0].type()->kind());
    EXPECT_EQ(Type::Kind::INT, es[1].type()->kind());
    EXPECT_EQ(Type::Kind::FLOAT, es[2].type()->kind());
    EXPECT_EQ("a", es[0].name());
    EXPECT_EQ("b", es[1].name());
    EXPECT_TRUE(es[2].name().empty());

    EXPECT_TRUE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Error) {
    Error t {};
    EXPECT_EQ(Type::Kind::ERROR, t.kind());
    EXPECT_EQ(Type::Nullity::NULLABLE, t.nullity());
    EXPECT_FALSE(t.is_valid());
    output_test(t);
    clone_test(t);
}

TEST_F(CoreTypeTest, Bool_eq) {
    Bool a {};
    Bool b { Type::Nullity::NEVER_NULL };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
}

TEST_F(CoreTypeTest, Int_eq) {
    Int a { 32U };
    Int b { 64U };
    Int c { 32U, Type::Nullity::NEVER_NULL };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(b, c);
}

TEST_F(CoreTypeTest, Float_eq) {
    Float a { 32U };
    Float b { 64U };
    Float c { 32U, Type::Nullity::NEVER_NULL };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(b, c);
}

TEST_F(CoreTypeTest, Char_eq) {
    Char a { 10U };
    Char b { 20U };
    Char c { 10U, Type::Nullity::NEVER_NULL };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(b, c);
}

TEST_F(CoreTypeTest, String_eq) {
    String a {};
    String b { Type::Nullity::NEVER_NULL };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
}

TEST_F(CoreTypeTest, Null_eq) {
    Null a {};
    Null b {};
    EXPECT_EQ(a, a);
    EXPECT_EQ(a, b);
}

TEST_F(CoreTypeTest, Tuple_eq) {
    Tuple a {};
    Tuple b {
        Int(32U),
    };
    Tuple c {
        Int(32U, Type::Nullity::NEVER_NULL),
    };
    Tuple d {
        Int(64U),
    };
    Tuple e {
        { Int(32U), },
        Type::Nullity::NEVER_NULL
    };
    Tuple f {
        Int(32U),
        Int(64U),
    };
    Tuple g {
        { "a", Int(32U) },
    };
    Tuple h {
        { "b", Int(32U) },
    };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_EQ(b, b);
    EXPECT_NE(b, c);
    EXPECT_NE(b, d);
    EXPECT_NE(b, e);
    EXPECT_NE(b, f);
    EXPECT_NE(b, g);
    EXPECT_NE(g, h);
}

TEST_F(CoreTypeTest, Array_eq) {
    Array a { Int(32U), 10U };
    Array b { Int(32U), 20U };
    Array c { Int(64U), 10U };
    Array d { Int(32U), 10U, Type::Nullity::NEVER_NULL };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
    EXPECT_NE(a, d);
}

TEST_F(CoreTypeTest, Vector_eq) {
    Vector a { Int(32U) };
    Vector b { Int(64U) };
    Vector c { Int(32U), Type::Nullity::NEVER_NULL };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_NE(a, c);
}

TEST_F(CoreTypeTest, Relation_eq) {
    Relation a {};
    Relation b {
        Int(32U),
    };
    Relation c {
        Int(32U, Type::Nullity::NEVER_NULL),
    };
    Relation d {
        Int(64U),
    };
    Relation f {
        Int(32U),
        Int(64U),
    };
    Relation g {
        { "a", Int(32U) },
    };
    Relation h {
        { "b", Int(32U) },
    };
    Relation i {
        { { Name("q") }, "a", Int(32U) },
    };
    Relation j {
        { { Name("r") }, "a", Int(32U) },
    };
    Relation k {
        { { Name("q"), Name("r") }, "a", Int(32U) },
    };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
    EXPECT_EQ(b, b);
    EXPECT_NE(b, c);
    EXPECT_NE(b, d);

    EXPECT_NE(b, f);
    EXPECT_NE(b, g);
    EXPECT_NE(g, h);
    EXPECT_NE(g, i);

    EXPECT_NE(i, j);
    EXPECT_NE(i, k);
}

TEST_F(CoreTypeTest, Cursor_eq) {
    Cursor a {
        Relation {
            Int(32U),
        }
    };
    Cursor b {
        Relation {
            Int(64U),
        }
    };
    EXPECT_EQ(a, a);
    EXPECT_NE(a, b);
}

TEST_F(CoreTypeTest, Error_eq) {
    Error a {};
    Error b {};
    EXPECT_EQ(a, a);
    EXPECT_EQ(a, b);
}
}  // namespace shakujo::common::core
