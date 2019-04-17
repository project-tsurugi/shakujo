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
#ifndef SHAKUJO_MODEL_EXPRESSION_TUPLE_ELEMENT_LOAD_EXPRESSION_H_
#define SHAKUJO_MODEL_EXPRESSION_TUPLE_ELEMENT_LOAD_EXPRESSION_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/name/Index.h"

namespace shakujo::model::expression {
/**
 * @brief Represents loading element from tuples.
 */
class TupleElementLoadExpression
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit TupleElementLoadExpression(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    TupleElementLoadExpression();

    /**
     * @brief Destroys this object.
     */
    ~TupleElementLoadExpression() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    TupleElementLoadExpression(TupleElementLoadExpression const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    TupleElementLoadExpression& operator=(TupleElementLoadExpression const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    TupleElementLoadExpression(TupleElementLoadExpression&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    TupleElementLoadExpression& operator=(TupleElementLoadExpression&& other) noexcept;

public:
    /**
     * @brief Returns source tuple.
     * @return source tuple.
     */
    Expression* data();

    /**
     * @brief Returns source tuple.
     * @return source tuple.
     */
    inline Expression const* data() const {
        return const_cast<TupleElementLoadExpression*>(this)->data();
    }

    /**
     * @brief Sets source tuple.
     * @param data source tuple
     * @return this
     */
    TupleElementLoadExpression& data(std::unique_ptr<Expression> data);

    /**
     * @brief Releases source tuple from this node.
     * @return the released node
     */
    std::unique_ptr<Expression> release_data();

    /**
     * @brief Returns element index.
     * @return element index.
     */
    name::Index* index();

    /**
     * @brief Returns element index.
     * @return element index.
     */
    inline name::Index const* index() const {
        return const_cast<TupleElementLoadExpression*>(this)->index();
    }

    /**
     * @brief Sets element index.
     * @param index element index
     * @return this
     */
    TupleElementLoadExpression& index(std::unique_ptr<name::Index> index);

    /**
     * @brief Releases element index from this node.
     * @return the released node
     */
    std::unique_ptr<name::Index> release_index();

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
        return const_cast<TupleElementLoadExpression*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    TupleElementLoadExpression& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TupleElementLoadExpression* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TupleElementLoadExpression* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = ExpressionKind::TUPLE_ELEMENT_LOAD_EXPRESSION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_TUPLE_ELEMENT_LOAD_EXPRESSION_H_
