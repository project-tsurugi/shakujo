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
#ifndef SHAKUJO_ANALYZER_ANALYZE_SCOPE_CONTEXT_H_
#define SHAKUJO_ANALYZER_ANALYZE_SCOPE_CONTEXT_H_

#include <stdexcept>

#include "shakujo/analyzer/scope/Scope.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/FunctionBinding.h"

namespace shakujo::analyzer::analyze {

class ScopeContext {
private:
    scope::Scope<binding::VariableBinding>* variables_;
    scope::Scope<binding::FunctionBinding>* functions_;

public:
    /**
     * @brief constructs a new object.
     * @param variables the variable scope
     * @param functions the function scope
     */
    ScopeContext(
            scope::Scope<binding::VariableBinding>* variables,
            scope::Scope<binding::FunctionBinding>* functions)
        : variables_(variables), functions_(functions)
    {
        if (!variables) throw std::invalid_argument("variable scope must not be null");
        if (!functions) throw std::invalid_argument("function scope must not be null");
    }

    /**
     * @brief constructs a new object.
     * @param variables the variable scope
     * @param functions the function scope
     */
    ScopeContext(
            scope::Scope<binding::VariableBinding>& variables,
            scope::Scope<binding::FunctionBinding>& functions)
        : ScopeContext(&variables, &functions)
    {}

    /**
     * @brief destructs this object.
     */
    ~ScopeContext() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    ScopeContext(ScopeContext const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    ScopeContext(ScopeContext&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     */
    ScopeContext& operator=(ScopeContext const& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     */
    ScopeContext& operator=(ScopeContext&& other) noexcept = default;

    /**
     * @brief returns the variable scope.
     * @return the variable scope.
     */
    scope::Scope<binding::VariableBinding>& variables() {
        return *variables_;
    }

    /**
     * @brief returns the variable scope.
     * @return the variable scope.
     */
    const scope::Scope<binding::VariableBinding>& variables() const {
        return *variables_;
    }

    /**
     * @brief returns the function scope.
     * @return the function scope.
     */
    scope::Scope<binding::FunctionBinding>& functions() {
        return *functions_;
    }

    /**
     * @brief returns the function scope.
     * @return the function scope.
     */
    const scope::Scope<binding::FunctionBinding>& functions() const {
        return *functions_;
    }
};
}  // namespace shakujo::analyzer::analyze

#endif  //SHAKUJO_ANALYZER_ANALYZE_SCOPE_CONTEXT_H_
