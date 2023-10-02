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
#ifndef SHAKUJO_MODEL_EXPRESSION_ARRAY_ELEMENT_STORE_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_ARRAY_ELEMENT_STORE_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents storing element to arrays.
 */
class ArrayElementStoreExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ArrayElementStoreExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ArrayElementStoreExpression();

    /**
     * @brief Destroys this object.
     */
    ~ArrayElementStoreExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ArrayElementStoreExpression(ArrayElementStoreExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ArrayElementStoreExpression& operator=(ArrayElementStoreExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ArrayElementStoreExpression(ArrayElementStoreExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ArrayElementStoreExpression& operator=(ArrayElementStoreExpression&& other) noexcept;

public:
    /**
     * @brief Returns target array.
     * @return target array.
     */
    Expression* data();

    /**
     * @brief Returns target array.
     * @return target array.
     */
    inline Expression const* data() const {
        return const_cast<ArrayElementStoreExpression*>(this)->data();
    }

    /**
     * @brief Sets target array.
     * @param data target array
     * @return this
     */
    ArrayElementStoreExpression& data(std::unique_ptr<Expression> data);

    /**
     * @brief Releases target array from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_data();

    /**
     * @brief Returns array element position.
     * @return array element position.
     */
    Expression* index();

    /**
     * @brief Returns array element position.
     * @return array element position.
     */
    inline Expression const* index() const {
        return const_cast<ArrayElementStoreExpression*>(this)->index();
    }

    /**
     * @brief Sets array element position.
     * @param index array element position
     * @return this
     */
    ArrayElementStoreExpression& index(std::unique_ptr<Expression> index);

    /**
     * @brief Releases array element position from this node.
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
        return const_cast<ArrayElementStoreExpression*>(this)->value();
    }

    /**
     * @brief Sets storing value.
     * @param value storing value
     * @return this
     */
    ArrayElementStoreExpression& value(std::unique_ptr<Expression> value);

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
        return const_cast<ArrayElementStoreExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    ArrayElementStoreExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ArrayElementStoreExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ArrayElementStoreExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::ARRAY_ELEMENT_STORE_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_ARRAY_ELEMENT_STORE_EXPRESSION_H_
