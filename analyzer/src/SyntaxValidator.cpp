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
#include "shakujo/analyzer/SyntaxValidator.h"

#include <sstream>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/Node.h"
#include "shakujo/model/util/ConstNodeWalker.h"

namespace shakujo::analyzer {
namespace {

using common::util::is_defined;
using common::util::is_valid;
using common::util::to_string;

class Engine final : public model::util::ConstNodeWalker {
private:
    Reporter& reporter_;

    template<typename... Args>
    void report(model::Node const* node, Diagnostic::Code code, const Args&... args) {
        auto message = to_string(args...);
        reporter_.report({ node->region(), code, std::move(message) });
    }

public:
    explicit Engine(Reporter& reporter) : reporter_(reporter) {}

protected:
    using ConstNodeWalker::enter;
    using ConstNodeWalker::enterDefault;

    bool enterDefault(model::program::GlobalDeclaration const*) override {
        return true;
    }

    bool enterDefault(model::name::Name const*) override {
        return true;
    }

    bool enterDefault(model::expression::Expression const*) override {
        return true;
    }

    bool enterDefault(model::statement::Statement const*) override {
        return true;
    }

    bool enterDefault(model::type::Type const*) override {
        return true;
    }

    bool enter(model::program::Program const* node) override {
        if (!is_defined(node->main())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "program body must be defined");
        }
        return true;
    }

    bool enter(model::program::GlobalVariableDeclaration const* node) override {
        if (!is_defined(node->name())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "global variable declaration must have a valid name");
        }
        if (!is_defined(node->type()) && !is_defined(node->value())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT,
                   "global variable declaration \"", node->name() , "\" must have either type or initial value");
        }
        return true;
    }

    //bool enter(model::program::GlobalFunctionDeclaration const* node) override {
    //    if (!is_defined(node->name())) {
    //        report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "global function must have a valid name");
    //    }
    //    // FIXME impl
    //
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::name::SimpleName const* node) override {
        if (node->token().empty()) {
            report(node, Diagnostic::Code::INVALID_NAME, "simple name must have one or more characters");
        }
        return true;
    }

    bool enter(model::name::QualifiedName const* node) override {
        if (!is_defined(node->qualifier())) {
            report(node, Diagnostic::Code::INVALID_NAME, "qualified name must have a valid qualifier");
        }
        if (!is_defined(node->name())) {
            report(node, Diagnostic::Code::INVALID_NAME, "qualified name must have a valid name");
        }
        return true;
    }

    bool enter(model::statement::BlockStatement const* node) override {
        for (auto element : node->elements()) {
            if (!is_defined(element)) {
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "block statement must have a valid element statement");
            }
        }
        return true;
    }

    bool enter(model::statement::LocalVariableDeclaration const* node) override {
        if (!is_defined(node->name())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "local variable declaration must have a valid name");
        }
        if (!is_defined(node->type()) && !is_defined(node->value())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT,
                   "local variable declaration \"", node->name() , "\" must have either type or initial value");
        }
        return true;
    }

    bool enter(model::statement::BranchStatement const* node) override {
        if (!is_defined(node->condition())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "branch statement must have a valid condition expression");
        }
        if (!is_defined(node->then_statement())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "branch statement must have a valid then statement");
        }
        return true;
    }

    //bool enter(model::statement::WhileStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::ForStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::ForEachStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::ReturnStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::AnchorDeclaration const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::BreakStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::ContinueStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::statement::ExpressionStatement const* node) override {
        if (!is_defined(node->body())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "expression statement must have a valid body expression");
        }
        return ConstNodeWalker::enter(node);
    }

    //bool enter(model::statement::RaiseStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::statement::LogStatement const* node) override {
        if (!is_defined(node->value())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "log statement must have a valid message expression");
        }
        return true;
    }

    bool enter(model::expression::AssignExpression const* node) override {
        if (!is_defined(node->name())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "assign statement must have a valid variable name");
        }
        if (!is_defined(node->value())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "assign statement must have a valid right hand side");
        }
        return true;
    }

    bool enter(model::expression::BinaryOperator const* node) override {
        if (node->operator_kind() == model::expression::BinaryOperator::Kind::INVALID) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "binary operator must have a valid operator");
        }
        if (!is_defined(node->left())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "binary operator must have a valid left term");
        }
        if (!is_defined(node->right())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "binary operator must have a valid right term");
        }
        return ConstNodeWalker::enter(node);
    }

    bool enter(model::expression::BlockExpression const* node) override {
        for (auto e : node->head()) {
            if (!is_defined(e)) {
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "block expression must have valid statements");
            }
        }
        if (!is_defined(node->body())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "block expression must have an valid body expression");
        }
        return ConstNodeWalker::enter(node);
    }

    //bool enter(model::expression::CaseExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::CursorAdvanceExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::CursorCreationExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::CursorElementLoadExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::expression::FunctionCall const* node) override {
        using Quantifier = model::expression::FunctionCall::Quantifier;
        if (!is_defined(node->name())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "function call must have a valid name");
        }
        switch (node->quantifier()) {
            case Quantifier::ABSENT:
                break; // don't care
            case Quantifier::ALL:
            case Quantifier::DISTINCT:
                if (node->arguments().size() != 1U) {
                    report(node, Diagnostic::Code::INVALID_SET_QUANTIFIER,
                        "function call with set quantifier must have just one argument");
                }
                break;
            case Quantifier::ASTERISK:
                if (!node->arguments().empty()) {
                    report(node, Diagnostic::Code::INVALID_SET_QUANTIFIER,
                        "function call with * (asterisk) argument must not have any other arguments");
                }
                break;
            default:
                std::abort();
        }
        return ConstNodeWalker::enter(node);
    }

    bool enter(model::expression::ImplicitCast const*) override {
        // never appear in syntax
        return true;
    }

    bool enter(model::expression::Literal const* node) override {
        if (!is_defined(node->type())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "literal must have a valid type");
        }
        if (!is_defined(node->value())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "literal must have a valid value");
        }
        return ConstNodeWalker::enter(node);
    }

    //bool enter(model::expression::StringOperator const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::TypeOperator const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::expression::UnaryOperator const* node) override {
        if (node->operator_kind() == model::expression::UnaryOperator::Kind::INVALID) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "unary operator must have a valid operator");
        }
        if (!is_defined(node->operand())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "unary operator must have a valid term");
        }
        return true;
    }

    bool enter(model::expression::VariableReference const* node) override {
        // FIXME: allow unnamed variable reference?
        if (!is_defined(node->name())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "variable reference must have a valid name");
        }
        return true;
    }

    bool enter(model::expression::Placeholder const* node) override {
        report(node, Diagnostic::Code::UNEXPECTED_ELEMENT, "expression placeholder must have been resolved");
        return true;
    }

    //bool enter(model::expression::ArrayCreationExpression const* node) override {
    //    // FIXME impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::ArrayElementLoadExpression const* node) override {
    //    // FIXME impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::ArrayElementStoreExpression const* node) override {
    //    // FIXME impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::VectorCreationExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::VectorElementLoadExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::VectorElementStoreExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::VectorLengthExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::TupleCreationExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::expression::TupleElementLoadExpression const* node) override {
        if (!is_defined(node->data())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "tuple element load expression must have a valid tuple expression");
        }
        if (!is_valid(node->index())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "tuple element load expression must have a valid index");
        }
        return true;
    }

    bool enter(model::expression::TupleElementStoreExpression const* node) override {
        if (!is_defined(node->data())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "tuple element store expression must have a valid tuple expression");
        }
        if (!is_valid(node->index())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "tuple element store expression must have a valid index");
        }
        if (!is_defined(node->value())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "tuple element store expression must have a valid right hand side");
        }
        return true;
    }

    bool enter(model::expression::relation::ScanExpression const* node) override {
        if (!is_defined(node->table())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "scan expression must have a valid source name");
        }
        return true;
    }

    bool enter(model::expression::relation::ProjectionExpression const* node) override {
        if (!is_defined(node->operand())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "projection expression must have a valid operand");
        }
        for (auto s : node->initialize()) {
            if (!is_defined(s)) {
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "projection expression must not have undefined initialization statement");
            }
        }
        if (node->columns().empty()) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "projection expression must have one or more columns");
        }
        for (auto column : node->columns()) {
            if (!is_defined(column->value())) {
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "projection expression must have valid column expression");
            }
        }
        return true;
    }

    bool enter(model::expression::relation::SelectionExpression const* node) override {
        if (!is_defined(node->operand())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "selection expression must have a valid operand");
        }
        if (!is_defined(node->condition())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "selection expression must have a valid condition expression");
        }
        return true;
    }

    bool enter(model::expression::relation::JoinExpression const* node) override {
        using Kind = model::expression::relation::JoinExpression::Kind;
        if (!is_defined(node->left()) || !is_defined(node->right())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "join expression must have a valid operand");
        }
        switch (node->operator_kind()) {
            case Kind::INNER:
            case Kind::LEFT_OUTER:
            case Kind::RIGHT_OUTER:
            case Kind::FULL_OUTER:
                // don't care whether or not join specification exists
                break;

            case Kind::CROSS:
            case Kind::NATURAL_INNER:
            case Kind::NATURAL_LEFT_OUTER:
            case Kind::NATURAL_RIGHT_OUTER:
            case Kind::NATURAL_FULL_OUTER:
            case Kind::UNION_OUTER:
                if (is_defined(node->condition())) {
                    report(node, Diagnostic::Code::UNEXPECTED_ELEMENT,
                        node->operator_kind(), " must not have any join predicates");
                }
                break;

            case Kind::LEFT_SEMI:
            case Kind::RIGHT_SEMI:
                if (!is_defined(node->condition())) {
                    report(node, Diagnostic::Code::UNDEFINED_ELEMENT,
                        node->operator_kind(), " must have a join predicate");
                }
                break;

            case Kind::INVALID:
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "join expression must have a valid operator");
                break;
        }
        return true;
    }

    //bool enter(model::expression::relation::DistinctExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::relation::AggregationExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::relation::OrderExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::expression::relation::LimitExpression const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::VectorElementDeleteStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::VectorElementInsertStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::statement::ddl::CreateTableStatement const* node) override {
        if (!is_defined(node->table())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "create table statement must have a valid table name");
        }
        if (node->columns().empty()) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "create table statement must have one or more columns");
        }
        for (auto column : node->columns()) {
            if (!is_defined(column->name())) {
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "table column must have a valid name");
            }
            if (!is_defined(column->type())) {
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "table column must have a valid type");
            }
        }
        return true;
    }

    //bool enter(model::statement::ddl::DropTableStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::dml::DeleteStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::statement::dml::EmitStatement const* node) override {
        if (!is_defined(node->source())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "emit statement must have a valid source expression");
        }
        return true;
    }

    //bool enter(model::statement::dml::InsertRelationStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    bool enter(model::statement::dml::InsertValuesStatement const* node) override {
        if (!is_defined(node->table())) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "insert statement must have a valid target table name");
        }
        if (node->columns().empty()) {
            report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "insert statement must have a valid column specification");
        }
        for (auto column : node->columns()) {
            if (!is_defined(column->value())) {
                report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "insert statement must have a valid column value");
            }
        }
        bool saw_explicit_name = false;
        for (auto* column : node->columns()) {
            if (is_defined(column->name())) {
                saw_explicit_name = true;
                break;
            }
        }
        if (saw_explicit_name) {
            for (auto* column : node->columns()) {
                if (!is_defined(column->name())) {
                    report(node, Diagnostic::Code::UNDEFINED_ELEMENT, "insert statement must have a valid column name");
                }
            }
        }
        return true;
    }

    //bool enter(model::statement::dml::UpdateStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::statement::transaction::TransactionBlockStatement const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::type::ArrayType const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::type::RelationType const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::type::TupleType const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}

    //bool enter(model::type::VectorType const* node) override {
    //    // FIXME: impl
    //    return ConstNodeWalker::enter(node);
    //}
};
}  // namespace

template<typename T>
static bool do_analyze(Reporter &reporter, T& node) {
    Reporter next;
    Engine { next }.walk(node);
    for (auto& d : next.diagnostics()) {
        reporter.report(d);
    }
    return !next.saw_error();
}

bool SyntaxValidator::analyze(Reporter &reporter, model::program::Program const* node) {
    return do_analyze(reporter, node);
}

bool SyntaxValidator::analyze(Reporter &reporter, model::statement::Statement const* node) {
    return do_analyze(reporter, node);
}

bool SyntaxValidator::analyze(Reporter &reporter, model::expression::Expression const* node) {
    return do_analyze(reporter, node);
}
}  // namespace shakujo::analyzer
