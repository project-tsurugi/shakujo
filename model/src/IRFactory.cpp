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
#include "shakujo/model/IRFactory.h"

#include <memory>
#include <string>

#include "shakujo/common/core/type/ConstVisitor.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model {

std::unique_ptr<name::Name> IRFactory::Name(std::vector<std::string> segments) {
    if (segments.empty()) {
        return {};
    }
    std::unique_ptr<name::Name> current;
    for (auto &s : segments) {
        if (current) {
            current = Name(std::move(current), s);
        } else {
            current = Name(s);
        }
    }
    return current;
}

std::unique_ptr<type::Type> IRFactory::Type(common::core::Type const* type) {
    using namespace common::core::type;
    class TypeConv : public ConstVisitor<std::unique_ptr<type::Type>, IRFactory&> {
    public:
        std::unique_ptr<type::Type> visitDefault(common::core::Type const*, IRFactory&) override {
            return {};
        }
        std::unique_ptr<type::Type> visit(Int const* node, IRFactory& f) override {
            if (node->size() == 32U) {
                return f.Int32Type();
            }
            if (node->size() == 64U) {
                return f.Int64Type();
            }
            return visitDefault(node, f);
        }
        std::unique_ptr<type::Type> visit(Float const* node, IRFactory& f) override {
            if (node->size() == 32U) {
                return f.Float32Type();
            }
            if (node->size() == 64U) {
                return f.Float64Type();
            }
            return visitDefault(node, f);
        }
        std::unique_ptr<type::Type> visit(Char const* node, IRFactory& f) override {
            return f.CharType(node->size());
        }
        std::unique_ptr<type::Type> visit(String const*, IRFactory& f) override {
            return f.StringType();
        }
        std::unique_ptr<type::Type> visit(Bool const*, IRFactory& f) override {
            return f.BooleanType();
        }
        std::unique_ptr<type::Type> visit(Null const*, IRFactory& f) override {
            return f.NullType();
        }
        std::unique_ptr<type::Type> visit(Tuple const* node, IRFactory& f) override {
            std::vector<std::unique_ptr<type::TupleType::Element>> elements;
            elements.reserve(node->elements().size());
            for (auto &e : node->elements()) {
                elements.emplace_back(f.TupleTypeElement(f.Name(e.name()), dispatch(e.type(), f)));
            }
            return f.TupleType(std::move(elements));
        }
        std::unique_ptr<type::Type> visit(Array const* node, IRFactory& f) override {
            return f.ArrayType(dispatch(node->element_type(), f), node->size());
        }
        std::unique_ptr<type::Type> visit(Vector const* node, IRFactory& f) override {
            return f.VectorType(dispatch(node->element_type(), f));
        }
        std::unique_ptr<type::Type> visit(Relation const* node, IRFactory& f) override {
            std::vector<std::unique_ptr<type::TupleType::Element>> elements;
            elements.reserve(node->columns().size());
            for (auto &e : node->columns()) {
                elements.emplace_back(f.TupleTypeElement(f.Name(e.name()), dispatch(e.type(), f)));
            }
            return f.RelationType(f.TupleType(std::move(elements)));
        }
        // FIXME impl cursor type
    };
    return TypeConv {}.dispatch(type, *this);
}

}  // namespace shakujo::model
