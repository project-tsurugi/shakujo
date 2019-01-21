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
#ifndef SHAKUJO_PARSER_PARSER_H_
#define SHAKUJO_PARSER_PARSER_H_

#include <iostream>
#include <memory>
#include <string>

#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::parser {

/**
 * @brief parses source program.
 */
class Parser {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief constructs a new object.
     */
    Parser();

    /**
     * @brief destroys this object.
     */
    ~Parser() noexcept;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    Parser(Parser const& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Parser& operator=(Parser const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Parser(Parser&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Parser& operator=(Parser&& other) noexcept = default;

    // FIXME: logger instead
    /**
     * @brief sets enable debugging.
     * @param on the debug flag
     * @return this
     */
    Parser& debug(bool on);

    /**
     * @brief parses source input and parses it as a program model.
     * @param path the input path
     * @param input the source input stream, must be UTF-8 formatted
     * @return the parsed model
     */
    std::unique_ptr<model::program::Program> parse_program(std::string path, std::istream& input);

    /**
     * @brief parses source input and parses it as a program model.
     * @param path the input path
     * @param input the source input, must be UTF-8 formatted
     * @return the parsed model
     */
    std::unique_ptr<model::program::Program> parse_program(std::string path, std::string const& input);

    /**
     * @brief parses source input and parses it as a statement model.
     * @param path the input path
     * @param input the source input stream, must be UTF-8 formatted
     * @return the parsed model
     */
    std::unique_ptr<model::statement::Statement> parse_statement(std::string path, std::istream& input);

    /**
     * @brief parses source input and parses it as an expression model.
     * @param path the input path
     * @param input the source input stream, must be UTF-8 formatted
     * @return the parsed model
     */
    std::unique_ptr<model::expression::Expression> parse_expression(std::string path, std::istream& input);
};
}  // namespace shakujo::parser

#endif  // SHAKUJO_PARSER_PARSER_H_
