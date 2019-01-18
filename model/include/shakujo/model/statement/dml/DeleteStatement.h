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
#ifndef SHAKUJO_MODEL_STATEMENT_DML_DELETE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DML_DELETE_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement::dml {
/**
 * @brief Represents delete table rows statement.
 */
class DeleteStatement
        : public Statement
        , public key::VariableKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit DeleteStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    DeleteStatement();

    /**
     * @brief Destroys this object.
     */
    ~DeleteStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    DeleteStatement(DeleteStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    DeleteStatement& operator=(DeleteStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    DeleteStatement(DeleteStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    DeleteStatement& operator=(DeleteStatement&& other) noexcept;

public:
    /**
     * @brief Returns table name.
     * @return table name.
     */
    name::Name* table();

    /**
     * @brief Returns table name.
     * @return table name.
     */
    inline name::Name const* table() const {
        return const_cast<DeleteStatement*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    DeleteStatement& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns row filter predicate expression.
     * @return row filter predicate expression.
     */
    expression::Expression* condition();

    /**
     * @brief Returns row filter predicate expression.
     * @return row filter predicate expression.
     */
    inline expression::Expression const* condition() const {
        return const_cast<DeleteStatement*>(this)->condition();
    }

    /**
     * @brief Sets row filter predicate expression.
     * @param condition row filter predicate expression
     * @return this
     */
    DeleteStatement& condition(std::unique_ptr<expression::Expression> condition);

    /**
     * @brief Releases row filter predicate expression from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_condition();

    /**
     * @brief Returns referring variable key.
     * @return referring variable key.
     */
    key::VariableKey* variable_key() override;

    /**
     * @brief Returns referring variable key.
     * @return referring variable key.
     */
    inline key::VariableKey const* variable_key() const override {
        return const_cast<DeleteStatement*>(this)->variable_key();
    }

    /**
     * @brief Sets referring variable key.
     * @param variable_key referring variable key
     * @return this
     */
    DeleteStatement& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DeleteStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DeleteStatement* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    StatementKind kind() const override {
        return StatementKind::DELETE_STATEMENT;
    }

};
}  // namespace shakujo::model::statement::dml

#endif  // SHAKUJO_MODEL_STATEMENT_DML_DELETE_STATEMENT_H_
