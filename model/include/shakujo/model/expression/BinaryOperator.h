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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_EXPRESSION_BINARY_OPERATOR_H_
#define SHAKUJO_MODEL_EXPRESSION_BINARY_OPERATOR_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents binary operator expression.
 */
class BinaryOperator
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit BinaryOperator(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents binary operator kind.
     */
    enum class Kind {
        /**
         * @brief addition.
         */
        ADD,

        /**
         * @brief subtraction.
         */
        SUBTRACT,

        /**
         * @brief multiplication.
         */
        MULTIPLY,

        /**
         * @brief division.
         */
        DIVIDE,

        /**
         * @brief remainder.
         */
        REMAINDER,

        /**
         * @brief bit-wise and.
         */
        BITWISE_AND,

        /**
         * @brief bit-wise or.
         */
        BITWISE_OR,

        /**
         * @brief bit-wise exclusive or.
         */
        BITWISE_XOR,

        /**
         * @brief left bit shift.
         */
        SHIFT_LEFT,

        /**
         * @brief signed right bit shift.
         */
        SHIFT_RIGHT_ARITHMETIC,

        /**
         * @brief unsigned right bit shift.
         */
        SHIFT_RIGHT_LOGICAL,

        /**
         * @brief equal to.
         */
        EQUAL,

        /**
         * @brief not equal to.
         */
        NOT_EQUAL,

        /**
         * @brief less than.
         */
        LESS_THAN,

        /**
         * @brief greater than.
         */
        GREATER_THAN,

        /**
         * @brief less than or equal to.
         */
        LESS_THAN_OR_EQUAL,

        /**
         * @brief greater than or equal to.
         */
        GREATER_THAN_OR_EQUAL,

        /**
         * @brief membership.
         */
        IN,

        /**
         * @brief logical and.
         */
        LOGICAL_AND,

        /**
         * @brief logical or.
         */
        LOGICAL_OR,

        /**
         * @brief logical exclusive or.
         */
        LOGICAL_XOR,

        /**
         * @brief conditional and.
         */
        CONDITIONAL_AND,

        /**
         * @brief conditional or.
         */
        CONDITIONAL_OR,

        /**
         * @brief string concatenation.
         */
        CONCATENATION,

        /**
         * @brief like.
         */
        LIKE,

        /**
         * @brief not like.
         */
        NOT_LIKE,

        /**
         * @brief relational membership.
         */
        RELATION_IN,

        /**
         * @brief relational set union.
         */
        RELATION_UNION,

        /**
         * @brief relational set intersection.
         */
        RELATION_INTERSECTION,

        /**
         * @brief relational set difference.
         */
        RELATION_DIFFERENCE,

        /**
         * @brief relational bag union.
         */
        RELATION_UNION_ALL,

        /**
         * @brief relational bag intersection.
         */
        RELATION_INTERSECTION_ALL,

        /**
         * @brief relational bag difference.
         */
        RELATION_DIFFERENCE_ALL,

        /**
         * @brief invalid operator.
         */
        INVALID,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    BinaryOperator();

    /**
     * @brief Destroys this object.
     */
    ~BinaryOperator() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    BinaryOperator(BinaryOperator const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    BinaryOperator& operator=(BinaryOperator const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    BinaryOperator(BinaryOperator&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    BinaryOperator& operator=(BinaryOperator&& other) noexcept;

public:
    /**
     * @brief Returns binary operator kind.
     * @return binary operator kind.
     */
    BinaryOperator::Kind operator_kind() const;

    /**
     * @brief Sets binary operator kind.
     * @param operator_kind binary operator kind
     * @return this
     */
    BinaryOperator& operator_kind(BinaryOperator::Kind operator_kind);

    /**
     * @brief Returns left term.
     * @return left term.
     */
    Expression* left();

    /**
     * @brief Returns left term.
     * @return left term.
     */
    inline Expression const* left() const {
        return const_cast<BinaryOperator*>(this)->left();
    }

    /**
     * @brief Sets left term.
     * @param left left term
     * @return this
     */
    BinaryOperator& left(std::unique_ptr<Expression> left);

    /**
     * @brief Releases left term from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_left();

    /**
     * @brief Returns right term.
     * @return right term.
     */
    Expression* right();

    /**
     * @brief Returns right term.
     * @return right term.
     */
    inline Expression const* right() const {
        return const_cast<BinaryOperator*>(this)->right();
    }

    /**
     * @brief Sets right term.
     * @param right right term
     * @return this
     */
    BinaryOperator& right(std::unique_ptr<Expression> right);

    /**
     * @brief Releases right term from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_right();

    /**
     * @brief Returns expression key.
     * @return expression key.
     */
    key::ExpressionKey* expression_key() override;

    /**
     * @brief Returns expression key.
     * @return expression key.
     */
    inline key::ExpressionKey const* expression_key() const override {
        return const_cast<BinaryOperator*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    BinaryOperator& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BinaryOperator* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BinaryOperator* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::BINARY_OPERATOR;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see BinaryOperator::Kind
 */
inline constexpr std::string_view to_string_view(BinaryOperator::Kind value) {
    switch (value) {
        case BinaryOperator::Kind::ADD: return "ADD";
        case BinaryOperator::Kind::SUBTRACT: return "SUBTRACT";
        case BinaryOperator::Kind::MULTIPLY: return "MULTIPLY";
        case BinaryOperator::Kind::DIVIDE: return "DIVIDE";
        case BinaryOperator::Kind::REMAINDER: return "REMAINDER";
        case BinaryOperator::Kind::BITWISE_AND: return "BITWISE_AND";
        case BinaryOperator::Kind::BITWISE_OR: return "BITWISE_OR";
        case BinaryOperator::Kind::BITWISE_XOR: return "BITWISE_XOR";
        case BinaryOperator::Kind::SHIFT_LEFT: return "SHIFT_LEFT";
        case BinaryOperator::Kind::SHIFT_RIGHT_ARITHMETIC: return "SHIFT_RIGHT_ARITHMETIC";
        case BinaryOperator::Kind::SHIFT_RIGHT_LOGICAL: return "SHIFT_RIGHT_LOGICAL";
        case BinaryOperator::Kind::EQUAL: return "EQUAL";
        case BinaryOperator::Kind::NOT_EQUAL: return "NOT_EQUAL";
        case BinaryOperator::Kind::LESS_THAN: return "LESS_THAN";
        case BinaryOperator::Kind::GREATER_THAN: return "GREATER_THAN";
        case BinaryOperator::Kind::LESS_THAN_OR_EQUAL: return "LESS_THAN_OR_EQUAL";
        case BinaryOperator::Kind::GREATER_THAN_OR_EQUAL: return "GREATER_THAN_OR_EQUAL";
        case BinaryOperator::Kind::IN: return "IN";
        case BinaryOperator::Kind::LOGICAL_AND: return "LOGICAL_AND";
        case BinaryOperator::Kind::LOGICAL_OR: return "LOGICAL_OR";
        case BinaryOperator::Kind::LOGICAL_XOR: return "LOGICAL_XOR";
        case BinaryOperator::Kind::CONDITIONAL_AND: return "CONDITIONAL_AND";
        case BinaryOperator::Kind::CONDITIONAL_OR: return "CONDITIONAL_OR";
        case BinaryOperator::Kind::CONCATENATION: return "CONCATENATION";
        case BinaryOperator::Kind::LIKE: return "LIKE";
        case BinaryOperator::Kind::NOT_LIKE: return "NOT_LIKE";
        case BinaryOperator::Kind::RELATION_IN: return "RELATION_IN";
        case BinaryOperator::Kind::RELATION_UNION: return "RELATION_UNION";
        case BinaryOperator::Kind::RELATION_INTERSECTION: return "RELATION_INTERSECTION";
        case BinaryOperator::Kind::RELATION_DIFFERENCE: return "RELATION_DIFFERENCE";
        case BinaryOperator::Kind::RELATION_UNION_ALL: return "RELATION_UNION_ALL";
        case BinaryOperator::Kind::RELATION_INTERSECTION_ALL: return "RELATION_INTERSECTION_ALL";
        case BinaryOperator::Kind::RELATION_DIFFERENCE_ALL: return "RELATION_DIFFERENCE_ALL";
        case BinaryOperator::Kind::INVALID: return "INVALID";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see BinaryOperator::Kind
 */
inline std::ostream& operator<<(std::ostream& out, BinaryOperator::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_BINARY_OPERATOR_H_
