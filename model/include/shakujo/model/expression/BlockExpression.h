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
#ifndef SHAKUJO_MODEL_EXPRESSION_BLOCK_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_BLOCK_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::expression {
/**
 * @brief Represents expression with initialization statements.
 */
class BlockExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit BlockExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    BlockExpression();

    /**
     * @brief Destroys this object.
     */
    ~BlockExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    BlockExpression(BlockExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    BlockExpression& operator=(BlockExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    BlockExpression(BlockExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    BlockExpression& operator=(BlockExpression&& other) noexcept;

public:
    /**
     * @brief Returns initialization statements.
     * @return initialization statements.
     */
    util::NodeList<statement::Statement>& head();

    /**
     * @brief Returns initialization statements.
     * @return initialization statements.
     */
    inline util::NodeList<statement::Statement> const& head() const {
        return const_cast<BlockExpression*>(this)->head();
    }
    /**
     * @brief Returns body expression.
     * @return body expression.
     */
    Expression* body();

    /**
     * @brief Returns body expression.
     * @return body expression.
     */
    inline Expression const* body() const {
        return const_cast<BlockExpression*>(this)->body();
    }

    /**
     * @brief Sets body expression.
     * @param body body expression
     * @return this
     */
    BlockExpression& body(std::unique_ptr<Expression> body);

    /**
     * @brief Releases body expression from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_body();

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
        return const_cast<BlockExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    BlockExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BlockExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BlockExpression* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::BLOCK_EXPRESSION;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_BLOCK_EXPRESSION_H_
