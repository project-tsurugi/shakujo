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

#ifndef SHAKUJO_COMMON_SCHEMA_INDEX_INFO_H_
#define SHAKUJO_COMMON_SCHEMA_INDEX_INFO_H_

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "shakujo/common/util/utility.h"

#include "shakujo/common/core/Direction.h"
#include "shakujo/common/core/Name.h"

namespace shakujo::common::schema {

/**
 * @brief provides index schema information.
 */
class IndexInfo {
public:
    /**
     * @brief provides column information.
     */
    class Column {
    public:
        /**
         * @brief represents sort order of columns.
         */
        using Direction = core::Direction;

    private:
        std::string name_;
        Direction direction_;

    public:
        /**
         * @brief constructs a new object.
         * @param name the column name
         * @param direction the sort direction
         */
        Column(std::string_view name, Direction direction = Direction::ASCENDANT)  // NOLINT
            : name_(name)
            , direction_(direction)
        {}

        /**
         * @brief returns the column name.
         * @return the column name
         */
        std::string const& name() const {
            return name_;
        }

        /**
         * @brief returns the sort direction
         * @return the sort direction
         */
        Direction direction() const {
            return direction_;
        }

        /**
         * @brief Appends the object information into the given output stream.
         * @param out the target output stream
         * @param value the target object
         * @return the output stream
         */
        friend std::ostream& operator<<(std::ostream& out, IndexInfo::Column const& value);
    };

private:
    core::Name name_;
    std::vector<Column> columns_;

public:

    /**
     * @brief constructs a new invalid object.
     */
    IndexInfo() noexcept = default;

    /**
     * @brief constructs a new object that represents a primary index.
     * @param columns the columns
     */
    IndexInfo(std::vector<Column> columns)  // NOLINT
        : columns_(std::move(columns))
    {}

    /**
     * @brief constructs a new object that represents a secondary index.
     * @param name the index name
     * @param columns the columns
     */
    IndexInfo(
            core::Name name,
            std::vector<Column> columns)
        : name_(std::move(name))
        , columns_(std::move(columns))
    {}

    /**
     * @brief constructs a new object.
     * @param name the index name
     * @param columns the columns
     */
    IndexInfo(
            std::string_view name,
            std::vector<Column> columns)
        : name_(name)
        , columns_(std::move(columns))
    {}

    /**
     * @brief returns whether or not this index is primary.
     * @return true if it is primary index
     * @return false otherwise
     */
    bool is_primary() const {
        return is_valid() && name_.empty();
    }

    /**
     * @brief returns the index name.
     * @return the index name
     * @return empty string if this is the primary index
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
     * @brief returns whether or not this index information is valid.
     * @return true if this is valid
     * @return false otherwise
     */
    bool is_valid() const noexcept {
        return !columns_.empty();
    }

    /**
     * @brief returns whether or not this index information is valid.
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
    friend std::ostream& operator<<(std::ostream& out, IndexInfo const& value);
};
}  // namespace shakujo::common::schema

#endif  // SHAKUJO_COMMON_SCHEMA_INDEX_INFO_H_
