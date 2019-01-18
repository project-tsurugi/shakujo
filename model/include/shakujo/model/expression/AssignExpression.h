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
#ifndef SHAKUJO_MODEL_EXPRESSION_ASSIGN_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_ASSIGN_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::expression {
/**
 * @brief Represents storing value into variables.
 */
class AssignExpression
        : public Expression
        , public key::VariableKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit AssignExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    AssignExpression();

    /**
     * @brief Destroys this object.
     */
    ~AssignExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    AssignExpression(AssignExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    AssignExpression& operator=(AssignExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    AssignExpression(AssignExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    AssignExpression& operator=(AssignExpression&& other) noexcept;

public:
    /**
     * @brief Returns target variable name.
     * @return target variable name.
     */
    name::Name* name();

    /**
     * @brief Returns target variable name.
     * @return target variable name.
     */
    inline name::Name const* name() const {
        return const_cast<AssignExpression*>(this)->name();
    }

    /**
     * @brief Sets target variable name.
     * @param name target variable name
     * @return this
     */
    AssignExpression& name(std::unique_ptr<name::Name> name);

    /**
     * @brief Releases target variable name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_name();

    /**
     * @brief Returns assignment value.
     * @return assignment value.
     */
    Expression* value();

    /**
     * @brief Returns assignment value.
     * @return assignment value.
     */
    inline Expression const* value() const {
        return const_cast<AssignExpression*>(this)->value();
    }

    /**
     * @brief Sets assignment value.
     * @param value assignment value
     * @return this
     */
    AssignExpression& value(std::unique_ptr<Expression> value);

    /**
     * @brief Releases assignment value from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_value();

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
        return const_cast<AssignExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    AssignExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

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
        return const_cast<AssignExpression*>(this)->variable_key();
    }

    /**
     * @brief Sets referring variable key.
     * @param variable_key referring variable key
     * @return this
     */
    AssignExpression& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    AssignExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    AssignExpression* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::ASSIGN_EXPRESSION;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_ASSIGN_EXPRESSION_H_
