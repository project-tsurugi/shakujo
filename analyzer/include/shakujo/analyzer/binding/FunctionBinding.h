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

#include "Id.h"
#include "VariableBinding.h"

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
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
            std::vector<std::shared_ptr<VariableBinding>> parameters = {});

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
            common::core::Type const* type,
            std::vector<std::shared_ptr<VariableBinding>> parameters = {})
        : FunctionBinding(
                std::move(id),
                std::move(name),
                common::util::make_clone(type),
                std::move(parameters))
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
            common::core::Type&& type,
            std::vector<std::shared_ptr<VariableBinding>> parameters = {})
        : FunctionBinding(
                std::move(id),
                std::move(name),
                common::util::make_clone(std::move(type)),
                std::move(parameters))
    {}

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
     * @return the function return type, or undefined type if it does not return anything
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
     * @brief returns the function parameters.
     * @return the function parameters
     */
    std::vector<std::shared_ptr<VariableBinding>>& parameters();

    /**
     * @brief returns the function parameters.
     * @return the function parameters
     */
    inline const std::vector<std::shared_ptr<VariableBinding>>& parameters() const {
        return const_cast<FunctionBinding*>(this)->parameters();
    }
};
}  // namespace shakujo::analyzer::binding

#endif  // SHAKUJO_ANALYZER_BINDING_FUNCTION_BINDING_H_
