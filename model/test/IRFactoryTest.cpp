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
#include <gtest/gtest.h>

#include "shakujo/model/IRFactory.h"
#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Array.h"
#include "shakujo/common/core/type/Vector.h"
#include "shakujo/common/core/type/Tuple.h"
#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/type/Cursor.h"
#include "shakujo/common/core/type/Error.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/value/Error.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::model {

class IRFactoryTest : public ::testing::Test {
public:
    template<class S, class T>
    bool eq(S&& s, T const* t) {
        return t && s == *t;
    }
};

using namespace shakujo::model::expression;
using namespace shakujo::model::statement;

namespace c = shakujo::common::core;
namespace v = shakujo::common::core::value;
namespace t = shakujo::common::core::type;

using str_vector = std::vector<std::string>;

static std::unique_ptr<shakujo::common::core::Value> as_value(Expression const* e) {
    if (e->kind() == ExpressionKind::LITERAL) {
        return common::util::make_clone(dynamic_cast<Literal const *>(e)->value());
    }
    return std::make_unique<v::Error>();
}

TEST_F(IRFactoryTest, simple) {
    IRFactory f;
    auto e = f.BinaryOperator(
        BinaryOperator::Kind::ADD,
        f.Literal(t::Int(32U), v::Int(100)),
        f.Literal(t::Int(32U), v::Int(200))
    );
    ASSERT_EQ(ExpressionKind::BINARY_OPERATOR, e->kind());
    EXPECT_EQ(v::Int(100), *as_value(e->left()));
    EXPECT_EQ(v::Int(200), *as_value(e->right()));
}

TEST_F(IRFactoryTest, clone_empty_object) {
    IRFactory f;
    auto e = f.GlobalVariableDeclaration();
    std::unique_ptr<program::GlobalVariableDeclaration> copy { e->clone() };
    ASSERT_NE(e.get(), copy.get());
}

TEST_F(IRFactoryTest, node_list) {
    IRFactory f;
    auto e = f.ArrayCreationExpression(
        f.ArrayType(f.Int32Type(), 3),
        {
            f.Literal(t::Int(32U), v::Int(100)),
            f.Literal(t::Int(32U), v::Int(200)),
            f.Literal(t::Int(32U), v::Int(300)),
        }
    );
    ASSERT_EQ(ExpressionKind::ARRAY_CREATION_EXPRESSION, e->kind());
    EXPECT_EQ(type::TypeKind::ARRAY_TYPE, e->type()->kind());
    ASSERT_EQ(3U, e->values().size());
    EXPECT_EQ(v::Int(100), *as_value(e->values()[0]));
    EXPECT_EQ(v::Int(200), *as_value(e->values()[1]));
    EXPECT_EQ(v::Int(300), *as_value(e->values()[2]));
}

TEST_F(IRFactoryTest, fragment_list) {
    IRFactory f;
    auto e = f.CaseExpression(
        {
            f.CaseExpressionClause(
                f.Literal(t::Bool(), v::Bool(true)),
                f.Literal(t::Int(32U), v::Int(100))
            ),
            f.CaseExpressionClause(
                f.Literal(t::Bool(), v::Bool(false)),
                f.Literal(t::Int(32U), v::Int(200))
            ),
        },
        f.Literal(t::Int(32U), v::Int(-1))
    );
    ASSERT_EQ(ExpressionKind::CASE_EXPRESSION, e->kind());
    ASSERT_EQ(2U, e->cases().size());
    EXPECT_EQ(v::Bool(true), *as_value(e->cases()[0]->condition()));
    EXPECT_EQ(v::Int(100), *as_value(e->cases()[0]->body()));
    EXPECT_EQ(v::Bool(false), *as_value(e->cases()[1]->condition()));
    EXPECT_EQ(v::Int(200), *as_value(e->cases()[1]->body()));
    EXPECT_EQ(v::Int(-1), *as_value(e->default_expression()));
}

TEST_F(IRFactoryTest, value_list) {
    IRFactory f;
    auto e = f.InsertRelationStatement(
        f.Name("TBL"),
        {
            f.Name("c1"),
            f.Name("c2"),
            f.Name("c3"),
        },
        f.Literal(t::Bool(), v::Bool(true))
    );
    ASSERT_EQ(StatementKind::INSERT_RELATION_STATEMENT, e->kind());
    EXPECT_EQ(str_vector({"TBL"}), e->table()->segments());
    EXPECT_EQ(3U, e->columns().size());
    EXPECT_EQ(str_vector({"c1"}), e->columns()[0]->segments());
    EXPECT_EQ(str_vector({"c2"}), e->columns()[1]->segments());
    EXPECT_EQ(str_vector({"c3"}), e->columns()[2]->segments());
    EXPECT_EQ(v::Bool(true), *as_value(e->source()));
}

TEST_F(IRFactoryTest, enum_list) {
    IRFactory f;
    auto e = f.LocalVariableDeclaration(
        f.Name("v"),
        f.Int32Type(),
        {},
        {
            LocalVariableDeclaration::Attribute::CONST,
        }
    );
    auto& attrs = e->attributes();
    ASSERT_EQ(1U, attrs.size());
    EXPECT_NE(attrs.end(), attrs.find(LocalVariableDeclaration::Attribute::CONST));
}

TEST_F(IRFactoryTest, name_simple) {
    IRFactory f;
    auto e = f.Name("a");
    EXPECT_EQ(name::NameKind::SIMPLE_NAME, e->kind());
    EXPECT_EQ("a", e->token());
}

TEST_F(IRFactoryTest, name_qualified) {
    IRFactory f;
    auto e = f.Name("a", "b", "c");
    EXPECT_EQ(name::NameKind::QUALIFIED_NAME, e->kind());
    EXPECT_EQ(str_vector({"a", "b", "c"}), e->segments());
}

TEST_F(IRFactoryTest, name_resolve) {
    IRFactory f;
    auto q = f.Name("a");
    auto e = f.Name(std::move(q), "b");
    EXPECT_EQ(name::NameKind::QUALIFIED_NAME, e->kind());
    EXPECT_EQ(str_vector({"a", "b"}), e->segments());
}

TEST_F(IRFactoryTest, name_resolve2) {
    IRFactory f;
    auto q = f.Name("a");
    auto e = f.Name(std::move(q), "b", "c");
    EXPECT_EQ(name::NameKind::QUALIFIED_NAME, e->kind());
    EXPECT_EQ(str_vector({"a", "b", "c"}), e->segments());
}

TEST_F(IRFactoryTest, name_resolve3) {
    IRFactory f;
    auto q = f.Name("a");
    auto e = f.Name(std::move(q), "b", "c", "d");
    EXPECT_EQ(name::NameKind::QUALIFIED_NAME, e->kind());
    EXPECT_EQ(str_vector({"a", "b", "c", "d"}), e->segments());
}

TEST_F(IRFactoryTest, name_vector_simple) {
    IRFactory f;
    str_vector segments {"a"};
    auto e = f.Name(segments);
    EXPECT_EQ(name::NameKind::SIMPLE_NAME, e->kind());
    EXPECT_EQ(segments, e->segments());
}

TEST_F(IRFactoryTest, name_vector_qualified) {
    IRFactory f;
    str_vector segments {"a", "b", "c"};
    auto e = f.Name(segments);
    EXPECT_EQ(name::NameKind::QUALIFIED_NAME, e->kind());
    EXPECT_EQ(segments, e->segments());
}

TEST_F(IRFactoryTest, name_core) {
    IRFactory f;
    str_vector segments {"a", "b", "c"};
    auto e = f.Name(c::Name(segments));
    EXPECT_EQ(name::NameKind::QUALIFIED_NAME, e->kind());
    EXPECT_EQ(segments, e->segments());
}

TEST_F(IRFactoryTest, index_name) {
    IRFactory f;
    auto e = f.Index(f.Name("a"));
    EXPECT_TRUE(e);
    EXPECT_EQ(str_vector({"a"}), e->name()->segments());
    EXPECT_EQ(name::Index::INVALID_POSITION, e->position());
}

TEST_F(IRFactoryTest, index_position) {
    IRFactory f;
    auto e = f.Index(3U);
    EXPECT_TRUE(e);
    EXPECT_EQ(nullptr, e->name());
    EXPECT_EQ(3U, e->position());
}

TEST_F(IRFactoryTest, literal_bool) {
    IRFactory f;
    auto e = f.Literal(t::Bool(), true);
    EXPECT_TRUE(e);
    EXPECT_TRUE(eq(t::Bool(), e->type()));
    EXPECT_TRUE(eq(v::Bool(true), e->value()));
}

TEST_F(IRFactoryTest, literal_int) {
    IRFactory f;
    auto e = f.Literal(t::Int(32U), 100);
    EXPECT_TRUE(e);
    EXPECT_TRUE(eq(t::Int(32U), e->type()));
    EXPECT_TRUE(eq(v::Int(100), e->value()));
}

TEST_F(IRFactoryTest, literal_float) {
    IRFactory f;
    auto e = f.Literal(t::Float(64U), 3.14);
    EXPECT_TRUE(e);
    EXPECT_TRUE(eq(t::Float(64U), e->type()));
    EXPECT_TRUE(eq(v::Float(3.14), e->value()));
}

TEST_F(IRFactoryTest, literal_char) {
    IRFactory f;
    auto e = f.Literal(t::Char(3), "hello");
    EXPECT_TRUE(e);
    EXPECT_TRUE(eq(t::Char(3), e->type()));
    EXPECT_TRUE(eq(v::String("hello"), e->value()));
}

TEST_F(IRFactoryTest, literal_string) {
    IRFactory f;
    auto e = f.Literal(t::String(), "hello");
    EXPECT_TRUE(e);
    EXPECT_TRUE(eq(t::String(), e->type()));
    EXPECT_TRUE(eq(v::String("hello"), e->value()));
}

TEST_F(IRFactoryTest, literal_null) {
    IRFactory f;
    auto e = f.Literal(t::Null());
    EXPECT_TRUE(e);
    EXPECT_TRUE(eq(t::Null(), e->type()));
    EXPECT_TRUE(eq(v::Null(), e->value()));
}

TEST_F(IRFactoryTest, type_bool) {
    IRFactory f;
    auto e = f.Type(t::Bool());
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.BooleanType(), *e);
}

TEST_F(IRFactoryTest, type_char) {
    IRFactory f;
    auto e = f.Type(t::Char(256U));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.CharType(256U), *e);
}

TEST_F(IRFactoryTest, type_int32) {
    IRFactory f;
    auto e = f.Type(t::Int(32U));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.Int32Type(), *e);
}

TEST_F(IRFactoryTest, type_int64) {
    IRFactory f;
    auto e = f.Type(t::Int(64U));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.Int64Type(), *e);
}

TEST_F(IRFactoryTest, type_float32) {
    IRFactory f;
    auto e = f.Type(t::Float(32U));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.Float32Type(), *e);
}

TEST_F(IRFactoryTest, type_float64) {
    IRFactory f;
    auto e = f.Type(t::Float(64U));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.Float64Type(), *e);
}

TEST_F(IRFactoryTest, type_string) {
    IRFactory f;
    auto e = f.Type(t::String());
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.StringType(), *e);
}

TEST_F(IRFactoryTest, type_null) {
    IRFactory f;
    auto e = f.Type(t::Null());
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.NullType(), *e);
}

TEST_F(IRFactoryTest, type_array) {
    IRFactory f;
    auto e = f.Type(t::Array(t::String(), 10U));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.ArrayType(f.StringType(), 10U), *e);
}

TEST_F(IRFactoryTest, type_varchar) {
    IRFactory f;
    auto e = f.Type(t::Char(true, 256U));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.VarCharType(256U), *e);
}

TEST_F(IRFactoryTest, type_vector) {
    IRFactory f;
    auto e = f.Type(t::Vector(t::String()));
    EXPECT_TRUE(e);
    EXPECT_EQ(*f.VectorType(f.StringType()), *e);
}

TEST_F(IRFactoryTest, type_tuple) {
    IRFactory f;
    auto e = f.Type(t::Tuple({
        { "a", t::Int(32U) },
        { "b", t::Int(64U) },
        { t::String() },
    }));
    EXPECT_TRUE(e);
    auto a = f.TupleType({
        f.TupleTypeElement(f.Name("a"), f.Int32Type()),
        f.TupleTypeElement(f.Name("b"), f.Int64Type()),
        f.TupleTypeElement(f.Name(""), f.StringType()),
    });
    EXPECT_EQ(*a, *e);
}

TEST_F(IRFactoryTest, type_relation) {
    IRFactory f;
    auto e = f.Type(t::Relation({
        { "a", t::Int(32U) },
        { "b", t::Int(64U) },
        { t::String() },
    }));
    EXPECT_TRUE(e);
    auto a = f.RelationType(f.TupleType({
        f.TupleTypeElement(f.Name("a"), f.Int32Type()),
        f.TupleTypeElement(f.Name("b"), f.Int64Type()),
        f.TupleTypeElement(f.Name(""), f.StringType()),
    }));
    EXPECT_EQ(*a, *e) << *a;
}

TEST_F(IRFactoryTest, type_cursor) {
    IRFactory f;
    auto e = f.Type(t::Cursor(t::Relation({
        { "a", t::Int(32U) },
        { "b", t::Int(64U) },
        { t::String() },
    })));
    EXPECT_FALSE(e) << "cursor type node is not yet implemented";
}

TEST_F(IRFactoryTest, type_error) {
    IRFactory f;
    auto e = f.Type(t::Error());
    EXPECT_FALSE(e);
}

TEST_F(IRFactoryTest, implicit_cast) {
    IRFactory f;
    auto e = f.ImplicitCast(t::Int(32U), f.Literal(t::Int(64U), 100));

    EXPECT_TRUE(eq(t::Int(32U), e->type()));

    auto v = e->operand();
    ASSERT_EQ(ExpressionKind::LITERAL, v->kind());
    auto literal = dynamic_cast<Literal const*>(v);
    EXPECT_TRUE(eq(t::Int(64U), literal->type()));
    EXPECT_TRUE(eq(v::Int(100), literal->value()));
}

}  // namespace shakujo::model
