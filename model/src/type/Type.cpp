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
#include "shakujo/model/type/Type.h"

#include <memory>
#include <iostream>

#include "shakujo/model/type/ConstTypeVisitor.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::model::type {

using common::util::dynamic_pointer_cast;

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
            auto that = dynamic_pointer_cast<ArrayType>(other);
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
            auto that = dynamic_pointer_cast<CharType>(other);
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
            auto that = dynamic_pointer_cast<RelationType>(other);
            return dispatch(node->element_type(), that->element_type());
        }

        bool visit(StringType const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(TupleType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_pointer_cast<TupleType>(other);
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
            auto that = dynamic_pointer_cast<VarCharType>(other);
            return node->size() == that->size();
        }

        bool visit(VectorType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_pointer_cast<VectorType>(other);
            return dispatch(node->element_type(), that->element_type());
        }

        bool visit(DecimalType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_pointer_cast<DecimalType>(other);
            return node->precision() == that->precision()
                && node->scale() == that->scale();
        }

        bool visit(BinaryType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_pointer_cast<BinaryType>(other);
            return node->size() == that->size();
        }

        bool visit(VarBinaryType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_pointer_cast<VarBinaryType>(other);
            return node->size() == that->size();
        }

        bool visit(DateType const* node, Type const* other) override {
            return node->kind() == other->kind();
        }

        bool visit(TimeType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_pointer_cast<TimeType>(other);
            return node->has_time_zone() == that->has_time_zone();
        }

        bool visit(TimestampType const* node, Type const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_pointer_cast<TimestampType>(other);
            return node->has_time_zone() == that->has_time_zone();
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
            out << "VarCharType(";
            print_flexible_size(node->size(), out);
            out << ")";
        }

        void visit(VectorType const* node, std::ostream& out) override {
            out << "VectorType(";
            dispatch(node->element_type(), out);
            out << ")";
        }

        void visit(DecimalType const* node, std::ostream& out) override {
            out << "DecimalType(";
            if (node->precision()) {
                print_flexible_size(*node->precision(), out);
            }
            if (node->scale()) {
                out << ", ";
                print_flexible_size(*node->scale(), out);
            }
            out << ")";
        }

        void visit(BinaryType const* node, std::ostream& out) override {
            out << "BinaryType(" << node->size() << ")";
        }

        void visit(VarBinaryType const* node, std::ostream& out) override {
            out << "VarBinaryType(";
            print_flexible_size(node->size(), out);
            out << ")";
        }

        void visit(DateType const*, std::ostream& out) override {
            out << "DateType()";
        }

        void visit(TimeType const* node, std::ostream& out) override {
            out << "TimeType(";
            if (node->has_time_zone()) {
                out << "with_time_zone";
            }
            out << ")";
        }

        void visit(TimestampType const* node, std::ostream& out) override {
            out << "TimestampType(";
            if (node->has_time_zone()) {
                out << "with_time_zone";
            }
            out << ")";
        }

    private:
        void print_flexible_size(std::size_t size, std::ostream& out) {
            if (size == static_cast<std::size_t>(-1)) {
                out << "*";
            } else {
                out << size;
            }
        }
    };
    Printer{}.dispatch(&value, out);
    return out;
}
}  // namespace shakujo::model::type
