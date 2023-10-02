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
#ifndef SHAKUJO_MODEL_EXPRESSION_LITERAL_H_
#define SHAKUJO_MODEL_EXPRESSION_LITERAL_H_

#include <utility>
#include <memory>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents literal.
 */
class Literal
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit Literal(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    Literal();

    /**
     * @brief Destroys this object.
     */
    ~Literal() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    Literal(Literal const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    Literal& operator=(Literal const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    Literal(Literal&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    Literal& operator=(Literal&& other) noexcept;

public:
    /**
     * @brief Returns literal type.
     * @return literal type.
     */
    common::core::Type* type();

    /**
     * @brief Returns literal type.
     * @return literal type.
     */
    inline common::core::Type const* type() const {
        return const_cast<Literal*>(this)->type();
    }

    /**
     * @brief Sets literal type.
     * @param type literal type
     * @return this
     */
    Literal& type(std::unique_ptr<common::core::Type> type);

    /**
     * @brief Returns literal value.
     * @return literal value.
     */
    common::core::Value* value();

    /**
     * @brief Returns literal value.
     * @return literal value.
     */
    inline common::core::Value const* value() const {
        return const_cast<Literal*>(this)->value();
    }

    /**
     * @brief Sets literal value.
     * @param value literal value
     * @return this
     */
    Literal& value(std::unique_ptr<common::core::Value> value);

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
        return const_cast<Literal*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    Literal& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Literal* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Literal* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::LITERAL;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_LITERAL_H_
