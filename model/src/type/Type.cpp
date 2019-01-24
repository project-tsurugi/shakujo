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
#include "shakujo/model/type/Type.h"

#include <memory>
#include <iostream>

#include "shakujo/model/type/ConstTypeVisitor.h"

namespace shakujo::model::type {

bool Type::operator==(Type const& other) const {
    if (kind() != other.kind()) {
        return false;
    }
    class Eq : public ConstTypeVisitor<bool, Type const*> {
    public:
        bool visit(ArrayType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<ArrayType const*>(other);
            return node->size() == that->size()
                && dispatch(node->element_type(), that->element_type());
        }

        bool visit(BooleanType const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(CharType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<CharType const*>(other);
            return node->size() == that->size();
        }

        bool visit(Float32Type const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(Float64Type const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(Int32Type const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(Int64Type const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(NullType const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(RelationType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<RelationType const*>(other);
            return dispatch(node->element_type(), that->element_type());
        }

        bool visit(StringType const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(TupleType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<TupleType const*>(other);
            auto &a = node->elements();
            auto &b = that->elements();
            if (a.size() != b.size()) {
                return false;
            }
            for (std::size_t i = 0, n = a.size(); i < n; i++) {
                if (!common::util::equals(a[i]->name(), b[i]->name()) || !dispatch(a[i]->type(), b[i]->type())) {
                    return false;
                }
            }
            return true;
        }

        bool visit(VarCharType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<VarCharType const*>(other);
            return node->size() == that->size();
        }

        bool visit(VectorType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<VectorType const*>(other);
            return dispatch(node->element_type(), that->element_type());
        }
    };
    return Eq {}.dispatch(this, &other);
}

std::ostream& operator<<(std::ostream& out, Type const& value) {
    class Printer : public ConstTypeVisitor<void, std::ostream&> {
    public:
        void visit(ArrayType const* node, std::ostream& out) override {
            out << "ArrayType(";
            dispatch(node->element_type(), out);
            out << ", " << node->size() << ")";
        }

        void visit(BooleanType const*, std::ostream& out) override {
            out << "BooleanType()";
        }

        void visit(CharType const* node, std::ostream& out) override {
            out << "CharType(" << node->size() << ")";
        }

        void visit(Float32Type const*, std::ostream& out) override {
            out << "Float32Type()";
        }

        void visit(Float64Type const*, std::ostream& out) override {
            out << "Float64Type()";
        }

        void visit(Int32Type const*, std::ostream& out) override {
            out << "Int32Type()";
        }

        void visit(Int64Type const*, std::ostream& out) override {
            out << "Int64Type()";
        }

        void visit(NullType const*, std::ostream& out) override {
            out << "NullType()";
        }

        void visit(RelationType const* node, std::ostream& out) override {
            out << "RelationType(";
            dispatch(node->element_type(), out);
            out << ")";
        }

        void visit(StringType const*, std::ostream& out) override {
            out << "StringType()";
        }

        void visit(TupleType const* node, std::ostream& out) override {
            out << "TupleType(";
            int position = 0;
            for (auto e : node->elements()) {
                if (position > 0) out << ", ";
                out << position << ":";
                if (e->name()) out << *e->name();
                out << ":";
                dispatch(e->type(), out);
                position++;
            }
            out << ")";
        }

        void visit(VarCharType const* node, std::ostream& out) override {
            out << "VarCharType(" << node->size() << ")";
        }

        void visit(VectorType const* node, std::ostream& out) override {
            out << "VectorType(";
            dispatch(node->element_type(), out);
            out << ")";
        }
    };
    Printer{}.dispatch(&value, out);
    return out;
}
}  // namespace shakujo::model::type
