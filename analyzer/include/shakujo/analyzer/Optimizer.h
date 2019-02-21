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
#ifndef SHAKUJO_ANALYZER_OPTIMIZER_H_
#define SHAKUJO_ANALYZER_OPTIMIZER_H_

#include <memory>

#include "binding/BindingContext.h"
#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::analyzer {

/**
 * @brief optimizes analyzed IR models.
 */
class Optimizer {
public:
    /**
     * @brief constructs a new object.
     * @param bindings the binding context
     */
    explicit Optimizer(binding::BindingContext& bindings);

    /**
     * @brief destructs this object.
     */
    ~Optimizer() noexcept;

    Optimizer(Optimizer const& other) = delete;
    Optimizer& operator=(Optimizer const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Optimizer(Optimizer&& other) noexcept;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Optimizer& operator=(Optimizer&& other) noexcept;

    /**
     * @brief optimizes the given program.
     * The given node must be analyzed by Analyzer.
     * @param context the analysis context
     * @param node the target node
     */
    void operator()(model::program::Program* node);

    /**
     * @brief optimizes the given statement.
     * The given node must be analyzed by Analyzer.
     * @param context the analysis context
     * @param node the target node
     */
    void operator()(model::statement::Statement* node);

    /**
     * @brief optimizes the given expression.
     * The given node must be analyzed by Analyzer.
     * The expression must be managed because it may be rewrite to another expression.
     * @param context the analysis context
     * @param node the target node
     */
    void operator()(model::expression::Expression* node);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
}  // namespace shakujo::analyzer

#endif  //SHAKUJO_ANALYZER_OPTIMIZER_H_
