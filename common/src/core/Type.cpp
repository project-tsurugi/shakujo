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
#include "shakujo/common/core/Type.h"

#include <iostream>
#include <memory>

#include "shakujo/common/core/type/ConstVisitor.h"

namespace shakujo::common::core {

bool Type::operator==(Type const& other) const {
    using namespace ::shakujo::common::core::type;
    class Eq : public type::ConstVisitor<bool, Type const*> {
    private:
        inline bool check(Type const* a, Type const* b) {
            return a->kind() == b->kind() && a->nullity() == b->nullity();
        }

    public:
        bool visit(Int const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Int const*>(other);
            return node->size() == that->size();
        }

        bool visit(Float const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Float const*>(other);
            return node->size() == that->size();
        }

        bool visit(Char const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Char const*>(other);
            return node->size() == that->size();
        }

        bool visit(String const* node, Type const* other) override {
            return check(node, other);
        }

        bool visit(Bool const* node, Type const* other) override {
            return check(node, other);
        }

        bool visit(Null const* node, Type const* other) override {
            return check(node, other);
        }

        bool visit(Tuple const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Tuple const*>(other);
            auto& a = node->elements();
            auto& b = that->elements();
            if (a.size() != b.size()) {
                return false;
            }
            for (std::size_t i = 0, n = a.size(); i < n; i++) {
                auto& ae = a[i];
                auto& be = b[i];
                if (ae.name() != be.name() || !dispatch(ae.type(), be.type())) {
                    return false;
                }
            }
            return true;
        }

        bool visit(Array const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Array const*>(other);
            return node->size() == that->size()
                    && dispatch(node->element_type(), that->element_type());
        }

        bool visit(Vector const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Vector const*>(other);
            return dispatch(node->element_type(), that->element_type());
        }

        bool visit(Relation const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Relation const*>(other);
            auto& a = node->columns();
            auto& b = that->columns();
            if (a.size() != b.size()) {
                return false;
            }
            for (std::size_t i = 0, n = a.size(); i < n; i++) {
                auto& ae = a[i];
                auto& be = b[i];
                if (ae.name() != be.name()
                        || !dispatch(ae.type(), be.type())
                        || ae.qualifiers() != be.qualifiers()) {
                    return false;
                }
            }
            return true;
        }

        bool visit(Cursor const* node, Type const* other) override {
            if (!check(node, other)) {
                return false;
            }
            auto that = dynamic_cast<Cursor const*>(other);
            return dispatch(node->element_type(), that->element_type());
        }

        bool visit(Error const* node, Type const* other) override {
            return check(node, other);
        }
    };
    return Eq {}.dispatch(this, &other);
}

namespace {
using namespace shakujo::common::core::type;
class Printer : public ConstVisitor<void, std::ostream&> {
    void visit(Int const* node, std::ostream& out) final {
        out << "Int(" << node->size() << ")";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(Float const* node, std::ostream& out) final {
        out << "Float(" << node->size() << ")";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(Char const* node, std::ostream& out) final {
        out << "Char(" << node->size() << ")";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(String const* node, std::ostream& out) final {
        out << "String()";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(Bool const* node, std::ostream& out) final {
        out << "Bool()";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(Tuple const* node, std::ostream& out) final {
        out << "Tuple(";
        bool cont = false;
        for (auto& e : node->elements()) {
            if (cont) out << ", ";
            if (!e.name().empty()) out << e.name() << ":";
            dispatch(e.type(), out);
            cont = true;
        }
        out << ")";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(Array const* node, std::ostream& out) final {
        out << "Array(";
        dispatch(node->element_type(), out);
        out << ", ";
        out << node->size();
        out << ")";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(Vector const* node, std::ostream& out) final {
        out << "Vector(";
        dispatch(node->element_type(), out);
        out << ")";
        if (node->nullity() == Type::Nullity::NULLABLE) out << "?";
    }
    void visit(Relation const* node, std::ostream& out) final {
        out << "Relation(";
        bool cont = false;
        for (auto& e : node->columns()) {
            if (cont) out << ", ";
            if (!e.name().empty()) out << e.name() << ":";
            dispatch(e.type(), out);
            cont = true;
        }
        out << ")";
    }
    void visit(Cursor const* node, std::ostream& out) final {
        out << "Cursor(";
        dispatch(node->element_type(), out);
        out << ")";
    }
    void visit(Null const*, std::ostream& out) final {
        out << "Null";
    }
    void visit(Error const*, std::ostream& out) final {
        out << "Error";
    }
};
}  // namespace

std::ostream& operator<<(std::ostream& out, Type const& value) {
    Printer {}.dispatch(&value, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, Type::Kind value) {
    switch (value) {
    case Type::Kind::INT:
        out << "INT";
        break;
    case Type::Kind::FLOAT:
        out << "FLOAT";
        break;
    case Type::Kind::CHAR:
        out << "CHAR";
        break;
    case Type::Kind::STRING:
        out << "STRING";
        break;
    case Type::Kind::BOOL:
        out << "BOOL";
        break;
    case Type::Kind::NULL_:
        out << "NULL";
        break;
    case Type::Kind::TUPLE:
        out << "TUPLE";
        break;
    case Type::Kind::ARRAY:
        out << "ARRAY";
        break;
    case Type::Kind::VECTOR:
        out << "VECTOR";
        break;
    case Type::Kind::RELATION:
        out << "RELATION";
        break;
    case Type::Kind::CURSOR:
        out << "CURSOR";
        break;
    case Type::Kind::ERROR:
        out << "ERROR";
        break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, Type::Nullity value) {
    switch (value) {
    case Type::Nullity::NEVER_NULL:
        out << "NEVER_NULL";
        break;
    case Type::Nullity::NULLABLE:
        out << "NULLABLE";
        break;
    }
    return out;
}

Type::Nullity operator|(Type::Nullity a, Type::Nullity b) {
    return static_cast<Type::Nullity>(static_cast<bool>(a) || static_cast<bool>(b));
}

Type::Nullity operator&(Type::Nullity a, Type::Nullity b) {
    return static_cast<Type::Nullity>(static_cast<bool>(a) && static_cast<bool>(b));
}

Type::Nullity operator~(Type::Nullity nullity) {
    return static_cast<Type::Nullity>(!static_cast<bool>(nullity));
}

}  // namespace shakujo::common::core
