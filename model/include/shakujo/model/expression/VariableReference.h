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
#ifndef SHAKUJO_MODEL_EXPRESSION_VARIABLE_REFERENCE_H_
#define SHAKUJO_MODEL_EXPRESSION_VARIABLE_REFERENCE_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::expression {
/**
 * @brief Represents variable reference.
 */
class VariableReference
        : public Expression
        , public key::VariableKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VariableReference(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    VariableReference();

    /**
     * @brief Destroys this object.
     */
    ~VariableReference() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VariableReference(VariableReference const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VariableReference& operator=(VariableReference const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VariableReference(VariableReference&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VariableReference& operator=(VariableReference&& other) noexcept;

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
        return const_cast<VariableReference*>(this)->name();
    }

    /**
     * @brief Sets variable name.
     * @param name variable name
     * @return this
     */
    VariableReference& name(std::unique_ptr<name::Name> name);

    /**
     * @brief Releases variable name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_name();

    /**
     * @brief Returns expression key.
     * @return expression key.
     */
    key::ExpressionKey* expression_key() override;

    /**
     * @brief Returns expression key.
     * @return expression key.
     */
    inline key::ExpressionKey const* expression_key() const override {
        return const_cast<VariableReference*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    VariableReference& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

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
        return const_cast<VariableReference*>(this)->variable_key();
    }

    /**
     * @brief Sets referring variable key.
     * @param variable_key referring variable key
     * @return this
     */
    VariableReference& variable_key(std::unique_ptr<key::VariableKey> variable_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VariableReference* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VariableReference* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::VARIABLE_REFERENCE;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_VARIABLE_REFERENCE_H_
