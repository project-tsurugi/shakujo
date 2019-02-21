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
#ifndef SHAKUJO_MODEL_EXPRESSION_IMPLICIT_CAST_H_
#define SHAKUJO_MODEL_EXPRESSION_IMPLICIT_CAST_H_

#include <utility>
#include <memory>

#include "shakujo/common/core/Type.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents synthetic type casting operator.
 */
class ImplicitCast
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit ImplicitCast(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ImplicitCast();

    /**
     * @brief Destroys this object.
     */
    ~ImplicitCast() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ImplicitCast(ImplicitCast const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ImplicitCast& operator=(ImplicitCast const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ImplicitCast(ImplicitCast&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ImplicitCast& operator=(ImplicitCast&& other) noexcept;

public:
    /**
     * @brief Returns target type.
     * @return target type.
     */
    common::core::Type* type();

    /**
     * @brief Returns target type.
     * @return target type.
     */
    inline common::core::Type const* type() const {
        return const_cast<ImplicitCast*>(this)->type();
    }

    /**
     * @brief Sets target type.
     * @param type target type
     * @return this
     */
    ImplicitCast& type(std::unique_ptr<common::core::Type> type);

    /**
     * @brief Returns operand.
     * @return operand.
     */
    Expression* operand();

    /**
     * @brief Returns operand.
     * @return operand.
     */
    inline Expression const* operand() const {
        return const_cast<ImplicitCast*>(this)->operand();
    }

    /**
     * @brief Sets operand.
     * @param operand operand
     * @return this
     */
    ImplicitCast& operand(std::unique_ptr<Expression> operand);

    /**
     * @brief Releases operand from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_operand();

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
        return const_cast<ImplicitCast*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    ImplicitCast& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ImplicitCast* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    ImplicitCast* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr ExpressionKind tag = ExpressionKind::IMPLICIT_CAST;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_IMPLICIT_CAST_H_
