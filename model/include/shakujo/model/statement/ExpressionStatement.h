/*
 * Copyright 2018-2023 Project Tsurugi.
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
#ifndef SHAKUJO_MODEL_STATEMENT_EXPRESSION_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_EXPRESSION_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents expression statement.
 */
class ExpressionStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ExpressionStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ExpressionStatement();

    /**
     * @brief Destroys this object.
     */
    ~ExpressionStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ExpressionStatement(ExpressionStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ExpressionStatement& operator=(ExpressionStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ExpressionStatement(ExpressionStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ExpressionStatement& operator=(ExpressionStatement&& other) noexcept;

public:
    /**
     * @brief Returns body expression.
     * @return body expression.
     */
    expression::Expression* body();

    /**
     * @brief Returns body expression.
     * @return body expression.
     */
    inline expression::Expression const* body() const {
        return const_cast<ExpressionStatement*>(this)->body();
    }

    /**
     * @brief Sets body expression.
     * @param body body expression
     * @return this
     */
    ExpressionStatement& body(std::unique_ptr<expression::Expression> body);

    /**
     * @brief Releases body expression from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_body();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ExpressionStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ExpressionStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::EXPRESSION_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_EXPRESSION_STATEMENT_H_
