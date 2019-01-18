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
#ifndef SHAKUJO_MODEL_EXPRESSION_CASE_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_CASE_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::expression {
/**
 * @brief Represents case expression.
 */
class CaseExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit CaseExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents case clause.
     */
    class Clause final {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Clause(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Constructs a new empty object.
         */
        Clause();

        /**
         * @brief Destroys this object.
         */
        ~Clause() noexcept;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Clause(Clause const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Clause& operator=(Clause const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Clause(Clause&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Clause& operator=(Clause&& other) noexcept;

    public:
        /**
         * @brief Returns case condition.
         * @return case condition.
         */
        Expression* condition();

        /**
         * @brief Returns case condition.
         * @return case condition.
         */
        inline Expression const* condition() const {
            return const_cast<CaseExpression::Clause*>(this)->condition();
        }

        /**
         * @brief Sets case condition.
         * @param condition case condition
         * @return this
         */
        CaseExpression::Clause& condition(std::unique_ptr<Expression> condition);

        /**
         * @brief Releases case condition from this node.
         * @return the released node
         */
        std::unique_ptr<Expression> release_condition();

        /**
         * @brief Returns body expression.
         * @return body expression.
         */
        Expression* body();

        /**
         * @brief Returns body expression.
         * @return body expression.
         */
        inline Expression const* body() const {
            return const_cast<CaseExpression::Clause*>(this)->body();
        }

        /**
         * @brief Sets body expression.
         * @param body body expression
         * @return this
         */
        CaseExpression::Clause& body(std::unique_ptr<Expression> body);

        /**
         * @brief Releases body expression from this node.
         * @return the released node
         */
        std::unique_ptr<Expression> release_body();

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Clause* clone() const &;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Clause* clone() &&;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    CaseExpression();

    /**
     * @brief Destroys this object.
     */
    ~CaseExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    CaseExpression(CaseExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    CaseExpression& operator=(CaseExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    CaseExpression(CaseExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    CaseExpression& operator=(CaseExpression&& other) noexcept;

public:
    /**
     * @brief Returns case clauses.
     * @return case clauses.
     */
    util::FragmentList<CaseExpression::Clause>& cases();

    /**
     * @brief Returns case clauses.
     * @return case clauses.
     */
    inline util::FragmentList<CaseExpression::Clause> const& cases() const {
        return const_cast<CaseExpression*>(this)->cases();
    }
    /**
     * @brief Returns default expression.
     * @return default expression.
     */
    Expression* default_expression();

    /**
     * @brief Returns default expression.
     * @return default expression.
     */
    inline Expression const* default_expression() const {
        return const_cast<CaseExpression*>(this)->default_expression();
    }

    /**
     * @brief Sets default expression.
     * @param default_expression default expression
     * @return this
     */
    CaseExpression& default_expression(std::unique_ptr<Expression> default_expression);

    /**
     * @brief Releases default expression from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_default_expression();

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
        return const_cast<CaseExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    CaseExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CaseExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CaseExpression* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::CASE_EXPRESSION;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_CASE_EXPRESSION_H_
