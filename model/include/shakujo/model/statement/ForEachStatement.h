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
#ifndef SHAKUJO_MODEL_STATEMENT_FOR_EACH_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_FOR_EACH_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::statement {
/**
 * @brief Represents iterating elements over sequences.
 */
class ForEachStatement
        : public Statement
        , public key::VariableKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ForEachStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ForEachStatement();

    /**
     * @brief Destroys this object.
     */
    ~ForEachStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ForEachStatement(ForEachStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ForEachStatement& operator=(ForEachStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ForEachStatement(ForEachStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ForEachStatement& operator=(ForEachStatement&& other) noexcept;

public:
    /**
     * @brief Returns variable name.
     * @return variable name.
     */
    name::Name* name();

    /**
     * @brief Returns variable name.
     * @return variable name.
     */
    inline name::Name const* name() const {
        return const_cast<ForEachStatement*>(this)->name();
    }

    /**
     * @brief Sets variable name.
     * @param name variable name
     * @return this
     */
    ForEachStatement& name(std::unique_ptr<name::Name> name);

    /**
     * @brief Releases variable name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_name();

    /**
     * @brief Returns variable type.
     * @return variable type.
     */
    type::Type* type();

    /**
     * @brief Returns variable type.
     * @return variable type.
     */
    inline type::Type const* type() const {
        return const_cast<ForEachStatement*>(this)->type();
    }

    /**
     * @brief Sets variable type.
     * @param type variable type
     * @return this
     */
    ForEachStatement& type(std::unique_ptr<type::Type> type);

    /**
     * @brief Releases variable type from this node.
     * @return the released node
     */
    std::unique_ptr<type::Type> release_type();

    /**
     * @brief Returns target sequence.
     * @return target sequence.
     */
    expression::Expression* enumeration();

    /**
     * @brief Returns target sequence.
     * @return target sequence.
     */
    inline expression::Expression const* enumeration() const {
        return const_cast<ForEachStatement*>(this)->enumeration();
    }

    /**
     * @brief Sets target sequence.
     * @param enumeration target sequence
     * @return this
     */
    ForEachStatement& enumeration(std::unique_ptr<expression::Expression> enumeration);

    /**
     * @brief Releases target sequence from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_enumeration();

    /**
     * @brief Returns body statement.
     * @return body statement.
     */
    Statement* body();

    /**
     * @brief Returns body statement.
     * @return body statement.
     */
    inline Statement const* body() const {
        return const_cast<ForEachStatement*>(this)->body();
    }

    /**
     * @brief Sets body statement.
     * @param body body statement
     * @return this
     */
    ForEachStatement& body(std::unique_ptr<Statement> body);

    /**
     * @brief Releases body statement from this node.
     * @return the released node
     */
    std::unique_ptr<Statement> release_body();

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
        return const_cast<ForEachStatement*>(this)->variable_key();
    }

    /**
     * @brief Sets referring variable key.
     * @param variable_key referring variable key
     * @return this
     */
    ForEachStatement& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ForEachStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ForEachStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::FOR_EACH_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_FOR_EACH_STATEMENT_H_
