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

#include "shakujo/common/core/Direction.h"
#include "shakujo/common/schema/TableInfo.h"
#include "shakujo/common/schema/IndexInfo.h"

#include "shakujo/model/key/RelationKey.h"

namespace shakujo::analyzer::binding {

/**
 * @brief provides semantic information of current processing relations.
 */
class RelationBinding final {
public:
    /**
     * @brief the key type.
     */
    using key_type = model::key::RelationKey;

    /**
     * @brief an order term.
     */
    class Order {
    public:
        /**
         * @brief represents sort order of columns.
         */
        using Direction = common::core::Direction;

        /**
         * @brief constructs a new object.
         * @param column the column binding
         * @param direction the sort direction
         */
        Order(std::shared_ptr<VariableBinding> column, Direction direction = Direction::ASCENDANT)  // NOLINT
                : column_(std::move(column))
                , direction_(direction)
        {}

        /**
         * @brief returns the column binding.
         * @return the column binding
         */
        std::shared_ptr<VariableBinding> column() const {
            return column_;
        }

        /**
         * @brief returns the sort direction
         * @return the sort direction
         */
        Direction direction() const {
            return direction_;
        }

    private:
        std::shared_ptr<VariableBinding> column_;
        Direction direction_;
    };


    /**
     * @brief Constructs a new object.
     * @param columns bindings of individual columns
     */
    explicit RelationBinding(std::vector<std::shared_ptr<VariableBinding>> columns = {})
        : columns_(std::move(columns))
    {}

    /**
     * @brief Destroys this object.
     */
    ~RelationBinding() noexcept = default;

    RelationBinding(RelationBinding const&) = delete;
    RelationBinding(RelationBinding&&) noexcept = delete;
    RelationBinding& operator=(RelationBinding const&) = delete;
    RelationBinding& operator=(RelationBinding&&) noexcept = delete;

    /**
     * @brief returns the variables that reflect individual row columns.
     * @return the column variables
     */
    std::vector<std::shared_ptr<VariableBinding>>& columns() {
        return columns_;
    }

    /**
     * @brief returns the variables that reflect individual row columns.
     * @return the column variables
     */
    std::vector<std::shared_ptr<VariableBinding>> const& columns() const {
        return const_cast<RelationBinding*>(this)->columns();
    }

    /**
     * @brief returns the source table information.
     * @return the source table information
     *      only if the corresponded relation only contains the set of rows in the table
     * @return the invalid TableInfo otherwise
     */
    common::schema::TableInfo const& source_table() const {
        if (source_table_) {
            return *source_table_;
        }
        static common::schema::TableInfo const INVALID {};
        return INVALID;
    }

    /**
     * @brief sets the source table information.
     * @param info the source table
     * @return this
     */
    RelationBinding& source_table(common::schema::TableInfo const& info) {
        source_table_ = info.is_valid() ? &info : nullptr;
        return *this;
    }

    /**
     * @brief return the order of rows in the corresponded relation.
     * @return order if rows are sorted by the resulting elements
     * @return empty if rows are not ordered
     */
    std::vector<Order>& order() {
        return order_;
    }

    /**
     * @brief return the order of rows in the corresponded relation.
     * @return order if rows are sorted by the resulting elements
     * @return empty if rows are not ordered
     */
    inline std::vector<Order> const& order() const {
        return const_cast<RelationBinding*>(this)->order();
    }

    /**
     * @brief returns whether or not the the corresponded relation only consists of distinct rows.
     * @return true if the relation does not have any duplicated rows
     * @return false if the relation may have duplicated rows
     */
    bool distinct() const {
        return distinct_;
    }

    /**
     * @brief sets whether or not the the corresponded relation only consists of distinct rows.
     * @param distinct true if the relation dows not have any duplicated rows, otherwise false
     * @return this
     */
    RelationBinding& distinct(bool distinct) {
        distinct_ = distinct;
        return *this;
    }

    /**
     * @brief returns the view of extra attributes.
     * @return the view of extra attributes
     */
    std::map<std::string, std::any>& attributes() {
        return attributes_;
    }

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
    std::any const& find_attribute(std::string const& key) const {
        if (auto it = attributes().find(key); it != attributes().end()) {
            return it->second;
        }
        static std::any const EMPTY;
        return EMPTY;
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

private:
    std::vector<std::shared_ptr<VariableBinding>> columns_ {};
    std::map<std::string, std::any> attributes_ {};

    common::schema::TableInfo const* source_table_ {};
    std::vector<Order> order_ {};
    bool distinct_ { false };
};
}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_RELATION_BINDING_H_
