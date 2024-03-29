/*
 * Copyright 2018-2024 Project Tsurugi.
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
#ifndef SHAKUJO_MODEL_EXPRESSION_VECTOR_ELEMENT_LOAD_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_VECTOR_ELEMENT_LOAD_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents loading element from vectors.
 */
class VectorElementLoadExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VectorElementLoadExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    VectorElementLoadExpression();

    /**
     * @brief Destroys this object.
     */
    ~VectorElementLoadExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VectorElementLoadExpression(VectorElementLoadExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementLoadExpression& operator=(VectorElementLoadExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VectorElementLoadExpression(VectorElementLoadExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementLoadExpression& operator=(VectorElementLoadExpression&& other) noexcept;

public:
    /**
     * @brief Returns source vector.
     * @return source vector.
     */
    Expression* data();

    /**
     * @brief Returns source vector.
     * @return source vector.
     */
    inline Expression const* data() const {
        return const_cast<VectorElementLoadExpression*>(this)->data();
    }

    /**
     * @brief Sets source vector.
     * @param data source vector
     * @return this
     */
    VectorElementLoadExpression& data(std::unique_ptr<Expression> data);

    /**
     * @brief Releases source vector from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_data();

    /**
     * @brief Returns element position.
     * @return element position.
     */
    Expression* index();

    /**
     * @brief Returns element position.
     * @return element position.
     */
    inline Expression const* index() const {
        return const_cast<VectorElementLoadExpression*>(this)->index();
    }

    /**
     * @brief Sets element position.
     * @param index element position
     * @return this
     */
    VectorElementLoadExpression& index(std::unique_ptr<Expression> index);

    /**
     * @brief Releases element position from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_index();

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
        return const_cast<VectorElementLoadExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    VectorElementLoadExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementLoadExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementLoadExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::VECTOR_ELEMENT_LOAD_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_VECTOR_ELEMENT_LOAD_EXPRESSION_H_
