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
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include "shakujo/parser/Parser.h"
#include "shakujo/analyzer/Analyzer.h"
#include "shakujo/analyzer/AnalyzerContext.h"
#include "shakujo/analyzer/Optimizer.h"
#include "shakujo/analyzer/binding/BindingSerializer.h"
#include "shakujo/common/schema/ConfigurableStorageInfoProvider.h"
#include "shakujo/common/util/JsonSerializer.h"

namespace shakujo::example::analyzer {

static void initialize_functions(shakujo::analyzer::AnalyzerContext& context) {
    using FunctionBinding = shakujo::analyzer::binding::FunctionBinding;
    namespace t = shakujo::common::core::type;
    std::vector<std::shared_ptr<FunctionBinding>> overload_candidates{};
    {
        // INT SUM(C1 INT)
        std::vector<FunctionBinding::Parameter> params{};
        auto resolved = std::make_shared<FunctionBinding>(
                context.binding_context().next_function_id(),
                shakujo::common::core::Name("SUM"),
                std::make_unique<t::Int>(32U),
                FunctionBinding::Quantifier::ALL,
                std::vector{
                        FunctionBinding::Parameter("arg",
                                                   std::make_unique<t::Int>(32U))
                }
        );
        overload_candidates.emplace_back(std::move(resolved));
    }
    {
        // DOUBLE SUM(C1 DOUBLE)
        std::vector<FunctionBinding::Parameter> params{};
        auto resolved = std::make_shared<FunctionBinding>(
                context.binding_context().next_function_id(),
                shakujo::common::core::Name("SUM"),
                std::make_unique<t::Float>(64U),
                FunctionBinding::Quantifier::ALL,
                std::vector{
                        FunctionBinding::Parameter("arg",
                                                   std::make_unique<t::Float>(64U))
                }
        );
        overload_candidates.emplace_back(std::move(resolved));
    }
    context.register_builtin(
            std::make_unique<FunctionBinding>(
                    context.binding_context().next_function_id(),
                    shakujo::common::core::Name("SUM"),
                    overload_candidates)
    );
}

static std::shared_ptr<common::schema::ConfigurableStorageInfoProvider> table_definitions() {
    auto tables = std::make_shared<common::schema::ConfigurableStorageInfoProvider>();
    tables->add(common::schema::TableInfo {
            "example",
            {
                    { // C1 INT64 NOT NULL
                            "C1",
                            common::core::type::Int(64U, common::core::Type::Nullity::NEVER_NULL),
                    },
                    { // C2 FLOAT64 NULL
                            "C2",
                            common::core::type::Float(64U),
                    },
                    { // C3 CHAR(20) NOT NULL DEFAULT "Hello, shakujo!"
                            "C3",
                            common::core::type::Char(20U, common::core::Type::Nullity::NEVER_NULL),
                            common::core::value::String("Hello, shakujo!"),
                    },
            },
            {  // primary key
                    {
                            { "C1" }
                    }
            },
            {  // secondary keys
                    {
                            "example_C2",
                            {
                                    { "C2" }
                            },
                    },
            }
    });
    tables->add(common::schema::TableInfo {
            "side",
            {
                    { // K INT64 NOT NULL
                            "K",
                            common::core::type::Int(64U, common::core::Type::Nullity::NEVER_NULL),
                    },
                    { // C2 FLOAT64 NOT NULL
                            "VALUE",
                            common::core::type::Float(64U),
                            common::core::value::Float(0.0),
                    },
            },
            {  // primary key
                    {
                            { "K" }
                    }
            },
            {  // secondary keys
                    {
                            "side_VALUE",
                            {
                                    { "VALUE" }
                            },
                    },
            }
    });
    return tables;
}

static int run(std::vector<char*> const& args) {
    if (args.size() != 2U) {
        std::cout << "usage: " << args[0] << " <shakujo program text>" << std::endl;
        return -1;
    }
    std::string input { args[1] };
    std::cout << "input:" << std::endl;
    std::cout << "    " << input << std::endl;
    std::cout << std::endl;

    // syntactic analysis
    shakujo::parser::Parser parser;
    auto program = parser.parse_program("<argv[1]>", input);
    if (!program) {
        std::cout << "parse error";
        return -1;
    }

    // semantic analysis
    shakujo::analyzer::AnalyzerContext context { table_definitions() };
    initialize_functions(context);
    shakujo::analyzer::Analyzer analyzer;
    analyzer.analyze(context, program.get());

    shakujo::analyzer::Optimizer optimizer { context.binding_context() };
    optimizer(program.get());

    // show diagnostics
    std::cout << "diagnostics:" << std::endl;
    if (context.reporter().diagnostics().empty()) {
        std::cout << "    " << "no diagnostics" << std::endl;
    } else {
        for (auto& d : context.reporter().diagnostics()) {
            std::cout << "    " << d << std::endl;
        }
    }
    std::cout << std::endl;

    // pretty printing
    std::cout << "IR tree: ";
    common::util::JsonSerializer json { std::cout };
    shakujo::analyzer::binding::BindingSerializer serializer { context.binding_context() };
    serializer.serialize(json, program.get());
    std::cout << std::endl;

    if (context.reporter().saw_error()) {
        return 1;
    }
    return 0;
}

}  // namespace shakujo::example::analyzer

int main(int argc, char* argv[]) {
    try {
        return shakujo::example::analyzer::run(std::vector<char*> { argv, argv + argc });  // NOLINT
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}