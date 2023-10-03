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
#ifndef SHAKUJO_MODEL_STATEMENT_BLOCK_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_BLOCK_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::statement {
/**
 * @brief Represents statement block.
 */
class BlockStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit BlockStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    BlockStatement();

    /**
     * @brief Destroys this object.
     */
    ~BlockStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    BlockStatement(BlockStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    BlockStatement& operator=(BlockStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    BlockStatement(BlockStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    BlockStatement& operator=(BlockStatement&& other) noexcept;

public:
    /**
     * @brief Returns body statements.
     * @return body statements.
     */
    util::NodeList<Statement>& elements();

    /**
     * @brief Returns body statements.
     * @return body statements.
     */
    inline util::NodeList<Statement> const& elements() const {
        return const_cast<BlockStatement*>(this)->elements();
    }
    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BlockStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BlockStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::BLOCK_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_BLOCK_STATEMENT_H_
