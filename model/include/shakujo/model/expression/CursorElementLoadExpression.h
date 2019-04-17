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
#ifndef SHAKUJO_MODEL_EXPRESSION_CURSOR_ELEMENT_LOAD_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_CURSOR_ELEMENT_LOAD_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents loading element from cursors.
 */
class CursorElementLoadExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit CursorElementLoadExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    CursorElementLoadExpression();

    /**
     * @brief Destroys this object.
     */
    ~CursorElementLoadExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    CursorElementLoadExpression(CursorElementLoadExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    CursorElementLoadExpression& operator=(CursorElementLoadExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    CursorElementLoadExpression(CursorElementLoadExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    CursorElementLoadExpression& operator=(CursorElementLoadExpression&& other) noexcept;

public:
    /**
     * @brief Returns source cursor.
     * @return source cursor.
     */
    Expression* data();

    /**
     * @brief Returns source cursor.
     * @return source cursor.
     */
    inline Expression const* data() const {
        return const_cast<CursorElementLoadExpression*>(this)->data();
    }

    /**
     * @brief Sets source cursor.
     * @param data source cursor
     * @return this
     */
    CursorElementLoadExpression& data(std::unique_ptr<Expression> data);

    /**
     * @brief Releases source cursor from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_data();

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
        return const_cast<CursorElementLoadExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    CursorElementLoadExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CursorElementLoadExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CursorElementLoadExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::CURSOR_ELEMENT_LOAD_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_CURSOR_ELEMENT_LOAD_EXPRESSION_H_
