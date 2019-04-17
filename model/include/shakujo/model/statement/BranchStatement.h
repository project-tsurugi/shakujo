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
#ifndef SHAKUJO_MODEL_STATEMENT_BRANCH_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_BRANCH_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents branch statement.
 */
class BranchStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit BranchStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    BranchStatement();

    /**
     * @brief Destroys this object.
     */
    ~BranchStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    BranchStatement(BranchStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    BranchStatement& operator=(BranchStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    BranchStatement(BranchStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    BranchStatement& operator=(BranchStatement&& other) noexcept;

public:
    /**
     * @brief Returns branch condition.
     * @return branch condition.
     */
    expression::Expression* condition();

    /**
     * @brief Returns branch condition.
     * @return branch condition.
     */
    inline expression::Expression const* condition() const {
        return const_cast<BranchStatement*>(this)->condition();
    }

    /**
     * @brief Sets branch condition.
     * @param condition branch condition
     * @return this
     */
    BranchStatement& condition(std::unique_ptr<expression::Expression> condition);

    /**
     * @brief Releases branch condition from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_condition();

    /**
     * @brief Returns true statement.
     * @return true statement.
     */
    Statement* then_statement();

    /**
     * @brief Returns true statement.
     * @return true statement.
     */
    inline Statement const* then_statement() const {
        return const_cast<BranchStatement*>(this)->then_statement();
    }

    /**
     * @brief Sets true statement.
     * @param then_statement true statement
     * @return this
     */
    BranchStatement& then_statement(std::unique_ptr<Statement> then_statement);

    /**
     * @brief Releases true statement from this node.
     * @return the released node
     */
    std::unique_ptr<Statement> release_then_statement();

    /**
     * @brief Returns false statement.
     * @return false statement.
     */
    Statement* else_statement();

    /**
     * @brief Returns false statement.
     * @return false statement.
     */
    inline Statement const* else_statement() const {
        return const_cast<BranchStatement*>(this)->else_statement();
    }

    /**
     * @brief Sets false statement.
     * @param else_statement false statement
     * @return this
     */
    BranchStatement& else_statement(std::unique_ptr<Statement> else_statement);

    /**
     * @brief Releases false statement from this node.
     * @return the released node
     */
    std::unique_ptr<Statement> release_else_statement();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BranchStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BranchStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::BRANCH_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_BRANCH_STATEMENT_H_
