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
#ifndef SHAKUJO_MODEL_EXPRESSION_RELATION_ORDER_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_RELATION_ORDER_EXPRESSION_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Index.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::expression::relation {
/**
 * @brief Represents sorting records in relations.
 */
class OrderExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit OrderExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents sort direction.
     */
    enum class Direction {
        /**
         * @brief ascendant order.
         */
        ASCENDANT,

        /**
         * @brief descendant order.
         */
        DESCENDANT,

    };

    /**
     * @brief Represents sort order.
     */
    class Element final
            : public key::VariableKey::Provider {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Element(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Constructs a new empty object.
         */
        Element();

        /**
         * @brief Destroys this object.
         */
        ~Element() noexcept override;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Element(Element const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Element& operator=(Element const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Element(Element&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Element& operator=(Element&& other) noexcept;

    public:
        /**
         * @brief Returns column index.
         * @return column index.
         */
        name::Index* column();

        /**
         * @brief Returns column index.
         * @return column index.
         */
        inline name::Index const* column() const {
            return const_cast<OrderExpression::Element*>(this)->column();
        }

        /**
         * @brief Sets column index.
         * @param column column index
         * @return this
         */
        OrderExpression::Element& column(std::unique_ptr<name::Index> column);

        /**
         * @brief Releases column index from this node.
         * @return the released node
         */
        std::unique_ptr<name::Index> release_column();

        /**
         * @brief Returns sort direction.
         * @return sort direction.
         */
        OrderExpression::Direction direction() const;

        /**
         * @brief Sets sort direction.
         * @param direction sort direction
         * @return this
         */
        OrderExpression::Element& direction(OrderExpression::Direction direction);

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
            return const_cast<OrderExpression::Element*>(this)->variable_key();
        }

        /**
         * @brief Sets referring variable key.
         * @param variable_key referring variable key
         * @return this
         */
        OrderExpression::Element& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Element* clone() const & override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Element* clone() && override;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    OrderExpression();

    /**
     * @brief Destroys this object.
     */
    ~OrderExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    OrderExpression(OrderExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    OrderExpression& operator=(OrderExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    OrderExpression(OrderExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    OrderExpression& operator=(OrderExpression&& other) noexcept;

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
        return const_cast<OrderExpression*>(this)->operand();
    }

    /**
     * @brief Sets source relation.
     * @param operand source relation
     * @return this
     */
    OrderExpression& operand(std::unique_ptr<Expression> operand);

    /**
     * @brief Releases source relation from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_operand();

    /**
     * @brief Returns sort orders specification.
     * @return sort orders specification.
     */
    util::FragmentList<OrderExpression::Element>& elements();

    /**
     * @brief Returns sort orders specification.
     * @return sort orders specification.
     */
    inline util::FragmentList<OrderExpression::Element> const& elements() const {
        return const_cast<OrderExpression*>(this)->elements();
    }
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
        return const_cast<OrderExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    OrderExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    OrderExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    OrderExpression* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::ORDER_EXPRESSION;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see OrderExpression::Direction
 */
inline constexpr std::string_view to_string_view(OrderExpression::Direction value) {
    switch (value) {
        case OrderExpression::Direction::ASCENDANT: return "ASCENDANT";
        case OrderExpression::Direction::DESCENDANT: return "DESCENDANT";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see OrderExpression::Direction
 */
inline std::ostream& operator<<(std::ostream& out, OrderExpression::Direction value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression::relation

#endif  // SHAKUJO_MODEL_EXPRESSION_RELATION_ORDER_EXPRESSION_H_
