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

void BindingSerializer::serialize(common::util::DataSerializer& printer, Diagnostic const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Diagnostic");
    {
        printer.enter_property("region");
        serialize(printer, &value->region());
        printer.exit_property("region");
    }
    {
        printer.enter_property("code");
        printer.value(to_string_view(value->code()));
        printer.exit_property("code");
    }
    {
        printer.enter_property("severity");
        printer.value(to_string_view(value->severity()));
        printer.exit_property("severity");
    }
    {
        printer.enter_property("message");
        printer.value(value->message());
        printer.exit_property("message");
    }
    printer.exit_object("Diagnostic");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, ExpressionBinding const* value) {
    if (!is_valid(value)) {
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
    if (!is_valid(value)) {
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
    if (!is_valid(value)) {
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
            serialize(printer, &element);
        }
        printer.exit_array(size);
        printer.exit_property("parameters");
    }
    printer.exit_object("FunctionBinding");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, FunctionBinding::Parameter const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("Parameter");
    } else {
        printer.enter_object("FunctionBinding::Parameter");
    }
    {
        printer.enter_property("name");
        printer.value(value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    if (show_qualified_kind()) {
        printer.exit_object("Parameter");
    } else {
        printer.exit_object("FunctionBinding::Parameter");
    }
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
    if (!is_valid(value)) {
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
        printer.enter_property("destination_columns");
        auto& list = value->destination_columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, element.get());
        }
        printer.exit_array(size);
        printer.exit_property("destination_columns");
    }
    {
        printer.enter_property("scan_strategy");
        serialize(printer, &value->scan_strategy());
        printer.exit_property("scan_strategy");
    }
    {
        printer.enter_property("join_strategy");
        serialize(printer, &value->join_strategy());
        printer.exit_property("join_strategy");
    }
    printer.exit_object("RelationBinding");
}

void BindingSerializer::serialize(common::util::DataSerializer &printer, RelationBinding::Profile const *value) {
    if (!is_valid(value)) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("Profile");
    } else {
        printer.enter_object("RelationBinding::Profile");
    }
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

void BindingSerializer::serialize(common::util::DataSerializer& printer, ScanStrategy const* value) {
    if (!is_valid(value)) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ScanStrategy");
    {
        printer.enter_property("kind");
        serialize(printer, value->kind());
        printer.exit_property("kind");
    }
    {
        printer.enter_property("table");
        serialize(printer, &value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("index");
        serialize(printer, &value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("key_columns");
        auto& list = value->key_columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, element.get());
        }
        printer.exit_array(size);
        printer.exit_property("key_columns");
    }
    {
        printer.enter_property("prefix");
        auto& list = value->prefix();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, element.get());
        }
        printer.exit_array(size);
        printer.exit_property("prefix");
    }
    {
        printer.enter_property("lower_suffix");
        serialize(printer, &value->lower_suffix());
        printer.exit_property("lower_suffix");
    }
    {
        printer.enter_property("upper_suffix");
        serialize(printer, &value->upper_suffix());
        printer.exit_property("upper_suffix");
    }
    printer.exit_object("ScanStrategy");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, ScanStrategy::Kind value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Kind");
        } else {
            printer.enter_object("ScanStrategy::Kind");
        }
        printer.enter_property("value");
        printer.value(to_string_view(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Kind");
        } else {
            printer.exit_object("ScanStrategy::Kind");
        }
    } else {
        printer.value(to_string_view(value));
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, ScanStrategy::Suffix const* value) {
    if (value == nullptr || value->value() == nullptr) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("Suffix");
    } else {
        printer.enter_object("ScanStrategy::Suffix");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("inclusive");
        printer.value(value->inclusive());
        printer.exit_property("inclusive");
    }
    if (show_qualified_kind()) {
        printer.exit_object("Suffix");
    } else {
        printer.exit_object("ScanStrategy::Suffix");
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, JoinStrategy const* value) {
    if (!is_valid(value)) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("JoinStrategy");
    {
        printer.enter_property("kind");
        serialize(printer, value->kind());
        printer.exit_property("kind");
    }
    {
        printer.enter_property("left_outer");
        printer.value(value->left_outer());
        printer.exit_property("left_outer");
    }
    {
        printer.enter_property("right_outer");
        printer.value(value->right_outer());
        printer.exit_property("right_outer");
    }
    {
        printer.enter_property("left_semi");
        printer.value(value->left_semi());
        printer.exit_property("left_semi");
    }
    {
        printer.enter_property("right_semi");
        printer.value(value->right_semi());
        printer.exit_property("right_semi");
    }
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, &element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    {
        printer.enter_property("equalities");
        auto& list = value->equalities();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& [left, right] : list) {
            printer.enter_array(2);
            serialize(printer, left.get());
            serialize(printer, right.get());
            printer.exit_array(2);
        }
        printer.exit_array(size);
        printer.exit_property("equalities");
    }
    {
        printer.enter_property("seek_columns");
        auto& list = value->seek_columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto& element : list) {
            serialize(printer, &element);
        }
        printer.exit_array(size);
        printer.exit_property("seek_columns");
    }
    printer.exit_object("JoinStrategy");
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, JoinStrategy::Kind value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Kind");
        } else {
            printer.enter_object("JoinStrategy::Kind");
        }
        printer.enter_property("value");
        printer.value(to_string_view(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Kind");
        } else {
            printer.exit_object("JoinStrategy::Kind");
        }
    } else {
        printer.value(to_string_view(value));
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, JoinStrategy::Column const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("Column");
    } else {
        printer.enter_object("JoinStrategy::Column");
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
        printer.exit_object("Column");
    } else {
        printer.exit_object("JoinStrategy::Column");
    }
}

void BindingSerializer::serialize(common::util::DataSerializer& printer, JoinStrategy::ColumnData const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    if (show_qualified_kind()) {
        printer.enter_object("ColumnData");
    } else {
        printer.enter_object("JoinStrategy::ColumnData");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("variable");
        serialize(printer, value->variable().get());
        printer.exit_property("variable");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    if (show_qualified_kind()) {
        printer.exit_object("ColumnData");
    } else {
        printer.exit_object("JoinStrategy::ColumnData");
    }

}

}  // namespace shakujo::analyzer::binding
