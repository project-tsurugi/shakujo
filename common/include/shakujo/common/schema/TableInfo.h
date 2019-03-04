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

#ifndef SHAKUJO_COMMON_SCHEMA_TABLE_INFO_H_
#define SHAKUJO_COMMON_SCHEMA_TABLE_INFO_H_

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <optional>
#include <utility>
#include <vector>

#include "IndexInfo.h"

#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/utility.h"

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"

namespace shakujo::common::schema {

/**
 * @brief provides table schema information.
 */
class TableInfo {
public:
    /**
     * @brief provides column schema information.
     */
    class Column {
    private:
        std::string name_;
        util::ClonablePtr<core::Type> type_;
        util::ClonablePtr<core::Value> default_value_;

    public:
        /**
         * @brief Constructs a new object.
         * @param name the column name
         * @param type the column type
         * @param default_value the default value
         */
        Column(
                std::string_view name,
                std::unique_ptr<core::Type> type,
                std::unique_ptr<core::Value> default_value = {})
            : name_(name)
            , type_(std::move(type))
            , default_value_(std::move(default_value))
        {}

        /**
         * @brief Constructs a new object.
         * @param name the column name
         * @param type the column type
         * @param default_value the default value
         */
        Column(
                std::string_view name,
                core::Type const* type,
                core::Value const* default_value = nullptr)
            : Column(name, util::make_clone(type), util::make_clone(default_value))
        {}

        /**
         * @brief Constructs a new object.
         * @param name the column name
         * @param type the column type
         * @param default_value the default value
         */
        Column(
                std::string_view name,
                core::Type&& type,
                core::Value&& default_value)
            : Column(name, util::make_clone(std::move(type)), util::make_clone(std::move(default_value)))
        {}

        /**
         * @brief Constructs a new object.
         * @param name the column name
         * @param type the column type
         */
        Column(
                std::string_view name,
                core::Type&& type)
            : Column(name, util::make_clone(std::move(type)), {})
        {}

        /**
         * @brief returns the column name.
         * @return the column name
         */
        std::string const& name() const {
            return name_;
        }

        /**
         * @brief returns the column value.
         * @return the column value
         */
        core::Type const* type() const {
            return type_.get();
        }

        /**
         * @brief returns the default column value
         * @return the default value, or undefined value if it is not defined
         * @see core::value::Undefined
         */
        core::Value const* default_value() const {
            return default_value_.get();
        }

        /**
         * @brief Appends the object information into the given output stream.
         * @param out the target output stream
         * @param value the target object
         * @return the output stream
         */
        friend std::ostream& operator<<(std::ostream& out, const TableInfo::Column& value);

    };

private:
    core::Name name_;
    std::vector<Column> columns_;
    IndexInfo primary_index_;
    std::vector<IndexInfo> secondary_indices_;

public:

    /**
     * @brief constructs a new invalid object.
     */
    TableInfo() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param name the table name
     * @param columns the columns
     * @param primary_index the primary index
     * @param secondary_indices the secondary indices
     */
    TableInfo(
            core::Name name,
            std::vector<Column> columns,
            IndexInfo primary_index = {},
            std::vector<IndexInfo> secondary_indices = {})
        : name_(std::move(name))
        , columns_(std::move(columns))
        , primary_index_(std::move(primary_index))
        , secondary_indices_(std::move(secondary_indices))
    {}

    /**
     * @brief constructs a new object.
     * @param name the table name
     * @param columns the columns
     * @param primary_index the primary index
     * @param secondary_indices the secondary indices
     */
    TableInfo(
            std::string_view name,
            std::vector<Column> columns,
            IndexInfo primary_index = {},
            std::vector<IndexInfo> secondary_indices = {})
        : name_(name)
        , columns_(std::move(columns))
        , primary_index_(std::move(primary_index))
        , secondary_indices_(std::move(secondary_indices))
    {}

    /**
     * @brief returns the table name.
     * @return the table name
     */
    core::Name const& name() const {
        return name_;
    }

    /**
     * @brief returns the columns information.
     * @return the columns information
     */
    std::vector<Column> const& columns() const {
        return columns_;
    }

    /**
     * @brief returns the column index of the name.
     * @param name the column name
     * @return the index of found column (0-origin)
     * @return empty if there no such a column
     */
    std::optional<std::size_t> index_of(std::string_view name) const {
        for (std::size_t i = 0U, n = columns_.size(); i < n; ++i) {
            if (columns_[i].name() == name) {
                return std::make_optional(i);
            }
        }
        return {};
    }

    /**
     * @brief returns the column index of the name.
     * @param name the column name
     * @return the index of found column (0-origin)
     * @return empty if there no such a column
     */
    std::optional<std::size_t> index_of(core::Name const& name) const {
        for (std::size_t i = 0U, n = columns_.size(); i < n; ++i) {
            if (columns_[i].name() == name) {
                return std::make_optional(i);
            }
        }
        return {};
    }

    /**
     * @brief returns the primary index information.
     * @return the primary index information
     */
    IndexInfo const& primary_index() const {
        return primary_index_;
    }

    /**
     * @brief returns the secondary indices information.
     * @return the secondary indices information
     */
    std::vector<IndexInfo> const& secondary_indices() const {
        return secondary_indices_;
    }

    /**
     * @brief returns the secondary index of the given name.
     * @param name the index name
     * @return the related secondary index
     * @return invalid index if it does not exist
     */
    IndexInfo const& find_secondary_index(std::string_view name) const {
        for (auto&& index : secondary_indices_) {
            if (index.name() == name) {
                return index;
            }
        }
        static IndexInfo const INVALID {};
        return INVALID;
    }

    /**
     * @brief returns the secondary index of the given name.
     * @param name the index name
     * @return the related secondary index
     * @return invalid index if it does not exist
     */
    IndexInfo const& find_secondary_index(common::core::Name const& name) const {
        for (auto&& index : secondary_indices_) {
            if (index.name() == name) {
                return index;
            }
        }
        static IndexInfo const INVALID {};
        return INVALID;
    }

    /**
     * @brief returns whether or not this table information is valid.
     * @return true if this is valid
     * @return false otherwise
     */
    bool is_valid() const noexcept {
        return !name_.empty() && !columns_.empty();
    }

    /**
     * @brief returns whether or not this table information is valid.
     * @return true if this is valid
     * @return false otherwise
     */
    inline explicit operator bool() const noexcept {
        return is_valid();
    }

    /**
     * @brief Appends the object information into the given output stream.
     * @param out the target output stream
     * @param value the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, const TableInfo& value);
};

}  // namespace shakujo::common::schema

#endif  // SHAKUJO_COMMON_SCHEMA_TABLE_INFO_H_
