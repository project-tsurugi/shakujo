/*
 * Copyright 2018-2019 shakujo project.
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
#ifndef SHAKUJO_MODEL_PROGRAM_GLOBAL_FUNCTION_DECLARATION_H_
#define SHAKUJO_MODEL_PROGRAM_GLOBAL_FUNCTION_DECLARATION_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <set>
#include <iostream>

#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/program/GlobalDeclaration.h"
#include "shakujo/model/program/GlobalDeclarationKind.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::program {
/**
 * @brief Represents declaring program level functions.
 */
class GlobalFunctionDeclaration
        : public GlobalDeclaration
        , public key::FunctionKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit GlobalFunctionDeclaration(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents function declaration attribute.
     */
    enum class Attribute {
        /**
         * @brief function should be inlined.
         */
        INLINE,

    };

    /**
     * @brief Represents function parameter declaration.
     */
    class Parameter final
            : public key::VariableKey::Provider {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Parameter(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Represents function parameter attribute.
         */
        enum class Attribute {
            /**
             * @brief parameter is not modifiable.
             */
            CONST,

        };

    public:
        /**
         * @brief Constructs a new empty object.
         */
        Parameter();

        /**
         * @brief Destroys this object.
         */
        ~Parameter() noexcept override;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Parameter(Parameter const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Parameter& operator=(Parameter const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Parameter(Parameter&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Parameter& operator=(Parameter&& other) noexcept;

    public:
        /**
         * @brief Returns parameter name.
         * @return parameter name.
         */
        name::Name* name();

        /**
         * @brief Returns parameter name.
         * @return parameter name.
         */
        inline name::Name const* name() const {
            return const_cast<GlobalFunctionDeclaration::Parameter*>(this)->name();
        }

        /**
         * @brief Sets parameter name.
         * @param name parameter name
         * @return this
         */
        GlobalFunctionDeclaration::Parameter& name(std::unique_ptr<name::Name> name);

        /**
         * @brief Releases parameter name from this node.
         * @return the released node
         */
        std::unique_ptr<name::Name> release_name();

        /**
         * @brief Returns parameter type.
         * @return parameter type.
         */
        type::Type* type();

        /**
         * @brief Returns parameter type.
         * @return parameter type.
         */
        inline type::Type const* type() const {
            return const_cast<GlobalFunctionDeclaration::Parameter*>(this)->type();
        }

        /**
         * @brief Sets parameter type.
         * @param type parameter type
         * @return this
         */
        GlobalFunctionDeclaration::Parameter& type(std::unique_ptr<type::Type> type);

        /**
         * @brief Releases parameter type from this node.
         * @return the released node
         */
        std::unique_ptr<type::Type> release_type();

        /**
         * @brief Returns parameter attributes.
         * @return parameter attributes.
         */
        std::set<GlobalFunctionDeclaration::Parameter::Attribute>& attributes();

        /**
         * @brief Returns parameter attributes.
         * @return parameter attributes.
         */
        inline std::set<GlobalFunctionDeclaration::Parameter::Attribute> const& attributes() const {
            return const_cast<GlobalFunctionDeclaration::Parameter*>(this)->attributes();
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
            return const_cast<GlobalFunctionDeclaration::Parameter*>(this)->variable_key();
        }

        /**
         * @brief Sets referring variable key.
         * @param variable_key referring variable key
         * @return this
         */
        GlobalFunctionDeclaration::Parameter& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Parameter* clone() const & override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Parameter* clone() && override;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    GlobalFunctionDeclaration();

    /**
     * @brief Destroys this object.
     */
    ~GlobalFunctionDeclaration() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    GlobalFunctionDeclaration(GlobalFunctionDeclaration const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    GlobalFunctionDeclaration& operator=(GlobalFunctionDeclaration const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    GlobalFunctionDeclaration(GlobalFunctionDeclaration&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    GlobalFunctionDeclaration& operator=(GlobalFunctionDeclaration&& other) noexcept;

public:
    /**
     * @brief Returns function name.
     * @return function name.
     */
    name::Name* name();

    /**
     * @brief Returns function name.
     * @return function name.
     */
    inline name::Name const* name() const {
        return const_cast<GlobalFunctionDeclaration*>(this)->name();
    }

    /**
     * @brief Sets function name.
     * @param name function name
     * @return this
     */
    GlobalFunctionDeclaration& name(std::unique_ptr<name::Name> name);

    /**
     * @brief Releases function name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_name();

    /**
     * @brief Returns function return type.
     * @return function return type.
     */
    type::Type* type();

    /**
     * @brief Returns function return type.
     * @return function return type.
     */
    inline type::Type const* type() const {
        return const_cast<GlobalFunctionDeclaration*>(this)->type();
    }

    /**
     * @brief Sets function return type.
     * @param type function return type
     * @return this
     */
    GlobalFunctionDeclaration& type(std::unique_ptr<type::Type> type);

    /**
     * @brief Releases function return type from this node.
     * @return the released node
     */
    std::unique_ptr<type::Type> release_type();

    /**
     * @brief Returns function parameters specification.
     * @return function parameters specification.
     */
    util::FragmentList<GlobalFunctionDeclaration::Parameter>& parameters();

    /**
     * @brief Returns function parameters specification.
     * @return function parameters specification.
     */
    inline util::FragmentList<GlobalFunctionDeclaration::Parameter> const& parameters() const {
        return const_cast<GlobalFunctionDeclaration*>(this)->parameters();
    }
    /**
     * @brief Returns function body.
     * @return function body.
     */
    statement::Statement* body();

    /**
     * @brief Returns function body.
     * @return function body.
     */
    inline statement::Statement const* body() const {
        return const_cast<GlobalFunctionDeclaration*>(this)->body();
    }

    /**
     * @brief Sets function body.
     * @param body function body
     * @return this
     */
    GlobalFunctionDeclaration& body(std::unique_ptr<statement::Statement> body);

    /**
     * @brief Releases function body from this node.
     * @return the released node
     */
    std::unique_ptr<statement::Statement> release_body();

    /**
     * @brief Returns function attributes.
     * @return function attributes.
     */
    std::set<GlobalFunctionDeclaration::Attribute>& attributes();

    /**
     * @brief Returns function attributes.
     * @return function attributes.
     */
    inline std::set<GlobalFunctionDeclaration::Attribute> const& attributes() const {
        return const_cast<GlobalFunctionDeclaration*>(this)->attributes();
    }
    /**
     * @brief Returns referring function key.
     * @return referring function key.
     */
    key::FunctionKey* function_key() override;

    /**
     * @brief Returns referring function key.
     * @return referring function key.
     */
    inline key::FunctionKey const* function_key() const override {
        return const_cast<GlobalFunctionDeclaration*>(this)->function_key();
    }

    /**
     * @brief Sets referring function key.
     * @param function_key referring function key
     * @return this
     */
    GlobalFunctionDeclaration& function_key(std::unique_ptr<key::FunctionKey> function_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    GlobalFunctionDeclaration* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    GlobalFunctionDeclaration* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    GlobalDeclarationKind kind() const override {
        return GlobalDeclarationKind::GLOBAL_FUNCTION_DECLARATION;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see GlobalFunctionDeclaration::Attribute
 */
inline constexpr std::string_view to_string_view(GlobalFunctionDeclaration::Attribute value) {
    switch (value) {
        case GlobalFunctionDeclaration::Attribute::INLINE: return "INLINE";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see GlobalFunctionDeclaration::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, GlobalFunctionDeclaration::Attribute value) {
    return out << to_string_view(value);
}

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see GlobalFunctionDeclaration::Parameter::Attribute
 */
inline constexpr std::string_view to_string_view(GlobalFunctionDeclaration::Parameter::Attribute value) {
    switch (value) {
        case GlobalFunctionDeclaration::Parameter::Attribute::CONST: return "CONST";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see GlobalFunctionDeclaration::Parameter::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, GlobalFunctionDeclaration::Parameter::Attribute value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_GLOBAL_FUNCTION_DECLARATION_H_
