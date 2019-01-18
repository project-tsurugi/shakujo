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
#ifndef SHAKUJO_ANALYZER_SYNTAX_VALIDATOR_H_
#define SHAKUJO_ANALYZER_SYNTAX_VALIDATOR_H_

#include <memory>
#include <vector>

#include "Reporter.h"
#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::analyzer {

/**
 * @brief syntax validator over IR models.
 */
class SyntaxValidator {
public:
    /**
     * @brief analyzes the given program.
     * @param reporter diagnostic sink
     * @param node the target node
     * @return true if it is syntactically valid
     * @return false otherwise
     */
    bool analyze(Reporter& reporter, model::program::Program const* node);

    /**
     * @brief analyzes the given statement.
     * @param reporter diagnostic sink
     * @param node the target node
     * @return true if it is syntactically valid
     * @return false otherwise
     */
    bool analyze(Reporter& reporter, model::statement::Statement const* node);

    /**
     * @brief analyzes the given expression.
     * @param reporter diagnostic sink
     * @param node the target node
     * @return true if it is syntactically valid
     * @return false otherwise
     */
    bool analyze(Reporter& reporter, model::expression::Expression const* node);
};
}  // namespace shakujo::analyzer

#endif  //SHAKUJO_ANALYZER_SYNTAX_VALIDATOR_H_
