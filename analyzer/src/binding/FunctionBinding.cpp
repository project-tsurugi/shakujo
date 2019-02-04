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
#include <shakujo/analyzer/binding/FunctionBinding.h>

#include "shakujo/analyzer/binding/FunctionBinding.h"

#include "shakujo/analyzer/binding/Id.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"

#include "impl/typing.h"

namespace shakujo::analyzer::binding {

class FunctionBinding::Impl {
public:
    Id<FunctionBinding> id_;
    common::core::Name name_;
    std::unique_ptr<common::core::Type> type_ {};
    std::vector<std::shared_ptr<VariableBinding>> parameters_ {};

    FunctionBinding::Quantifier quantifier_ { FunctionBinding::Quantifier::GROUND };
    std::vector<std::shared_ptr<FunctionBinding>> overload_candidates_ {};

    Impl(Id<FunctionBinding>&& id,
             common::core::Name&& name,
             std::unique_ptr<common::core::Type>&& type,
             std::vector<std::shared_ptr<VariableBinding>>&& parameters)
        : id_(std::move(id)), name_(std::move(name)), type_(std::move(type)), parameters_(std::move(parameters))
    {}

    Impl(Id<FunctionBinding>&& id,
            common::core::Name&& name,
            std::vector<std::shared_ptr<FunctionBinding>> overload_candidates)
        : id_(std::move(id)), name_(std::move(name))
        , quantifier_(FunctionBinding::Quantifier::UNRESOLVED), overload_candidates_(std::move(overload_candidates))
    {}
};

FunctionBinding::~FunctionBinding() noexcept = default;

FunctionBinding::FunctionBinding(
        Id<FunctionBinding> &&id,
        common::core::Name name,
        std::unique_ptr<common::core::Type> type,
        std::vector<std::shared_ptr<VariableBinding>> parameters)
    : impl_(new Impl { std::move(id), std::move(name), std::move(type), std::move(parameters) })
{}

FunctionBinding::FunctionBinding(
        Id<FunctionBinding> &&id,
        common::core::Name name,
        std::vector<std::shared_ptr<FunctionBinding>> overload_candidates)
    : impl_(new Impl { std::move(id), std::move(name), std::move(overload_candidates) })
{}

Id<FunctionBinding> const& FunctionBinding::id() const {
    return impl_->id_;
}

common::core::Name const& FunctionBinding::name() const {
    return impl_->name_;
}

common::core::Type const* FunctionBinding::type() const {
    return impl_->type_.get();
}

FunctionBinding &FunctionBinding::type(std::unique_ptr<common::core::Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::vector<std::shared_ptr<VariableBinding>>& FunctionBinding::parameters() {
    return impl_->parameters_;
}

FunctionBinding::Quantifier FunctionBinding::quantifier() const {
    return impl_->quantifier_;
}

FunctionBinding &FunctionBinding::quantifier(FunctionBinding::Quantifier quantifier) {
    impl_->quantifier_ = quantifier;
    return *this;
}

using tester_t = std::function<bool(VariableBinding const&, ExpressionBinding const&)>;

static bool eq_strict(VariableBinding const& parameter, ExpressionBinding const& argument) {
    return parameter.type()->equals(*argument.type(), true);
}

static bool eq_nullable(VariableBinding const& parameter, ExpressionBinding const& argument) {
    return parameter.type()->equals(*argument.type(), false);
}

static bool eq_assignable(VariableBinding const& parameter, ExpressionBinding const& argument) {
    return impl::typing::is_assignment_convertible(parameter.type(), argument);
}

std::shared_ptr<FunctionBinding>
FunctionBinding::resolve_overload(std::vector<ExpressionBinding const*> const& arguments) {
    if (!is_overload_stub()) {
        return {};
    }
    for (tester_t const& tester : { eq_strict, eq_nullable, eq_assignable }) {
        for (auto&& candidate : impl_->overload_candidates_) {
            if (candidate->quantifier() != Quantifier::GROUND && candidate->quantifier() != Quantifier::ALL) {
                continue;
            }
            auto&& parameters = candidate->parameters();
            if (parameters.size() != arguments.size()) {
                continue;
            }
            for (std::size_t i = 0, n = parameters.size(); i < n; ++i) {
                if (tester(*parameters[i], *arguments[i])) {
                    return candidate;
                }
            }
        }
    }
    return {};
}

std::shared_ptr<FunctionBinding>
FunctionBinding::resolve_overload(FunctionBinding::Quantifier quantifier, ExpressionBinding const* argument) {
    if (!is_overload_stub()) {
        return {};
    }
    std::size_t parameter_count = argument == nullptr ? 0U: 1U;
    for (tester_t const& tester : { eq_strict, eq_nullable, eq_assignable }) {
        for (auto&& candidate : impl_->overload_candidates_) {
            if (candidate->quantifier() != quantifier) {
                continue;
            }
            auto&& parameters = candidate->parameters();
            if (parameters.size() != parameter_count) {
                continue;
            }
            if (tester(*parameters[0], *argument)) {
                return candidate;
            }
        }
    }
    return {};
}

std::vector<std::shared_ptr<FunctionBinding>> &FunctionBinding::overload_candidates() {
    return impl_->overload_candidates_;
}
}  // namespace shakujo::analyzer::binding
