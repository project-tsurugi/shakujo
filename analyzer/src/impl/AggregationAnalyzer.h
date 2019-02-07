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
#ifndef SHAKUJO_ANALYZER_IMPL_AGGREGATION_ANALYZER_H_
#define SHAKUJO_ANALYZER_IMPL_AGGREGATION_ANALYZER_H_

#include <vector>

#include "shakujo/analyzer/AnalyzerContext.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/FunctionBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"
#include "shakujo/analyzer/scope/Scope.h"

#include "shakujo/model/IRFactory.h"

namespace shakujo::analyzer::impl {

class AggregationAnalyzer {
public:
    AggregationAnalyzer(AnalyzerContext& env, model::expression::Expression* source);
    ~AggregationAnalyzer() noexcept;
    AggregationAnalyzer(const AggregationAnalyzer& other) = delete;
    AggregationAnalyzer(AggregationAnalyzer&& other) noexcept = delete;
    AggregationAnalyzer& operator=(const AggregationAnalyzer& other) = delete;
    AggregationAnalyzer& operator=(AggregationAnalyzer&& other) noexcept = delete;

    void process(model::expression::Expression* node);
    model::expression::relation::AggregationExpression* build();

private:
    class Impl;
    friend class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace shakujo::analyzer::impl

#endif  //SHAKUJO_ANALYZER_IMPL_AGGREGATION_ANALYZER_H_
