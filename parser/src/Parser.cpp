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

#include <glog/logging.h>

#include "antlr4-runtime.h"
#include "ShakujoLexer.h"
#include "ShakujoParser.h"

#include "impl/Engine.h"

namespace shakujo::parser {

using namespace shakujo_lang;

class ErrorListener : public antlr4::BaseErrorListener {
public:
    explicit ErrorListener(std::string_view path) noexcept : path_(path) {}

    void syntaxError(
            antlr4::Recognizer* recognizer,
            antlr4::Token* offendingSymbol,
            std::size_t line,
            std::size_t charPositionInLine,
            std::string const& msg,
            std::exception_ptr e) override {
        common::core::DocumentRegion region { path_, { line, charPositionInLine, } };
        throw Parser::Exception(msg, std::move(region));
    }

    void reportAmbiguity(
            antlr4::Parser *recognizer,
            antlr4::dfa::DFA const &dfa,
            std::size_t startIndex,
            std::size_t stopIndex,
            bool exact,
            antlrcpp::BitSet const& ambigAlts,
            antlr4::atn::ATNConfigSet *configs) override {
        (void) recognizer;
        (void) dfa;
        (void) startIndex;
        (void) stopIndex;
        (void) exact;
        (void) ambigAlts;
        (void) configs;
        VLOG(2) << "ambiguity" << std::endl;
    }

    void reportAttemptingFullContext(
            antlr4::Parser *recognizer,
            antlr4::dfa::DFA const& dfa,
            std::size_t startIndex,
            std::size_t stopIndex,
            antlrcpp::BitSet const& conflictingAlts,
            antlr4::atn::ATNConfigSet *configs) override {
        (void) recognizer;
        (void) dfa;
        (void) startIndex;
        (void) stopIndex;
        (void) conflictingAlts;
        (void) configs;
        VLOG(2) << "attempting full context" << std::endl;
    }

    void reportContextSensitivity(
            antlr4::Parser *recognizer,
            antlr4::dfa::DFA const& dfa,
            std::size_t startIndex,
            std::size_t stopIndex,
            std::size_t prediction,
            antlr4::atn::ATNConfigSet* configs) override {
        (void) recognizer;
        (void) dfa;
        (void) startIndex;
        (void) stopIndex;
        (void) prediction;
        (void) configs;
        VLOG(2) << "context sensitivity" << std::endl;
    }

private:
    std::string_view path_;
};

class Parser::Impl {
public:
    void debug(antlr4::RuleContext *context, antlr4::Parser &parser) {
        VLOG(2) << context->toStringTree(&parser) << std::endl;
    }

    template<class In, class Context>
    auto parse(std::string&& path, In& input, std::function<Context*(ShakujoParser&)> entry) {
        antlr4::ANTLRInputStream is { input };
        ShakujoLexer lexer { &is };
        antlr4::CommonTokenStream tokens { &lexer };
        ShakujoParser parser { &tokens };

        ErrorListener listener { path };
        parser.removeErrorListeners();
        parser.addErrorListener(&listener);

        auto tree = entry(parser);
        debug(tree, parser);
        impl::Engine engine { parser, std::move(path) };
        return engine.visit(tree);
    }
};

Parser::Parser() : impl_(new Impl()) {}

Parser::~Parser() noexcept = default;

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
