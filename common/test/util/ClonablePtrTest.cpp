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
#include <gtest/gtest.h>
#include <string>
#include <memory>

#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/Clonable.h"

namespace shakujo::common::util {

class ClonablePtrTest : public ::testing::Test {};

class TBase : public Clonable {
public:
    std::string value;
    explicit TBase(std::string v) : value(std::move(v)) {}
    TBase* clone() const & override { return new TBase(value); }
    TBase* clone() && override { return new TBase(std::move(value)); }
};

class TVal : public TBase {
public:
    explicit TVal(std::string v) : TBase(std::move(v)) {}
    TVal* clone() const & override { return new TVal(value); }
    TVal* clone() && override { return new TVal(std::move(value)); }
};

TEST_F(ClonablePtrTest, simple) {
    ClonablePtr<TVal> p { std::make_unique<TVal>("OK") };
    EXPECT_TRUE(p);
    EXPECT_EQ("OK", p->value);
}

TEST_F(ClonablePtrTest, nil) {
    ClonablePtr<TVal> p;
    EXPECT_FALSE(p);
    EXPECT_EQ(nullptr, p.get());

    auto c = p;
    EXPECT_FALSE(c);
    EXPECT_EQ(nullptr, c.get());

    auto m = std::move(c);
    EXPECT_FALSE(m);
    EXPECT_EQ(nullptr, m.get());
}

TEST_F(ClonablePtrTest, copy_base) {
    ClonablePtr<TVal> p { std::make_unique<TVal>("OK") };
    ClonablePtr<TBase> b = p;
    EXPECT_EQ("OK", b->value);
    EXPECT_NE(nullptr, p.get());
    EXPECT_NE(b.get(), p.get());
}

TEST_F(ClonablePtrTest, move_base) {
    ClonablePtr<TVal> p { std::make_unique<TVal>("OK") };
    ClonablePtr<TBase> b = std::move(p);
    EXPECT_EQ("OK", b->value);
    EXPECT_EQ(nullptr, p.get());
}

TEST_F(ClonablePtrTest, assign_unique) {
    ClonablePtr<TVal> p;
    auto u = std::make_unique<TVal>("OK");
    p = std::move(u);
    EXPECT_EQ("OK", p->value);
}

}  // namespace shakujo::common::util
