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
#ifndef SHAKUJO_ANALYZER_OPTIMIZE_CONTEXT_H_
#define SHAKUJO_ANALYZER_OPTIMIZE_CONTEXT_H_

#include <memory>
#include <type_traits>

#include "shakujo/analyzer/Optimizer.h"
#include "shakujo/analyzer/binding/BindingContext.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::optimize {

class Context {
public:
    Context(Optimizer::Options options, binding::BindingContext& bindings) noexcept
        : options_(options)
        , bindings_(bindings)
    {}

    explicit Context(binding::BindingContext& bindings) noexcept
        : bindings_(bindings)
    {}

    Optimizer::Options& options() {
        return options_;
    }

    Optimizer::Options const& options() const {
        return options_;
    }

    binding::BindingContext& bindings() {
        return bindings_;
    }

    template<class T>
    auto binding_of(T const* key) -> decltype(std::declval<binding::BindingContext>().get(key)) {
        return bindings_.get(key);
    }

    template<class T>
    std::shared_ptr<binding::RelationBinding> relation_of(T* node) {
        auto* provider = common::util::dynamic_pointer_cast<model::key::RelationKey::Provider>(node);
        return binding_of(provider->relation_key());
    }

private:
    Optimizer::Options options_ {};
    binding::BindingContext& bindings_;
};
}  // namespace shakujo::analyzer::optimize

#endif  //SHAKUJO_ANALYZER_OPTIMIZE_CONTEXT_H_
