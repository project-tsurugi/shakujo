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
#ifndef SHAKUJO_MODEL_EXPRESSION_PLACEHOLDER_H_
#define SHAKUJO_MODEL_EXPRESSION_PLACEHOLDER_H_

#include <utility>
#include <memory>
#include <string>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {
/**
 * @brief Represents expression placeholder.
 */
class Placeholder
        : public Expression {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit Placeholder(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    Placeholder();

    /**
     * @brief Destroys this object.
     */
    ~Placeholder() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    Placeholder(Placeholder const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    Placeholder& operator=(Placeholder const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    Placeholder(Placeholder&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    Placeholder& operator=(Placeholder&& other) noexcept;

public:
    /**
     * @brief Returns placeholder name.
     * @return placeholder name.
     */
    std::string const& name() const;
    /**
     * @brief Sets placeholder name.
     * @param name placeholder name
     * @return this
     */
    Placeholder& name(std::string name);

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
        return const_cast<Placeholder*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    Placeholder& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Placeholder* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Placeholder* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    ExpressionKind kind() const override {
        return ExpressionKind::PLACEHOLDER;
    }

};
}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_PLACEHOLDER_H_
