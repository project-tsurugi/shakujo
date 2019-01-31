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
#ifndef SHAKUJO_MODEL_STATEMENT_DML_INSERT_VALUES_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DML_INSERT_VALUES_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::statement::dml {
/**
 * @brief Represents insert values statement.
 */
class InsertValuesStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit InsertValuesStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents insert column specification.
     */
    class Column final
            : public key::VariableKey::Provider {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Column(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Constructs a new empty object.
         */
        Column();

        /**
         * @brief Destroys this object.
         */
        ~Column() noexcept override;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Column(Column const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Column(Column&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column&& other) noexcept;

    public:
        /**
         * @brief Returns destination column name.
         * @return destination column name.
         */
        name::SimpleName* name();

        /**
         * @brief Returns destination column name.
         * @return destination column name.
         */
        inline name::SimpleName const* name() const {
            return const_cast<InsertValuesStatement::Column*>(this)->name();
        }

        /**
         * @brief Sets destination column name.
         * @param name destination column name
         * @return this
         */
        InsertValuesStatement::Column& name(std::unique_ptr<name::SimpleName> name);

        /**
         * @brief Releases destination column name from this node.
         * @return the released node
         */
        std::unique_ptr<name::SimpleName> release_name();

        /**
         * @brief Returns column value.
         * @return column value.
         */
        expression::Expression* value();

        /**
         * @brief Returns column value.
         * @return column value.
         */
        inline expression::Expression const* value() const {
            return const_cast<InsertValuesStatement::Column*>(this)->value();
        }

        /**
         * @brief Sets column value.
         * @param value column value
         * @return this
         */
        InsertValuesStatement::Column& value(std::unique_ptr<expression::Expression> value);

        /**
         * @brief Releases column value from this node.
         * @return the released node
         */
        std::unique_ptr<expression::Expression> release_value();

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
            return const_cast<InsertValuesStatement::Column*>(this)->variable_key();
        }

        /**
         * @brief Sets referring variable key.
         * @param variable_key referring variable key
         * @return this
         */
        InsertValuesStatement::Column& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() const & override;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() && override;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    InsertValuesStatement();

    /**
     * @brief Destroys this object.
     */
    ~InsertValuesStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    InsertValuesStatement(InsertValuesStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    InsertValuesStatement& operator=(InsertValuesStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    InsertValuesStatement(InsertValuesStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    InsertValuesStatement& operator=(InsertValuesStatement&& other) noexcept;

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
        return const_cast<InsertValuesStatement*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    InsertValuesStatement& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns destination column specifications.
     * @return destination column specifications.
     */
    util::FragmentList<InsertValuesStatement::Column>& columns();

    /**
     * @brief Returns destination column specifications.
     * @return destination column specifications.
     */
    inline util::FragmentList<InsertValuesStatement::Column> const& columns() const {
        return const_cast<InsertValuesStatement*>(this)->columns();
    }
    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    InsertValuesStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    InsertValuesStatement* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    StatementKind kind() const override {
        return StatementKind::INSERT_VALUES_STATEMENT;
    }

};
}  // namespace shakujo::model::statement::dml

#endif  // SHAKUJO_MODEL_STATEMENT_DML_INSERT_VALUES_STATEMENT_H_
