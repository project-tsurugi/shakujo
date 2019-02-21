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
#include "SimplifyCast.h"

#include "shakujo/common/util/utility.h"

#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/analyzer/binding/ExpressionBinding.h"

namespace shakujo::analyzer::optimize {

using common::util::equals;
using common::util::dynamic_pointer_cast_if;

namespace {
class Engine : public model::util::NodeWalker {
public:
    explicit Engine(Context& context) noexcept : context_(context) {}

    using NodeWalker::enter;

    bool enter(model::expression::ImplicitCast* node) override {
        while (auto* nest = dynamic_pointer_cast_if<model::expression::ImplicitCast>(node->operand())) {
            nest->replace_with(nest->release_operand());
        }
        auto input = extract(node->operand());
        if (equals(input->type(), node->type())) {
            auto* replaced = node->replace_with(node->release_operand());
            walk(replaced);
            return false;
        }
        return true;
    }

    std::shared_ptr<binding::ExpressionBinding> extract(model::key::ExpressionKey::Provider* node) {
        return context_.bindings().get(node->expression_key());
    }

private:
    Context& context_;
};
}  // namespace

void SimplifyCast::operator()(model::program::Program* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void SimplifyCast::operator()(model::statement::Statement* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void SimplifyCast::operator()(model::expression::Expression *node) {
    Engine engine { context_ };
    engine.walk(node);
}
}  // namespace shakujo::analyzer::optimize
