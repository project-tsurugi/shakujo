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
#include <functional>
#include <memory>
#include <string>

#include "antlr4-runtime.h"
#include "ShakujoLexer.h"
#include "ShakujoParser.h"

#include "impl/Engine.h"

namespace shakujo::parser {

using namespace shakujo_lang;

class Parser::Impl {
public:
    bool debug_;

    void debug(antlr4::RuleContext *context, antlr4::Parser &parser) {
        if (debug_) {
            std::cout << context->toStringTree(&parser) << std::endl;
        }
    }

    template<class In, class Context>
    auto parse(std::string&& path, In& input, std::function<Context*(ShakujoParser&)> entry) {
        antlr4::ANTLRInputStream is { input };
        ShakujoLexer lexer { &is };
        antlr4::CommonTokenStream tokens { &lexer };
        ShakujoParser parser { &tokens };

        auto tree = entry(parser);
        debug(tree, parser);
        impl::Engine engine { std::move(path) };
        return engine.visit(tree);
    }
};

Parser::Parser() : impl_(new Impl()) {}

Parser::~Parser() noexcept = default;

Parser& Parser::debug(bool on) {
    impl_->debug_ = on;
    return *this;
}

std::unique_ptr<model::program::Program> Parser::parse_program(std::string path, std::istream& input) {
    std::function<ShakujoParser::ProgramEntryContext*(ShakujoParser&)> entry = &ShakujoParser::programEntry;
    return impl_->parse(std::move(path), input, entry);
}

std::unique_ptr<model::program::Program> Parser::parse_program(std::string path, std::string const& input) {
    std::function<ShakujoParser::ProgramEntryContext*(ShakujoParser&)> entry = &ShakujoParser::programEntry;
    return impl_->parse(std::move(path), input, entry);
}

std::unique_ptr<model::statement::Statement> Parser::parse_statement(std::string path, std::istream& input) {
    std::function<ShakujoParser::StatementEntryContext*(ShakujoParser&)> entry = &ShakujoParser::statementEntry;
    return impl_->parse(std::move(path), input, entry);
}

std::unique_ptr<model::statement::Statement> Parser::parse_statement(std::string path, std::string const& input) {
    std::function<ShakujoParser::StatementEntryContext*(ShakujoParser&)> entry = &ShakujoParser::statementEntry;
    return impl_->parse(std::move(path), input, entry);
}

std::unique_ptr<model::expression::Expression> Parser::parse_expression(std::string path, std::istream& input) {
    std::function<ShakujoParser::ExpressionEntryContext*(ShakujoParser&)> entry = &ShakujoParser::expressionEntry;
    return impl_->parse(std::move(path), input, entry);
}

std::unique_ptr<model::expression::Expression> Parser::parse_expression(std::string path, std::string const& input) {
    std::function<ShakujoParser::ExpressionEntryContext*(ShakujoParser&)> entry = &ShakujoParser::expressionEntry;
    return impl_->parse(std::move(path), input, entry);
}
}  // namespace shakujo::parser
