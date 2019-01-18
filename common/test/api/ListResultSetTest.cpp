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
#include "shakujo/common/api/ListResultSet.h"

#include <gtest/gtest.h>

#include "shakujo/common/util/utility.h"

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/type/Float.h"
#include "shakujo/common/core/type/Char.h"
#include "shakujo/common/core/type/String.h"

#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/core/value/String.h"
#include "shakujo/common/core/value/Null.h"

namespace shakujo::common::api {

namespace t = core::type;
namespace v = core::value;
const static core::Type::Nullity NON_NULL = core::Type::Nullity::NEVER_NULL;
const static core::Type::Nullity NULLABLE = core::Type::Nullity::NULLABLE;

class ListResultSetTest : public ::testing::Test {};

TEST_F(ListResultSetTest, simple) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U, NON_NULL) }
        }, {
            v::Tuple {
                v::Int(100),
            }
        }
    };

    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_FALSE(rs.is_null(0U));
        EXPECT_EQ(100, rs.get_int32(0U));
    }

    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, empty_list) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U, NON_NULL) }
        }, {
        }
    };

    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, multiple_rows) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U, NON_NULL) }
        }, {
            v::Tuple { v::Int(1) },
            v::Tuple { v::Int(2) },
            v::Tuple { v::Int(3) },
        }
    };

    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_FALSE(rs.is_null(0U));
        EXPECT_EQ(1, rs.get_int32(0U));
    }
    ASSERT_TRUE(rs.next());
    {
        EXPECT_FALSE(rs.is_null(0U));
        EXPECT_EQ(2, rs.get_int32(0U));
    }
    ASSERT_TRUE(rs.next());
    {
        EXPECT_FALSE(rs.is_null(0U));
        EXPECT_EQ(3, rs.get_int32(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, multiple_cols) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U, NON_NULL) },
            { "C1", t::Int(32U, NON_NULL) },
            { "C2", t::Int(32U, NON_NULL) },
        }, {
            v::Tuple {
                v::Int(1),
                v::Int(2),
                v::Int(3),
            }
        }
    };

    EXPECT_EQ(3U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_FALSE(rs.is_null(0U));
        EXPECT_EQ(1, rs.get_int32(0U));

        EXPECT_FALSE(rs.is_null(1U));
        EXPECT_EQ(2, rs.get_int32(1U));

        EXPECT_FALSE(rs.is_null(2U));
        EXPECT_EQ(3, rs.get_int32(2U));
    }

    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, null_column) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U, NULLABLE) }
        }, {
            v::Tuple { v::Null() },
        }
    };

    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }

    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, lack_columns) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U, NULLABLE) },
            { "C1", t::Int(32U, NULLABLE) },
        }, {
            v::Tuple { v::Int(1) },
        }
    };

    EXPECT_EQ(2U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_FALSE(rs.is_null(0U));
        EXPECT_TRUE(rs.is_null(1U));
    }

    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_bool) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Bool() },
        }, {
            v::Tuple { v::Bool(true) },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        EXPECT_TRUE(rs.get_bool(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_bool_null) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Bool() },
        }, {
            v::Tuple { v::Null() },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_bool_inconsistent) {
    auto lambda = []() {
        return ListResultSet {
            core::type::Relation{
                {"C0", t::Bool()},
            }, {
                v::Tuple{v::Int(100)},
            }
        };
    };
    EXPECT_THROW({ lambda(); }, std::invalid_argument);
}

TEST_F(ListResultSetTest, type_int32) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U) },
        }, {
            v::Tuple { v::Int(100) },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        EXPECT_EQ(100, rs.get_int32(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_int32_null) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(32U) },
        }, {
            v::Tuple { v::Null() },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_int32_inconsistent) {
    auto lambda = []() {
        return ListResultSet {
            core::type::Relation{
                {"C0", t::Int(32U)},
            }, {
                v::Tuple{v::Bool(false)},
            }
        };
    };
    EXPECT_THROW({ lambda(); }, std::invalid_argument);
}

TEST_F(ListResultSetTest, type_int64) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(64U) },
        }, {
            v::Tuple { v::Int(100) },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        EXPECT_EQ(100, rs.get_int64(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_int64_null) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Int(64U) },
        }, {
            v::Tuple { v::Null() },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_int64_inconsistent) {
    auto lambda = []() {
        return ListResultSet {
            core::type::Relation{
                {"C0", t::Int(64U)},
            }, {
                v::Tuple{v::Bool(false)},
            }
        };
    };
    EXPECT_THROW({ lambda(); }, std::invalid_argument);
}

TEST_F(ListResultSetTest, type_float32) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Float(32U) },
        }, {
            v::Tuple { v::Float(100) },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        EXPECT_NEAR(100, rs.get_float32(0U), 0.1);
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_float32_null) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Float(32U) },
        }, {
            v::Tuple { v::Null() },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_float32_inconsistent) {
    auto lambda = []() {
        return ListResultSet {
            core::type::Relation{
                {"C0", t::Float(32U)},
            }, {
                v::Tuple{v::Bool(false)},
            }
        };
    };
    EXPECT_THROW({ lambda(); }, std::invalid_argument);
}

TEST_F(ListResultSetTest, type_float64) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Float(64U) },
        }, {
            v::Tuple { v::Float(100) },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        EXPECT_NEAR(100, rs.get_float64(0U), 0.1);
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_float64_null) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Float(64U) },
        }, {
            v::Tuple { v::Null() },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_float64_inconsistent) {
    auto lambda = []() {
        return ListResultSet {
            core::type::Relation{
                {"C0", t::Float(64U)},
            }, {
                v::Tuple{v::Bool(false)},
            }
        };
    };
    EXPECT_THROW({ lambda(); }, std::invalid_argument);
}

TEST_F(ListResultSetTest, type_char_return_buffer) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Char(5U) },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        auto buffer = rs.get_text(0U);
        ASSERT_EQ(buffer, "Hello");
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_char_fill_buffer) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Char(5U) },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        char buffer[100];
        auto text = rs.get_text(0U, &buffer[0], 5U);
        ASSERT_EQ(text, "Hello");
        EXPECT_EQ(text.data(), &buffer[0]);
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_char_return_string) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Char(5U) },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        EXPECT_EQ("Hello", rs.get_text(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_char_fill_string) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Char(5U) },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        std::string buffer;
        rs.get_text(0U, buffer);
        EXPECT_EQ("Hello", buffer);
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_char_null) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::Char(5U) },
        }, {
            v::Tuple { v::Null() },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_char_inconsistent) {
    auto lambda = []() {
        return ListResultSet {
            core::type::Relation{
                {"C0", t::Char(5U)},
            }, {
                v::Tuple{v::Int(100)},
            }
        };
    };
    EXPECT_THROW({ lambda(); }, std::invalid_argument);
}

TEST_F(ListResultSetTest, type_string_return_buffer) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::String() },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        auto text = rs.get_text(0U);
        ASSERT_GE(text, "Hello");
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_string_fill_buffer) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::String() },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        char buffer[100];
        auto text = rs.get_text(0U, &buffer[0], 100);
        EXPECT_EQ("Hello", text);
        EXPECT_EQ(text.data(), &buffer[0]);
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_string_return_string) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::String() },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        EXPECT_EQ("Hello", rs.get_text(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_string_fill_string) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::String() },
        }, {
            v::Tuple { v::String("Hello") },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        ASSERT_FALSE(rs.is_null(0U));
        std::string buffer;
        rs.get_text(0U, buffer);
        EXPECT_EQ("Hello", buffer);
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_string_null) {
    ListResultSet rs {
        core::type::Relation {
            { "C0", t::String() },
        }, {
            v::Tuple { v::Null() },
        }
    };
    EXPECT_EQ(1U, rs.size());
    ASSERT_TRUE(rs.next());
    {
        EXPECT_TRUE(rs.is_null(0U));
    }
    ASSERT_FALSE(rs.next());
}

TEST_F(ListResultSetTest, type_string_inconsistent) {
    auto lambda = []() {
        return ListResultSet {
            core::type::Relation{
                {"C0", t::String()},
            }, {
                v::Tuple{v::Int(100)},
            }
        };
    };
    EXPECT_THROW({ lambda(); }, std::invalid_argument);
}

}  // namespace shakujo::common::api
