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
#ifndef SHAKUJO_MODEL_STATEMENT_BREAK_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_BREAK_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents break statement.
 */
class BreakStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit BreakStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    BreakStatement();

    /**
     * @brief Destroys this object.
     */
    ~BreakStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    BreakStatement(BreakStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    BreakStatement& operator=(BreakStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    BreakStatement(BreakStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    BreakStatement& operator=(BreakStatement&& other) noexcept;

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
        return const_cast<BreakStatement*>(this)->anchor();
    }

    /**
     * @brief Sets target anchor.
     * @param anchor target anchor
     * @return this
     */
    BreakStatement& anchor(std::unique_ptr<name::Name> anchor);

    /**
     * @brief Releases target anchor from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_anchor();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BreakStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BreakStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::BREAK_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_BREAK_STATEMENT_H_
