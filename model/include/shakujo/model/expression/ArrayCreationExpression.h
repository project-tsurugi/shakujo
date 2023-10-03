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
#ifndef SHAKUJO_MODEL_EXPRESSION_ARRAY_CREATION_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_ARRAY_CREATION_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression {
/**
 * @brief Represents creating arrays.
 */
class ArrayCreationExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ArrayCreationExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ArrayCreationExpression();

    /**
     * @brief Destroys this object.
     */
    ~ArrayCreationExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ArrayCreationExpression(ArrayCreationExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ArrayCreationExpression& operator=(ArrayCreationExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ArrayCreationExpression(ArrayCreationExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ArrayCreationExpression& operator=(ArrayCreationExpression&& other) noexcept;

public:
    /**
     * @brief Returns array type.
     * @return array type.
     */
    type::Type* type();

    /**
     * @brief Returns array type.
     * @return array type.
     */
    inline type::Type const* type() const {
        return const_cast<ArrayCreationExpression*>(this)->type();
    }

    /**
     * @brief Sets array type.
     * @param type array type
     * @return this
     */
    ArrayCreationExpression& type(std::unique_ptr<type::Type> type);

    /**
     * @brief Releases array type from this node.
     * @return the released node
     */
    std::unique_ptr<type::Type> release_type();

    /**
     * @brief Returns initial array elements.
     * @return initial array elements.
     */
    util::ManagedNodeList<Expression>& values();

    /**
     * @brief Returns initial array elements.
     * @return initial array elements.
     */
    inline util::ManagedNodeList<Expression> const& values() const {
        return const_cast<ArrayCreationExpression*>(this)->values();
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
        return const_cast<ArrayCreationExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    ArrayCreationExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ArrayCreationExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ArrayCreationExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::ARRAY_CREATION_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_ARRAY_CREATION_EXPRESSION_H_
