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
#ifndef SHAKUJO_ANALYZER_ANALYZE_TYPING_H_
#define SHAKUJO_ANALYZER_ANALYZE_TYPING_H_

#include <memory>
#include <string>
#include <cassert>

#include "shakujo/common/core/type/ConstVisitor.h"
#include "shakujo/model/type/ConstTypeVisitor.h"
#include "shakujo/analyzer/binding/ExpressionBinding.h"

namespace shakujo::analyzer::analyze::typing {

using Type = common::core::Type;
using Value = common::core::Value;
using namespace common::core::type;

using common::util::dynamic_pointer_cast;
using common::util::make_clone;
using common::util::equals;
using common::util::is_defined;
using common::util::is_valid;
using common::util::to_string;

inline bool is_nullable(common::core::Type const* type) {
    return is_defined(type) && type->nullity() == common::core::Type::Nullity::NULLABLE;
}

inline bool is_boolean(Type const* type) {
    return is_defined(type) && type->kind() == Type::Kind::BOOL;
}

inline bool is_integral(Type const* type) {
    return is_valid(type) && type->kind() == Type::Kind::INT;
}

inline bool is_floating_point(Type const* type) {
    return is_valid(type) && type->kind() == Type::Kind::FLOAT;
}

inline bool is_numeric(Type const* type) {
    return is_integral(type) || is_floating_point(type);
}

inline bool is_textual(Type const* type) {
    return is_valid(type) && (type->kind() == Type::Kind::STRING || type->kind() == Type::Kind::CHAR);
}

inline bool is_atom(Type const* type) {
    return is_valid(type) && (is_boolean(type) || is_numeric(type) || is_textual(type));
}

inline bool is_boolean_convertible(common::core::Type const* type) {
    // FIXME: boolean convertible
    return is_atom(type);
}

inline std::unique_ptr<common::core::Type> binary_promotion(Type const*, Type const*);

inline bool is_equality_comparable(common::core::Type const* a, common::core::Type const* b) {
    if (is_valid(a) && is_valid(b) && is_atom(a) && is_atom(b)) {
        auto promoted = binary_promotion(a, b);
        return is_valid(promoted);
    }
    return false;
}

inline bool is_order_comparable(common::core::Type const* a, common::core::Type const* b) {
    return is_equality_comparable(a, b);
}

inline bool is_assignment_convertible(
        Type const* variable,
        binding::ExpressionBinding const& expression,
        bool force = false) {
    if (!expression.is_valid()) {
        return false;
    }
    // convert nulls
    if (expression.type()->kind() == Type::Kind::NULL_) {
        return is_nullable(variable);
    }

    // FIXME: constant promotion
    if (expression.constant()) {
        // FIXME: convert strings
        if (is_textual(variable) && is_textual(expression.type())) {
            return true;
        }
        if (is_integral(variable)
                && is_integral(expression.type())
                && expression.value()->kind() == Value::Kind::INT) {
            auto type = dynamic_pointer_cast<Int>(variable);
            auto value = dynamic_pointer_cast<common::core::value::Int>(expression.value());
            return type->min_value() <= value->get() && value->get() <= type->max_value();
        }
        if (is_floating_point(variable) && is_floating_point(expression.type())) {
            // FIXME: check exp bound
            return true;
        }
    }
    auto result = binary_promotion(variable, expression.type());
    // FIXME: check conversion rule
    return is_valid(result) && (force || equals(result, variable));
}

static inline std::unique_ptr<Type> nullity_binary_promotion(Type const* a, Type const* b) {
    assert(a->kind() == b->kind());  // NOLINT
    if (is_nullable(a) || !is_nullable(b)) {
        // cases (NULLABLE, *), (NEVER_NULL, NEVER_NULL) -> a
        return make_clone(a);
    }
    // cases (NEVER_NULL, NEVER_NULL), (*, NULLABLE)
    return make_clone(b);
}

static inline std::unique_ptr<Type> numeric_binary_promotion(Numeric const* a, Numeric const* b) {
    if (a->kind() == Type::Kind ::FLOAT || b->kind() == Type::Kind::FLOAT) {
        if (a->kind() == Type::Kind ::FLOAT && b->kind() == Type::Kind::FLOAT) {
            return std::make_unique<Float>(std::max(a->size(), b->size()), a->nullity() | b->nullity());
        }
        return std::make_unique<Float>(64U, a->nullity() | b->nullity());
    }
    assert(a->kind() == Type::Kind::INT);  // NOLINT
    assert(b->kind() == Type::Kind::INT);  // NOLINT
    return std::make_unique<Int>(std::max(a->size(), b->size()), a->nullity() | b->nullity());
}

static inline std::unique_ptr<Type> textual_binary_promotion(Textual const* a, Textual const* b) {
    if (a->kind() == Type::Kind::CHAR && b->kind() == Type::Kind::CHAR) {
        auto* ac = dynamic_pointer_cast<Char>(a);
        auto* bc = dynamic_pointer_cast<Char>(b);
        if (ac->variant() == bc->variant() && ac->size() == bc->size()) {
            return std::make_unique<Char>(ac->variant(), ac->size(), a->nullity() | b->nullity());
        }
    }
    return std::make_unique<String>(a->nullity() | b->nullity());
}

inline std::unique_ptr<Type> binary_promotion(Type const* a, Type const* b) {
    if (!is_valid(a) || !is_valid(b)) {
        return std::make_unique<Error>();
    }
    if (equals(a, b)) {
        return make_clone(a);
    }
    if (is_boolean(a) && is_boolean(b)) {
        return nullity_binary_promotion(a, b);
    }
    if (is_numeric(a) && is_numeric(b)) {
        return numeric_binary_promotion(dynamic_pointer_cast<Numeric>(a), dynamic_pointer_cast<Numeric>(b));
    }
    if (is_textual(a) && is_textual(b)) {
        return textual_binary_promotion(dynamic_pointer_cast<Textual>(a), dynamic_pointer_cast<Textual>(b));
    }
    return std::make_unique<Error>();
}

inline std::unique_ptr<Type> nullity(Type const* type, Type::Nullity nullity) {
    if (!is_valid(type)) {
        return std::make_unique<Error>();
    }
    if (type->nullity() == nullity) {
        return make_clone(type);
    }
    class NullChanger : public ConstVisitor<std::unique_ptr<Type>, Type::Nullity> {
    public:
        std::unique_ptr<Type> visit(Int const* node, Type::Nullity nullity) override {
            return std::make_unique<Int>(node->size(), nullity);
        }
        std::unique_ptr<Type> visit(Float const* node, Type::Nullity nullity) override {
            return std::make_unique<Float>(node->size(), nullity);
        }
        std::unique_ptr<Type> visit(Char const* node, Type::Nullity nullity) override {
            return std::make_unique<Char>(node->varying(), node->size(), nullity);
        }
        std::unique_ptr<Type> visit(String const* , Type::Nullity nullity) override {
            return std::make_unique<String>(nullity);
        }
        std::unique_ptr<Type> visit(Bool const* , Type::Nullity nullity) override {
            return std::make_unique<Bool>(nullity);
        }
        std::unique_ptr<Type> visit(Null const* , Type::Nullity) override {
            return std::make_unique<Null>();
        }
        std::unique_ptr<Type> visit(Tuple const* node, Type::Nullity nullity) override {
            return std::make_unique<Tuple>(node->elements(), nullity);
        }
        std::unique_ptr<Type> visit(Array const* node, Type::Nullity nullity) override {
            return std::make_unique<Array>(node->element_type(), node->size(), nullity);
        }
        std::unique_ptr<Type> visit(Vector const* node, Type::Nullity nullity) override {
            return std::make_unique<Vector>(node->element_type(), nullity);
        }
        std::unique_ptr<Type> visit(Relation const* node, Type::Nullity) override {
            return make_clone(node);
        }
        std::unique_ptr<Type> visit(Cursor const* node, Type::Nullity) override {
            return make_clone(node);
        }
        std::unique_ptr<Type> visit(Error const* , Type::Nullity) override {
            return std::make_unique<Error>();
        }
    };
    return NullChanger {}.dispatch(type, nullity);
}

inline std::unique_ptr<common::core::Type> convert(model::type::Type const* type) {
    class Converter : public model::type::ConstTypeVisitor<std::unique_ptr<Type>> {
    public:
        std::unique_ptr<Type> visit(model::type::ArrayType const* node) override {
            return std::make_unique<Array>(dispatch(node->element_type()), node->size(), Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::BooleanType const*) override {
            return std::make_unique<Bool>(Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::CharType const* node) override {
            return std::make_unique<Char>(node->size(), Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::Float32Type const*) override {
            return std::make_unique<Float>(32U, Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::Float64Type const*) override {
            return std::make_unique<Float>(64U, Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::Int32Type const*) override {
            return std::make_unique<Int>(32U, Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::Int64Type const*) override {
            return std::make_unique<Int>(64U, Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::StringType const*) override {
            return std::make_unique<String>(Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::NullType const*) override {
            return std::make_unique<Null>();
        }
        std::unique_ptr<Type> visit(model::type::TupleType const* node) override {
            std::vector<Tuple::Element> elements;
            elements.reserve(node->elements().size());
            for (auto* element : node->elements()) {
                elements.emplace_back(element->name()->token(), dispatch(element->type()));
            }
            return std::make_unique<Tuple>(std::move(elements), Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::RelationType const* node) override {
            std::vector<Relation::Column> columns;
            columns.reserve(node->element_type()->elements().size());
            for (auto* element : node->element_type()->elements()) {
                columns.emplace_back(element->name()->token(), dispatch(element->type()));
            }
            return std::make_unique<Relation>(std::move(columns));
        }
        std::unique_ptr<Type> visit(model::type::VarCharType const* node) override {
            return std::make_unique<Char>(true, node->size(), Type::Nullity::NULLABLE);
        }
        std::unique_ptr<Type> visit(model::type::VectorType const* node) override {
            return std::make_unique<Vector>(dispatch(node->element_type()), Type::Nullity::NULLABLE);
        }
    };
    return Converter {}.dispatch(type);
}

}  // namespace shakujo::analyzer::analyze::typing

#endif  //SHAKUJO_ANALYZER_ANALYZE_TYPING_H_
