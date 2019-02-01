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
#ifndef SHAKUJO_MODEL_EXPRESSION_TYPE_OPERATOR_H_
#define SHAKUJO_MODEL_EXPRESSION_TYPE_OPERATOR_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::expression {
/**
 * @brief Represents type operator expression.
 */
class TypeOperator
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit TypeOperator(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents type operator kind.
     */
    enum class Kind {
        /**
         * @brief type casting.
         */
        CAST,

        /**
         * @brief invalid operator.
         */
        INVALID,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    TypeOperator();

    /**
     * @brief Destroys this object.
     */
    ~TypeOperator() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    TypeOperator(TypeOperator const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    TypeOperator& operator=(TypeOperator const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    TypeOperator(TypeOperator&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    TypeOperator& operator=(TypeOperator&& other) noexcept;

public:
    /**
     * @brief Returns type operator kind.
     * @return type operator kind.
     */
    TypeOperator::Kind operator_kind() const;

    /**
     * @brief Sets type operator kind.
     * @param operator_kind type operator kind
     * @return this
     */
    TypeOperator& operator_kind(TypeOperator::Kind operator_kind);

    /**
     * @brief Returns type argument.
     * @return type argument.
     */
    type::Type* type();

    /**
     * @brief Returns type argument.
     * @return type argument.
     */
    inline type::Type const* type() const {
        return const_cast<TypeOperator*>(this)->type();
    }

    /**
     * @brief Sets type argument.
     * @param type type argument
     * @return this
     */
    TypeOperator& type(std::unique_ptr<type::Type> type);

    /**
     * @brief Releases type argument from this node.
     * @return the released node
     */
    std::unique_ptr<type::Type> release_type();

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
        return const_cast<TypeOperator*>(this)->operand();
    }

    /**
     * @brief Sets operand.
     * @param operand operand
     * @return this
     */
    TypeOperator& operand(std::unique_ptr<Expression> operand);

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
        return const_cast<TypeOperator*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    TypeOperator& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TypeOperator* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TypeOperator* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::TYPE_OPERATOR;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see TypeOperator::Kind
 */
inline constexpr std::string_view to_string_view(TypeOperator::Kind value) {
    switch (value) {
        case TypeOperator::Kind::CAST: return "CAST";
        case TypeOperator::Kind::INVALID: return "INVALID";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see TypeOperator::Kind
 */
inline std::ostream& operator<<(std::ostream& out, TypeOperator::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_TYPE_OPERATOR_H_
