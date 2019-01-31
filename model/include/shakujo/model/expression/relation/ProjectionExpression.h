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
#ifndef SHAKUJO_MODEL_EXPRESSION_RELATION_PROJECTION_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_RELATION_PROJECTION_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/FragmentList.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::expression::relation {
/**
 * @brief Represents transform records in relations.
 */
class ProjectionExpression
        : public Expression
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ProjectionExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents projection specification.
     */
    class Column final
            : public key::VariableKey::Provider {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Column(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Constructs a new empty object.
         */
        Column();

        /**
         * @brief Destroys this object.
         */
        ~Column() noexcept override;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Column(Column const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Column(Column&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column&& other) noexcept;

    public:
        /**
         * @brief Returns column value.
         * @return column value.
         */
        Expression* value();

        /**
         * @brief Returns column value.
         * @return column value.
         */
        inline Expression const* value() const {
            return const_cast<ProjectionExpression::Column*>(this)->value();
        }

        /**
         * @brief Sets column value.
         * @param value column value
         * @return this
         */
        ProjectionExpression::Column& value(std::unique_ptr<Expression> value);

        /**
         * @brief Releases column value from this node.
         * @return the released node
         */
        std::unique_ptr<Expression> release_value();

        /**
         * @brief Returns column name.
         * @return column name.
         */
        name::SimpleName* alias();

        /**
         * @brief Returns column name.
         * @return column name.
         */
        inline name::SimpleName const* alias() const {
            return const_cast<ProjectionExpression::Column*>(this)->alias();
        }

        /**
         * @brief Sets column name.
         * @param alias column name
         * @return this
         */
        ProjectionExpression::Column& alias(std::unique_ptr<name::SimpleName> alias);

        /**
         * @brief Releases column name from this node.
         * @return the released node
         */
        std::unique_ptr<name::SimpleName> release_alias();

        /**
         * @brief Returns referring variable key.
         * @return referring variable key.
         */
        key::VariableKey* variable_key() override;

        /**
         * @brief Returns referring variable key.
         * @return referring variable key.
         */
        inline key::VariableKey const* variable_key() const override {
            return const_cast<ProjectionExpression::Column*>(this)->variable_key();
        }

        /**
         * @brief Sets referring variable key.
         * @param variable_key referring variable key
         * @return this
         */
        ProjectionExpression::Column& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() const & override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() && override;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    ProjectionExpression();

    /**
     * @brief Destroys this object.
     */
    ~ProjectionExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ProjectionExpression(ProjectionExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ProjectionExpression& operator=(ProjectionExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ProjectionExpression(ProjectionExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ProjectionExpression& operator=(ProjectionExpression&& other) noexcept;

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
        return const_cast<ProjectionExpression*>(this)->operand();
    }

    /**
     * @brief Sets source relation.
     * @param operand source relation
     * @return this
     */
    ProjectionExpression& operand(std::unique_ptr<Expression> operand);

    /**
     * @brief Releases source relation from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_operand();

    /**
     * @brief Returns initialization statements.
     * @return initialization statements.
     */
    util::NodeList<statement::Statement>& initialize();

    /**
     * @brief Returns initialization statements.
     * @return initialization statements.
     */
    inline util::NodeList<statement::Statement> const& initialize() const {
        return const_cast<ProjectionExpression*>(this)->initialize();
    }
    /**
     * @brief Returns projection columns.
     * @return projection columns.
     */
    util::FragmentList<ProjectionExpression::Column>& columns();

    /**
     * @brief Returns projection columns.
     * @return projection columns.
     */
    inline util::FragmentList<ProjectionExpression::Column> const& columns() const {
        return const_cast<ProjectionExpression*>(this)->columns();
    }
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
        return const_cast<ProjectionExpression*>(this)->alias();
    }

    /**
     * @brief Sets alias name.
     * @param alias alias name
     * @return this
     */
    ProjectionExpression& alias(std::unique_ptr<name::SimpleName> alias);

    /**
     * @brief Releases alias name from this node.
     * @return the released node
     */
    std::unique_ptr<name::SimpleName> release_alias();

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
        return const_cast<ProjectionExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    ProjectionExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

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
        return const_cast<ProjectionExpression*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    ProjectionExpression& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ProjectionExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ProjectionExpression* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::PROJECTION_EXPRESSION;
    }

};
}  // namespace shakujo::model::expression::relation

#endif  // SHAKUJO_MODEL_EXPRESSION_RELATION_PROJECTION_EXPRESSION_H_
