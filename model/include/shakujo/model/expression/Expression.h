/*
 * Copyright 2018-2018 shakujo project.
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
#ifndef SHAKUJO_MODEL_EXPRESSION_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_EXPRESSION_H_

#include <memory>

#include "../Node.h"
#include "shakujo/common/util/Managed.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

/**
 * @brief An abstract super interface of expressions in IR model.
 * @see ExpressionKind
 * @see ExpressionVisitor
 * @see ConstExpressionVisitor
 */
class Expression : public Node,
        public common::util::Managed<Expression>,
        public key::ExpressionKey::Provider {
protected:
    /**
     * @brief Constructs a new object.
     */
    Expression() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Expression(const Expression& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Expression(Expression&& other) noexcept = default;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Expression& operator=(const Expression& other) = default;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Expression& operator=(Expression&& other) noexcept = default;

public:
    ~Expression() noexcept override = default;

    Expression* clone() const & override = 0;
    Expression* clone() && override = 0;

    /**
     * @brief Returns the kind of this expression.
     * @return the expression kind
     */
    virtual ExpressionKind kind() const = 0;
};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_EXPRESSION_H_
