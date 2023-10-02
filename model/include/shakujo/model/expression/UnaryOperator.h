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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_EXPRESSION_UNARY_OPERATOR_H_
#define SHAKUJO_MODEL_EXPRESSION_UNARY_OPERATOR_H_

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
 * @brief Represents unary operator expression.
 */
class UnaryOperator
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit UnaryOperator(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents unary operator kind.
     */
    enum class Kind {
        /**
         * @brief unary plus.
         */
        PLUS,

        /**
         * @brief inverts sign of numeric values.
         */
        SIGN_INVERSION,

        /**
         * @brief bit-wise complement.
         */
        BITWISE_COMPLEMENT,

        /**
         * @brief logical negate.
         */
        LOGICAL_NOT,

        /**
         * @brief conditional negate.
         */
        CONDITIONAL_NOT,

        /**
         * @brief test whether is null.
         */
        IS_NULL,

        /**
         * @brief test whether is NOT null.
         */
        IS_NOT_NULL,

        /**
         * @brief test whether is true.
         */
        IS_TRUE,

        /**
         * @brief test whether is not true.
         */
        IS_NOT_TRUE,

        /**
         * @brief test whether is false.
         */
        IS_FALSE,

        /**
         * @brief test whether is not false.
         */
        IS_NOT_FALSE,

        /**
         * @brief raise exception if null.
         */
        CHECK_NULL,

        /**
         * @brief test whether the all elements in the relation is just true.
         */
        RELATION_ALL,

        /**
         * @brief test whether the any elements in the relation contains true.
         */
        RELATION_ANY,

        /**
         * @brief test whether the relation is empty.
         */
        RELATION_EXISTS,

        /**
         * @brief test whether the relation is NOT empty.
         */
        RELATION_NOT_EXISTS,

        /**
         * @brief invalid operator.
         */
        INVALID,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    UnaryOperator();

    /**
     * @brief Destroys this object.
     */
    ~UnaryOperator() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    UnaryOperator(UnaryOperator const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    UnaryOperator& operator=(UnaryOperator const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    UnaryOperator(UnaryOperator&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    UnaryOperator& operator=(UnaryOperator&& other) noexcept;

public:
    /**
     * @brief Returns unary operator kind.
     * @return unary operator kind.
     */
    UnaryOperator::Kind operator_kind() const;

    /**
     * @brief Sets unary operator kind.
     * @param operator_kind unary operator kind
     * @return this
     */
    UnaryOperator& operator_kind(UnaryOperator::Kind operator_kind);

    /**
     * @brief Returns operand.
     * @return operand.
     */
    Expression* operand();

    /**
     * @brief Returns operand.
     * @return operand.
     */
    inline Expression const* operand() const {
        return const_cast<UnaryOperator*>(this)->operand();
    }

    /**
     * @brief Sets operand.
     * @param operand operand
     * @return this
     */
    UnaryOperator& operand(std::unique_ptr<Expression> operand);

    /**
     * @brief Releases operand from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_operand();

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
        return const_cast<UnaryOperator*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    UnaryOperator& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    UnaryOperator* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    UnaryOperator* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::UNARY_OPERATOR;

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
 * @see UnaryOperator::Kind
 */
inline constexpr std::string_view to_string_view(UnaryOperator::Kind value) {
    switch (value) {
        case UnaryOperator::Kind::PLUS: return "PLUS";
        case UnaryOperator::Kind::SIGN_INVERSION: return "SIGN_INVERSION";
        case UnaryOperator::Kind::BITWISE_COMPLEMENT: return "BITWISE_COMPLEMENT";
        case UnaryOperator::Kind::LOGICAL_NOT: return "LOGICAL_NOT";
        case UnaryOperator::Kind::CONDITIONAL_NOT: return "CONDITIONAL_NOT";
        case UnaryOperator::Kind::IS_NULL: return "IS_NULL";
        case UnaryOperator::Kind::IS_NOT_NULL: return "IS_NOT_NULL";
        case UnaryOperator::Kind::IS_TRUE: return "IS_TRUE";
        case UnaryOperator::Kind::IS_NOT_TRUE: return "IS_NOT_TRUE";
        case UnaryOperator::Kind::IS_FALSE: return "IS_FALSE";
        case UnaryOperator::Kind::IS_NOT_FALSE: return "IS_NOT_FALSE";
        case UnaryOperator::Kind::CHECK_NULL: return "CHECK_NULL";
        case UnaryOperator::Kind::RELATION_ALL: return "RELATION_ALL";
        case UnaryOperator::Kind::RELATION_ANY: return "RELATION_ANY";
        case UnaryOperator::Kind::RELATION_EXISTS: return "RELATION_EXISTS";
        case UnaryOperator::Kind::RELATION_NOT_EXISTS: return "RELATION_NOT_EXISTS";
        case UnaryOperator::Kind::INVALID: return "INVALID";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see UnaryOperator::Kind
 */
inline std::ostream& operator<<(std::ostream& out, UnaryOperator::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_UNARY_OPERATOR_H_
