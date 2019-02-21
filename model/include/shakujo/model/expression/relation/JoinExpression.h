/*
 * Copyright 2018-2019 shakujo project.
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
#ifndef SHAKUJO_MODEL_EXPRESSION_RELATION_JOIN_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_RELATION_JOIN_EXPRESSION_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::model::expression::relation {
/**
 * @brief Represents joining relations.
 */
class JoinExpression
        : public Expression
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit JoinExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents join operation kind.
     */
    enum class Kind {
        /**
         * @brief cross join.
         */
        CROSS,

        /**
         * @brief inner join.
         */
        INNER,

        /**
         * @brief left outer join.
         */
        LEFT_OUTER,

        /**
         * @brief right outer join.
         */
        RIGHT_OUTER,

        /**
         * @brief full outer join.
         */
        FULL_OUTER,

        /**
         * @brief natural inner join.
         */
        NATURAL_INNER,

        /**
         * @brief natural left outer join.
         */
        NATURAL_LEFT_OUTER,

        /**
         * @brief natural right outer join.
         */
        NATURAL_RIGHT_OUTER,

        /**
         * @brief natural full outer join.
         */
        NATURAL_FULL_OUTER,

        /**
         * @brief union join.
         */
        UNION_OUTER,

        /**
         * @brief left semi-join.
         */
        LEFT_SEMI,

        /**
         * @brief right semi-join.
         */
        RIGHT_SEMI,

        /**
         * @brief invalid operator.
         */
        INVALID,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    JoinExpression();

    /**
     * @brief Destroys this object.
     */
    ~JoinExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    JoinExpression(JoinExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    JoinExpression& operator=(JoinExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    JoinExpression(JoinExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    JoinExpression& operator=(JoinExpression&& other) noexcept;

public:
    /**
     * @brief Returns join operator kind.
     * @return join operator kind.
     */
    JoinExpression::Kind operator_kind() const;

    /**
     * @brief Sets join operator kind.
     * @param operator_kind join operator kind
     * @return this
     */
    JoinExpression& operator_kind(JoinExpression::Kind operator_kind);

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
        return const_cast<JoinExpression*>(this)->left();
    }

    /**
     * @brief Sets left term.
     * @param left left term
     * @return this
     */
    JoinExpression& left(std::unique_ptr<Expression> left);

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
        return const_cast<JoinExpression*>(this)->right();
    }

    /**
     * @brief Sets right term.
     * @param right right term
     * @return this
     */
    JoinExpression& right(std::unique_ptr<Expression> right);

    /**
     * @brief Releases right term from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_right();

    /**
     * @brief Returns join condition.
     * @return join condition.
     */
    Expression* condition();

    /**
     * @brief Returns join condition.
     * @return join condition.
     */
    inline Expression const* condition() const {
        return const_cast<JoinExpression*>(this)->condition();
    }

    /**
     * @brief Sets join condition.
     * @param condition join condition
     * @return this
     */
    JoinExpression& condition(std::unique_ptr<Expression> condition);

    /**
     * @brief Releases join condition from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_condition();

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
        return const_cast<JoinExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    JoinExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns relation key.
     * @return relation key.
     */
    key::RelationKey* relation_key() override;

    /**
     * @brief Returns relation key.
     * @return relation key.
     */
    inline key::RelationKey const* relation_key() const override {
        return const_cast<JoinExpression*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    JoinExpression& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    JoinExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    JoinExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr ExpressionKind tag = ExpressionKind::JOIN_EXPRESSION;

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
 * @see JoinExpression::Kind
 */
inline constexpr std::string_view to_string_view(JoinExpression::Kind value) {
    switch (value) {
        case JoinExpression::Kind::CROSS: return "CROSS";
        case JoinExpression::Kind::INNER: return "INNER";
        case JoinExpression::Kind::LEFT_OUTER: return "LEFT_OUTER";
        case JoinExpression::Kind::RIGHT_OUTER: return "RIGHT_OUTER";
        case JoinExpression::Kind::FULL_OUTER: return "FULL_OUTER";
        case JoinExpression::Kind::NATURAL_INNER: return "NATURAL_INNER";
        case JoinExpression::Kind::NATURAL_LEFT_OUTER: return "NATURAL_LEFT_OUTER";
        case JoinExpression::Kind::NATURAL_RIGHT_OUTER: return "NATURAL_RIGHT_OUTER";
        case JoinExpression::Kind::NATURAL_FULL_OUTER: return "NATURAL_FULL_OUTER";
        case JoinExpression::Kind::UNION_OUTER: return "UNION_OUTER";
        case JoinExpression::Kind::LEFT_SEMI: return "LEFT_SEMI";
        case JoinExpression::Kind::RIGHT_SEMI: return "RIGHT_SEMI";
        case JoinExpression::Kind::INVALID: return "INVALID";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see JoinExpression::Kind
 */
inline std::ostream& operator<<(std::ostream& out, JoinExpression::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression::relation

#endif  // SHAKUJO_MODEL_EXPRESSION_RELATION_JOIN_EXPRESSION_H_
