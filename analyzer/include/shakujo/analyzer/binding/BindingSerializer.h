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
#ifndef SHAKUJO_ANALYZER_BINDING_BINDING_SERIALIZER_H_
#define SHAKUJO_ANALYZER_BINDING_BINDING_SERIALIZER_H_

#include <memory>

#include "shakujo/model/util/NodeSerializer.h"
#include "BindingContext.h"
#include "ExpressionBinding.h"
#include "VariableBinding.h"
#include "FunctionBinding.h"
#include "RelationBinding.h"

namespace shakujo::analyzer::binding {

/**
 * @brief IR node serializer with bindings.
 * Binding information will appear in place of the corresponded 'key' properties of IR nodes.
 * @see model::key::ExpressionKey
 * @see model::key::FunctionKey
 * @see model::key::VariableKey
 */
class BindingSerializer : public model::util::NodeSerializer {
private:
    const BindingContext& context_;

public:
    /**
     * @brief constructs a new object.
     * You must not destroy the binding context until this serializer will be disposed.
     * @param context the current context
     */
    explicit BindingSerializer(const BindingContext& context)
        : context_(context)
    {}

    using NodeSerializer::serialize;

    void serialize(common::util::DataSerializer& printer, model::key::ExpressionKey const* value) override;
    void serialize(common::util::DataSerializer& printer, model::key::FunctionKey const* value) override;
    void serialize(common::util::DataSerializer& printer, model::key::VariableKey const* value) override;
    void serialize(common::util::DataSerializer& printer, model::key::RelationKey const* value) override;

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, ExpressionBinding const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, VariableBinding const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, FunctionBinding const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, FunctionBinding::Quantifier value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, RelationBinding const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, RelationBinding::Profile const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, RelationBinding::Order const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::schema::TableInfo const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::schema::IndexInfo const* value);  // NOLINT
};

}  // namespace shakujo::analyzer::binding

#endif  //SHAKUJO_ANALYZER_BINDING_BINDING_SERIALIZER_H_
