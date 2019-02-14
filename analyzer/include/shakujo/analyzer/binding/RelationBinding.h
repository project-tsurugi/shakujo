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

#include <algorithm>
#include <any>
#include <map>
#include <memory>
#include <optional>
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
            return {};
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
         * @brief returns the source index information.
         * @return the source index information
         *      only if the source_table() is valid and the corresponded relation comes from the index
         * @return the invalid IndexInfo otherwise
         */
        common::schema::IndexInfo const& source_index() const {
            using common::util::is_defined;
            if (is_defined(source_index_)) {
                return *source_index_;
            }
            static common::schema::IndexInfo const INVALID {};
            return INVALID;
        }

        /**
         * @brief sets the source index information.
         * @param info the source index
         * @return this
         */
        Profile& source_index(common::schema::IndexInfo const& info) {
            source_index_ = info.is_valid() ? &info : nullptr;
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
            return const_cast<Profile*>(this)->order();
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
        Profile& distinct(bool distinct) {
            distinct_ = distinct;
            return *this;
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
        common::schema::IndexInfo const* source_index_ {};
        std::vector<Order> order_ {};
        bool distinct_ { false };
    };

    /**
     * @brief join operation detail of each resulting column.
     */
    class JoinColumn {
    public:
        /**
         * @brief constructs a new object.
         * @param qualifiers the output variable qualifiers - FIXME: variable binding with aliases instead
         * @param output the output column binding
         * @param left_source the source column binding from the left operand
         * @param nullify_left_source whether or not the value from the left operand must be nullified
         * @param right_source the source column binding from the right operand
         * @param nullify_right_source whether or not the value from the right operand must be nullified
         */
        JoinColumn(
                std::vector<common::core::Name> qualifiers,
                std::shared_ptr<binding::VariableBinding> output,
                std::shared_ptr<binding::VariableBinding> left_source, bool nullify_left_source,
                std::shared_ptr<binding::VariableBinding> right_source, bool nullify_right_source)
            : qualifiers_(std::move(qualifiers)), output_(std::move(output))
            , left_source_(std::move(left_source)), nullify_left_source_(nullify_left_source)
            , right_source_(std::move(right_source)), nullify_right_source_(nullify_right_source)
        {}

        /**
         * @brief constructs a new object which refers the left operand.
         * @param output the output column binding
         * @param source the source column binding from the left operand
         * @param nullify whether or not the value from the left operand must be nullified
         * @return the created object
         */
        inline static JoinColumn left(
                std::vector<common::core::Name> qualifiers,
                std::shared_ptr<binding::VariableBinding> output,
                std::shared_ptr<binding::VariableBinding> source, bool nullify) {
            return { std::move(qualifiers), std::move(output), std::move(source), nullify, {}, {} };
        }

        /**
         * @brief constructs a new object which refers the left operand.
         * @param output the output column binding
         * @param source the source column binding from the left operand
         * @param nullify whether or not the value from the left operand must be nullified
         * @return the created object
         */
        inline static JoinColumn right(
                std::vector<common::core::Name> qualifiers,
                std::shared_ptr<binding::VariableBinding> output,
                std::shared_ptr<binding::VariableBinding> source, bool nullify) {
            return { std::move(qualifiers), std::move(output), {}, {}, std::move(source), nullify };
        }

        /**
         * @brief returns the available output column qualifiers.
         * @return output column qualifiers
         */
        std::vector<common::core::Name> const& qualifiers() const {
            return qualifiers_;
        }

        /**
         * @brief returns the output column binding.
         * @return the output column binding
         */
        std::shared_ptr<binding::VariableBinding> output() const {
            return output_;
        }

        /**
         * @brief returns the source column binding from the left operand.
         * @return the source column binding
         * @return empty if the target output does not refer column from the left operand
         */
        std::shared_ptr<binding::VariableBinding> left_source() const {
            return left_source_;
        }

        /**
         * @brief returns the source column binding from the right operand.
         * @return the source column binding
         * @return empty if the target output does not refer column from the right operand
         */
        std::shared_ptr<binding::VariableBinding> right_source() const {
            return right_source_;
        }

        /**
         * @brief returns whether or not the value from left operand must be nullified before compare or output.
         * @return true if the output column value must be nullified
         * @return false otherwise, or the target output does not refer value from the left operand
         */
        bool nullify_left_source() const {
            return nullify_left_source_;
        }

        /**
         * @brief returns whether or not the value from right operand must be nullified before compare or output.
         * @return true if the output column value must be nullified
         * @return false otherwise, or the target output does not refer value from the right operand
         */
        bool nullify_right_source() const {
            return nullify_right_source_;
        }

    private:
        std::vector<common::core::Name> qualifiers_;
        std::shared_ptr<binding::VariableBinding> output_;
        std::shared_ptr<binding::VariableBinding> left_source_;
        bool nullify_left_source_;
        std::shared_ptr<binding::VariableBinding> right_source_;
        bool nullify_right_source_;
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
     * @brief returns the join operation of individual columns.
     * This is only available for JoinExpressions.
     * @return the join operations
     * @return empty if the corresponded operation is not a valid JoinExpression
     */
    std::vector<JoinColumn>& join_columns() {
        return join_columns_;
    }

    /**
     * @brief returns the join operation of individual columns.
     * This is only available for JoinExpressions.
     * @return the join operations
     * @return empty if the corresponded operation is not a valid JoinExpression
     */
    std::vector<JoinColumn> const& join_columns() const {
        return join_columns_;
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
            || !destination_columns_.empty();
    }

private:
    Profile process_;
    Profile output_;
    common::schema::TableInfo const* destination_table_ {};
    std::vector<std::shared_ptr<VariableBinding>> destination_columns_ {};
    std::vector<JoinColumn> join_columns_ {};
    std::map<std::string, std::any> attributes_ {};
};
}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_RELATION_BINDING_H_
