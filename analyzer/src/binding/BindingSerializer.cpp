/*
 * Copyright 2018 shakujo project.
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
#include <shakujo/analyzer/binding/BindingSerializer.h>

#include "shakujo/analyzer/binding/BindingSerializer.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::binding {

using common::util::is_valid;

void BindingSerializer::serialize(common::util::DataSerializer& printer, model::key::ExpressionKey const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    auto binding = context_.find(value);
    if (!binding) {
        printer.value(nullptr);
    } else {
        serialize(printer, binding.get());
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, model::key::FunctionKey const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    auto binding = context_.find(value);
    if (!binding) {
        printer.value(nullptr);
    } else {
        serialize(printer, binding.get());
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, model::key::VariableKey const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    auto binding = context_.find(value);
    if (!binding) {
        printer.value(nullptr);
    } else {
        serialize(printer, binding.get());
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, model::key::RelationKey const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    auto binding = context_.find(value);
    if (!binding) {
        printer.value(nullptr);
    } else {
        serialize(printer, binding.get());
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, ExpressionBinding const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ExpressionBinding");
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    printer.exit_object("ExpressionBinding");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, VariableBinding const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VariableBinding");
    {
        printer.enter_property("id");
        if (!value->id()) {
            printer.value(nullptr);
        } else {
            printer.value(value->id().get());
        }
        printer.exit_property("id");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    printer.exit_object("VariableBinding");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, FunctionBinding const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("FunctionBinding");
    {
        printer.enter_property("id");
        if (!value->id()) {
            printer.value(nullptr);
        } else {
            printer.value(value->id().get());
        }
        printer.exit_property("id");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("quantifier");
        serialize(printer, value->type());
        printer.exit_property("quantifier");
    }
    {
        printer.enter_property("parameters");
        auto& list = value->parameters();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            if (!element) {
                printer.value(nullptr);
            } else {
                serialize(printer, element.get());
            }
        }
        printer.exit_array(size);
        printer.exit_property("parameters");
    }
    printer.exit_object("FunctionBinding");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, FunctionBinding::Quantifier value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Quantifier");
        } else {
            printer.enter_object("FunctionBinding::Quantifier");
        }
        printer.enter_property("value");
        printer.value(to_string_view(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Quantifier");
        } else {
            printer.exit_object("FunctionBinding::Quantifier");
        }
    } else {
        printer.value(to_string_view(value));
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, RelationBinding const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("RelationBinding");
    {
        printer.enter_property("process");
        serialize(printer, &value->process());
        printer.exit_property("process");
    }
    {
        printer.enter_property("output");
        serialize(printer, &value->output());
        printer.exit_property("output");
    }
    {
        printer.enter_property("destination_table");
        serialize(printer, &value->destination_table());
        printer.exit_property("destination_table");
    }
    {
        printer.enter_property("join_columns");
        auto& list = value->join_columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, &element);
        }
        printer.exit_array(size);
        printer.exit_property("join_columns");
    }
    printer.exit_object("RelationBinding");
}

void BindingSerializer::serialize(common::util::DataSerializer &printer, RelationBinding::Profile const *value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("Profile");
    } else {
        printer.enter_object("RelationBinding::Profile");
    }
    if (!value->columns().empty()) {
        {
            printer.enter_property("columns");
            auto& list = value->columns();
            auto size = list.size();
            printer.enter_array(size);
            for (auto& element : list) {
                if (!element) {
                    printer.value(nullptr);
                } else {
                    serialize(printer, element.get());
                }
            }
            printer.exit_array(size);
            printer.exit_property("columns");
        }
        {
            printer.enter_property("source_table");
            serialize(printer, &value->source_table());
            printer.exit_property("source_table");
        }
        {
            printer.enter_property("source_index");
            serialize(printer, &value->source_index());
            printer.exit_property("source_index");
        }
        {
            printer.enter_property("order");
            auto& list = value->order();
            auto size = list.size();
            printer.enter_array(size);
            for (auto& element : list) {
                serialize(printer, &element);
            }
            printer.exit_array(size);
            printer.exit_property("order");
        }
    }
    if (show_qualified_kind()) {
        printer.exit_object("Profile");
    } else {
        printer.exit_object("RelationBinding::Profile");
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, RelationBinding::Order const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("Order");
    } else {
        printer.enter_object("RelationBinding::Order");
    }
    {
        printer.enter_property("column");
        serialize(printer, value->column().get());
        printer.exit_property("column");
    }
    {
        printer.enter_property("direction");
        printer.value(to_string_view(value->direction()));
        printer.exit_property("direction");
    }
    if (show_qualified_kind()) {
        printer.exit_object("Order");
    } else {
        printer.exit_object("RelationBinding::Order");
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, RelationBinding::JoinColumn const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("JoinColumn");
    } else {
        printer.enter_object("RelationBinding::JoinColumn");
    }
    {
        printer.enter_property("qualifiers");
        auto& list = value->qualifiers();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("qualifiers");
    }
    {
        printer.enter_property("output");
        serialize(printer, value->output().get());
        printer.exit_property("output");
    }
    {
        printer.enter_property("left_source");
        serialize(printer, value->left_source().get());
        printer.exit_property("left_source");
    }
    {
        printer.enter_property("right_source");
        serialize(printer, value->right_source().get());
        printer.exit_property("right_source");
    }
    {
        printer.enter_property("nullify_left_source");
        printer.value(value->nullify_left_source());
        printer.exit_property("nullify_left_source");
    }
    {
        printer.enter_property("nullify_right_source");
        printer.value(value->nullify_right_source());
        printer.exit_property("nullify_right_source");
    }
    if (show_qualified_kind()) {
        printer.exit_object("JoinColumn");
    } else {
        printer.exit_object("RelationBinding::JoinColumn");
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, common::schema::TableInfo const* value) {
    if (!is_valid(value)) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("TableInfo");
    {
        printer.enter_property("name");
        printer.value(value->name());
        printer.exit_property("name");
    }
    printer.exit_object("TableInfo");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, common::schema::IndexInfo const* value) {
    if (!is_valid(value)) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("IndexInfo");
    {
        printer.enter_property("is_primary");
        printer.value(value->is_primary());
        printer.exit_property("is_primary");
    }
    {
        printer.enter_property("name");
        printer.value(value->name());
        printer.exit_property("name");
    }
    printer.exit_object("IndexInfo");
}

}  // namespace shakujo::analyzer::binding
