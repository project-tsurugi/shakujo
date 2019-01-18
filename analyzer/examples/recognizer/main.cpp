/*
 * Copyright 2018-2018 shakujo project.
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
#include <string>
#include <sstream>

#include "shakujo/parser/Parser.h"
#include "shakujo/analyzer/Analyzer.h"
#include "shakujo/analyzer/AnalyzerContext.h"
#include "shakujo/analyzer/binding/BindingSerializer.h"
#include "shakujo/common/schema/ConfigurableStorageInfoProvider.h"
#include "shakujo/common/util/JsonSerializer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <shakujo program text>" << std::endl;
        return -1;
    }
    std::cout << "input:" << std::endl;
    std::cout << "    " << argv[1] << std::endl;
    std::cout << std::endl;

    // syntactic analysis
    std::istringstream ss { argv[1] };
    shakujo::parser::Parser parser;
    auto program = parser.parse_program("<argv[1]>", ss);
    if (!program) {
        std::cout << "parse error";
        return -1;
    }

    // semantic analysis
    auto tables = std::make_shared<shakujo::common::schema::ConfigurableStorageInfoProvider>();
    tables->add(shakujo::common::schema::TableInfo {
        "example",
        {
            { // C1 INT64 NOT NULL
                "C1",
                shakujo::common::core::type::Int(64U, shakujo::common::core::Type::Nullity::NEVER_NULL),
            },
            { // C2 FLOAT64 NULL
                "C2",
                shakujo::common::core::type::Float(64U),
            },
            { // C3 CHAR(20) NOT NULL DEFAULT "Hello, shakujo!"
                "C3",
                shakujo::common::core::type::Char(20U, shakujo::common::core::Type::Nullity::NEVER_NULL),
                shakujo::common::core::value::String("Hello, shakujo!"),
            },
        }
    });
    shakujo::analyzer::AnalyzerContext context { tables };
    shakujo::analyzer::Analyzer analyzer;
    analyzer.analyze(context, program.get());

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
    shakujo::common::util::JsonSerializer json { std::cout };
    shakujo::analyzer::binding::BindingSerializer serializer { context.binding_context() };
    serializer.serialize(json, program.get());
    std::cout << std::endl;

    if (context.reporter().saw_error()) {
        return 1;
    }
    return 0;
}
