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
#include "shakujo/analyzer/Analyzer.h"

#include <sstream>
#include <stdexcept>

#include "analyze/Engine.h"
#include "analyze/ScopeContext.h"

#include "shakujo/analyzer/SyntaxValidator.h"
#include "shakujo/analyzer/scope/UnitScope.h"
#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/FunctionBinding.h"

namespace shakujo::analyzer {

template<typename T>
static void do_analyze(AnalyzerContext& context, T node) {
    if (!SyntaxValidator {}.analyze(context.reporter(), node)) {
        return;
    }
    analyze::Engine engine { context };
    auto vars = engine.unit_scope(context.variable_scope());
    auto funcs = engine.unit_scope(context.function_scope());
    analyze::ScopeContext scope { vars, funcs };
    engine.process(node, scope);
}

void Analyzer::analyze(AnalyzerContext &context, model::program::Program* node) {
    do_analyze(context, node);
}

void Analyzer::analyze(AnalyzerContext &context, model::statement::Statement* node) {
    do_analyze(context, node);
}

void Analyzer::analyze(AnalyzerContext &context, model::expression::Expression* node) {
    if (!node->is_managed()) {
        throw std::invalid_argument("expression must be managed");
    }
    do_analyze(context, node);
}
}  // namespace shakujo::analyzer
