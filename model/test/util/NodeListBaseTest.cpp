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

#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::util {

    class TVal {
    private:
        const std::string str_;
        const int version_;

    public:
        TVal(std::string const& str, int version = 0)
            : str_(str)
            , version_(version) {}

        TVal* clone() const { return new TVal(str_, version_ + 1); }
        const std::string& str() const { return str_; }
        int version() const { return version_; }
    };

    template<typename... Args>
    static std::unique_ptr<TVal> make(Args&&... args) {
        return std::unique_ptr<TVal>(new TVal(std::forward<Args>(args)...));
    }

    using list_t = NodeListBase<std::unique_ptr<TVal>>;
    using ptr_vector = std::vector<std::unique_ptr<TVal>>;

    template<typename... Args>
    static ptr_vector vec(Args&&... args) {
        ptr_vector ret;
        using swallow = std::initializer_list<int>;
        (void)swallow{ (void(ret.push_back(std::move(args))), 0)... };
        return ret;
    }

    TEST(NodeListBaseTest, ctor_empty) {
        list_t list;
        ASSERT_EQ(0U, list.size());
    }

    TEST(NodeListBaseTest, ctor_vector) {
        list_t list(vec(make("A"), make("B"), make("C")));
        ASSERT_EQ(3U, list.size());
        EXPECT_EQ("A", list[0]->str());
        EXPECT_EQ("B", list[1]->str());
        EXPECT_EQ("C", list[2]->str());
    }

    TEST(NodeListBaseTest, iterator) {
        list_t list(vec(make("A"), make("B"), make("C")));
        std::string s;
        for (auto v : list) {
            s += v->str();
        }
        EXPECT_EQ("ABC", s);
    }

    TEST(NodeListBaseTest, const_iterator) {
        list_t list(vec(make("A"), make("B"), make("C")));
        std::string s;
        for (auto v : list) {
            s += v->str();
        }
        EXPECT_EQ("ABC", s);
    }

    TEST(NodeListBaseTest, push_back) {
        list_t list(vec(make("A")));
        std::unique_ptr<TVal> v {new TVal("X")};
        list.push_back(std::move(v));

        ASSERT_EQ(2U, list.size());
        EXPECT_EQ("X", list[1]->str());
        EXPECT_EQ(0, list[1]->version());
    }

    TEST(NodeListBaseTest, insert) {
        list_t list(vec(make("A"), make("B")));
        list.insert(0, make("0"));
        list.insert(2, make("2"));
        list.insert(4, make("4"));
        ASSERT_EQ(5U, list.size());
        EXPECT_EQ("0", list[0]->str());
        EXPECT_EQ("A", list[1]->str());
        EXPECT_EQ("2", list[2]->str());
        EXPECT_EQ("B", list[3]->str());
        EXPECT_EQ("4", list[4]->str());
    }

    TEST(NodeListBaseTest, assign) {
        list_t list(vec(make("A"), make("B"), make("C")));
        list.assign(1, make("X", 100));
        ASSERT_EQ(3U, list.size());
        EXPECT_EQ("A", list[0]->str());
        EXPECT_EQ("X", list[1]->str());
        EXPECT_EQ("C", list[2]->str());

        EXPECT_EQ(100, list[1]->version());
    }

    TEST(NodeListBaseTest, remove) {
        list_t list(vec(make("A"), make("B"), make("C")));
        list.remove(1);
        ASSERT_EQ(2U, list.size());
        EXPECT_EQ("A", list[0]->str());
        EXPECT_EQ("C", list[1]->str());
    }

    TEST(NodeListBaseTest, release) {
        list_t list(vec(make("A"), make("B", 100), make("C")));
        auto b = list.release(1);

        ASSERT_EQ(2U, list.size());
        EXPECT_EQ("A", list[0]->str());
        EXPECT_EQ("C", list[1]->str());

        EXPECT_EQ("B", b->str());
        EXPECT_EQ(100, b->version());
    }
}  // namespace shakujo::model::util
