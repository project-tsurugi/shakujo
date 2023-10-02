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
#ifndef SHAKUJO_MODEL_STATEMENT_WHILE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_WHILE_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents while statement.
 */
class WhileStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit WhileStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    WhileStatement();

    /**
     * @brief Destroys this object.
     */
    ~WhileStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    WhileStatement(WhileStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    WhileStatement& operator=(WhileStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    WhileStatement(WhileStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    WhileStatement& operator=(WhileStatement&& other) noexcept;

public:
    /**
     * @brief Returns loop condition.
     * @return loop condition.
     */
    expression::Expression* condition();

    /**
     * @brief Returns loop condition.
     * @return loop condition.
     */
    inline expression::Expression const* condition() const {
        return const_cast<WhileStatement*>(this)->condition();
    }

    /**
     * @brief Sets loop condition.
     * @param condition loop condition
     * @return this
     */
    WhileStatement& condition(std::unique_ptr<expression::Expression> condition);

    /**
     * @brief Releases loop condition from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_condition();

    /**
     * @brief Returns body statement.
     * @return body statement.
     */
    Statement* body();

    /**
     * @brief Returns body statement.
     * @return body statement.
     */
    inline Statement const* body() const {
        return const_cast<WhileStatement*>(this)->body();
    }

    /**
     * @brief Sets body statement.
     * @param body body statement
     * @return this
     */
    WhileStatement& body(std::unique_ptr<Statement> body);

    /**
     * @brief Releases body statement from this node.
     * @return the released node
     */
    std::unique_ptr<Statement> release_body();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    WhileStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    WhileStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::WHILE_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_WHILE_STATEMENT_H_
