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
#include "shakujo/common/util/utility.h"

#include <gtest/gtest.h>

namespace shakujo::common::util {

class CommonUtilityTest : public ::testing::Test {};

struct C {
    int value;
    C(int v) : value(v) {}
    virtual ~C() = default;
    virtual C* clone() const& { return new C(value); }
    virtual C* clone() && { return new C(value + 1); }
};

struct D : public C {
    using C::C;
    D* clone() const& override { return new D(value); }
    D* clone() && override { return new D(value + 1); }
};

TEST_F(CommonUtilityTest, equals_raw_ptr) {
    int a = 1;
    int b = 1;
    EXPECT_TRUE(equals(&a, &b));
}

TEST_F(CommonUtilityTest, equals_unique_ptr) {
    auto a = std::make_unique<int>(1);
    auto b = std::make_unique<int>(1);
    EXPECT_TRUE(equals(a, b));
}

TEST_F(CommonUtilityTest, equals_mixed) {
    auto a = std::make_unique<int>(1);
    auto b = std::make_unique<int>(1);
    EXPECT_TRUE(equals(a, b.get()));
}

TEST_F(CommonUtilityTest, dynamic_pointer_cast_raw_ptr) {
    D object { 1 };
    C* ptr = &object;
    auto narrow = dynamic_pointer_cast<D>(ptr);
    EXPECT_TRUE((std::is_same_v<decltype(narrow), D*>));
}

TEST_F(CommonUtilityTest, dynamic_pointer_cast_raw_ptr_const) {
    D object { 1 };
    C const* ptr = &object;
    auto narrow = dynamic_pointer_cast<D>(ptr);
    EXPECT_TRUE((std::is_same_v<decltype(narrow), D const*>));
}

TEST_F(CommonUtilityTest, dynamic_pointer_cast_unique_ptr) {
    std::unique_ptr<C> ptr = std::make_unique<D>(1);
    auto narrow = dynamic_pointer_cast<D>(std::move(ptr));
    EXPECT_TRUE((std::is_same_v<decltype(narrow), std::unique_ptr<D>>));
}

TEST_F(CommonUtilityTest, dynamic_pointer_cast_unique_ptr_const) {
    std::unique_ptr<C const> ptr = std::make_unique<D>(1);
    auto narrow = dynamic_pointer_cast<D>(std::move(ptr));
    EXPECT_TRUE((std::is_same_v<decltype(narrow), std::unique_ptr<D const>>));
}

TEST_F(CommonUtilityTest, make_clone_raw_ptr) {
    auto v = std::make_unique<C>(1);
    auto c = make_clone(v.get());
    EXPECT_EQ(1, c->value);
    EXPECT_NE(v.get(), c.get());
}

TEST_F(CommonUtilityTest, make_clone_unique_ptr) {
    auto v = std::make_unique<C>(1);
    auto c = make_clone(v);
    EXPECT_EQ(1, c->value);
    EXPECT_NE(v.get(), c.get());
}

TEST_F(CommonUtilityTest, make_clone_ref) {
    auto v = std::make_unique<C>(1);
    auto c = make_clone(*v);
    EXPECT_EQ(1, c->value);
    EXPECT_NE(v.get(), c.get());
}

TEST_F(CommonUtilityTest, make_clone_rvref) {
    auto v = std::make_unique<C>(1);
    auto c = make_clone(std::move(*v));
    EXPECT_EQ(2, c->value);
    EXPECT_NE(v.get(), c.get());
}

TEST_F(CommonUtilityTest, to_string_simple) {
    auto s = to_string(1);
    EXPECT_EQ("1", s);
}

TEST_F(CommonUtilityTest, to_string_nothing) {
    auto s = to_string();
    EXPECT_EQ("", s);
}

TEST_F(CommonUtilityTest, to_string_concat) {
    auto s = to_string(1, 2, 3);
    EXPECT_EQ("123", s);
}

TEST_F(CommonUtilityTest, to_string_string) {
    auto s = to_string(std::string {"hello"});
    EXPECT_EQ("hello", s);
}

TEST_F(CommonUtilityTest, to_string_object_ptr) {
    std::string str { "hello" };
    auto s = to_string(&str);
    EXPECT_EQ("hello", s);
}

TEST_F(CommonUtilityTest, to_string_char_array) {
    auto s = to_string("hello");
    EXPECT_EQ("hello", s);
}

TEST_F(CommonUtilityTest, to_string_char_ptr) {
    const char* str = "hello";
    auto s = to_string(str);
    EXPECT_EQ("hello", s);
}

TEST_F(CommonUtilityTest, to_string_vector) {
    std::vector<std::string> v { "a", "b", "c" };
    auto s = to_string(v);
    EXPECT_EQ("[a, b, c]", s);
}

TEST_F(CommonUtilityTest, to_string_vector_nested) {
    std::vector<std::vector<std::string>> v {
        {},
        { "a" },
        { "b", "c" },
    };
    auto s = to_string(v);
    EXPECT_EQ("[[], [a], [b, c]]", s);
}

TEST_F(CommonUtilityTest, to_string_mixed) {
    auto s = to_string("a", 1, "b", 2);
    EXPECT_EQ("a1b2", s);
}

}  // namespace shakujo::common::util
