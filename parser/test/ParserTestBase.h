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
#ifndef PARSER_TEST_BASE_H_
#define PARSER_TEST_BASE_H_

#include "shakujo/parser/Parser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#include "shakujo/model/IRFactory.h"

#include "shakujo/model/util/NodeSerializer.h"
#include "shakujo/common/util/JsonSerializer.h"

namespace shakujo::parser {

auto NON_NULL = shakujo::common::core::Type::Nullity::NEVER_NULL;

using common::util::dynamic_pointer_cast;

class ParserTestBase {
public:
    parser::Parser parser;
    model::IRFactory f;

    std::unique_ptr<model::program::Program> parse_program(std::string const& text) {
        std::cout << "== INPUT ==" << std::endl << text << std::endl << "===========" << std::endl;
        return parser.parse_program("<testing>", text);
    }

    template<typename T = model::statement::Statement>
    std::unique_ptr<T> parse_program_main(std::string const& text) {
        auto program = parse_program(text);
        auto result = dynamic_pointer_cast<T>(program->release_main());
        return result;
    }

    template<typename T = model::expression::Expression>
    std::unique_ptr<T> parse_expression(std::string const& text) {
        auto node = parser.parse_expression("<testing>", text);
        auto result = dynamic_pointer_cast<T>(std::move(node));
        return result;
    }

    template<class T>
    typename T::type value_of(model::expression::Expression const* node) const {
        using common::util::dynamic_pointer_cast;
        auto literal = dynamic_pointer_cast<model::expression::Literal>(node);
        auto value = dynamic_pointer_cast<T>(literal->value());
        return value->get();
    }

    std::string name_of(model::expression::Expression const* node) const {
        using common::util::dynamic_pointer_cast;
        auto ref = dynamic_pointer_cast<model::expression::VariableReference>(node);
        auto name = dynamic_pointer_cast<model::name::SimpleName>(ref->name());
        return name->token();
    }

    template<typename T>
    std::string to_string(const T& node) {
        std::ostringstream ss;
        shakujo::common::util::JsonSerializer json { ss };
        shakujo::model::util::NodeSerializer serializer {};
        serializer.serialize(json, node);
        return ss.str();
    }
};
}  // namespace shakujo::parser

#endif  // PARSER_TEST_BASE_H_