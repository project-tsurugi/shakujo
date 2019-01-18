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
#ifndef SHAKUJO_MODEL_EXPRESSION_VECTOR_CREATION_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_VECTOR_CREATION_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression {
/**
 * @brief Represents creating vectors.
 */
class VectorCreationExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VectorCreationExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    VectorCreationExpression();

    /**
     * @brief Destroys this object.
     */
    ~VectorCreationExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VectorCreationExpression(VectorCreationExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorCreationExpression& operator=(VectorCreationExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VectorCreationExpression(VectorCreationExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorCreationExpression& operator=(VectorCreationExpression&& other) noexcept;

public:
    /**
     * @brief Returns vector type.
     * @return vector type.
     */
    type::Type* type();

    /**
     * @brief Returns vector type.
     * @return vector type.
     */
    inline type::Type const* type() const {
        return const_cast<VectorCreationExpression*>(this)->type();
    }

    /**
     * @brief Sets vector type.
     * @param type vector type
     * @return this
     */
    VectorCreationExpression& type(std::unique_ptr<type::Type> type);

    /**
     * @brief Releases vector type from this node.
     * @return the released node
     */
    std::unique_ptr<type::Type> release_type();

    /**
     * @brief Returns initial vector elements.
     * @return initial vector elements.
     */
    util::ManagedNodeList<Expression>& values();

    /**
     * @brief Returns initial vector elements.
     * @return initial vector elements.
     */
    inline util::ManagedNodeList<Expression> const& values() const {
        return const_cast<VectorCreationExpression*>(this)->values();
    }
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
        return const_cast<VectorCreationExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    VectorCreationExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorCreationExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorCreationExpression* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::VECTOR_CREATION_EXPRESSION;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_VECTOR_CREATION_EXPRESSION_H_
