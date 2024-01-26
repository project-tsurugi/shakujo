/*
 * Copyright 2018-2024 Project Tsurugi.
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
#ifndef SHAKUJO_MODEL_STATEMENT_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_STATEMENT_H_

#include "../Node.h"
#include "StatementKind.h"

namespace shakujo::model::statement {

/**
 * @brief An abstract super interface of statements in IR model.
 * @see StatementKind
 * @see StatementVisitor
 * @see ConstStatementVisitor
 */
class Statement : public Node {
protected:
    /**
     * @brief Constructs a new object.
     */
    Statement() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Statement(const Statement& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Statement(Statement&& other) noexcept = default;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Statement& operator=(const Statement& other) = default;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Statement& operator=(Statement&& other) noexcept = default;

public:
    ~Statement() noexcept override = default;

    Statement* clone() const & override = 0;

    Statement* clone() && override = 0;

    /**
     * @brief Returns the kind of this statement.
     * @return the statement kind
     */
    virtual StatementKind kind() const = 0;
};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_STATEMENT_H_
