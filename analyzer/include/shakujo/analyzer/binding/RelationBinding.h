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
#ifndef SHAKUJO_ANALYZER_BINDING_RELATION_BINDING_H_
#define SHAKUJO_ANALYZER_BINDING_RELATION_BINDING_H_

#include <any>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "shakujo/common/util/utility.h"

#include "VariableBinding.h"

#include "shakujo/model/key/RelationKey.h"

namespace shakujo::analyzer::binding {

/**
 * @brief provides semantic information of current processing relations.
 */
class RelationBinding final {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief the key type.
     */
    using key_type = model::key::RelationKey;

    /**
     * @brief Constructs a new object.
     * @param columns bindings of individual columns
     */
    explicit RelationBinding(std::vector<std::shared_ptr<VariableBinding>> columns = {});

    /**
     * @brief Destroys this object.
     */
    ~RelationBinding() noexcept;

    RelationBinding(RelationBinding const&) = delete;
    RelationBinding(RelationBinding&&) noexcept = delete;
    RelationBinding& operator=(RelationBinding const&) = delete;
    RelationBinding& operator=(RelationBinding&&) noexcept = delete;

    /**
     * @brief returns the variables that reflect individual row columns.
     * @return the column variables
     */
    std::vector<std::shared_ptr<VariableBinding>>& columns();

    /**
     * @brief returns the variables that reflect individual row columns.
     * @return the column variables
     */
    std::vector<std::shared_ptr<VariableBinding>> const& columns() const {
        return const_cast<RelationBinding*>(this)->columns();
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
        return const_cast<RelationBinding*>(this)->attributes();
    }

    /**
     * @brief puts an attribute.
     * If the attribute already exists on this binding, this operation will overwrite it.
     * @param key the attribute key
     * @param value the attribute value
     * @return this
     */
    RelationBinding& put_attribute(std::string_view key, std::any value) {
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
        return const_cast<RelationBinding*>(this)->get_attribute<T>(key);
    }

    /**
     * @brief returns whether or not this binding is valid.
     * @return true if it is valid
     * @return false if it has some errors
     */
    bool is_valid() const {
        namespace util = common::util;
        if (columns().empty()) {
            return false;
        }
        for (auto&& c : columns()) {
            if (!util::is_valid(c)) {
                return false;
            }
        }
        return true;
    }
};
}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_RELATION_BINDING_H_
