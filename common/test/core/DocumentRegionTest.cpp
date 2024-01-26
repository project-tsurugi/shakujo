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
#include "shakujo/common/core/DocumentRegion.h"

#include <gtest/gtest.h>
#include <utility>
#include <sstream>

namespace shakujo::common::core {

using Region = shakujo::common::core::DocumentRegion;
using Pos = Region::Position;

class DocumentRegionTest : public ::testing::Test {
public:
    template<typename T>
    void output_test(const T& v) {
        std::ostringstream ss;
        ss << v;
        auto str = ss.str();
        EXPECT_FALSE(str.empty()) << str;
    }

    template<typename T>
    void compare_order(const T&) {}

    template<typename T, typename... Args>
    void compare_order(const T& first, const T& second, const Args&... rest) {
        EXPECT_EQ(first, first);
        EXPECT_LT(first, second);
        EXPECT_GT(second, first);
        compare_order(first, rest...);
        compare_order(second, rest...);
    }
};

TEST_F(DocumentRegionTest, pos_default) {
    Pos p;
    EXPECT_FALSE(p);
    EXPECT_EQ(0U, p.line_number());
    EXPECT_EQ(0U, p.column_number());

    output_test(p);
}

TEST_F(DocumentRegionTest, pos_line_only) {
    Pos p { 10U };
    EXPECT_TRUE(p);
    EXPECT_EQ(10U, p.line_number());
    EXPECT_EQ(0U, p.column_number());

    output_test(p);
}

TEST_F(DocumentRegionTest, pos_line_column) {
    Pos p { 1U, 2U };
    EXPECT_TRUE(p);
    EXPECT_EQ(1U, p.line_number());
    EXPECT_EQ(2U, p.column_number());

    output_test(p);
}

TEST_F(DocumentRegionTest, pos_column_only) {
    Pos p { 0U, 10U };
    EXPECT_FALSE(p);
    EXPECT_EQ(0U, p.line_number());
    EXPECT_EQ(0U, p.column_number());
}

TEST_F(DocumentRegionTest, pos_compare) {
    Pos a;
    Pos b { 1U };
    Pos c { 1U, 1U };
    Pos d { 1U, 2U };
    Pos e { 2U };
    Pos f { 2U, 1U };
    compare_order(a, b, c, d, e, f);
}

TEST_F(DocumentRegionTest, reg_default) {
    Region r;
    EXPECT_FALSE(r);

    EXPECT_EQ("", r.path());
    EXPECT_FALSE(r.begin());
    EXPECT_FALSE(r.end());

    output_test(r);
}

TEST_F(DocumentRegionTest, reg_path) {
    auto path = std::make_shared<std::string>("testing");

    Region r { path };
    EXPECT_TRUE(r);

    EXPECT_EQ("testing", r.path());
    EXPECT_EQ(path.get(), r.path_shared().get());

    EXPECT_FALSE(r.begin());
    EXPECT_FALSE(r.end());

    output_test(r);
}

TEST_F(DocumentRegionTest, reg_begin) {
    auto path = std::make_shared<std::string>("testing");
    Region r { path, Pos { 1U, 2U } };
    EXPECT_TRUE(r);

    EXPECT_EQ("testing", r.path());
    EXPECT_EQ(path.get(), r.path_shared().get());

    EXPECT_EQ(Pos(1U, 2U), r.begin());
    EXPECT_FALSE(r.end());

    output_test(r);
}

TEST_F(DocumentRegionTest, reg_all) {
    auto path = std::make_shared<std::string>("testing");
    Region r { path, Pos { 1U, 2U }, Pos { 3U } };
    EXPECT_TRUE(r);

    EXPECT_EQ("testing", r.path());
    EXPECT_EQ(path.get(), r.path_shared().get());

    EXPECT_EQ(Pos(1U, 2U), r.begin());
    EXPECT_EQ(Pos(3U), r.end());

    output_test(r);
}


class Acceptor {
    Region region_;
public:
    void region(const Region& r) {
        region_ = r;
    }
    Region& region() {
        return region_;
    }
};
TEST_F(DocumentRegionTest, inject) {
    Region r { "OK" };
    std::unique_ptr<Acceptor> p { new Acceptor() };

    EXPECT_EQ("", p->region().path());

    auto p2 = std::move(p) << r;
    EXPECT_EQ("OK", p2->region().path());
}

}  // namespace shakujo::common::core
