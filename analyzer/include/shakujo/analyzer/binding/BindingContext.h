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
#ifndef SHAKUJO_ANALYZER_BINDING_CONTEXT_H_
#define SHAKUJO_ANALYZER_BINDING_CONTEXT_H_

#include <memory>
#include <stdexcept>

#include "Id.h"
#include "ExpressionBinding.h"
#include "VariableBinding.h"
#include "FunctionBinding.h"
#include "RelationBinding.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::analyzer::binding {

/**
 * @brief Provides bindings.
 */
class BindingContext {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief Constructs a new object.
     */
    BindingContext();

    /**
     * @brief destroys this object.
     */
    ~BindingContext() noexcept;

    BindingContext(const BindingContext&) = delete;
    BindingContext(BindingContext&&) noexcept = delete;
    BindingContext& operator=(const BindingContext&) = delete;
    BindingContext& operator=(BindingContext&&) noexcept = delete;

    /**
     * @brief returns the next binding ID in this context.
     * @tparam T the binding kind
     * @return the next ID
     */
    Id<VariableBinding> next_variable_id();

    /**
     * @brief returns the next binding ID in this context.
     * @tparam T the binding kind
     * @return the next ID
     */
    Id<FunctionBinding> next_function_id();

    /**
     * @brief returns a binding for the key.
     * @param binding the target binding
     * @return the corresponded key
     */
    std::unique_ptr<model::key::ExpressionKey> create_key(std::shared_ptr<ExpressionBinding> binding);

    /**
     * @brief returns a binding for the key.
     * @param binding the target binding
     * @return the corresponded key
     */
    std::unique_ptr<model::key::VariableKey> create_key(std::shared_ptr<VariableBinding> binding);

    /**
     * @brief returns a binding for the key.
     * @param binding the target binding
     * @return the corresponded key
     */
    std::unique_ptr<model::key::FunctionKey> create_key(std::shared_ptr<FunctionBinding> binding);

    /**
     * @brief returns a binding for the key.
     * @param binding the target binding
     * @return the corresponded key
     */
    std::unique_ptr<model::key::RelationKey> create_key(std::shared_ptr<RelationBinding> binding);

    /**
     * @brief creates a binding and returns the corresponded key.
     * @tparam T the binding type
     * @tparam Args the binding argument types
     * @param args the binding arguments
     * @return the corresponded key
     */
    template<class T, class... Args>
    std::unique_ptr<typename T::key_type> create_key(Args&&... args) {
        return create_key(std::make_shared<T>(std::forward<Args>(args)...));
    }

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return a shared pointer with the corresponded binding
     * @return an empty shared pointer if there is no such a binding for the key
     * @see get()
     */
    std::shared_ptr<ExpressionBinding> find(model::key::ExpressionKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return a shared pointer with the corresponded binding
     * @return an empty shared pointer if there is no such a binding for the key
     * @see get()
     */
    std::shared_ptr<VariableBinding> find(model::key::VariableKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return a shared pointer with the corresponded binding
     * @return an empty shared pointer if there is no such a binding for the key
     * @see get()
     */
    std::shared_ptr<FunctionBinding> find(model::key::FunctionKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return a shared pointer with the corresponded binding
     * @return an empty shared pointer if there is no such a binding for the key
     * @see get()
     */
    std::shared_ptr<RelationBinding> find(model::key::RelationKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return the corresponded binding
     * @throws if there is no such a binding for the key
     * @see exists()
     */
    std::shared_ptr<ExpressionBinding> get(model::key::ExpressionKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return the corresponded binding
     * @throws if there is no such a binding for the key
     * @see exists()
     */
    std::shared_ptr<VariableBinding> get(model::key::VariableKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return the corresponded binding
     * @throws if there is no such a binding for the key
     * @see exists()
     */
    std::shared_ptr<FunctionBinding> get(model::key::FunctionKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return the corresponded binding
     * @throws if there is no such a binding for the key
     * @see exists()
     */
    std::shared_ptr<RelationBinding> get(model::key::RelationKey const* key) const;

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return a shared pointer with the corresponded binding
     * @return an empty shared pointer if there is no such a binding for the key
     * @see get()
     */
    template<class K>
    auto find(std::unique_ptr<K> const& key) const -> decltype(this->find(key.get())) {
        return find(key.get());
    }

    /**
     * @brief returns a binding for the key.
     * @param key the target key
     * @return the corresponded binding
     * @throws if there is no such a binding for the key
     * @see exists()
     */
    template<class K>
    auto get(std::unique_ptr<K> const& key) const -> decltype(this->get(key.get())) {
        return get(key.get());
    }
};
}  // namespace shakujo::analyzer::binding

#endif  //SHAKUJO_ANALYZER_BINDING_CONTEXT_H_
