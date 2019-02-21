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
#ifndef SHAKUJO_ANALYZER_OPTIMIZE_VARIABLE_COLLECTOR_H_
#define SHAKUJO_ANALYZER_OPTIMIZE_VARIABLE_COLLECTOR_H_

#include <map>
#include <memory>
#include <vector>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/VariableReference.h"

#include "shakujo/analyzer/binding/VariableBinding.h"

namespace shakujo::analyzer::optimize {

class VariableRewriter {
public:
    static std::vector<model::expression::VariableReference*> collect(model::expression::Expression* node);

    std::shared_ptr<binding::VariableBinding> apply(std::shared_ptr<binding::VariableBinding> const& variable);

    void add_rule(
        std::shared_ptr<binding::VariableBinding> from,
        std::shared_ptr<binding::VariableBinding> to);

    void add_rule(
            std::vector<std::shared_ptr<binding::VariableBinding>> const& from,
            std::vector<std::shared_ptr<binding::VariableBinding>> const& to);

    void deny(std::shared_ptr<binding::VariableBinding> from);

    void deny(std::vector<std::shared_ptr<binding::VariableBinding>> const& from);

    VariableRewriter& merge(VariableRewriter const& other);

private:
    std::map<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>> mapping_ {};
};
}  // namespace shakujo::analyzer::optimize

#endif  //SHAKUJO_ANALYZER_OPTIMIZE_VARIABLE_COLLECTOR_H_
