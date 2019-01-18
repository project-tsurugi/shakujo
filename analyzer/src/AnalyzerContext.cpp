/*
 * Copyright 2018 shakujo project.
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
#include "shakujo/analyzer/AnalyzerContext.h"
#include "shakujo/analyzer/Diagnostic.h"
#include "shakujo/analyzer/binding/BindingContext.h"
#include "shakujo/analyzer/scope/Scope.h"
#include "shakujo/analyzer/scope/BlockScope.h"

namespace shakujo::analyzer {

class AnalyzerContext::Impl {
public:
    std::shared_ptr<common::schema::StorageInfoProvider> storage_info_provider_;
    std::shared_ptr<binding::BindingContext> binding_context_;
    Reporter reporter_;
    scope::BlockScope<binding::VariableBinding> variables_ { &reporter_ };
    scope::BlockScope<binding::FunctionBinding> functions_ { &reporter_ };
};

AnalyzerContext::AnalyzerContext(
        std::shared_ptr<common::schema::StorageInfoProvider> storage_info_provider,
        std::shared_ptr<binding::BindingContext> binding_context)
    : impl_(new Impl())
{
    impl_->storage_info_provider_ = std::move(storage_info_provider);
    impl_->binding_context_ = std::move(binding_context);
}

AnalyzerContext::~AnalyzerContext() noexcept = default;

Reporter &AnalyzerContext::reporter() {
    return impl_->reporter_;
}

common::schema::StorageInfoProvider &AnalyzerContext::storage_info_provider() {
    return *impl_->storage_info_provider_;
}

binding::BindingContext &AnalyzerContext::binding_context() {
    return *impl_->binding_context_;
}

scope::Scope<binding::VariableBinding>& AnalyzerContext::variable_scope() {
    return impl_->variables_;
}

scope::Scope<binding::FunctionBinding>& AnalyzerContext::function_scope() {
    return impl_->functions_;
}
}  // namespace shakujo::analyzer
