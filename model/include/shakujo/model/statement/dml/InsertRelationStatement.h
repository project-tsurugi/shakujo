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
#ifndef SHAKUJO_MODEL_STATEMENT_DML_INSERT_RELATION_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DML_INSERT_RELATION_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::statement::dml {
/**
 * @brief Represents insert relation statement.
 */
class InsertRelationStatement
        : public Statement
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit InsertRelationStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    InsertRelationStatement();

    /**
     * @brief Destroys this object.
     */
    ~InsertRelationStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    InsertRelationStatement(InsertRelationStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    InsertRelationStatement& operator=(InsertRelationStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    InsertRelationStatement(InsertRelationStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    InsertRelationStatement& operator=(InsertRelationStatement&& other) noexcept;

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
        return const_cast<InsertRelationStatement*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    InsertRelationStatement& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns destination column names.
     * @return destination column names.
     */
    util::NodeList<name::SimpleName>& columns();

    /**
     * @brief Returns destination column names.
     * @return destination column names.
     */
    inline util::NodeList<name::SimpleName> const& columns() const {
        return const_cast<InsertRelationStatement*>(this)->columns();
    }
    /**
     * @brief Returns source relation.
     * @return source relation.
     */
    expression::Expression* source();

    /**
     * @brief Returns source relation.
     * @return source relation.
     */
    inline expression::Expression const* source() const {
        return const_cast<InsertRelationStatement*>(this)->source();
    }

    /**
     * @brief Sets source relation.
     * @param source source relation
     * @return this
     */
    InsertRelationStatement& source(std::unique_ptr<expression::Expression> source);

    /**
     * @brief Releases source relation from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_source();

    /**
     * @brief Returns relation key.
     * @return relation key.
     */
    key::RelationKey* relation_key() override;

    /**
     * @brief Returns relation key.
     * @return relation key.
     */
    inline key::RelationKey const* relation_key() const override {
        return const_cast<InsertRelationStatement*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    InsertRelationStatement& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    InsertRelationStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    InsertRelationStatement* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    StatementKind kind() const override {
        return StatementKind::INSERT_RELATION_STATEMENT;
    }

};
}  // namespace shakujo::model::statement::dml

#endif  // SHAKUJO_MODEL_STATEMENT_DML_INSERT_RELATION_STATEMENT_H_
