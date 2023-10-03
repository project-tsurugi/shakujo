/*
 * Copyright 2018-2023 Project Tsurugi.
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
#ifndef SHAKUJO_ANALYZER_ANALYZER_CONTEXT_H_
#define SHAKUJO_ANALYZER_ANALYZER_CONTEXT_H_

#include <memory>
#include <vector>

#include "Reporter.h"
#include "Diagnostic.h"
#include "scope/Scope.h"
#include "binding/BindingContext.h"
#include "binding/VariableBinding.h"
#include "binding/FunctionBinding.h"
#include "shakujo/common/schema/StorageInfoProvider.h"

namespace shakujo::analyzer {

/**
 * @brief represents a context of semantic analysis.
 */
class AnalyzerContext {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief constructs a new object.
     * FIXME shared_ptr?
     * @param storage_info_provider provides storage meta data
     * @param binding_context provides bindings
     */
    explicit AnalyzerContext(
            std::shared_ptr<common::schema::StorageInfoProvider> storage_info_provider =
                    std::make_shared<common::schema::StorageInfoProvider>(),
            std::shared_ptr<binding::BindingContext> binding_context =
                    std::make_shared<binding::BindingContext>());

    /**
     * @brief destroys this object.
     */
    ~AnalyzerContext() noexcept;

    AnalyzerContext(const AnalyzerContext&) = delete;
    AnalyzerContext(AnalyzerContext&&) noexcept = delete;
    AnalyzerContext& operator=(const AnalyzerContext&) = delete;
    AnalyzerContext& operator=(AnalyzerContext&&) noexcept = delete;

    /**
     * @brief returns the context reporter.
     * @return the context reporter
     */
    Reporter& reporter();

    /**
     * @brief returns the root binding context.
     * @return the root binding context
     */
    inline const Reporter& reporter() const {
        return const_cast<AnalyzerContext*>(this)->reporter();
    }

    /**
     * @brief returns the storage information provider.
     * @return the storage information provider
     */
    common::schema::StorageInfoProvider& storage_info_provider();

    /**
     * @brief returns the storage information provider.
     * @return the storage information provider
     */
    inline const common::schema::StorageInfoProvider& storage_info_provider() const {
        return const_cast<AnalyzerContext*>(this)->storage_info_provider();
    }

    /**
     * @brief returns the root binding context.
     * @return the root binding context
     */
    binding::BindingContext& binding_context();

    /**
     * @brief returns the root binding context.
     * @return the root binding context
     */
    inline const binding::BindingContext& binding_context() const {
        return const_cast<AnalyzerContext*>(this)->binding_context();
    }

    /**
     * @brief returns the variable scope of this context.
     * @return the context variable scope
     */
    scope::Scope<binding::VariableBinding>& variable_scope();

    /**
     * @brief returns the variable scope of this context.
     * @return the context variable scope
     */
    inline const scope::Scope<binding::VariableBinding>& variable_scope() const {
        return const_cast<AnalyzerContext*>(this)->variable_scope();
    }

    /**
     * @brief returns the function scope of this context.
     * @return the context function scope
     */
    scope::Scope<binding::FunctionBinding>& function_scope();

    /**
     * @brief returns the function scope of this context.
     * @return the context function scope
     */
    inline const scope::Scope<binding::FunctionBinding>& function_scope() const {
        return const_cast<AnalyzerContext*>(this)->function_scope();
    }

    /**
     * @brief registers an built-in variable.
     * The built-in variables are always case in-sensitive.
     * @param binding the variable to register
     */
    void register_builtin(std::shared_ptr<binding::VariableBinding> binding);

    /**
     * @brief registers an built-in function.
     * The built-in functions are always case in-sensitive.
     * @param binding the function to register
     */
    void register_builtin(std::shared_ptr<binding::FunctionBinding> binding);
};
}  // namespace shakujo::analyzer

#endif  //SHAKUJO_ANALYZER_ANALYZER_CONTEXT_H_
