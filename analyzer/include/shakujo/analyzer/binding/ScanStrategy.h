/*
 * Copyright 2018-2023 Project Tsurugi.
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
#ifndef SHAKUJO_ANALYZER_SCAN_STRATEGY_H_
#define SHAKUJO_ANALYZER_SCAN_STRATEGY_H_

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <cstdlib>

#include "VariableBinding.h"

#include "shakujo/common/core/Value.h"
#include "shakujo/common/schema/TableInfo.h"
#include "shakujo/common/schema/IndexInfo.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::binding {

/**
 * @brief represents how perform scan operation.
 */
class ScanStrategy {
public:
    /**
     * @brief represents scan kind.
     */
    enum class Kind {
        /**
         * @brief range scan.
         */
        RANGE,

        /**
         * @brief prefix scan.
         */
        PREFIX,

        /**
         * @brief full scan.
         */
        FULL,
    };

    /**
     * @brief represents suffix of range scan key.
     */
    class Suffix {
    public:
        /**
         * @brief constructs a new object.
         */
        Suffix() noexcept
            : inclusive_(true)
        {}

        /**
         * @brief constructs a new object.
         * @param value the suffix key value
         * @param inclusive the corresponded range bound is inclusive or not
         */
        Suffix(std::unique_ptr<common::core::Value> value, bool inclusive)
            : value_(std::move(value))
            , inclusive_(inclusive)
        {}

        /**
         * @brief returns the suffix key value.
         * @return the suffix key value
         * @return nullptr if the corresponded range does not require suffix
         */
        common::core::Value const* value() const {
            return value_.get();
        }

        /**
         * @brief returns whether or not the corresponded range bound is inclusive.
         * @return true if the bound is inclusive
         * @return false otherwise
         */
        bool inclusive() const {
            return inclusive_;
        }

        /**
         * @brief returns whether or not this suffix has a valid value.
         * @return true if this has a valid value
         * @return false otherwise
         */
        bool is_valid() const {
            return common::util::is_valid(value_);
        }

        /**
         * @brief returns whether or not this suffix has a valid value.
         * @return true if this has a valid value
         * @return false otherwise
         */
        explicit operator bool() const {
            return is_valid();
        }

    private:
        common::util::ClonablePtr<common::core::Value> value_ {};
        bool inclusive_ {};
    };

    /**
     * @brief constructs a new object.
     */
    ScanStrategy() noexcept : kind_(Kind::FULL) {}

    /**
     * @brief constructs a new object.
     * @param table the target table
     * @param kind the strategy kind
     */
    ScanStrategy(common::schema::TableInfo const& table, Kind kind = Kind::FULL) noexcept  // NOLINT
        : kind_(kind)
        , table_(&table)
    {}

    /**
     * @brief constructs a new object.
     * @param table the target table
     * @param index the target index
     * @param key_columns the key columns
     * @param prefix the prefix values
     * @param lower_suffix suffix of the lower bound
     * @param upper_suffix suffix of the upper bound
     */
    ScanStrategy(
            common::schema::TableInfo const& table,
            common::schema::IndexInfo const& index,
            std::vector<std::shared_ptr<VariableBinding>> key_columns,
            std::vector<std::unique_ptr<common::core::Value>> prefix,
            Suffix lower_suffix = {},
            Suffix upper_suffix = {}) noexcept  // NOLINT
        : kind_(common::util::is_valid(lower_suffix.value()) || common::util::is_valid(upper_suffix.value()) ? Kind::RANGE : Kind::PREFIX)
        , table_(&table)
        , index_(&index)
        , key_columns_(std::move(key_columns))
        , lower_suffix_(std::move(lower_suffix))
        , upper_suffix_(std::move(upper_suffix))
    {
        prefix_.reserve(prefix.size());
        for (auto&& v : prefix) {
            prefix_.emplace_back(std::move(v));
        }
    }

    /**
     * @brief returns the kind of this strategy.
     * @return the strategy kind
     */
    Kind kind() const {
        return kind_;
    }

    /**
     * @brief sets the kind of this strategy.
     * @param kind the strategy kind
     * @return this
     */
    ScanStrategy& kind(Kind kind) {
        kind_ = kind;
        return *this;
    }

    /**
     * @brief returns information about the scan target table.
     * @return the scan target table
     */
    common::schema::TableInfo const& table() const {
        if (common::util::is_valid(table_)) {
            return *table_;
        }
        static common::schema::TableInfo const INVALID {};
        return INVALID;
    }

    /**
     * @brief returns information about the scan target index.
     * @return the scan target index
     * @return invalid information if this strategy just represents table full scan
     */
    common::schema::IndexInfo const& index() const {
        if (common::util::is_valid(index_)) {
            return *index_;
        }
        static common::schema::IndexInfo const INVALID {};
        return INVALID;
    }

    /**
     * @brief returns the bindings of target key columns.
     * The resulting variables just correspond to key of the target index.
     * @return bindings of the target index columns
     * @return empty if this strategy just represents table full scan
     */
    std::vector<std::shared_ptr<VariableBinding>> const& key_columns() const {
        return key_columns_;
    }

    /**
     * @brief returns the key prefix of prefix scan or range scan.
     * If this returned as many values as the index key columns, this strategy intends to obtain single entry.
     * @return the key or key prefix
     * @return empty if this strategy is not a kind of prefix scan or range scan
     */
    std::vector<std::shared_ptr<common::core::Value>> const& prefix() const {
        return prefix_;
    }

    /**
     * @brief returns the suffix key of lower bound of range scan.
     * This is a suffix of prefix(), so that the actual lower bound must be concat of prefix() and lower_suffix().
     * This is only available if this->kind() == Kind::RANGE and the lower limit has suffix.
     * If the returned value is available, it corresponded to this->key_columns()[prefix().size()].
     * @return suffix of the lower limit of range scan
     * @return empty if this strategy is not a kind of range scan, or range scan with no lower suffix
     */
    Suffix const& lower_suffix() const {
        return lower_suffix_;
    }

    /**
     * @brief returns the suffix key of upper bound of range scan.
     * This is a suffix of prefix(), so that the actual upper bound must be concat of prefix() and lower_suffix().
     * This is only available if this->kind() == Kind::RANGE and the upper limit has suffix.
     * If the returned value is available, it corresponded to this->key_columns()[prefix().size()].
     * @return suffix of the upper limit of range scan
     * @return empty if this strategy is not a kind of range scan, or range scan with no upper suffix
     */
    Suffix const& upper_suffix() const {
        return upper_suffix_;
    }

    /**
     * @brief returns whether or not this object is valid.
     * @return true if this is valid
     * @return false otherwise
     */
    bool is_valid() const {
        if (!common::util::is_valid(table_)) {
            return false;
        }
        if (kind_ == Kind::FULL) {
            if (common::util::is_valid(index_)) {
                if (index_->columns().size() != key_columns_.size()) {
                    return false;
                }
            }
            return true;
        }
        if (!common::util::is_valid(index_)) {
            return false;
        }
        if (index_->columns().size() != key_columns_.size()) {
            return false;
        }
        if (kind_ == Kind::PREFIX) {
            return !prefix_.empty();
        }
        if (kind_ == Kind::RANGE) {
            return common::util::is_valid(lower_suffix_.value()) || common::util::is_valid(upper_suffix_.value());
        }
        std::abort();
    }

    /**
     * @brief returns whether or not this object is valid.
     * @return true if this is valid
     * @return false otherwise
     */
    explicit operator bool() const {
        return is_valid();
    }

private:
    Kind kind_;  // NOLINT - by clang issue, we cannot initialize with value
    common::schema::TableInfo const* table_ {};
    common::schema::IndexInfo const* index_ {};
    std::vector<std::shared_ptr<VariableBinding>> key_columns_ {};
    std::vector<std::shared_ptr<common::core::Value>> prefix_ {};
    Suffix lower_suffix_ {};
    Suffix upper_suffix_ {};
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(ScanStrategy::Kind value) {
    using Kind = ScanStrategy::Kind;
    switch (value) {
        case Kind::RANGE: return "RANGE";
        case Kind::PREFIX: return "PREFIX";
        case Kind::FULL: return "FULL";
    }
    std::abort();
}

/**
 * @brief append textual representation of the given value.
 * @param out the target output stream
 * @param value the target value
 * @return the output stream
 */
inline std::ostream& operator<<(std::ostream& out, ScanStrategy::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::analyzer::binding

#endif  //SHAKUJO_ANALYZER_SCAN_STRATEGY_H_
