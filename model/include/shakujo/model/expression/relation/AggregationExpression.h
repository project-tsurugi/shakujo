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
#ifndef SHAKUJO_MODEL_EXPRESSION_RELATION_AGGREGATION_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_RELATION_AGGREGATION_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/expression/FunctionCall.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Index.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/util/FragmentList.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::expression::relation {
/**
 * @brief Represents aggregating relations.
 */
class AggregationExpression
        : public Expression
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit AggregationExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents aggregation specification.
     */
    class Column final
            : public key::FunctionKey::Provider
            , public key::VariableKey::Provider {
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
         * @brief Returns aggregation function.
         * @return aggregation function.
         */
        name::Name* function();

        /**
         * @brief Returns aggregation function.
         * @return aggregation function.
         */
        inline name::Name const* function() const {
            return const_cast<AggregationExpression::Column*>(this)->function();
        }

        /**
         * @brief Sets aggregation function.
         * @param function aggregation function
         * @return this
         */
        AggregationExpression::Column& function(std::unique_ptr<name::Name> function);

        /**
         * @brief Releases aggregation function from this node.
         * @return the released node
         */
        std::unique_ptr<name::Name> release_function();

        /**
         * @brief Returns set quantifier.
         * @return set quantifier.
         */
        FunctionCall::Quantifier quantifier() const;

        /**
         * @brief Sets set quantifier.
         * @param quantifier set quantifier
         * @return this
         */
        AggregationExpression::Column& quantifier(FunctionCall::Quantifier quantifier);

        /**
         * @brief Returns aggregation operand.
         * @return aggregation operand.
         */
        Expression* operand();

        /**
         * @brief Returns aggregation operand.
         * @return aggregation operand.
         */
        inline Expression const* operand() const {
            return const_cast<AggregationExpression::Column*>(this)->operand();
        }

        /**
         * @brief Sets aggregation operand.
         * @param operand aggregation operand
         * @return this
         */
        AggregationExpression::Column& operand(std::unique_ptr<Expression> operand);

        /**
         * @brief Releases aggregation operand from this node.
         * @return the released node
         */
        std::unique_ptr<Expression> release_operand();

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
            return const_cast<AggregationExpression::Column*>(this)->alias();
        }

        /**
         * @brief Sets column name.
         * @param alias column name
         * @return this
         */
        AggregationExpression::Column& alias(std::unique_ptr<name::SimpleName> alias);

        /**
         * @brief Releases column name from this node.
         * @return the released node
         */
        std::unique_ptr<name::SimpleName> release_alias();

        /**
         * @brief Returns referring function key.
         * @return referring function key.
         */
        key::FunctionKey* function_key() override;

        /**
         * @brief Returns referring function key.
         * @return referring function key.
         */
        inline key::FunctionKey const* function_key() const override {
            return const_cast<AggregationExpression::Column*>(this)->function_key();
        }

        /**
         * @brief Sets referring function key.
         * @param function_key referring function key
         * @return this
         */
        AggregationExpression::Column& function_key(std::unique_ptr<key::FunctionKey> function_key) override;

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
            return const_cast<AggregationExpression::Column*>(this)->variable_key();
        }

        /**
         * @brief Sets referring variable key.
         * @param variable_key referring variable key
         * @return this
         */
        AggregationExpression::Column& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

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
    AggregationExpression();

    /**
     * @brief Destroys this object.
     */
    ~AggregationExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    AggregationExpression(AggregationExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    AggregationExpression& operator=(AggregationExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    AggregationExpression(AggregationExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    AggregationExpression& operator=(AggregationExpression&& other) noexcept;

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
        return const_cast<AggregationExpression*>(this)->operand();
    }

    /**
     * @brief Sets source relation.
     * @param operand source relation
     * @return this
     */
    AggregationExpression& operand(std::unique_ptr<Expression> operand);

    /**
     * @brief Releases source relation from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_operand();

    /**
     * @brief Returns group keys.
     * @return group keys.
     */
    util::NodeList<name::Index>& keys();

    /**
     * @brief Returns group keys.
     * @return group keys.
     */
    inline util::NodeList<name::Index> const& keys() const {
        return const_cast<AggregationExpression*>(this)->keys();
    }
    /**
     * @brief Returns aggregation columns.
     * @return aggregation columns.
     */
    util::FragmentList<AggregationExpression::Column>& columns();

    /**
     * @brief Returns aggregation columns.
     * @return aggregation columns.
     */
    inline util::FragmentList<AggregationExpression::Column> const& columns() const {
        return const_cast<AggregationExpression*>(this)->columns();
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
        return const_cast<AggregationExpression*>(this)->alias();
    }

    /**
     * @brief Sets alias name.
     * @param alias alias name
     * @return this
     */
    AggregationExpression& alias(std::unique_ptr<name::SimpleName> alias);

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
        return const_cast<AggregationExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    AggregationExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

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
        return const_cast<AggregationExpression*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    AggregationExpression& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    AggregationExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    AggregationExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr ExpressionKind tag = ExpressionKind::AGGREGATION_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression::relation

#endif  // SHAKUJO_MODEL_EXPRESSION_RELATION_AGGREGATION_EXPRESSION_H_
