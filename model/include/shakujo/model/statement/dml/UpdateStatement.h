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
#ifndef SHAKUJO_MODEL_STATEMENT_DML_UPDATE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DML_UPDATE_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::statement::dml {
/**
 * @brief Represents update table rows statement.
 */
class UpdateStatement
        : public Statement
        , public key::VariableKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit UpdateStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents update column specification.
     */
    class Column final {
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
        ~Column() noexcept;

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
        name::Name* name();

        /**
         * @brief Returns destination column name.
         * @return destination column name.
         */
        inline name::Name const* name() const {
            return const_cast<UpdateStatement::Column*>(this)->name();
        }

        /**
         * @brief Sets destination column name.
         * @param name destination column name
         * @return this
         */
        UpdateStatement::Column& name(std::unique_ptr<name::Name> name);

        /**
         * @brief Releases destination column name from this node.
         * @return the released node
         */
        std::unique_ptr<name::Name> release_name();

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
            return const_cast<UpdateStatement::Column*>(this)->value();
        }

        /**
         * @brief Sets column value.
         * @param value column value
         * @return this
         */
        UpdateStatement::Column& value(std::unique_ptr<expression::Expression> value);

        /**
         * @brief Releases column value from this node.
         * @return the released node
         */
        std::unique_ptr<expression::Expression> release_value();

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() const &;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() &&;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    UpdateStatement();

    /**
     * @brief Destroys this object.
     */
    ~UpdateStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    UpdateStatement(UpdateStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    UpdateStatement& operator=(UpdateStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    UpdateStatement(UpdateStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    UpdateStatement& operator=(UpdateStatement&& other) noexcept;

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
        return const_cast<UpdateStatement*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    UpdateStatement& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns destination column specification.
     * @return destination column specification.
     */
    util::FragmentList<UpdateStatement::Column>& columns();

    /**
     * @brief Returns destination column specification.
     * @return destination column specification.
     */
    inline util::FragmentList<UpdateStatement::Column> const& columns() const {
        return const_cast<UpdateStatement*>(this)->columns();
    }
    /**
     * @brief Returns record filter predicate expression.
     * @return record filter predicate expression.
     */
    expression::Expression* condition();

    /**
     * @brief Returns record filter predicate expression.
     * @return record filter predicate expression.
     */
    inline expression::Expression const* condition() const {
        return const_cast<UpdateStatement*>(this)->condition();
    }

    /**
     * @brief Sets record filter predicate expression.
     * @param condition record filter predicate expression
     * @return this
     */
    UpdateStatement& condition(std::unique_ptr<expression::Expression> condition);

    /**
     * @brief Releases record filter predicate expression from this node.
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
        return const_cast<UpdateStatement*>(this)->variable_key();
    }

    /**
     * @brief Sets referring variable key.
     * @param variable_key referring variable key
     * @return this
     */
    UpdateStatement& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    UpdateStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    UpdateStatement* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    StatementKind kind() const override {
        return StatementKind::UPDATE_STATEMENT;
    }

};
}  // namespace shakujo::model::statement::dml

#endif  // SHAKUJO_MODEL_STATEMENT_DML_UPDATE_STATEMENT_H_
