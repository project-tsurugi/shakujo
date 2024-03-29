/*
 * Copyright 2018-2024 Project Tsurugi.
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
#ifndef SHAKUJO_ANALYZER_OPTIMIZE_FIX_RELATION_INFO_H_
#define SHAKUJO_ANALYZER_OPTIMIZE_FIX_RELATION_INFO_H_

#include "Context.h"

#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::analyzer::optimize {

class FixRelationInfo {
public:
    explicit FixRelationInfo(Context& context) : context_(context) {}
    void operator()(model::program::Program *node);
    void operator()(model::statement::Statement *node);
    void operator()(model::expression::Expression *node);

private:
    Context& context_;
};

}  // namespace shakujo::analyzer::optimize

#endif  //SHAKUJO_ANALYZER_OPTIMIZE_FIX_RELATION_INFO_H_
