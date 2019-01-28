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
#ifndef SHAKUJO_MODEL_EXPRESSION_RELATION_SCAN_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_RELATION_SCAN_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"

namespace shakujo::model::expression::relation {
/**
 * @brief Represents retrieves relation from tables.
 */
class ScanExpression
        : public Expression
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ScanExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ScanExpression();

    /**
     * @brief Destroys this object.
     */
    ~ScanExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ScanExpression(ScanExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ScanExpression& operator=(ScanExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ScanExpression(ScanExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ScanExpression& operator=(ScanExpression&& other) noexcept;

public:
    /**
     * @brief Returns table name.
     * @return table name.
     */
    name::Name* table();

    /**
     * @brief Returns table name.
     * @return table name.
     */
    inline name::Name const* table() const {
        return const_cast<ScanExpression*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    ScanExpression& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns alias name.
     * @return alias name.
     */
    name::SimpleName* alias();

    /**
     * @brief Returns alias name.
     * @return alias name.
     */
    inline name::SimpleName const* alias() const {
        return const_cast<ScanExpression*>(this)->alias();
    }

    /**
     * @brief Sets alias name.
     * @param alias alias name
     * @return this
     */
    ScanExpression& alias(std::unique_ptr<name::SimpleName> alias);

    /**
     * @brief Releases alias name from this node.
     * @return the released node
     */
    std::unique_ptr<name::SimpleName> release_alias();

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
        return const_cast<ScanExpression*>(this)->condition();
    }

    /**
     * @brief Sets record filter predicate expression.
     * @param condition record filter predicate expression
     * @return this
     */
    ScanExpression& condition(std::unique_ptr<Expression> condition);

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
        return const_cast<ScanExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    ScanExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

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
        return const_cast<ScanExpression*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    ScanExpression& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ScanExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ScanExpression* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::SCAN_EXPRESSION;
    }

};
}  // namespace shakujo::model::expression::relation

#endif  // SHAKUJO_MODEL_EXPRESSION_RELATION_SCAN_EXPRESSION_H_
