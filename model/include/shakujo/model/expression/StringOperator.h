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
#ifndef SHAKUJO_MODEL_EXPRESSION_STRING_OPERATOR_H_
#define SHAKUJO_MODEL_EXPRESSION_STRING_OPERATOR_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents string operator expression.
 */
class StringOperator
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit StringOperator(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents string operator kind.
     */
    enum class Kind {
        /**
         * @brief like.
         */
        LIKE,

        /**
         * @brief like with ignoring cases.
         */
        LIKE_IGNORECASE,

        /**
         * @brief regex.
         */
        REGEX,

        /**
         * @brief invalid operator.
         */
        INVALID,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    StringOperator();

    /**
     * @brief Destroys this object.
     */
    ~StringOperator() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    StringOperator(StringOperator const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    StringOperator& operator=(StringOperator const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    StringOperator(StringOperator&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    StringOperator& operator=(StringOperator&& other) noexcept;

public:
    /**
     * @brief Returns string operator kind.
     * @return string operator kind.
     */
    StringOperator::Kind operator_kind() const;

    /**
     * @brief Sets string operator kind.
     * @param operator_kind string operator kind
     * @return this
     */
    StringOperator& operator_kind(StringOperator::Kind operator_kind);

    /**
     * @brief Returns string argument.
     * @return string argument.
     */
    std::string const& string() const;
    /**
     * @brief Sets string argument.
     * @param string string argument
     * @return this
     */
    StringOperator& string(std::string string);

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
        return const_cast<StringOperator*>(this)->operand();
    }

    /**
     * @brief Sets operand.
     * @param operand operand
     * @return this
     */
    StringOperator& operand(std::unique_ptr<Expression> operand);

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
        return const_cast<StringOperator*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    StringOperator& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    StringOperator* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    StringOperator* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::STRING_OPERATOR;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see StringOperator::Kind
 */
inline constexpr std::string_view to_string_view(StringOperator::Kind value) {
    switch (value) {
        case StringOperator::Kind::LIKE: return "LIKE";
        case StringOperator::Kind::LIKE_IGNORECASE: return "LIKE_IGNORECASE";
        case StringOperator::Kind::REGEX: return "REGEX";
        case StringOperator::Kind::INVALID: return "INVALID";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see StringOperator::Kind
 */
inline std::ostream& operator<<(std::ostream& out, StringOperator::Kind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_STRING_OPERATOR_H_
