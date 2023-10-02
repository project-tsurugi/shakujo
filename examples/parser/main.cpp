/*
 * Copyright 2018-2023 tsurugi project..
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
#include "shakujo/parser/Parser.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include "shakujo/model/util/NodeSerializer.h"
#include "shakujo/common/util/JsonSerializer.h"

namespace shakujo::example::parser {

static int run(std::vector<char*> const& args) {
    if (args.size() != 2U) {
        std::cout << "usage: " << args[0] << " <shakujo program text>" << std::endl;
        return -1;
    }
    std::string input { args[1] };
    std::cout << "input: " << input << std::endl;

    shakujo::parser::Parser parser;
    std::cout << "parse tree: ";
    auto result = parser.parse_program("<stdin>", input);
    if (!result) {
        return -1;
    }

    std::cout << "IR tree: ";
    common::util::JsonSerializer json { std::cout };
    model::util::NodeSerializer serializer {};
    serializer.serialize(json, result.get());
    std::cout << std::endl;
    return 0;
}
}  // namespace shakujo::example::parser

int main(int argc, char* argv[]) {
    try {
        return shakujo::example::parser::run(std::vector<char*> { argv, argv + argc });  // NOLINT
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
