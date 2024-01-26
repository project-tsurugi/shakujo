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
#ifndef SHAKUJO_MODEL_EXPRESSION_RELATION_SELECTION_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_RELATION_SELECTION_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::model::expression::relation {
/**
 * @brief Represents filtering records in relations.
 */
class SelectionExpression
        : public Expression
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit SelectionExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    SelectionExpression();

    /**
     * @brief Destroys this object.
     */
    ~SelectionExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    SelectionExpression(SelectionExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    SelectionExpression& operator=(SelectionExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    SelectionExpression(SelectionExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    SelectionExpression& operator=(SelectionExpression&& other) noexcept;

public:
    /**
     * @brief Returns source relation.
     * @return source relation.
     */
    Expression* operand();

    /**
     * @brief Returns source relation.
     * @return source relation.
     */
    inline Expression const* operand() const {
        return const_cast<SelectionExpression*>(this)->operand();
    }

    /**
     * @brief Sets source relation.
     * @param operand source relation
     * @return this
     */
    SelectionExpression& operand(std::unique_ptr<Expression> operand);

    /**
     * @brief Releases source relation from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_operand();

    /**
     * @brief Returns record filter predicate expression.
     * @return record filter predicate expression.
     */
    Expression* condition();

    /**
     * @brief Returns record filter predicate expression.
     * @return record filter predicate expression.
     */
    inline Expression const* condition() const {
        return const_cast<SelectionExpression*>(this)->condition();
    }

    /**
     * @brief Sets record filter predicate expression.
     * @param condition record filter predicate expression
     * @return this
     */
    SelectionExpression& condition(std::unique_ptr<Expression> condition);

    /**
     * @brief Releases record filter predicate expression from this node.
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
        return const_cast<SelectionExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    SelectionExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

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
        return const_cast<SelectionExpression*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    SelectionExpression& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    SelectionExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    SelectionExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::SELECTION_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression::relation

#endif  // SHAKUJO_MODEL_EXPRESSION_RELATION_SELECTION_EXPRESSION_H_
