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
#include "shakujo/parser/Parser.h"

#include <iostream>
#include <string>
#include <sstream>

#include "shakujo/model/util/NodeSerializer.h"
#include "shakujo/common/util/JsonSerializer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " <shakujo program text>" << std::endl;
        return -1;
    }
    std::cout << "input: " << argv[1] << std::endl;
    std::istringstream ss { argv[1] };

    shakujo::parser::Parser parser;
    parser.debug(true);
    std::cout << "parse tree: ";
    auto result = parser.parse_program("<stdin>", ss);
    if (!result) {
        return -1;
    }

    std::cout << "IR tree: ";
    shakujo::common::util::JsonSerializer json { std::cout };
    shakujo::model::util::NodeSerializer serializer {};
    serializer.serialize(json, result.get());
    std::cout << std::endl;
    return 0;
}
