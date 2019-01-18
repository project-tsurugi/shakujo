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
#include "shakujo/analyzer/binding/BindingContext.h"

#include <atomic>
#include <sstream>
#include <stdexcept>

#include "shakujo/analyzer/binding/Id.h"
#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/FunctionBinding.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/key/FunctionKey.h"

namespace shakujo::analyzer::binding {

namespace {
    template<typename B, typename K>
    class EntityImpl : public K::Entity {
    public:
        using binding_type = B;
        using key_type = K;
        using entity_type = typename K::Entity;

        std::shared_ptr<int> identity_;
        std::shared_ptr<B> binding_;
        explicit EntityImpl(std::shared_ptr<int> identity, std::shared_ptr<B> binding)
            : identity_(std::move(identity))
            , binding_(std::move(binding))
        {}
    };

    using ExpressionEntity = EntityImpl<ExpressionBinding, model::key::ExpressionKey>;
    using VariableEntity = EntityImpl<VariableBinding, model::key::VariableKey>;
    using FunctionEntity = EntityImpl<FunctionBinding, model::key::FunctionKey>;
}  // namespace

class BindingContext::Impl {
private:
    std::shared_ptr<int> identity_ { new int(0) };
    std::atomic<Id<VariableBinding>::type> variable_ids_ { Id<VariableBinding>::MIN_VALUE };
    std::atomic<Id<FunctionBinding>::type> function_ids_ { Id<FunctionBinding>::MIN_VALUE };

public:
    template<typename T>
    std::unique_ptr<typename T::key_type> key(std::shared_ptr<typename T::binding_type> binding) {
        return std::make_unique<typename T::key_type>(std::make_unique<T>(identity_, std::move(binding)));
    }

    template<typename T>
    typename std::shared_ptr<typename T::binding_type> extract(
            typename T::key_type const* key,
            std::string_view name = {}) const {
        if (!key->template has_entity<T>()) {
            if (name.empty()) return {};
            std::ostringstream ss;
            if (key->template has_entity<typename T::entity_type>()) {
                ss << name << " is not yet initialized";
            } else {
                ss << name << " is incompatible for this context kind";
            }
            throw std::domain_error(ss.str());
        }
        auto entity = key->template entity<T>();
        if (entity->identity_.get() != identity_.get()) {
            if (name.empty()) return {};
            std::ostringstream ss;
            ss << name << " is incompatible for this context epoch";
            throw std::domain_error(ss.str());
        }
        return entity->binding_;
    }

    Id<VariableBinding> next_variable_id() {
        return Id<VariableBinding>(variable_ids_++);
    }

    Id<FunctionBinding> next_function_id() {
        return Id<FunctionBinding>(function_ids_++);
    }
};

BindingContext::BindingContext() : impl_(new Impl()) {}

BindingContext::~BindingContext() noexcept = default;

Id<VariableBinding> BindingContext::next_variable_id() {
    return impl_->next_variable_id();
}

Id<FunctionBinding> BindingContext::next_function_id() {
    return impl_->next_function_id();
}

std::unique_ptr<model::key::ExpressionKey> BindingContext::create_key(std::shared_ptr<ExpressionBinding> binding) {
    return impl_->key<ExpressionEntity>(std::move(binding));
}

std::unique_ptr<model::key::VariableKey> BindingContext::create_key(std::shared_ptr<VariableBinding> binding) {
    return impl_->key<VariableEntity>(std::move(binding));
}

std::unique_ptr<model::key::FunctionKey> BindingContext::create_key(std::shared_ptr<FunctionBinding> binding) {
    return impl_->key<FunctionEntity>(std::move(binding));
}

std::shared_ptr<ExpressionBinding> BindingContext::find(model::key::ExpressionKey const* key) const {
    return impl_->extract<ExpressionEntity>(key);
}

std::shared_ptr<VariableBinding> BindingContext::find(model::key::VariableKey const* key) const {
    return impl_->extract<VariableEntity>(key);
}

std::shared_ptr<FunctionBinding> BindingContext::find(model::key::FunctionKey const* key) const {
    return impl_->extract<FunctionEntity>(key);
}

std::shared_ptr<ExpressionBinding> BindingContext::get(model::key::ExpressionKey const* key) const {
    return impl_->extract<ExpressionEntity>(key, "expression key");
}

std::shared_ptr<VariableBinding> BindingContext::get(model::key::VariableKey const* key) const {
    return impl_->extract<VariableEntity>(key, "variable key");
}

std::shared_ptr<FunctionBinding> BindingContext::get(model::key::FunctionKey const* key) const {
    return impl_->extract<FunctionEntity>(key, "function key");
}
}  // namespace shakujo::analyzer::binding
