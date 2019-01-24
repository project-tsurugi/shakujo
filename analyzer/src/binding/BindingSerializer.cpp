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

namespace shakujo::analyzer::binding {

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

void BindingSerializer::serialize(common::util::DataSerializer& printer, RelationBinding const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("RelationBinding");
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
        printer.exit_property("parameters");
    }
    printer.exit_object("RelationBinding");
}

}  // namespace shakujo::analyzer::binding
