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
#ifndef SHAKUJO_MODEL_STATEMENT_RAISE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_RAISE_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents rasing exceptions.
 */
class RaiseStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit RaiseStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    RaiseStatement();

    /**
     * @brief Destroys this object.
     */
    ~RaiseStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    RaiseStatement(RaiseStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    RaiseStatement& operator=(RaiseStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    RaiseStatement(RaiseStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    RaiseStatement& operator=(RaiseStatement&& other) noexcept;

public:
    /**
     * @brief Returns exception.
     * @return exception.
     */
    expression::Expression* value();

    /**
     * @brief Returns exception.
     * @return exception.
     */
    inline expression::Expression const* value() const {
        return const_cast<RaiseStatement*>(this)->value();
    }

    /**
     * @brief Sets exception.
     * @param value exception
     * @return this
     */
    RaiseStatement& value(std::unique_ptr<expression::Expression> value);

    /**
     * @brief Releases exception from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_value();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    RaiseStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    RaiseStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::RAISE_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_RAISE_STATEMENT_H_
