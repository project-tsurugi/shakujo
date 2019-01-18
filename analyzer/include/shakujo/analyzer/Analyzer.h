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
#ifndef SHAKUJO_ANALYZER_ANALYZER_H_
#define SHAKUJO_ANALYZER_ANALYZER_H_

#include <memory>
#include <vector>

#include "AnalyzerContext.h"
#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::analyzer {

/**
 * @brief semantic analyzer over IR models.
 */
class Analyzer {
public:
    /**
     * @brief analyzes the given program.
     * @param context the analysis context
     * @param node the target node
     */
    void analyze(AnalyzerContext& context, model::program::Program* node);

    /**
     * @brief analyzes the given statement.
     * @param context the analysis context
     * @param node the target node
     */
    void analyze(AnalyzerContext& context, model::statement::Statement* node);

    /**
     * @brief analyzes the given expression.
     * The expression must be managed because it may be rewrite to another expression.
     * @param context the analysis context
     * @param node the target node
     */
    void analyze(AnalyzerContext& context, model::expression::Expression* node);
};
}  // namespace shakujo::analyzer

#endif  //SHAKUJO_ANALYZER_ANALYZER_H_
