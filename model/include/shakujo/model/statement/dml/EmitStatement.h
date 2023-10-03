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
#ifndef SHAKUJO_MODEL_STATEMENT_DML_EMIT_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DML_EMIT_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement::dml {
/**
 * @brief Represents emit relation statement.
 */
class EmitStatement
        : public Statement
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit EmitStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    EmitStatement();

    /**
     * @brief Destroys this object.
     */
    ~EmitStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    EmitStatement(EmitStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    EmitStatement& operator=(EmitStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    EmitStatement(EmitStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    EmitStatement& operator=(EmitStatement&& other) noexcept;

public:
    /**
     * @brief Returns target relation.
     * @return target relation.
     */
    expression::Expression* source();

    /**
     * @brief Returns target relation.
     * @return target relation.
     */
    inline expression::Expression const* source() const {
        return const_cast<EmitStatement*>(this)->source();
    }

    /**
     * @brief Sets target relation.
     * @param source target relation
     * @return this
     */
    EmitStatement& source(std::unique_ptr<expression::Expression> source);

    /**
     * @brief Releases target relation from this node.
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
        return const_cast<EmitStatement*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    EmitStatement& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    EmitStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    EmitStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::EMIT_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement::dml

#endif  // SHAKUJO_MODEL_STATEMENT_DML_EMIT_STATEMENT_H_
