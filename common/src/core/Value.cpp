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
#include "shakujo/common/core/Value.h"

#include <cmath>
#include <limits>
#include <memory>

#include "shakujo/common/core/value/ConstVisitor.h"

namespace shakujo::common::core {

bool Value::operator==(Value const& other) const {
    class Eq : public value::ConstVisitor<bool, Value const*> {
    public:
        bool visit(value::Bool const* node, Value const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<value::Bool const*>(other);
            return node->get() == that->get();
        }
        bool visit(value::Int const* node, Value const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<value::Int const*>(other);
            return node->get() == that->get();
        }
        bool visit(value::Float const* node, Value const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<value::Float const*>(other);
            value::Float::type diff = node->get() - that->get();
            return std::fabs(diff) < std::numeric_limits<value::Float::type>::epsilon();
        }
        bool visit(value::String const* node, Value const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<value::String const*>(other);
            return node->get() == that->get();
        }
        bool visit(value::Tuple const* node, Value const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<value::Tuple const*>(other);
            auto& a = node->elements();
            auto& b = that->elements();
            if (a.size() != b.size()) {
                return false;
            }
            for (std::size_t i = 0, n = a.size(); i < n; i++) {
                if (!dispatch(a[i], b[i])) {
                    return false;
                }
            }
            return true;
        }
        bool visit(value::Null const* node, Value const* other) override {
            return node->kind() == other->kind();
        }
        bool visit(value::Error const* node, Value const* other) override {
            return node->kind() == other->kind();
        }
    };
    return Eq {}.dispatch(this, &other);
}

std::ostream& operator<<(std::ostream& out, Value const& value) {
    class Printer : public value::ConstVisitor<void, std::ostream&> {
        void visit(value::Bool const* node, std::ostream& out) override {
            out << "Bool(" << node->get() << ")";
        }
        void visit(value::Int const* node, std::ostream& out) override {
            out << "Int(" << node->get() << ")";
        }
        void visit(value::Float const* node, std::ostream& out) override {
            out << "Float(" << node->get() << ")";
        }
        void visit(value::String const* node, std::ostream& out) override {
            out << "String(" << node->get() << ")";
        }
        void visit(value::Tuple const* node, std::ostream& out) override {
            out << "Tuple(";
            bool cont = false;
            for (auto e : node->elements()) {
                if (cont) {
                    out << ", ";
                }
                dispatch(e, out);
                cont = true;
            }
            out << ")";
        }
        void visit(value::Null const*, std::ostream& out) override {
            out << "Null()";
        }
        void visit(value::Error const*, std::ostream& out) override {
            out << "Error()";
        }
    };
    Printer {}.dispatch(&value, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ::shakujo::common::core::Value::Kind value) {
    switch (value) {
    case Value::Kind::BOOL:
        out << "BOOL";
        break;
    case Value::Kind::INT:
        out << "INT";
        break;
    case Value::Kind::FLOAT:
        out << "FLOAT";
        break;
    case Value::Kind::STRING:
        out << "STRING";
        break;
    case Value::Kind::TUPLE:
        out << "TUPLE";
        break;
    case Value::Kind::NULL_:
        out << "NULL";
        break;
    case Value::Kind::PLACEHOLDER:
        out << "PLACEHOLDER";
        break;
    case Value::Kind::ERROR:
        out << "ERROR";
        break;
    }
    return out;
}

}  // namespace shakujo::common::core
