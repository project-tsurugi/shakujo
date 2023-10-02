/*
 * Copyright 2018-2023 tsurugi project..
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

#include <algorithm>
#include <any>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "VariableBinding.h"
#include "ScanStrategy.h"
#include "JoinStrategy.h"

#include "shakujo/common/core/Direction.h"
#include "shakujo/common/schema/TableInfo.h"
#include "shakujo/common/schema/IndexInfo.h"

#include "shakujo/model/key/RelationKey.h"

#include "shakujo/common/util/utility.h"

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
     * @brief represents input/output relation profile.
     */
    class Profile {
    public:
        /**
         * @brief constructs a new object.
         */
        Profile(std::vector<std::shared_ptr<VariableBinding>> columns = {})  // NOLINT
            : columns_(std::move(columns))
        {}

        /**
         * @brief returns the variables that reflect individual row columns.
         * Each element is sorted by the column of the corresponded relation.
         * @return the column variables
         */
        std::vector<std::shared_ptr<VariableBinding>>& columns() {
            return columns_;
        }

        /**
         * @brief returns the variables that reflect individual row columns.
         * Each element is sorted by the column of the corresponded relation.
         * @return the column variables
         */
        std::vector<std::shared_ptr<VariableBinding>> const& columns() const {
            return const_cast<Profile*>(this)->columns();
        }

        /**
         * @brief returns the index of the column.
         * @param binding the column binding
         * @return the column index (0-origin)
         * @return empty if the binding does not indicates any columns in the corresponded relation
         */
        std::optional<std::size_t> index_of(VariableBinding const& binding) const {
            for (std::size_t i = 0, n = columns_.size(); i < n; ++i) {
                if (columns_[i]->id() == binding.id()) {
                    return std::make_optional(i);
                }
            }
            return std::nullopt;
        }

        /**
         * @brief returns the source table information.
         * @return the source table information
         *      only if the corresponded relation only contains the set of rows/columns in the table
         * @return the invalid TableInfo otherwise
         */
        common::schema::TableInfo const& source_table() const {
            using common::util::is_defined;
            if (is_defined(source_table_)) {
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
        Profile& source_table(common::schema::TableInfo const& info) {
            source_table_ = info.is_valid() ? &info : nullptr;
            return *this;
        }

        /**
         * @brief returns a set of columns which is always constant.
         * @return the constant columns
         */
        std::set<std::shared_ptr<VariableBinding>>& constants() {
            return constants_;
        }

        /**
         * @brief returns a set of columns which is always constant.
         * @return the constant columns
         */
        std::set<std::shared_ptr<VariableBinding>> const& constants() const {
            return constants_;
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
            return const_cast<Profile*>(this)->order();
        }

        /**
         * @brief returns the unique keys of the corresponded relation.
         * @return the list of unique keys
         * @return empty if the relation may contain any duplicated rows
         */
        std::set<std::set<std::shared_ptr<VariableBinding>>>& unique_keys() {
            return unique_keys_;
        }

        /**
         * @brief returns the unique keys of the corresponded relation.
         * @return the list of unique keys
         * @return empty if the relation may contain any duplicated rows
         */
        std::set<std::set<std::shared_ptr<VariableBinding>>> const& unique_keys() const {
            return unique_keys_;
        }

        /**
         * @brief returns whether or not the corresponded relation only consists of distinct rows.
         * @return true if the relation does not have any duplicated rows
         * @return false if the relation may have duplicated rows
         */
        bool distinct() const {
            return !unique_keys_.empty();
        }

        /**
         * @brief returns whether or not this profile is valid.
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
        common::schema::TableInfo const* source_table_ {};
        std::set<std::shared_ptr<VariableBinding>> constants_ {};
        std::vector<Order> order_ {};
        std::set<std::set<std::shared_ptr<VariableBinding>>> unique_keys_ {};
    };

    /**
     * @brief constructs a new object.
     * @param process a profile while processing the corresponded operator itself
     * @param output a profile of operator result
     */
    explicit RelationBinding(Profile process = {}, Profile output = {})
        : process_(std::move(process)), output_(std::move(output))
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
     * @brief returns the profile for processing the corresponded operator.
     * @return the profile for processing the corresponded operator
     */
    inline Profile& process() {
        return process_;
    }

    /**
     * @brief returns the profile for processing the corresponded operator.
     * @return the profile for processing the corresponded operator
     */
    inline Profile const& process() const {
        return process_;
    }

    /**
     * @brief returns the output profile.
     * @return the output profile
     */
    inline Profile& output() {
        return output_;
    }

    /**
     * @brief returns the output profile.
     * @return the output profile
     */
    inline Profile const& output() const {
        return output_;
    }

    /**
     * @brief returns the destination table information.
     * This is only available for insert/update/delete statements.
     * @return the destination table information
     * @return the invalid TableInfo otherwise
     */
    common::schema::TableInfo const& destination_table() const {
        using common::util::is_defined;
        if (is_defined(destination_table_)) {
            return *destination_table_;
        }
        static common::schema::TableInfo const INVALID {};
        return INVALID;
    }

    /**
     * @brief sets the destination table information.
     * @param info the destination table
     * @return this
     */
    RelationBinding& destination_table(common::schema::TableInfo const& info) {
        destination_table_ = info.is_valid() ? &info : nullptr;
        return *this;
    }

    /**
     * @brief sets the destination table information.
     * @param info the destination table
     * @return this
     */
    RelationBinding& destination_table(common::schema::TableInfo&& info) {
        destination_table_own_ = std::move(info);
        if (destination_table_own_.is_valid()) {
            destination_table_ = &destination_table_own_;
        } else {
            destination_table_ = nullptr;
        }
        return *this;
    }

    /**
     * @brief returns the variables that reflect columns of the destination table.
     * @return the column variables, ordered by corresponded TableInfo::columns()
     * @return empty if the destination table is not defined
     */
    std::vector<std::shared_ptr<VariableBinding>>& destination_columns() {
        return destination_columns_;
    }

    /**
     * @brief returns the variables that reflect columns of the destination table.
     * @return the column variables, ordered by corresponded TableInfo::columns()
     * @return empty if the destination table is not defined
     */
    std::vector<std::shared_ptr<VariableBinding>> const& destination_columns() const {
        return destination_columns_;
    }

    /**
     * @brief returns the index of the destination column.
     * @param binding the column binding
     * @return the column index (0-origin)
     * @return empty if the binding does not indicates any columns in the destination table
     */
    std::optional<std::size_t> destination_index_of(VariableBinding const& binding) const {
        for (std::size_t i = 0, n = destination_columns_.size(); i < n; ++i) {
            if (destination_columns_[i]->id() == binding.id()) {
                return std::make_optional(i);
            }
        }
        return {};
    }

    /**
     * @brief returns the scan strategy.
     * @return the scan strategy of corresponded operation
     * @return undefined if the corresponded operation is not a scan
     */
    ScanStrategy& scan_strategy() {
        return scan_strategy_;
    }

    /**
     * @brief returns the scan strategy.
     * @return the scan strategy of corresponded operation
     * @return undefined if the corresponded operation is not a scan
     */
    ScanStrategy const& scan_strategy() const {
        return scan_strategy_;
    }

    /**
     * @brief sets the scan strategy
     * @param strategy the strategy
     * @return this
     */
    RelationBinding& scan_strategy(ScanStrategy strategy) {
        scan_strategy_ = std::move(strategy);
        return *this;
    }

    /**
     * @brief returns the join strategy.
     * @return the join strategy of corresponded operation
     * @return undefined if the corresponded operation is not a join
     */
    JoinStrategy& join_strategy() {
        return join_strategy_;
    }

    /**
     * @brief returns the join strategy.
     * @return the join strategy of corresponded operation
     * @return undefined if the corresponded operation is not a join
     */
    JoinStrategy const& join_strategy() const {
        return join_strategy_;
    }

    /**
     * @brief sets the join strategy
     * @param strategy the strategy
     * @return this
     */
    RelationBinding& join_strategy(JoinStrategy strategy) {
        join_strategy_ = std::move(strategy);
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
        if (!process_.columns().empty()) {
            if (!process_.is_valid()) {
                return false;
            }
        }
        if (!output_.columns().empty()) {
            if (!output_.is_valid()) {
                return false;
            }
        }
        for (auto&& c : destination_columns_) {
            if (!common::util::is_valid(c)) {
                return false;
            }
        }
        return !process_.columns().empty()
            || !output_.columns().empty()
            || common::util::is_valid(destination_table_)
            || !destination_columns_.empty();
    }

private:
    Profile process_;
    Profile output_;
    common::schema::TableInfo const* destination_table_ {};
    common::schema::TableInfo destination_table_own_ {};
    std::vector<std::shared_ptr<VariableBinding>> destination_columns_ {};
    ScanStrategy scan_strategy_;
    JoinStrategy join_strategy_;
    std::map<std::string, std::any> attributes_ {};
};
}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_RELATION_BINDING_H_
