/*
 * Copyright 2018 shakujo project.
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
     * @brief constructs a new object.
     */
    ScanStrategy() noexcept = default;

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
     * @brief returns the kind of this strategy.
     * If the kind is FULL, executor must evaluate the condition in ScanExpression manually.
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
     * @brief returns the lower limit of range scan.
     * This may return shorter vector than the key_columns().
     * @return the lower limit
     * @return empty if this strategy is not a kind of range scan, or range scan with no lower limit
     */
    std::vector<std::unique_ptr<common::core::Value>> const& lower_limit() const {
        return lower_limit_;
    }

    /**
     * @brief returns whether or not range scan includes entry on the lower limit.
     * @return true if the lower limit is inclusive
     * @return false otherwise
     */
    bool lower_inclusive() const {
        return lower_inclusive_;
    }

    /**
     * @brief returns the upper limit of range scan.
     * This may return shorter vector than the key_columns().
     * @return the upper limit
     * @return empty if this strategy is not a kind of range scan, or range scan with no upper limit
     */
    std::vector<std::unique_ptr<common::core::Value>> const& upper_limit() const {
        return upper_limit_;
    }

    /**
     * @brief returns whether or not range scan includes entry on the upper limit.
     * @return true if the upper limit is inclusive
     * @return false otherwise
     */
    bool upper_inclusive() const {
        return upper_inclusive_;
    }

    /**
     * @brief returns the key prefix of prefix scan.
     * If this returned as many values as the index key columns, this strategy intends to obtain single entry.
     * @return the key or key prefix
     * @return empty if this strategy is not a kind of prefix scan
     */
    std::vector<std::unique_ptr<common::core::Value>> const& prefix() const {
        return prefix_;
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
        if (kind_ == Kind::RANGE) {
            return !lower_limit_.empty() || !upper_limit_.empty();
        }
        if (kind_ == Kind::PREFIX) {
            return !prefix_.empty();
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
    Kind kind_ { Kind::FULL };
    common::schema::TableInfo const* table_ {};
    common::schema::IndexInfo const* index_ {};
    std::vector<std::shared_ptr<VariableBinding>> key_columns_ {};
    std::vector<std::unique_ptr<common::core::Value>> lower_limit_ {};
    bool lower_inclusive_ {};
    std::vector<std::unique_ptr<common::core::Value>> upper_limit_ {};
    bool upper_inclusive_ {};
    std::vector<std::unique_ptr<common::core::Value>> prefix_ {};
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
    return "(unknown)";
}

/**
 * @brief append textual representation of Diagnostic::Code.
 * @param out the target output stream
 * @param value the target value
 * @return the output stream
 */
inline std::ostream& operator<<(std::ostream& out, ScanStrategy::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::analyzer::binding

#endif  //SHAKUJO_ANALYZER_SCAN_STRATEGY_H_
