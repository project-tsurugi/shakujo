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
#ifndef SHAKUJO_ANALYZER_BINDING_VARIABLE_BINDING_H_
#define SHAKUJO_ANALYZER_BINDING_VARIABLE_BINDING_H_

#include <any>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "Id.h"
#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/util/utility.h"
#include "shakujo/model/key/VariableKey.h"

namespace shakujo::analyzer::binding {

/**
 * @brief Provides semantic information of variables.
 * FIXME: accept name aliases
 */
class VariableBinding final {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief the key type.
     */
    using key_type = model::key::VariableKey;

    /**
     * @brief Constructs a new undefined object.
     */
    VariableBinding() : VariableBinding({}, {}, std::unique_ptr<common::core::Type> {}) {}

    /**
     * @brief Construct a new object.
     * @param id the variable ID
     * @param name the variable name
     * @param type the variable type
     * @param value the constant value
     */
    VariableBinding(
            Id<VariableBinding>&& id,
            common::core::Name name,
            std::unique_ptr<common::core::Type> type,
            std::unique_ptr<common::core::Value> value = {});

    /**
     * @brief Construct a new object.
     * @param id the variable ID
     * @param name the variable name
     * @param type the variable type
     * @param value the constant value
     */
    VariableBinding(
            Id<VariableBinding>&& id,
            common::core::Name name,
            common::core::Type const* type,
            common::core::Value const* value = {})
        : VariableBinding(
                std::move(id),
                std::move(name),
                common::util::make_clone(type),
                common::util::make_clone(value))
    {}

    /**
     * @brief Destroys this object.
     */
    ~VariableBinding() noexcept;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    VariableBinding(VariableBinding const& other) = delete;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    VariableBinding(VariableBinding&& other) noexcept = delete;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    VariableBinding& operator=(VariableBinding const& other) = delete;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    VariableBinding& operator=(VariableBinding&& other) noexcept = delete;

    /**
     * @brief return the ID of the corresponded variable.
     * It may not be valid if there are no corresponded variable declarations.
     * @return the variable ID
     * @see Id::operator bool()
     */
    Id<VariableBinding> const& id() const;

    /**
     * @brief returns the corresponded variable name.
     * @return the variable name
     */
    common::core::Name const& name() const;

    /**
     * @brief returns the variable type.
     * It may be an erroneous type if type resolution was failed.
     * @return the variable type
     */
    common::core::Type const* type() const;

    /**
     * @brief sets the variable type.
     * @param type the variable type
     * @return this
     */
    VariableBinding& type(std::unique_ptr<common::core::Type> type);

    /**
     * @brief sets the variable type.
     * @param type the variable type
     * @return this
     */
    VariableBinding& type(common::core::Type const* type) {
        return this->type(common::util::make_clone(type));
    }

    /**
     * @brief sets the variable type.
     * @param type the variable type
     * @return this
     */
    inline VariableBinding& type(common::core::Type&& type) {
        return this->type(common::util::make_clone(std::move(type)));
    }

    /**
     * @brief returns the constant value of the corresponded variable.
     * It may be an undefined value if the variable does not have any compile-time constants.
     * @return the constant value
     */
    common::core::Value const* value() const;

    /**
     * @brief sets the resolved constant value of the corresponded variable.
     * @param value the resolved constant value
     * @return this
     */
    VariableBinding& value(std::unique_ptr<common::core::Value> value);

    /**
     * @brief sets the resolved constant value of the corresponded variable.
     * @param value the resolved constant value
     * @return this
     */
    inline VariableBinding& value(common::core::Value const* value) {
        return this->value(common::util::make_clone(value));
    }

    /**
     * @brief sets the resolved constant value of the corresponded variable.
     * @param value the resolved constant value
     * @return this
     */
    inline VariableBinding& value(common::core::Value&& value) {
        return this->value(common::util::make_clone(std::move(value)));
    }

    /**
     * @brief returns the view of extra attributes.
     * @return the view of extra attributes
     */
    std::map<std::string, std::any>& attributes();

    /**
     * @brief returns the view of extra attributes.
     * @return the view of extra attributes
     */
    inline std::map<std::string, std::any> const& attributes() const {
        return const_cast<VariableBinding*>(this)->attributes();
    }

    /**
     * @brief puts an attribute.
     * If the attribute already exists on this binding, this operation will overwrite it.
     * @param key the attribute key
     * @param value the attribute value
     * @return this
     */
    VariableBinding& put_attribute(std::string_view key, std::any value) {
        attributes().insert_or_assign(std::string { key }, std::move(value));
        return *this;
    }

    /**
     * @brief returns an attribute value for the given key.
     * @param key the attribute key
     * @return the corresponded attribute value if it is defined
     * @return empty object if there is no such an attribute (or explicitly added an empty value)
     */
    std::any const& find_attribute(std::string const& key) const;

    /**
     * @brief returns a raw attribute value for the given key.
     * @tparam T the attribute value type
     * @param key the attribute key
     * @return the raw value of corresponded attribute
     * @throw std::domain_error if there is no such an attribute
     * @throw std::bad_any_cast if the attribute value type is inconsistent
     */
    template<class T>
    T& get_attribute(std::string const& key) {
        if (auto it = attributes().find(key); it != attributes().end()) {
            return std::any_cast<T&>(it->second);
        }
        throw std::domain_error(key);
    }

    /**
     * @brief returns a raw attribute value for the given key.
     * @tparam T the attribute value type
     * @param key the attribute key
     * @return the raw value of corresponded attribute
     * @throw std::domain_error if there is no such an attribute
     * @throw std::bad_any_cast if the attribute value type is inconsistent
     */
    template<class T>
    inline T const& get_attribute(std::string const& key) const {
        return const_cast<VariableBinding*>(this)->get_attribute<T>(key);
    }

    /**
     * @brief returns whether or not this variable has valid ID.
     * @return true if this has valid ID
     * @return false otherwise
     */
    bool has_id() const {
        return static_cast<bool>(id());
    }

    /**
     * @brief returns whether or not the corresponded variable is valid.
     * @return true is the variable is valid
     * @return false otherwise
     */
    bool is_valid() const {
        namespace util = common::util;
        return has_id()
            && util::is_valid(type())
            && (!util::is_defined(value()) || util::is_valid(value()));
    }
};
}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_VARIABLE_BINDING_H_
