/*
 * Copyright 2018-2023 tsurugi project..
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
#ifndef SHAKUJO_MODEL_EXPRESSION_VECTOR_ELEMENT_STORE_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_VECTOR_ELEMENT_STORE_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents storing element into vector.
 */
class VectorElementStoreExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VectorElementStoreExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    VectorElementStoreExpression();

    /**
     * @brief Destroys this object.
     */
    ~VectorElementStoreExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VectorElementStoreExpression(VectorElementStoreExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementStoreExpression& operator=(VectorElementStoreExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VectorElementStoreExpression(VectorElementStoreExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementStoreExpression& operator=(VectorElementStoreExpression&& other) noexcept;

public:
    /**
     * @brief Returns target vector.
     * @return target vector.
     */
    Expression* data();

    /**
     * @brief Returns target vector.
     * @return target vector.
     */
    inline Expression const* data() const {
        return const_cast<VectorElementStoreExpression*>(this)->data();
    }

    /**
     * @brief Sets target vector.
     * @param data target vector
     * @return this
     */
    VectorElementStoreExpression& data(std::unique_ptr<Expression> data);

    /**
     * @brief Releases target vector from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_data();

    /**
     * @brief Returns element index.
     * @return element index.
     */
    Expression* index();

    /**
     * @brief Returns element index.
     * @return element index.
     */
    inline Expression const* index() const {
        return const_cast<VectorElementStoreExpression*>(this)->index();
    }

    /**
     * @brief Sets element index.
     * @param index element index
     * @return this
     */
    VectorElementStoreExpression& index(std::unique_ptr<Expression> index);

    /**
     * @brief Releases element index from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_index();

    /**
     * @brief Returns storing value.
     * @return storing value.
     */
    Expression* value();

    /**
     * @brief Returns storing value.
     * @return storing value.
     */
    inline Expression const* value() const {
        return const_cast<VectorElementStoreExpression*>(this)->value();
    }

    /**
     * @brief Sets storing value.
     * @param value storing value
     * @return this
     */
    VectorElementStoreExpression& value(std::unique_ptr<Expression> value);

    /**
     * @brief Releases storing value from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_value();

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
        return const_cast<VectorElementStoreExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    VectorElementStoreExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementStoreExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementStoreExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::VECTOR_ELEMENT_STORE_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_VECTOR_ELEMENT_STORE_EXPRESSION_H_
