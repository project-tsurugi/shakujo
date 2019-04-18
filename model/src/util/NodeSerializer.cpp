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
#include "shakujo/model/util/NodeSerializer.h"

#include <stdexcept>

#include "shakujo/common/util/utility.h"
#include "shakujo/common/util/DataSerializer.h"

namespace shakujo::model::util {

using common::util::dynamic_pointer_cast;

class NodeSerializer::Impl {
public:
    bool undefined_value_ { false };
    bool value_kind_ { false };
    bool name_as_string_ { false };
};

NodeSerializer::NodeSerializer() : impl_(new Impl()) {}

NodeSerializer::~NodeSerializer() noexcept = default;

NodeSerializer& NodeSerializer::show_qualified_kind(bool on) {
    NodeSerializerBase::show_qualified_kind(on);
    return *this;
}

NodeSerializer& NodeSerializer::show_enum_kind(bool on) {
    NodeSerializerBase::show_fragment_kind(on);
    return *this;
}

NodeSerializer& NodeSerializer::show_fragment_kind(bool on) {
    NodeSerializerBase::show_enum_kind(on);
    return *this;
}

bool NodeSerializer::show_undefined_value() const {
    return impl_->undefined_value_;
}

NodeSerializer& NodeSerializer::show_undefined_value(bool on) {
    impl_->undefined_value_ = on;
    return *this;
}

bool NodeSerializer::show_value_kind() const {
    return impl_->value_kind_;
}

NodeSerializer& NodeSerializer::show_value_kind(bool on) {
    impl_->value_kind_ = on;
    return *this;
}

bool NodeSerializer::show_name_as_string() const {
    return impl_->name_as_string_;
}

NodeSerializer& NodeSerializer::show_name_as_string(bool on) {
    impl_->name_as_string_ = on;
    return *this;
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, key::ExpressionKey const*) {
    printer.value(nullptr);
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, key::FunctionKey const*) {
    printer.value(nullptr);
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, key::VariableKey const*) {
    printer.value(nullptr);
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, name::QualifiedName const* value) {
    if (show_name_as_string()) {
        serialize(printer, common::core::Name { value->segments() });
    } else {
        NodeSerializerBase::serialize(printer, value);
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, name::SimpleName const* value) {
    if (show_name_as_string()) {
        serialize(printer, common::core::Name { value->segments() });
    } else {
        NodeSerializerBase::serialize(printer, value);
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::Name const& value) {
    printer.value(value);
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::Type const* value) {
    if (!value) {
        printer.value(nullptr);
        return;
    }
    using Type = shakujo::common::core::Type;
    using namespace shakujo::common::core::type;
    switch (value->kind()) {
    case Type::Kind::INT: return serialize(printer, dynamic_pointer_cast<Int>(value));
    case Type::Kind::FLOAT: return serialize(printer, dynamic_pointer_cast<Float>(value));
    case Type::Kind::CHAR: return serialize(printer, dynamic_pointer_cast<Char>(value));
    case Type::Kind::STRING: return serialize(printer, dynamic_pointer_cast<String>(value));
    case Type::Kind::BOOL: return serialize(printer, dynamic_pointer_cast<Bool>(value));
    case Type::Kind::NULL_: return serialize(printer, dynamic_pointer_cast<Null>(value));
    case Type::Kind::TUPLE: return serialize(printer, dynamic_pointer_cast<Tuple>(value));
    case Type::Kind::ARRAY: return serialize(printer, dynamic_pointer_cast<Array>(value));
    case Type::Kind::VECTOR: return serialize(printer, dynamic_pointer_cast<Vector>(value));
    case Type::Kind::RELATION: return serialize(printer, dynamic_pointer_cast<Relation>(value));
    case Type::Kind::CURSOR: return serialize(printer, dynamic_pointer_cast<Cursor>(value));
    case Type::Kind::ERROR: return serialize(printer, dynamic_pointer_cast<Error>(value));
    }
    throw std::invalid_argument(common::util::to_string("unknown object kind: ", value->kind()));
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::Type::Nullity value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Nullity");
        } else {
            printer.enter_object("Type::Nullity");
        }
        printer.enter_property("value");
        printer.value(to_string_view(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Nullity");
        } else {
            printer.exit_object("Type::Nullity");
        }
    } else {
        printer.value(to_string_view(value));
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Int const* value) {
    printer.enter_object("Int");
    {
        printer.enter_property("size");
        printer.value(value->size());
        printer.exit_property("size");
    }
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    printer.exit_object("Int");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Float const* value) {
    printer.enter_object("Float");
    {
        printer.enter_property("size");
        printer.value(value->size());
        printer.exit_property("size");
    }
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    printer.exit_object("Float");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Char const* value) {
    printer.enter_object("Char");
    {
        printer.enter_property("size");
        printer.value(value->size());
        printer.exit_property("size");
    }
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    {
        printer.enter_property("varying");
        printer.value(value->varying());
        printer.exit_property("varying");
    }
    printer.exit_object("Char");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::String const* value) {
    printer.enter_object("String");
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    printer.exit_object("String");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Bool const* value) {
    printer.enter_object("Bool");
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    printer.exit_object("Bool");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Null const*) {
    printer.enter_object("Null");
    printer.exit_object("Null");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Tuple const* value) {
    printer.enter_object("Tuple");
    {
        printer.enter_property("elements");
        auto& list = value->elements();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("elements");
    }
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    printer.exit_object("Tuple");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Tuple::Element const& value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Element");
    } else {
        printer.enter_object("Tuple::Element");
    }
    {
        printer.enter_property("name");
        printer.value(value.name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value.type());
        printer.exit_property("type");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Element");
    } else {
        printer.exit_object("Tuple::Element");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Array const* value) {
    printer.enter_object("Array");
    {
        printer.enter_property("element_type");
        serialize(printer, value->element_type());
        printer.exit_property("element_type");
    }
    {
        printer.enter_property("size");
        printer.value(value->size());
        printer.exit_property("size");
    }
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    printer.exit_object("Array");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Vector const* value) {
    printer.enter_object("Vector");
    {
        printer.enter_property("element_type");
        serialize(printer, value->element_type());
        printer.exit_property("element_type");
    }
    {
        printer.enter_property("nullity");
        serialize(printer, value->nullity());
        printer.exit_property("nullity");
    }
    printer.exit_object("Vector");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Relation const* value) {
    printer.enter_object("Relation");
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    printer.exit_object("Relation");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Relation::Column const& value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Column");
    } else {
        printer.enter_object("Relation::Column");
    }
    {
        printer.enter_property("qualifiers");
        auto& list = value.qualifiers();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("qualifiers");
    }
    {
        printer.enter_property("name");
        printer.value(value.name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value.type());
        printer.exit_property("type");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Column");
    } else {
        printer.exit_object("Relation::Column");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Cursor const* value) {
    printer.enter_object("Cursor");
    {
        printer.enter_property("element_type");
        serialize(printer, value->element_type());
        printer.exit_property("element_type");
    }
    printer.exit_object("Cursor");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::type::Error const*) {
    printer.enter_object("Error");
    printer.exit_object("Error");
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::Value const* value) {
    if (!value) {
        printer.value(nullptr);
        return;
    }
    using Value = shakujo::common::core::Value;
    using namespace shakujo::common::core::value;
    switch (value->kind()) {
    case Value::Kind::BOOL: return serialize(printer, dynamic_pointer_cast<Bool>(value));
    case Value::Kind::INT: return serialize(printer, dynamic_pointer_cast<Int>(value));
    case Value::Kind::FLOAT: return serialize(printer, dynamic_pointer_cast<Float>(value));
    case Value::Kind::PLACEHOLDER: return serialize(printer, dynamic_pointer_cast<Placeholder>(value));
    case Value::Kind::STRING: return serialize(printer, dynamic_pointer_cast<String>(value));
    case Value::Kind::TUPLE: return serialize(printer, dynamic_pointer_cast<Tuple>(value));
    case Value::Kind::NULL_: return serialize(printer, dynamic_pointer_cast<Null>(value));
    case Value::Kind::ERROR: return serialize(printer, dynamic_pointer_cast<Error>(value));
    }
    throw std::invalid_argument(common::util::to_string("unknown object kind: ", value->kind()));
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::Bool const* value) {
    if (!show_value_kind()) {
        printer.value(value->get());
    } else {
        printer.enter_object("Bool");
        {
            printer.enter_property("value");
            printer.value(value->get());
            printer.exit_property("value");
        }
        printer.exit_object("Bool");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::Int const* value) {
    if (!show_value_kind()) {
        printer.value(value->get());
    } else {
        printer.enter_object("Int");
        {
            printer.enter_property("value");
            printer.value(value->get());
            printer.exit_property("value");
        }
        printer.exit_object("Int");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::Float const* value) {
    if (!show_value_kind()) {
        printer.value(value->get());
    } else {
        printer.enter_object("Float");
        {
            printer.enter_property("value");
            printer.value(value->get());
            printer.exit_property("value");
        }
        printer.exit_object("Float");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::Placeholder const* value) {
    if (!show_value_kind()) {
        printer.value(value->get());
    } else {
        printer.enter_object("Placeholder");
        {
            printer.enter_property("value");
            printer.value(value->get());
            printer.exit_property("value");
        }
        printer.exit_object("Placeholder");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::String const* value) {
    if (!show_value_kind()) {
        printer.value(value->get());
    } else {
        printer.enter_object("String");
        {
            printer.enter_property("value");
            printer.value(value->get());
            printer.exit_property("value");
        }
        printer.exit_object("String");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::Tuple const* value) {
    if (!show_value_kind()) {
        auto& list = value->elements();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
    } else {
        printer.enter_object("Tuple");
        {
            printer.enter_property("value");
            auto& list = value->elements();
            auto size = list.size();
            printer.enter_array(size);
            for (auto element : list) {
                serialize(printer, element);
            }
            printer.exit_property("value");
        }
        printer.exit_object("Tuple");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::Null const*) {
    if (!show_value_kind()) {
        printer.value(nullptr);
    } else {
        printer.enter_object("Null");
        {
            printer.enter_property("value");
            printer.value(nullptr);
            printer.exit_property("value");
        }
        printer.exit_object("Null");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::value::Error const*) {
    if (!show_value_kind() || !show_undefined_value()) {
        printer.value(nullptr);
    } else {
        printer.enter_object("Error");
        {
            printer.enter_property("value");
            printer.value(nullptr);
            printer.exit_property("value");
        }
        printer.exit_object("Error");
    }
}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::DocumentRegion const *value) {
    if (value == nullptr || !*value) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("DocumentRegion");
    {
        printer.enter_property("path");
        if (auto&& v = value->path(); !v.empty()) {
            printer.value(v);
        } else {
            printer.value(nullptr);
        }
        printer.exit_property("path");
    }
    {
        printer.enter_property("begin");
        serialize(printer, &value->begin());
        printer.exit_property("begin");
    }
    {
        printer.enter_property("end");
        serialize(printer, &value->end());
        printer.exit_property("end");
    }
    printer.exit_object("DocumentRegion");

}

void NodeSerializer::serialize(common::util::DataSerializer& printer, common::core::DocumentRegion::Position const *value) {
    if (value == nullptr || !*value) {
        printer.value(nullptr);
        return;
    }
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Position");
    } else {
        printer.enter_object("DocumentRegion::Position");
    }
    {
        printer.enter_property("line_number");
        if (auto v = value->line_number(); v > 0) {
            printer.value(v);
        } else {
            printer.value(nullptr);
        }
        printer.exit_property("line_number");
    }
    {
        printer.enter_property("column_number");
        if (auto v = value->column_number(); v > 0) {
            printer.value(v);
        } else {
            printer.value(nullptr);
        }
        printer.exit_property("column_number");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Position");
    } else {
        printer.exit_object("DocumentRegion::Position");
    }
}

}  // namespace shakujo::model::util

