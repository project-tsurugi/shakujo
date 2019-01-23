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
#ifndef SHAKUJO_ANALYZER_BINDING_EXPRESSION_BINDING_H_
#define SHAKUJO_ANALYZER_BINDING_EXPRESSION_BINDING_H_

#include <any>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "shakujo/common/util/utility.h"

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"

namespace shakujo::analyzer::binding {

/**
 * @brief Provides semantic information of expressions.
 */
class ExpressionBinding final {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief Constructs a new object.
     * @param type the type binding
     * @param value the value binding
     * @param constant whether or not the corresponded expression is constant
     */
    explicit ExpressionBinding(
            std::unique_ptr<common::core::Type> type = {},
            std::unique_ptr<common::core::Value> value = {},
            bool constant = false);

    /**
     * @brief Constructs a new object.
     * @param type the type binding
     * @param value the value binding
     * @param constant whether or not the corresponded expression is constant
     */
    explicit ExpressionBinding(
            common::core::Type const* type,
            common::core::Value const* value = nullptr,
            bool constant = false)
        : ExpressionBinding(
                common::util::make_clone(type),
                common::util::make_clone(value),
                constant)
    {}

    /**
     * @brief Destroys this object.
     */
    ~ExpressionBinding() noexcept;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    ExpressionBinding(const ExpressionBinding& other) = delete;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    ExpressionBinding(ExpressionBinding&& other) noexcept = delete;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    ExpressionBinding& operator=(const ExpressionBinding& other) = delete;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    ExpressionBinding& operator=(ExpressionBinding&& other) noexcept = delete;

    /**
     * @brief Returns expression type.
     * The returned type may be unresolved if type resolution was failed.
     * @return the expression type
     */
    common::core::Type const* type() const;

    /**
     * @brief sets the corresponded expression type.
     * @param type the expression type
     * @return this
     */
    ExpressionBinding& type(std::unique_ptr<common::core::Type> type);

    /**
     * @brief sets the corresponded expression type.
     * @param type the expression type
     * @return this
     */
    inline ExpressionBinding& type(common::core::Type const* type) {
        return this->type(common::util::make_clone(type));
    }

    /**
     * @brief sets the corresponded expression type.
     * @param type the expression type
     * @return this
     */
    inline ExpressionBinding& type(common::core::Type&& type) {
        return this->type(common::util::make_clone(std::move(type)));
    }

    /**
     * @brief returns the resolved constant value of the corresponded expression.
     * It may be an erroneous value if the expression has a compile-time error,
     * or it may be undefined if it seems that the expression is not a compile-time constant.
     * @return the constant value
     * @see has_error()
     */
    common::core::Value const* value() const;

    /**
     * @brief sets the resolved constant value of the corresponded expression.
     * @param value the resolved constant value
     * @return this
     */
    ExpressionBinding& value(std::unique_ptr<common::core::Value> value);

    /**
     * @brief sets the resolved constant value of the corresponded expression.
     * @param value the resolved constant value
     * @return this
     */
    ExpressionBinding& value(common::core::Value const* value) {
        return this->value(common::util::make_clone(value));
    }

    /**
     * @brief sets the resolved constant value of the corresponded expression.
     * @param value the resolved constant value
     * @return this
     */
    ExpressionBinding& value(common::core::Value&& value) {
        return this->value(common::util::make_clone(std::move(value)));
    }

    /**
     * @brief returns whether or not the corresponded expression is a constant expression.
     * @return true if the the expression is constant
     * @return false otherwise
     */
    bool constant() const;

    /**
     * @brief sets whether or not the corresponded expression is a constant expression.
     * @param constant true if the expression is constant, otherwise false
     * @return this
     */
    ExpressionBinding& constant(bool constant);

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
        return const_cast<ExpressionBinding*>(this)->attributes();
    }

    /**
     * @brief puts an attribute.
     * If the attribute already exists on this binding, this operation will overwrite it.
     * @param key the attribute key
     * @param value the attribute valule
     * @return this
     */
    ExpressionBinding& put_attribute(std::string_view key, std::any value) {
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
        return const_cast<ExpressionBinding*>(this)->get_attribute<T>(key);
    }

    /**
     * @brief returns whether or not this binding is valid.
     * @return true if it is valid
     * @return false if it has some errors
     */
    bool is_valid() const {
        namespace util = common::util;
        return util::is_valid(type())
            && (!util::is_defined(value()) || util::is_valid(value()));
    }
};
}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_EXPRESSION_BINDING_H_
