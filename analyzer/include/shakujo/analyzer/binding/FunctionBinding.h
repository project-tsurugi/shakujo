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
#ifndef SHAKUJO_ANALYZER_BINDING_FUNCTION_BINDING_H_
#define SHAKUJO_ANALYZER_BINDING_FUNCTION_BINDING_H_

#include <memory>
#include <utility>
#include <string>
#include <string_view>

#include "Id.h"
#include "ExpressionBinding.h"

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/utility.h"
#include "shakujo/model/key/FunctionKey.h"

namespace shakujo::analyzer::binding {

/**
 * @brief Provides semantic information of functions.
 */
class FunctionBinding final {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief represents function parameter specification.
     */
    class Parameter {
    public:
        /**
         * @brief creates a new instance.
         * @param name the parameter name
         * @param type the parameter type
         */
        Parameter(
                std::string_view name,
                std::unique_ptr<common::core::Type> type)
            : name_(name)
            , type_(std::move(type))
        {}

        /**
         * @brief returns the parameter name.
         * @return the parameter name
         */
        std::string const& name() const {
            return name_;
        }

        /**
         * @brief returns the parameter type
         * @return the parameter type
         */
        common::core::Type const* type() const {
            return type_.get();
        }

    private:
        std::string name_;
        common::util::ClonablePtr<common::core::Type> type_;
    };

    /**
     * @brief represents a set quantifier.
     */
    enum Quantifier {
        /**
         * @brief a ground order function.
         */
        GROUND,

        /**
         * @brief a first order bag function.
         */
        ALL,

        /**
         * @brief a first order set function.
         */
        DISTINCT,

        /**
         * @brief special case for count(*).
         */
        ASTERISK,

        /**
         * @brief function is not yet resolved: it is just an overload stub.
         */
        UNRESOLVED,
    };

    /**
     * @brief the key type.
     */
    using key_type = model::key::FunctionKey;

    /**
     * @brief Constructs a new object.
     */
    FunctionBinding()
        : FunctionBinding({}, {}, std::unique_ptr<common::core::Type> {}, {})
    {}

    /**
     * @brief Constructs a new object.
     * @param id the function ID
     * @param name the function name
     * @param type the function return type, may be undefined if it does not return anything
     * @param parameters the function parameters
     */
    FunctionBinding(
            Id<FunctionBinding>&& id,
            common::core::Name name,
            std::unique_ptr<common::core::Type> type,
            std::vector<Parameter> parameters = {})
        : FunctionBinding(
            std::move(id),
            std::move(name),
            std::move(type),
            Quantifier::GROUND,
            std::move(parameters))
    {}

    /**
     * @brief Constructs a new object.
     * @param id the function ID
     * @param name the function name
     * @param type the function return type, may be undefined if it does not return anything
     * @param quantifier the function quantifier
     * @param parameters the function parameters
     */
    FunctionBinding(
            Id<FunctionBinding>&& id,
            common::core::Name name,
            std::unique_ptr<common::core::Type> type,
            Quantifier quantifier,
            std::vector<Parameter> parameters = {});

    /**
     * @brief constructs a new object as an overload stub.
     * @param id the function ID
     * @param name the function name
     * @param overload_candidates the overload candidates
     */
    FunctionBinding(
            Id<FunctionBinding>&& id,
            common::core::Name name,
            std::vector<std::shared_ptr<FunctionBinding>> overload_candidates = {});

    /**
     * @brief Destroys this object.
     */
    ~FunctionBinding() noexcept;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    FunctionBinding(const FunctionBinding& other) = delete;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    FunctionBinding(FunctionBinding&& other) noexcept = delete;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    FunctionBinding& operator=(const FunctionBinding& other) = delete;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    FunctionBinding& operator=(FunctionBinding&& other) noexcept = delete;

    /**
     * @brief return the ID of the corresponded function.
     * It may not be valid if there are no corresponded function declarations.
     * @return the function ID
     * @see Id::operator bool()
     */
    Id<FunctionBinding> const& id() const;

    /**
     * @brief returns the corresponded variable name.
     * @return the variable name
     */
    common::core::Name const& name() const;

    /**
     * @brief returns the variable type.
     * It may be an erroneous type if type resolution was failed.
     * @return the function return type
     * @return null if it does not return anything, or this is an overload stub
     */
    common::core::Type const* type() const;

    /**
     * @brief sets the function return type.
     * @param type the function return type, or undefined type if it does not return anything
     * @return this
     */
    FunctionBinding& type(std::unique_ptr<common::core::Type> type);

    /**
     * @brief sets the function return type.
     * @param type the function return type, or undefined type if it does not return anything
     * @return this
     */
    FunctionBinding& type(common::core::Type const* type) {
        return this->type(common::util::make_clone(type));
    }

    /**
     * @brief sets the function return type.
     * @param type the function return type, or undefined type if it does not return anything
     * @return this
     */
    FunctionBinding& type(common::core::Type&& type) {
        return this->type(common::util::make_clone(std::move(type)));
    }

    /**
     * @brief returns the quantifier of the corresponded function.
     * @return the quantifier
     */
    Quantifier quantifier() const;

    /**
     * @brief sets the quantifier of the corresponded function.
     * @param quantifier the quantifier
     * @return this
     */
    FunctionBinding& quantifier(Quantifier quantifier);

    /**
     * @brief returns the function parameters.
     * @return the function parameters
     * @return empty if this is an overload stub
     */
    std::vector<Parameter>& parameters();

    /**
     * @brief returns the function parameters.
     * @return the function parameters
     * @return empty if this is an overload stub
     */
    inline const std::vector<Parameter>& parameters() const {
        return const_cast<FunctionBinding*>(this)->parameters();
    }

    /**
     * @brief returns whether or not this binding represents a set function.
     * @return true if this represents a set function
     * @return false otherwise
     */
    bool is_set_function() const {
        return quantifier() == Quantifier::ALL
            || quantifier() == Quantifier::DISTINCT
            || quantifier() == Quantifier::ASTERISK;
    }

    /**
     * @brief returns whether or not this binding is an overload stub.
     * @return true if this binding is an overload stub
     * @return false otherwise
     */
    bool is_overload_stub() const {
        return quantifier() == Quantifier::UNRESOLVED;
    }

    /**
     * @brief resolves this overload stub and returns the suitable overload.
     * @param arguments the arguments
     * @return the resolved overload if it is available
     * @return empty if there are no suitable candidates, or this is not a overload stub
     */
    std::shared_ptr<FunctionBinding> resolve_overload(std::vector<ExpressionBinding const*> const& arguments);

    /**
     * @brief resolves this overload stub and returns the suitable overload.
     * @param quantifier the set quantifier
     * @param argument the argument, maybe nullptr if quantifier = ASTERISK
     * @return the resolved overload if it is available
     * @return empty if there are no suitable candidates, or this is not a overload stub
     */
    std::shared_ptr<FunctionBinding> resolve_overload(
            Quantifier quantifier,
            ExpressionBinding const* argument = nullptr);

    /**
     * @brief returns the function overload candidates.
     * This is only available if this binding is an overload stub.
     * @return the candidates
     */
    std::vector<std::shared_ptr<FunctionBinding>>& overload_candidates();

    /**
     * @brief returns the function overload candidates.
     * This is only available if this binding is an overload stub.
     * @return the candidates
     */
    inline std::vector<std::shared_ptr<FunctionBinding>>& overload_candidates() const {
        return const_cast<FunctionBinding*>(this)->overload_candidates();
    }

    /**
     * @brief returns whether or not this function has valid ID.
     * @return true if this has valid ID
     * @return false otherwise
     */
    bool has_id() const {
        return static_cast<bool>(id());
    }

    /**
     * @brief returns whether or not the corresponded function is valid.
     * @return true is the variable is valid
     * @return false otherwise
     */
    bool is_valid() const {
        namespace util = common::util;
        if (!has_id()) {
            return false;
        }
        if (is_overload_stub()) {
            return !overload_candidates().empty();
        }
        if (!util::is_valid(type())) {
            return false;
        }
        for (auto&& param : parameters()) {
            if (!util::is_valid(param.type())) {
                return false;
            }
        }
        if (is_set_function()) {
            return parameters().size() <= 1U;
        }
        return true;
    }
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 */
inline constexpr std::string_view to_string_view(FunctionBinding::Quantifier value) {
    using Quantifier = FunctionBinding::Quantifier;
    switch (value) {
        case Quantifier::GROUND: return "GROUND";
        case Quantifier::ALL: return "ALL";
        case Quantifier::DISTINCT: return "DISTINCT";
        case Quantifier::ASTERISK: return "ASTERISK";
        case Quantifier::UNRESOLVED: return "UNRESOLVED";
    }
    std::abort();
}

}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_FUNCTION_BINDING_H_
