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
#include "shakujo/analyzer/Optimizer.h"

#include <type_traits>

#include "optimize/Context.h"
#include "optimize/PredicatePushDown.h"
#include "optimize/SelectJoin.h"
#include "optimize/SelectScan.h"
#include "optimize/SimplifyCast.h"
#include "optimize/ProjectionPushDown.h"
#include "optimize/FixRelationInfo.h"

namespace shakujo::analyzer {

class Optimizer::Impl {
public:
    explicit Impl(binding::BindingContext& bindings)
        : context({}, bindings)
    {}

    template<class Node>
    void operator()(Node* node) {
        if (context.options().predicate_push_down) {
            apply_engine<optimize::PredicatePushDown>(node);
            apply_engine<optimize::SimplifyCast>(node);
        }
        if (context.options().scan) {
            apply_engine<optimize::SelectScan>(node);
            apply_engine<optimize::SimplifyCast>(node);
        }
        if (context.options().join) {
            // FIXME: this requires predicate push down
            apply_engine<optimize::SelectJoin>(node);
        }
        if (context.options().projection_push_down) {
            apply_engine<optimize::ProjectionPushDown>(node);
        }
        apply_engine<optimize::FixRelationInfo>(node);
    }

    template<class Engine, class Node>
    static constexpr bool is_applicable_v = std::is_invocable<Engine, Node*>::value;

    template<class Engine, class Node>
    std::enable_if_t<is_applicable_v<Engine, Node>> apply_engine(Node* node) {
        Engine engine { context };
        engine(node);
    }

    template<class Engine, class Node>
    std::enable_if_t<!is_applicable_v<Engine, Node>> apply_engine(Node*) {}

    optimize::Context context;
};

Optimizer::Optimizer(binding::BindingContext& bindings)
    : impl_(std::make_unique<Impl>(bindings))
{}

Optimizer::~Optimizer() noexcept = default;
Optimizer::Optimizer(Optimizer &&other) noexcept = default;
Optimizer &Optimizer::operator=(Optimizer &&other) noexcept = default;

void Optimizer::operator()(model::program::Program *node) {
    (*impl_)(node);
}

void Optimizer::operator()(model::statement::Statement *node) {
    (*impl_)(node);
}

void Optimizer::operator()(model::expression::Expression *node) {
    (*impl_)(node);
}

Optimizer::Options& Optimizer::options() {
    return impl_->context.options();
}

Optimizer::Options const& Optimizer::options() const {
    return impl_->context.options();
}

}  // namespace shakujo::analyzer
