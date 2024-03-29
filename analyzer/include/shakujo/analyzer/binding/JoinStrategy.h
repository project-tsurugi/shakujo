/*
 * Copyright 2018-2024 Project Tsurugi.
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
#ifndef SHAKUJO_ANALYZER_JOIN_STRATEGY_H_
#define SHAKUJO_ANALYZER_JOIN_STRATEGY_H_

#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "VariableBinding.h"

namespace shakujo::analyzer::binding {

/**
 * @brief represents how perform join operation.
 */
class JoinStrategy {
public:
    /**
     * @brief represents scan kind.
     */
    enum class Kind {
        /**
         * @brief union join.
         */
        UNION,

        /**
         * @brief nested loop join.
         */
        NESTED_LOOP,
    };

    /**
     * @brief join operation detail of each resulting column.
     */
    class Column {
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
        Column(
                std::vector<common::core::Name> qualifiers,
                std::shared_ptr<VariableBinding> output,
                std::shared_ptr<VariableBinding> left_source, bool nullify_left_source,
                std::shared_ptr<VariableBinding> right_source, bool nullify_right_source)
            : qualifiers_(std::move(qualifiers)), output_(std::move(output))
            , left_source_(std::move(left_source)), nullify_left_source_(nullify_left_source)
            , right_source_(std::move(right_source)), nullify_right_source_(nullify_right_source)
        {}

        /**
         * @brief returns the available output column qualifiers.
         * @return output column qualifiers
         */
        std::vector<common::core::Name> const& qualifiers() const {
            return qualifiers_;
        }

        /**
         * @brief returns the output column binding.
         * If the both left_source() and right_source() are invalid, this may return a pseud variable
         * which always provides nullptr.
         * @return the output column binding
         */
        std::shared_ptr<VariableBinding> output() const {
            return output_;
        }

        /**
         * @brief returns the source column binding from the left operand.
         * @return the source column binding
         * @return empty if the target output does not refer column from the left operand
         */
        std::shared_ptr<VariableBinding> left_source() const {
            return left_source_;
        }

        /**
         * @brief returns the source column binding from the right operand.
         * @return the source column binding
         * @return empty if the target output does not refer column from the right operand
         */
        std::shared_ptr<VariableBinding> right_source() const {
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
        std::shared_ptr<VariableBinding> output_;
        std::shared_ptr<VariableBinding> left_source_;
        bool nullify_left_source_;
        std::shared_ptr<VariableBinding> right_source_;
        bool nullify_right_source_;

        friend class JoinStrategy;

        /**
         * @brief swap left/right.
         * @return this
         */
        Column& commute() {
            std::swap(left_source_, right_source_);
            std::swap(nullify_left_source_, nullify_right_source_);
            return *this;
        }
    };

    /**
     * @brief represents a column data which is resolved or unresolved.
     */
    class ColumnData {
    public:
        /**
         * @brief constructs a new object.
         * @param type the value type
         * @param variable the variable which represents this data
         */
        ColumnData(
                std::unique_ptr<common::core::Type> type,
                std::shared_ptr<binding::VariableBinding> variable)
            : type_(std::move(type))
            , variable_(std::move(variable))
        {}

        /**
         * @brief constructs a new object.
         * @param type the value type
         * @param value the constant value
         */
        ColumnData(
                std::unique_ptr<common::core::Type> type,
                std::unique_ptr<common::core::Value> value)
            : type_(std::move(type))
            , value_(std::move(value))
        {}

        /**
         * @brief returns whether or not this column data has a resolved value.
         * @return true if value() returns a valid value
         * @return false if variable() returns a valid variable binding
         */
        bool is_resolved() const {
            return value_ != nullptr;
        }

        /**
         * @brief returns the data type.
         * @return the data type
         */
        common::core::Type const* type() const {
            return type_.get();
        }

        /**
         * @brief returns the variable.
         * @return the variable which represents this data
         * @return empty if it is not defined
         */
        std::shared_ptr<binding::VariableBinding> variable() const {
            return variable_;
        }

        /**
         * @brief returns the value.
         * @return the value which represents this data
         * @return nullptr if it is not defined
         */
        common::core::Value const* value() const {
            return value_.get();
        }

    private:
        std::shared_ptr<common::core::Type> type_;
        std::shared_ptr<binding::VariableBinding> variable_ {};
        std::shared_ptr<common::core::Value> value_ {};
    };

    /**
     * @brief constructs a new object.
     */
    JoinStrategy() noexcept : JoinStrategy(Kind::NESTED_LOOP, false, false, false, false) {}

    /**
     * @brief constructs a new object.
     * @param kind the join kind
     * @param left_outer is left/full outer join
     * @param right_outer is right/full outer join
     * @param left_semi is left semi join
     * @param right_semi is right semi join
     * @param columns the join columns
     * @param equalities the equality terms
     */
    JoinStrategy(
            Kind kind,
            bool left_outer,
            bool right_outer,
            bool left_semi,
            bool right_semi,
            std::vector<Column> columns = {},
            std::set<std::pair<std::shared_ptr<VariableBinding>, std::shared_ptr<VariableBinding>>> equalities = {})
        : kind_(kind)
        , left_outer_(left_outer)
        , right_outer_(right_outer)
        , left_semi_(left_semi)
        , right_semi_(right_semi)
        , columns_(std::move(columns))
        , equalities_(std::move(equalities))
    {}

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
    JoinStrategy& kind(Kind kind) {
        kind_ = kind;
        return *this;
    }

    /**
     * @brief returns whether or not left input always stay in the result even if opposite does not exist.
     * @return true is left or full outer join
     * @return false otherwise
     * @return undefined if this is union join
     */
    bool left_outer() const {
        return left_outer_;
    }

    /**
     * @brief sets whether or not left input always stay in the result even if opposite does not exist.
     * @param on true for left or full outer join
     * @return this
     */
    JoinStrategy& left_outer(bool on) {
        left_outer_ = on;
        return *this;
    }

    /**
     * @brief returns whether or not right input always stay in the result even if opposite does not exist.
     * @return true is right or full outer join
     * @return false otherwise
     * @return undefined if this is union join
     */
    bool right_outer() const {
        return right_outer_;
    }

    /**
     * @brief sets whether or not right input always stay in the result even if opposite right does not exist.
     * @param on true for right or full outer join
     * @return this
     */
    JoinStrategy& right_outer(bool on) {
        right_outer_ = on;
        return *this;
    }

    /**
     * @brief returns whether or not this operation is left semi join.
     * @return true if this is left semi join
     * @return false otherwise
     */
    bool left_semi() const {
        return left_semi_;
    }

    /**
     * @brief sets wether or not this operation is natural join.
     * @param on true to mark as natural join
     * @return this
     */
    JoinStrategy& left_semi(bool on) {
        left_semi_ = on;
        return *this;
    }

    /**
     * @brief returns whether or not this operation is right semi join.
     * @return true if this is right semi join
     * @return false otherwise
     */
    bool right_semi() const {
        return right_semi_;
    }

    /**
     * @brief sets wether or not this operation is natural join.
     * @param on true to mark as natural join
     * @return this
     */
    JoinStrategy& right_semi(bool on) {
        right_semi_ = on;
        return *this;
    }

    /**
     * @brief returns the join operation of individual columns.
     * @return the join operations
     * @return empty if the corresponded operation is not a valid join
     */
    std::vector<Column>& columns() {
        return columns_;
    }

    /**
     * @brief returns the join operation of individual columns.
     * This is only available for JoinExpressions.
     * @return the join operations
     * @return empty if the corresponded operation is not a valid join
     */
    std::vector<Column> const& columns() const {
        return columns_;
    }

    /**
     * @brief returns the join equality terms.
     * @return equality terms
     */
    std::set<std::pair<std::shared_ptr<VariableBinding>, std::shared_ptr<VariableBinding>>>& equalities() {
        return equalities_;
    }

    /**
     * @brief returns the join equality terms.
     * @return equality terms
     */
    std::set<std::pair<std::shared_ptr<VariableBinding>, std::shared_ptr<VariableBinding>>> const& equalities() const {
        return equalities_;
    }

    /**
     * @brief returns the list of column data for the seek operation.
     * This list represents just a prefix of scan operation on the *right* source.
     * @return the column data for seek operation
     * @return empty if this strategy does not perform seek operation
     */
    std::vector<ColumnData>& seek_columns() {
        return seek_columns_;
    }

    /**
     * @brief returns the list of column data for the seek operation.
     * This list represents just a prefix of scan operation on the *right* source,
     * and may appear varibles from *left* source.
     * @return the column data for seek operation
     * @return empty if this strategy does not perform any seek operations
     */
    std::vector<ColumnData> const& seek_columns() const {
        return seek_columns_;
    }

    /**
     * @brief returns whether or not this object is valid.
     * @return true if this is valid
     * @return false otherwise
     */
    bool is_valid() const {
        return !columns_.empty();
    }

    /**
     * @brief returns whether or not this object is valid.
     * @return true if this is valid
     * @return false otherwise
     */
    explicit operator bool() const {
        return is_valid();
    }

    /**
     * @brief returns where the given variable appears in join columns from the left source.
     * @param variable the variable
     * @return the index on left source
     * @return empty if the variable is not in left source
     */
    std::optional<std::size_t> left_index_of(VariableBinding const& variable) {
        std::size_t index = 0;
        for (auto&& column : columns_) {
            if (column.left_source().get() == &variable) {
                return { index };
            }
            ++index;
        }
        return {};
    }

    /**
     * @brief returns where the given variable appears in join columns from the right source.
     * @param variable the variable
     * @return the index on right source
     * @return empty if the variable is not in right source
     */
    std::optional<std::size_t> right_index_of(VariableBinding const& variable) {
        std::size_t index = 0;
        for (auto&& column : columns_) {
            if (column.right_source().get() == &variable) {
                return { index };
            }
            ++index;
        }
        return {};
    }

    /**
     * @brief swap left/right.
     * @return this
     */
    JoinStrategy& commute() {
        std::swap(left_outer_, right_outer_);
        std::swap(left_semi_, right_semi_);
        for (auto&& column : columns_) {
            column.commute();
        }
        decltype(equalities_) equalities {};
        for (auto&& term : equalities_) {
            equalities.emplace(term.second, term.first);
        }
        equalities_ = std::move(equalities);
        return *this;
    }

private:
    Kind kind_;
    bool left_outer_;
    bool right_outer_;
    bool left_semi_;
    bool right_semi_;
    std::vector<Column> columns_ {};
    std::set<std::pair<std::shared_ptr<VariableBinding>, std::shared_ptr<VariableBinding>>> equalities_ {};
    std::vector<ColumnData> seek_columns_ {};
};


/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(JoinStrategy::Kind value) {
    using Kind = JoinStrategy::Kind;
    switch (value) {
        case Kind::UNION: return "UNION";
        case Kind::NESTED_LOOP: return "NESTED_LOOP";
    }
    std::abort();
}

/**
 * @brief append textual representation of the given value.
 * @param out the target output stream
 * @param value the target value
 * @return the output stream
 */
inline std::ostream& operator<<(std::ostream& out, JoinStrategy::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::analyzer::binding

#endif  //SHAKUJO_ANALYZER_JOIN_STRATEGY_H_
