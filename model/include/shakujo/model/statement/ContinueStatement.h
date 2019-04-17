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
#ifndef SHAKUJO_MODEL_STATEMENT_CONTINUE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_CONTINUE_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents continue statement.
 */
class ContinueStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ContinueStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ContinueStatement();

    /**
     * @brief Destroys this object.
     */
    ~ContinueStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ContinueStatement(ContinueStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ContinueStatement& operator=(ContinueStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ContinueStatement(ContinueStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ContinueStatement& operator=(ContinueStatement&& other) noexcept;

public:
    /**
     * @brief Returns target anchor.
     * @return target anchor.
     */
    name::Name* anchor();

    /**
     * @brief Returns target anchor.
     * @return target anchor.
     */
    inline name::Name const* anchor() const {
        return const_cast<ContinueStatement*>(this)->anchor();
    }

    /**
     * @brief Sets target anchor.
     * @param anchor target anchor
     * @return this
     */
    ContinueStatement& anchor(std::unique_ptr<name::Name> anchor);

    /**
     * @brief Releases target anchor from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_anchor();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ContinueStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ContinueStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::CONTINUE_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_CONTINUE_STATEMENT_H_
