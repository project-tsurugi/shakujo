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
#ifndef SHAKUJO_MODEL_UTIL_NODE_SERIALIZER_H_
#define SHAKUJO_MODEL_UTIL_NODE_SERIALIZER_H_

#include "NodeSerializerBase.h"
#include "shakujo/common/util/DataSerializer.h"

#include "shakujo/common/core/type/Array.h"
#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/type/Char.h"
#include "shakujo/common/core/type/Cursor.h"
#include "shakujo/common/core/type/Error.h"
#include "shakujo/common/core/type/Float.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/type/Null.h"
#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/type/String.h"
#include "shakujo/common/core/type/Tuple.h"
#include "shakujo/common/core/type/Vector.h"

#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Error.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Null.h"
#include "shakujo/common/core/value/Placeholder.h"
#include "shakujo/common/core/value/String.h"
#include "shakujo/common/core/value/Tuple.h"

namespace shakujo::model::util {

/**
 * @brief IR node serializer.
 */
class NodeSerializer : public NodeSerializerBase {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief constructs a new object.
     */
    NodeSerializer();

    /**
     * @brief destroys this object.
     */
    ~NodeSerializer() noexcept override;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    NodeSerializer(const NodeSerializer& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    NodeSerializer& operator=(const NodeSerializer& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    NodeSerializer(NodeSerializer&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    NodeSerializer& operator=(NodeSerializer&& other) noexcept = default;

    using NodeSerializerBase::show_qualified_kind;
    using NodeSerializerBase::show_fragment_kind;
    using NodeSerializerBase::show_enum_kind;
    NodeSerializer& show_qualified_kind(bool on) override;
    NodeSerializer& show_fragment_kind(bool on) override;
    NodeSerializer& show_enum_kind(bool on) override;

    /**
     * @brief returns whether or not show undefined IR values.
     * @return true if show undefined IR values
     * @return false otherwise
     */
    virtual bool show_undefined_value() const;

    /**
     * @brief sets whether or not show undefined IR values.
     * @param on whether or not undefined IR values (default: false)
     * @return this
     */
    virtual NodeSerializer& show_undefined_value(bool on);

    /**
     * @brief returns whether or not show kind of value nodes.
     * @return true if show kind of value nodes
     * @return false otherwise
     */
    virtual bool show_value_kind() const;

    /**
     * @brief sets whether or not show kind of value nodes.
     * @param on whether or not kind of value nodes (default: false)
     * @return this
     */
    virtual NodeSerializer& show_value_kind(bool on);

    /**
     * @brief returns whether or not show IR name nodes as simple string.
     * @return true if show IR name nodes as simple string
     * @return false otherwise
     */
    virtual bool show_name_as_string() const;

    /**
     * @brief sets whether or not show IR name nodes as simple string.
     * @param on whether or not IR name nodes as simple string (default: false)
     * @return this
     */
    virtual NodeSerializer& show_name_as_string(bool on);

    using NodeSerializerBase::serialize;

    void serialize(common::util::DataSerializer& printer, common::core::Type const* value) override;
    void serialize(common::util::DataSerializer& printer, common::core::Value const* value) override;

    void serialize(common::util::DataSerializer& printer, key::ExpressionKey const* value) override;
    void serialize(common::util::DataSerializer& printer, key::FunctionKey const* value) override;
    void serialize(common::util::DataSerializer& printer, key::VariableKey const* value) override;

    void serialize(common::util::DataSerializer& printer, name::QualifiedName const* value) override;
    void serialize(common::util::DataSerializer& printer, name::SimpleName const* value) override;

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::Name const& value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::Type::Nullity value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Int const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Float const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Char const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::String const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Bool const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Null const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Tuple const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Tuple::Element const& value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Array const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Vector const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Relation const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Relation::Column const& value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Cursor const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::type::Error const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::Bool const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::Int const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::Float const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::Placeholder const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::String const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::Tuple const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::Null const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::value::Error const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::DocumentRegion const* value);  // NOLINT

    /**
     * @brief serializes the value into given printer.
     * @param printer the destination printer
     * @param value the target value
     */
    virtual void serialize(common::util::DataSerializer& printer, common::core::DocumentRegion::Position const* value);  // NOLINT
};

}  // namespace shakujo::model::util

#endif  // SHAKUJO_MODEL_UTIL_NODE_SERIALIZER_H_
