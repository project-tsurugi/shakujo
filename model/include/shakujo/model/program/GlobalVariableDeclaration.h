/*
 * Copyright 2018-2024 Project Tsurugi.
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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_PROGRAM_GLOBAL_VARIABLE_DECLARATION_H_
#define SHAKUJO_MODEL_PROGRAM_GLOBAL_VARIABLE_DECLARATION_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <set>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/program/GlobalDeclaration.h"
#include "shakujo/model/program/GlobalDeclarationKind.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::program {
/**
 * @brief Represents declaring program level variables.
 */
class GlobalVariableDeclaration
        : public GlobalDeclaration
        , public key::VariableKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit GlobalVariableDeclaration(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents variable attribute.
     */
    enum class Attribute {
        /**
         * @brief variable is not modifiable.
         */
        CONST,

        /**
         * @brief variable is just a program parameter.
         */
        PARAMETER,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    GlobalVariableDeclaration();

    /**
     * @brief Destroys this object.
     */
    ~GlobalVariableDeclaration() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    GlobalVariableDeclaration(GlobalVariableDeclaration const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    GlobalVariableDeclaration& operator=(GlobalVariableDeclaration const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    GlobalVariableDeclaration(GlobalVariableDeclaration&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    GlobalVariableDeclaration& operator=(GlobalVariableDeclaration&& other) noexcept;

public:
    /**
     * @brief Returns variable name.
     * @return variable name.
     */
    name::Name* name();

    /**
     * @brief Returns variable name.
     * @return variable name.
     */
    inline name::Name const* name() const {
        return const_cast<GlobalVariableDeclaration*>(this)->name();
    }

    /**
     * @brief Sets variable name.
     * @param name variable name
     * @return this
     */
    GlobalVariableDeclaration& name(std::unique_ptr<name::Name> name);

    /**
     * @brief Releases variable name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_name();

    /**
     * @brief Returns variable type.
     * @return variable type.
     */
    type::Type* type();

    /**
     * @brief Returns variable type.
     * @return variable type.
     */
    inline type::Type const* type() const {
        return const_cast<GlobalVariableDeclaration*>(this)->type();
    }

    /**
     * @brief Sets variable type.
     * @param type variable type
     * @return this
     */
    GlobalVariableDeclaration& type(std::unique_ptr<type::Type> type);

    /**
     * @brief Releases variable type from this node.
     * @return the released node
     */
    std::unique_ptr<type::Type> release_type();

    /**
     * @brief Returns initial value.
     * @return initial value.
     */
    expression::Expression* value();

    /**
     * @brief Returns initial value.
     * @return initial value.
     */
    inline expression::Expression const* value() const {
        return const_cast<GlobalVariableDeclaration*>(this)->value();
    }

    /**
     * @brief Sets initial value.
     * @param value initial value
     * @return this
     */
    GlobalVariableDeclaration& value(std::unique_ptr<expression::Expression> value);

    /**
     * @brief Releases initial value from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_value();

    /**
     * @brief Returns variable attributes.
     * @return variable attributes.
     */
    std::set<GlobalVariableDeclaration::Attribute>& attributes();

    /**
     * @brief Returns variable attributes.
     * @return variable attributes.
     */
    inline std::set<GlobalVariableDeclaration::Attribute> const& attributes() const {
        return const_cast<GlobalVariableDeclaration*>(this)->attributes();
    }
    /**
     * @brief Returns referring variable key.
     * @return referring variable key.
     */
    key::VariableKey* variable_key() override;

    /**
     * @brief Returns referring variable key.
     * @return referring variable key.
     */
    inline key::VariableKey const* variable_key() const override {
        return const_cast<GlobalVariableDeclaration*>(this)->variable_key();
    }

    /**
     * @brief Sets referring variable key.
     * @param variable_key referring variable key
     * @return this
     */
    GlobalVariableDeclaration& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    GlobalVariableDeclaration* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    GlobalVariableDeclaration* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = GlobalDeclarationKind::GLOBAL_VARIABLE_DECLARATION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */GlobalDeclarationKind kind() const override {
        return tag;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see GlobalVariableDeclaration::Attribute
 */
inline constexpr std::string_view to_string_view(GlobalVariableDeclaration::Attribute value) {
    switch (value) {
        case GlobalVariableDeclaration::Attribute::CONST: return "CONST";
        case GlobalVariableDeclaration::Attribute::PARAMETER: return "PARAMETER";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see GlobalVariableDeclaration::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, GlobalVariableDeclaration::Attribute value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_GLOBAL_VARIABLE_DECLARATION_H_
