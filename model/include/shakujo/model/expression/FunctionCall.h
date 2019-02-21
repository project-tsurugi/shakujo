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
#ifndef SHAKUJO_MODEL_EXPRESSION_FUNCTION_CALL_H_
#define SHAKUJO_MODEL_EXPRESSION_FUNCTION_CALL_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::expression {
/**
 * @brief Represents calling functions.
 */
class FunctionCall
        : public Expression
        , public key::FunctionKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit FunctionCall(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents set quantifier.
     */
    enum class Quantifier {
        /**
         * @brief no quantifiers.
         */
        ABSENT,

        /**
         * @brief aggregate on bag.
         */
        ALL,

        /**
         * @brief aggregate on set.
         */
        DISTINCT,

        /**
         * @brief special case for f(*).
         */
        ASTERISK,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    FunctionCall();

    /**
     * @brief Destroys this object.
     */
    ~FunctionCall() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    FunctionCall(FunctionCall const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    FunctionCall& operator=(FunctionCall const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    FunctionCall(FunctionCall&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    FunctionCall& operator=(FunctionCall&& other) noexcept;

public:
    /**
     * @brief Returns function name.
     * @return function name.
     */
    name::Name* name();

    /**
     * @brief Returns function name.
     * @return function name.
     */
    inline name::Name const* name() const {
        return const_cast<FunctionCall*>(this)->name();
    }

    /**
     * @brief Sets function name.
     * @param name function name
     * @return this
     */
    FunctionCall& name(std::unique_ptr<name::Name> name);

    /**
     * @brief Releases function name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_name();

    /**
     * @brief Returns function arguments.
     * @return function arguments.
     */
    util::ManagedNodeList<Expression>& arguments();

    /**
     * @brief Returns function arguments.
     * @return function arguments.
     */
    inline util::ManagedNodeList<Expression> const& arguments() const {
        return const_cast<FunctionCall*>(this)->arguments();
    }
    /**
     * @brief Returns set quantifier.
     * @return set quantifier.
     */
    FunctionCall::Quantifier quantifier() const;

    /**
     * @brief Sets set quantifier.
     * @param quantifier set quantifier
     * @return this
     */
    FunctionCall& quantifier(FunctionCall::Quantifier quantifier);

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
        return const_cast<FunctionCall*>(this)->expression_key();
    }

    /**
     * @brief Sets expression key.
     * @param expression_key expression key
     * @return this
     */
    FunctionCall& expression_key(std::unique_ptr<key::ExpressionKey> expression_key) override;

    /**
     * @brief Returns referring function key.
     * @return referring function key.
     */
    key::FunctionKey* function_key() override;

    /**
     * @brief Returns referring function key.
     * @return referring function key.
     */
    inline key::FunctionKey const* function_key() const override {
        return const_cast<FunctionCall*>(this)->function_key();
    }

    /**
     * @brief Sets referring function key.
     * @param function_key referring function key
     * @return this
     */
    FunctionCall& function_key(std::unique_ptr<key::FunctionKey> function_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    FunctionCall* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    FunctionCall* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr ExpressionKind tag = ExpressionKind::FUNCTION_CALL;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */ExpressionKind kind() const override {
        return tag;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see FunctionCall::Quantifier
 */
inline constexpr std::string_view to_string_view(FunctionCall::Quantifier value) {
    switch (value) {
        case FunctionCall::Quantifier::ABSENT: return "ABSENT";
        case FunctionCall::Quantifier::ALL: return "ALL";
        case FunctionCall::Quantifier::DISTINCT: return "DISTINCT";
        case FunctionCall::Quantifier::ASTERISK: return "ASTERISK";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see FunctionCall::Quantifier
 */
inline std::ostream& operator<<(std::ostream& out, FunctionCall::Quantifier value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::expression

#endif  // SHAKUJO_MODEL_EXPRESSION_FUNCTION_CALL_H_
