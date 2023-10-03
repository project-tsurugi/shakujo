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
#include <gtest/gtest.h>
#include <string>
#include <memory>
#include <functional>

#include "shakujo/common/util/Managed.h"
#include "shakujo/common/util/ManagedPtr.h"

namespace shakujo::common::util {

class ManagedPtrTest : public ::testing::Test {};

class TVal : public Managed<TVal> {
public:
    std::string val;
    bool* dtor;
    explicit TVal(std::string v, bool* f = nullptr) : val(std::move(v)), dtor(f) {}
    ~TVal() override {
        if (dtor) *dtor = true;
    };
};

TEST_F(ManagedPtrTest, simple) {
    ManagedPtr<TVal> p;
    ASSERT_FALSE(p);
    EXPECT_EQ(nullptr, p.get());
}

TEST_F(ManagedPtrTest, ctor_unique_ptr) {
    ManagedPtr<TVal> p { std::make_unique<TVal>("OK") };
    ASSERT_TRUE(p);
    EXPECT_TRUE(p->is_managed());
    EXPECT_EQ("OK", p->val);
}

TEST_F(ManagedPtrTest, ctor_raw_ptr) {
    ManagedPtr<TVal> p { std::make_unique<TVal>("OK") };
    ASSERT_TRUE(p);
    EXPECT_TRUE(p->is_managed());
    EXPECT_EQ("OK", p->val);
}

TEST_F(ManagedPtrTest, ctor_move) {
    ManagedPtr<TVal> p { std::make_unique<TVal>("OK") };
    ManagedPtr<TVal> q { std::move(p) };

    ASSERT_TRUE(q);
    EXPECT_TRUE(q->is_managed());
    q->replace([](std::unique_ptr<TVal>) {
        return std::make_unique<TVal>("REP");
    });
    EXPECT_EQ("REP", q->val);
}

TEST_F(ManagedPtrTest, assign_move) {
    bool destroyed = false;
    ManagedPtr<TVal> p { std::make_unique<TVal>("OLD", &destroyed) };
    p = ManagedPtr<TVal> { std::make_unique<TVal>("OK") };
    EXPECT_TRUE(destroyed);

    ASSERT_TRUE(p);
    EXPECT_TRUE(p->is_managed());
    EXPECT_EQ("OK", p->val);
}

TEST_F(ManagedPtrTest, assign_unique_ptr) {
    bool destroyed = false;
    ManagedPtr<TVal> p { std::make_unique<TVal>("OLD", &destroyed) };
    p = std::make_unique<TVal>("OK");
    EXPECT_TRUE(destroyed);

    ASSERT_TRUE(p);
    EXPECT_TRUE(p->is_managed());
    EXPECT_EQ("OK", p->val);
}

TEST_F(ManagedPtrTest, assign_nullptr) {
    bool destroyed = false;
    ManagedPtr<TVal> p { std::make_unique<TVal>("OLD", &destroyed) };
    p = nullptr;
    EXPECT_TRUE(destroyed);
    ASSERT_FALSE(p);
}

TEST_F(ManagedPtrTest, release) {
    ManagedPtr<TVal> p { std::make_unique<TVal>("OK") };

    TVal& v = *p;
    EXPECT_TRUE(v.is_managed());

    std::unique_ptr<TVal> r = p.release();
    EXPECT_FALSE(v.is_managed());
    EXPECT_EQ(&v, r.get());

    EXPECT_FALSE(p);
}

}  // namespace shakujo::common::util
