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
#include <functional>

#include "shakujo/common/util/Managed.h"
#include "shakujo/common/util/ManagedPtr.h"

namespace shakujo::common::util {

class ManagedTest : public ::testing::Test {};

class TVal : public Managed<TVal> {
public:
    std::string val;
    bool* dtor;
    explicit TVal(std::string v, bool* f = nullptr) : val(std::move(v)), dtor(f) {}
    ~TVal() override {
        if (dtor) {
            if (*dtor) std::cout << "dup dtor" << std::endl;
            *dtor = true;
        }
    };
    TVal(TVal const&) = default;
    TVal(TVal&&) noexcept {
        dtor = nullptr;
    }
    TVal& operator=(const TVal&) = default;
    TVal& operator=(TVal&&) noexcept {
        dtor = nullptr;
        return *this;
    }
};

TEST_F(ManagedTest, unmanaged) {
    TVal v { "OK" };
    EXPECT_FALSE(v.is_managed());
    EXPECT_THROW({
        v.replace([](auto) {
            return std::make_unique<TVal>("REP");
        });
    }, std::exception);
}

TEST_F(ManagedTest, replace) {
    bool v_d = false;
    ManagedPtr<TVal> p {std::make_unique<TVal>("OK", &v_d)};

    TVal& v = *p;
    ASSERT_TRUE(v.is_managed());
    EXPECT_EQ(&v, p.get());

    TVal* r = v.replace([](auto) {
        return std::make_unique<TVal>("REP");
    });
    ASSERT_TRUE(v_d);
    EXPECT_EQ(r, p.get());
}

TEST_F(ManagedTest, unmanaged_after_replaced) {
    ManagedPtr<TVal> p {std::make_unique<TVal>("OK")};
    std::unique_ptr<TVal> escaped;
    TVal* r = p->replace([&](auto p) {
        escaped = std::move(p);
        return std::make_unique<TVal>("REP");
    });
    EXPECT_TRUE(r->is_managed());
    EXPECT_FALSE(escaped->is_managed());
}

TEST_F(ManagedTest, copy) {
    ManagedPtr<TVal> p {std::make_unique<TVal>("OK")};
    TVal c { *p };
    EXPECT_TRUE(p->is_managed());
    EXPECT_FALSE(c.is_managed());
}

TEST_F(ManagedTest, copy_assign) {
    ManagedPtr<TVal> p {std::make_unique<TVal>("OK")};
    TVal c { "" };
    c = *p;
    EXPECT_TRUE(p->is_managed());
    EXPECT_FALSE(c.is_managed());
}

TEST_F(ManagedTest, move) {
    ManagedPtr<TVal> p {std::make_unique<TVal>("OK")};
    TVal c { std::move(*p) };
    EXPECT_FALSE(c.is_managed());
}

TEST_F(ManagedTest, move_assign) {
    ManagedPtr<TVal> p {std::make_unique<TVal>("OK")};
    TVal c { "" };
    c = std::move(*p);
    EXPECT_FALSE(c.is_managed());
}

}  // namespace shakujo::common::util
