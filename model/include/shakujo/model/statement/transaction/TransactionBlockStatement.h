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
#ifndef SHAKUJO_MODEL_STATEMENT_TRANSACTION_TRANSACTION_BLOCK_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_TRANSACTION_TRANSACTION_BLOCK_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement::transaction {
/**
 * @brief Represents transaction block.
 */
class TransactionBlockStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit TransactionBlockStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    TransactionBlockStatement();

    /**
     * @brief Destroys this object.
     */
    ~TransactionBlockStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    TransactionBlockStatement(TransactionBlockStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    TransactionBlockStatement& operator=(TransactionBlockStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    TransactionBlockStatement(TransactionBlockStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    TransactionBlockStatement& operator=(TransactionBlockStatement&& other) noexcept;

public:
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
        return const_cast<TransactionBlockStatement*>(this)->body();
    }

    /**
     * @brief Sets body statement.
     * @param body body statement
     * @return this
     */
    TransactionBlockStatement& body(std::unique_ptr<Statement> body);

    /**
     * @brief Releases body statement from this node.
     * @return the released node
     */
    std::unique_ptr<Statement> release_body();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TransactionBlockStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TransactionBlockStatement* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    StatementKind kind() const override {
        return StatementKind::TRANSACTION_BLOCK_STATEMENT;
    }

};
}  // namespace shakujo::model::statement::transaction

#endif  // SHAKUJO_MODEL_STATEMENT_TRANSACTION_TRANSACTION_BLOCK_STATEMENT_H_
