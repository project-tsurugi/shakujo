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
#include "Engine.h"

#include <cassert>
#include <map>
#include <stdexcept>
#include <sstream>
#include <set>
#include <vector>
#include <type_traits>

#include "typing.h"
#include "RelationScope.h"
#include "AggregationAnalyzer.h"

#include "shakujo/common/util/utility.h"

#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/value/Null.h"

namespace shakujo::analyzer::analyze {

using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;
using common::util::make_clone;
using common::util::equals;
using common::util::is_defined;
using common::util::is_valid;
using common::util::to_string;

void Engine::bless(model::key::ExpressionKey::Provider* node, std::shared_ptr<binding::ExpressionBinding> binding) {
    auto key = bindings().create_key(std::move(binding));
    node->expression_key(std::move(key));
}

void Engine::bless(model::key::VariableKey::Provider* node, std::shared_ptr<binding::VariableBinding> binding) {
    auto key = bindings().create_key(std::move(binding));
    node->variable_key(std::move(key));
}

void Engine::bless(model::key::FunctionKey::Provider* node, std::shared_ptr<binding::FunctionBinding> binding) {
    auto key = bindings().create_key(std::move(binding));
    node->function_key(std::move(key));
}

void Engine::bless(model::key::RelationKey::Provider* node, std::shared_ptr<binding::RelationBinding> binding) {
    auto key = bindings().create_key(std::move(binding));
    node->relation_key(std::move(key));
}

void Engine::visit(model::program::GlobalVariableDeclaration* node, ScopeContext& scope) {
    common::core::Name name { node->name()->segments() };
    std::shared_ptr<binding::ExpressionBinding> expr;
    if (is_defined(node->value())) {
        dispatch(node->value(), scope);
        expr = extract_binding(node->value());
    }
    std::shared_ptr<binding::VariableBinding> result;
    if (!is_defined(node->type())) {
        if (is_valid(expr)) {
            // propagate value type even if it is not valid
            result = std::make_shared<binding::VariableBinding>(
                    bindings().next_variable_id(),
                    name,
                    make_clone(expr->type()));
            // FIXME: propagate values
        } else {
            // may not come here; this is syntactic error
            report(node, Diagnostic::Code::INVALID_VARIABLE_TYPE, to_string(
                    "type of variable \"", name, "\" is not inferrable"));
            result = std::make_shared<binding::VariableBinding>(
                    bindings().next_variable_id(),
                    name,
                    std::make_unique<common::core::type::Error>());
        }
    } else {
        auto type = typing::convert(node->type());
        if (is_valid(expr)) {
            // FIXME: check conversion rule
            if (!typing::is_assignment_convertible(type.get(), *expr, false)) {
                report(node, Diagnostic::Code::INVALID_VARIABLE_TYPE, to_string(
                        "type of variable \"", name, "\" is not compatible to its initializer, ",
                        "variable type: ", type.get(), ", ",
                        "initializer type: ", expr->type()));
                result = std::make_shared<binding::VariableBinding>(
                        bindings().next_variable_id(),
                        name,
                        std::make_unique<common::core::type::Error>());
            }
        } else {
            result = std::make_shared<binding::VariableBinding>(
                    bindings().next_variable_id(),
                    name,
                    std::move(type));
        }
    }
    scope.variables().insert(node->name(), result);
    bless(node, result);
}

void Engine::visit(model::statement::EmptyStatement*, ScopeContext&) {
    // nothing to do
}

void Engine::visit(model::statement::BlockStatement* node, ScopeContext& prev) {
    auto vars = block_scope(prev.variables());
    ScopeContext scope { vars, prev.functions() };

    for (auto element : node->elements()) {
        dispatch(element, scope);
    }
}

void Engine::visit(model::statement::LocalVariableDeclaration* node, ScopeContext& scope) {
    // FIXME: refactor with GlobalVariableDeclaration
    common::core::Name name { node->name()->segments() };
    std::shared_ptr<binding::ExpressionBinding> expr;
    if (is_defined(node->value())) {
        dispatch(node->value(), scope);
        expr = extract_binding(node->value());
    }
    std::shared_ptr<binding::VariableBinding> result;
    if (!is_defined(node->type())) {
        if (is_valid(expr)) {
            // propagate value type even if it is not valid
            result = std::make_shared<binding::VariableBinding>(
                    bindings().next_variable_id(),
                    name,
                    make_clone(expr->type()));
            // FIXME: propagate values
        } else {
            // may not come here; this is syntactic error
            report(node, Diagnostic::Code::INVALID_VARIABLE_TYPE, to_string(
                    "type of variable \"", name, "\" is not inferrable"));
            result = std::make_shared<binding::VariableBinding>(
                    bindings().next_variable_id(),
                    name,
                    std::make_unique<common::core::type::Error>());
        }
    } else {
        auto type = typing::convert(node->type());
        if (is_valid(expr)) {
            // FIXME: check conversion rule
            if (!typing::is_assignment_convertible(type.get(), *expr, false)) {
                report(node, Diagnostic::Code::INVALID_VARIABLE_TYPE, to_string(
                        "type of variable \"", name, "\" is not compatible to its initializer, ",
                        "variable type: ", *type, ", ",
                        "initializer type: ", expr->type()));
                result = std::make_shared<binding::VariableBinding>(
                        bindings().next_variable_id(),
                        name,
                        std::make_unique<common::core::type::Error>());
            }
        } else {
            result = std::make_shared<binding::VariableBinding>(
                    bindings().next_variable_id(),
                    name,
                    std::move(type));
        }
    }
    scope.variables().insert(node->name(), result);
    bless(node, result);
}

// FIXME: move
std::tuple<std::unique_ptr<model::name::Name>, std::vector<std::unique_ptr<model::name::SimpleName>>>
Engine::split_name(std::unique_ptr<model::name::Name> name, std::size_t first_length) {
    std::size_t total_length = name->size();
    assert(total_length >= first_length);  // NOLINT
    using head_t = std::unique_ptr<model::name::Name>;
    using tail_t = std::vector<std::unique_ptr<model::name::SimpleName>>;

    if (first_length == total_length) {
        return std::make_tuple<head_t, tail_t>(std::move(name), tail_t {});
    }
    tail_t tail(total_length - first_length);
    head_t current = std::move(name);
    for (std::size_t i = 0, n = tail.size(); i < n; ++i) {
        assert(is_defined(current));  // NOLINT
        switch (current->kind()) {
            case model::name::NameKind::SIMPLE_NAME: {
                tail[n - i - 1] = common::util::dynamic_pointer_cast<model::name::SimpleName>(std::move(current));
                current = {};
                break;
            }
            case model::name::NameKind::QUALIFIED_NAME: {
                auto qname = common::util::dynamic_pointer_cast<model::name::QualifiedName>(std::move(current));
                tail[n - i - 1] = qname->release_name();
                current = qname->release_qualifier();
                break;
            }
        }
    }
    return std::make_tuple<head_t, tail_t>(std::move(current), std::move(tail));
}

void Engine::visit(model::statement::BranchStatement* node, ScopeContext& prev) {
    dispatch(node->condition(), prev);

    auto vars = unit_scope(prev.variables());
    ScopeContext scope { vars, prev.functions() };

    dispatch(node->then_statement(), scope);
    if (is_defined(node->else_statement())) {
        dispatch(node->else_statement(), scope);
    }

    auto condition_expr = extract_binding(node->condition());
    if (is_valid(condition_expr)) {
        common::core::type::Bool type { common::core::Type::Nullity::NEVER_NULL };
        if (typing::is_boolean_convertible(condition_expr->type())) {
            insert_cast(node->condition(), &type);
        } else {
            std::string message = to_string(
                    "required type: ", type, ", ",
                    "expression type: ", condition_expr->type());
            report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, std::move(message));
        }
    }
}

// FIXME: while
// FIXME: for
// FIXME: for-each
// FIXME: return
// FIXME: anchor
// FIXME: break
// FIXME: continue

void Engine::visit(model::statement::ExpressionStatement* node, ScopeContext& scope) {
    dispatch(node->body(), scope);
}

// FIXME: raise

void Engine::visit(model::statement::LogStatement* node, ScopeContext& scope) {
    dispatch(node->value(), scope);
}

void Engine::insert_cast(model::expression::Expression* node, common::core::Type const* type) {
    auto expr = extract_binding(node);
    if (!equals(type, expr->type())) {
        auto* replaced = node->replace([&](auto orig) {
            return ir_factory.ImplicitCast(type, std::move(orig));
        });
        bless(replaced, type, expr->value(), expr->constant());
    }
}

void Engine::visit(model::expression::Literal* node, ScopeContext&) {
    auto type = node->type();
    auto value = node->value();
    assert(is_defined(type));  // NOLINT
    assert(is_defined(value));  // NOLINT
    switch (type->kind()) {
    case common::core::Type::Kind::BOOL:
        if (value->kind() != common::core::Value::Kind::BOOL) {
            report(node, Diagnostic::Code::INVALID_LITERAL_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            bless(node, type, value, true);
        }
        break;
    case common::core::Type::Kind::INT:
        if (value->kind() != common::core::Value::Kind::INT) {
            report(node, Diagnostic::Code::INVALID_LITERAL_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            auto t = dynamic_pointer_cast<common::core::type::Int>(type);
            auto v = dynamic_pointer_cast<common::core::value::Int>(value);
            if (v->get() < t->min_value()) {
                report(node, Diagnostic::Code::INVALID_LITERAL_VALUE,
                       to_string(value, " is too small for ", t));
                bless_erroneous_expression(node);
            } else if (v->get() > t->max_value()) {
                report(node, Diagnostic::Code::INVALID_LITERAL_VALUE,
                       to_string(value, " is too large for ", t));
                bless_erroneous_expression(node);
            } else {
                bless(node, type, value, true);
            }
        }
        break;
    case common::core::Type::Kind::FLOAT:
        if (value->kind() != common::core::Value::Kind::FLOAT) {
            report(node, Diagnostic::Code::INVALID_LITERAL_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            bless(node, type, value, true);
        }
        break;
    case common::core::Type::Kind::CHAR:
        if (value->kind() != common::core::Value::Kind::STRING) {
            report(node, Diagnostic::Code::INVALID_LITERAL_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            auto t = dynamic_pointer_cast<common::core::type::Char>(type);
            auto v = dynamic_pointer_cast<common::core::value::String>(value);
            if (v->get().length() > t->size()) {
                report(node, Diagnostic::Code::INVALID_LITERAL_VALUE,
                       to_string(value, " is too long for ", t));
                bless_erroneous_expression(node);
            } else {
                bless(node, type, value, true);
            }
        }
        break;
    case common::core::Type::Kind::STRING:
        if (value->kind() != common::core::Value::Kind::STRING) {
            report(node, Diagnostic::Code::INVALID_LITERAL_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            bless(node, type, value, true);
        }
        break;
    case common::core::Type::Kind::NULL_:
        if (value->kind() != common::core::Value::Kind::NULL_) {
            report(node, Diagnostic::Code::INVALID_LITERAL_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            bless(node, type, value, true);
        }
        break;
    default:
        throw std::domain_error("invalid syntax");
    }
}

void Engine::visit(model::expression::VariableReference* node, ScopeContext& scope) {
    auto r = scope.variables().find(node->name());
    if (!r) {
        // target variable is not found
        report(node, Diagnostic::Code::VARIABLE_NOT_FOUND, to_string(node->name()));
        bless_erroneous_expression(node);
        bless_undefined<binding::VariableBinding>(node);
        return;
    }

    auto var = r.element();
    assert(is_defined(var));  // NOLINT
    if (!var->has_id()) {
        // target variable is ambiguous
        report(node, Diagnostic::Code::VARIABLE_NOT_IDENTICAL, to_string(node->name()));
        bless_erroneous_expression(node);
        bless_undefined<binding::VariableBinding>(node);
        return;
    }

    if (!is_valid(var)) {
        // target variable is identically defined, but it is not valid
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::VariableBinding>(node);
        return;
    }

    if (equals(r.name(), node->name())) {
        // target variable is defined and perfect match to the referred name

        // if perfect match, trailing indices must not be empty
        assert(r.succeeding_indices().empty());  // NOLINT

        bless(node, var->type());
        bless(node, var);
        return;
    }

    {
        // partial name matching
        auto matched_size = is_defined(r.name()) ? r.name()->size() : 0U;
        auto [var_name, rest_names] = split_name(node->release_name(), matched_size);
        node->name(std::move(var_name));

        bless(node, var->type());
        bless(node, var);

        model::expression::Expression* current = node;

        auto required_indices = r.succeeding_indices();
        std::size_t position_index = 0;
        for (auto& next : rest_names) {
            current = current->replace([&](auto p) {
                auto index = ir_factory.Index(std::move(next));
                if (position_index < required_indices.size()) {
                    index->position(required_indices[position_index]);
                }
                return ir_factory.TupleElementLoadExpression(std::move(p), std::move(index));
            });
            process_tuple_element(dynamic_pointer_cast<model::expression::TupleElementLoadExpression>(current));
            ++position_index;
        }
    }
}

void Engine::visit(model::expression::FunctionCall* node, ScopeContext& scope) {
    std::vector<model::expression::Expression*> arguments;
    arguments.reserve(node->arguments().size());
    for (auto* argument : node->arguments()) {
        dispatch(argument, scope);
        arguments.emplace_back(argument);
    }
    auto func = resolve_function(node->name(), node->quantifier(), std::move(arguments), scope);
    if (!is_valid(func)) {
        bless_erroneous_expression(node);
        bless_undefined<binding::FunctionBinding>(node);
    } else {
        bless(node, func->type());
        bless(node, std::move(func));
    }
}

std::shared_ptr<binding::FunctionBinding> Engine::resolve_function(
        model::name::Name* name,
        model::expression::FunctionCall::Quantifier quantifier,
        std::vector<model::expression::Expression*> arguments,
        ScopeContext& scope) {
    auto r = scope.functions().find(name);
    if (!r || !equals(r.name(), name)) {
        // target function is not found
        report(name, Diagnostic::Code::FUNCTION_NOT_FOUND, to_string(name));
        return std::make_shared<binding::FunctionBinding>();
    }

    auto func = r.element();
    assert(is_defined(func));  // NOLINT
    if (!func->has_id()) {
        // target variable is ambiguous
        report(name, Diagnostic::Code::FUNCTION_NOT_IDENTICAL, to_string(name));
        return std::make_shared<binding::FunctionBinding>();
    }

    if (!is_valid(func)) {
        // target function is identically defined, but it is not valid
        return std::make_shared<binding::FunctionBinding>();
    }

    std::vector<binding::ExpressionBinding const*> argument_exprs;
    argument_exprs.reserve(arguments.size());
    for (auto* argument : arguments) {
        auto expr = extract_binding(argument);
        if (!is_valid(expr)) {
            return std::make_shared<binding::FunctionBinding>();
        }
        argument_exprs.push_back(expr.get());
    }

    if (func->is_overload_stub()) {
        using NQ = model::expression::FunctionCall::Quantifier;
        using BQ = binding::FunctionBinding::Quantifier;
        switch (quantifier) {
            case NQ::ABSENT:
                func = func->resolve_overload(argument_exprs);
                break;
            case NQ::ASTERISK:
                assert(arguments.size() == 0U);  // NOLINT
                func = func->resolve_overload(BQ::ASTERISK);
                break;
            case NQ::ALL:
                assert(arguments.size() == 1U);  // NOLINT
                func = func->resolve_overload(BQ::ALL, argument_exprs[0]);
                break;
            case NQ::DISTINCT:
                assert(arguments.size() == 1U);  // NOLINT
                func = func->resolve_overload(BQ::DISTINCT, argument_exprs[0]);
                break;
            default:
                std::abort();
        }
        if (!is_valid(func) || func->is_overload_stub()) {
            std::vector<common::core::Type const*> types;
            types.reserve(argument_exprs.size());
            for (auto* expr : argument_exprs) {
                types.push_back(expr->type());
            }
            report(name, Diagnostic::Code::FUNCTION_NOT_FOUND, to_string("name=", name, ", parameters=", types));
            return std::make_shared<binding::FunctionBinding>();
        }
    }
    {
        using NQ = model::expression::FunctionCall::Quantifier;
        using BQ = binding::FunctionBinding::Quantifier;
        bool ok;
        switch (quantifier) {
            case NQ::ABSENT:
                ok = func->quantifier() == BQ::GROUND || func->quantifier() == BQ::ALL;
                break;
            case NQ::ASTERISK:
                ok = func->quantifier() == BQ::ASTERISK;
                break;
            case NQ::ALL:
                ok = func->quantifier() == BQ::ALL;
                break;
            case NQ::DISTINCT:
                ok = func->quantifier() == BQ::DISTINCT;
                break;
            default:
                std::abort();
        }
        if (!ok) {
            report(name, Diagnostic::Code::INCOMPATIBLE_FUNCTION_QUANTIFIER, to_string(
                "name=", name, ", ",
                "required=", quantifier, ", ",
                "found=", to_string_view(func->quantifier())));
            return std::make_shared<binding::FunctionBinding>();
        }
    }
    if (arguments.size() != func->parameters().size()) {
        report(name, Diagnostic::Code::INCOMPATIBLE_FUNCTION_ARGUMENT_COUNT, to_string(
            name, " requires ", func->parameters().size(), " parameters(s), ",
            "but passed ", arguments.size()));
        return std::make_shared<binding::FunctionBinding>();
    }

    for (std::size_t i = 0, n = func->parameters().size(); i < n; ++i) {
        auto&& param = func->parameters()[i];
        auto expr = argument_exprs[i];
        // FIXME: check conversion rule
        if (!typing::is_assignment_convertible(param.type(), *expr, false)) {
            report(name, Diagnostic::Code::INCOMPATIBLE_FUNCTION_ARGUMENT_TYPE, to_string(
                "function: ", func->name(), ", ",
                "parameter at: ", i, ", ",
                "parameter type: ", param.type(), ", ",
                "expression type: ", expr->type()));
        }
        insert_cast(arguments[i], param.type());
    }

    return func;
}

void Engine::visit(model::expression::AssignExpression* node, ScopeContext& scope) {
    dispatch(node->value(), scope);
    auto r = scope.variables().find(node->name());
    if (!r) {
        // target variable is not found
        report(node, Diagnostic::Code::VARIABLE_NOT_FOUND, to_string(node->name()));
        bless_undefined<binding::VariableBinding>(node);
        bless_erroneous_expression(node);
        return;
    }

    auto var = r.element();
    assert(is_defined(var));  // NOLINT
    if (!var->has_id()) {
        // target variable is ambiguous
        report(node, Diagnostic::Code::VARIABLE_NOT_IDENTICAL, to_string(node->name()));
        bless_undefined<binding::VariableBinding>(node);
        bless_erroneous_expression(node);
        return;
    }
    if (!is_valid(var)) {
        // target variable is identically defined, but it is not valid
        bless_undefined<binding::VariableBinding>(node);
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }

    auto expr = extract_binding(node->value());
    if (!is_valid(expr)) {
        // rhs is not valid, then we don't resolve the variable
        bless_undefined<binding::VariableBinding>(node);
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }

    if (equals(r.name(), node->name())) {
        // FIXME: check conversion rule
        if (!typing::is_assignment_convertible(var->type(), *expr, false)) {
            report(node, Diagnostic::Code::INCOMPATIBLE_VARIABLE_TYPE, to_string(
                "variable type: ", var->type(), ", ",
                "expression type: ", expr->type()));
            bless_undefined<binding::VariableBinding>(node);
            bless_erroneous_expression(node);
            return;
        }
        insert_cast(node->value(), var->type());
        bless(node, var);
        bless(node, var->type()); // FIXME value
        return;
    }
    {
        // FIXME: impl
        report(node, Diagnostic::Code::NOT_IMPLEMENTED, "assign statement promotion to tuple element store");
        bless_undefined<binding::VariableBinding>(node);
        bless_erroneous_expression(node);
    }
}

void Engine::visit(model::expression::TupleElementLoadExpression* node, ScopeContext& scope) {
    dispatch(node->data(), scope);
    process_tuple_element(node);
}

void Engine::process_tuple_element(model::expression::TupleElementLoadExpression* node) {
    auto expr = extract_binding(node->data());
    if (!is_valid(expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }
    if (!require_tuple(node->data())) {
        bless_erroneous_expression(node);
        return;
    }
    auto* tuple = dynamic_pointer_cast<common::core::type::Tuple>(expr->type());
    auto type = resolve_index(node->index(), tuple);
    if (is_valid(type)) {
        bless(node, std::move(type));
    } else {
        bless_erroneous_expression(node);
    }
}

std::unique_ptr<common::core::Type> Engine::resolve_index(
        model::name::Index* index,
        common::core::type::Tuple const* type) {
    if (index->position_optional().has_value()) {
        if (index->position() >= type->elements().size()) {
            report(index, Diagnostic::Code::TUPLE_ELEMENT_NOT_FOUND, to_string(
                    "tuple index ", index->position(), " is out of bounds"));
            return {};
        }
        auto& element = type->elements()[index->position()];
        if (is_defined(index->name())) {
            if (index->name()->token() != element.name()) {
                report(index, Diagnostic::Code::TUPLE_ELEMENT_NOT_FOUND, to_string(
                        "tuple element name \"", index->name(), "\" (at ", index->position(), ") ",
                        "is inconsistent: ", element.name()));
                return {};
            }
        } else if (!element.name().empty()) {
            index->name(ir_factory.Name(element.name()));
        }
        return make_clone(element.type());
    }
    if (is_defined(index->name())) {
        std::size_t position = 0;
        std::size_t found_at = 0;
        std::unique_ptr<common::core::Type> ret;
        for (auto& element : type->elements()) {
            if (index->name()->token() == element.name()) {
                if (ret) {
                    report(index, Diagnostic::Code::TUPLE_ELEMENT_NOT_FOUND, to_string(
                            "tuple element \"", index->name(), "\" is ambiguous at ",
                            found_at, " and ", position));
                    return {};
                }
                found_at = position;
                ret = make_clone(element.type());
            }
            ++position;
        }
        if (ret) {
            index->position(found_at);
            return ret;
        }
        report(index, Diagnostic::Code::TUPLE_ELEMENT_NOT_FOUND, to_string(
                "tuple element \"", index->name(), "\" is not found"));
        return {};
    }
    report(index, Diagnostic::Code::TUPLE_ELEMENT_NOT_FOUND, to_string("tuple index is not valid"));
    return {};
}

void Engine::visit(model::expression::UnaryOperator* node, ScopeContext& scope) {
    dispatch(node->operand(), scope);
    auto expr = extract_binding(node->operand());
    if (!is_valid(expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }
    switch (node->operator_kind()) {
    case model::expression::UnaryOperator::Kind::PLUS:
    case model::expression::UnaryOperator::Kind::SIGN_INVERSION:
    {
        if (!require_numeric(node->operand())) {
            bless_erroneous_expression(node);
        } else {
            // FIXME: integral promotion?
            bless(node, expr->type());
        }
        break;
    }
    case model::expression::UnaryOperator::Kind::BITWISE_COMPLEMENT:
    {
        if (!require_integral(node->operand())) {
            bless_erroneous_expression(node);
        } else {
            // FIXME: integral promotion?
            bless(node, expr->type());
        }
        break;
    }
    case model::expression::UnaryOperator::Kind::LOGICAL_NOT:
    case model::expression::UnaryOperator::Kind::CONDITIONAL_NOT:
    {
        if (!require_boolean(node->operand())) {
            // FIXME: boolean promotion?
            bless_erroneous_expression(node);
        } else {
            bless(node, expr->type());
        }
        break;
    }
    case model::expression::UnaryOperator::Kind::IS_TRUE:
    case model::expression::UnaryOperator::Kind::IS_FALSE:
    case model::expression::UnaryOperator::Kind::IS_NOT_TRUE:
    case model::expression::UnaryOperator::Kind::IS_NOT_FALSE:
    {
        common::core::type::Bool required { expr->type()->nullity() };
        if (!typing::is_boolean_convertible(expr->type())) {
            // FIXME: boolean promotion?
            std::string message = to_string(
                    "required type: ", required, ", ",
                    "expression type: ", expr->type());
            report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, std::move(message));
            bless_erroneous_expression(node);
        } else {
            insert_cast(node->operand(), &required);
            bless(node, common::core::type::Bool(common::core::Type::Nullity::NEVER_NULL));
        }
        break;
    }
    case model::expression::UnaryOperator::Kind::IS_NULL:
    case model::expression::UnaryOperator::Kind::IS_NOT_NULL:
    {
        if (!require_atom(node->operand())) {
            bless_erroneous_expression(node);
        } else {
            bless(node, common::core::type::Bool(common::core::Type::Nullity::NEVER_NULL));
        }
        break;
    }
    case model::expression::UnaryOperator::Kind::CHECK_NULL:
    {
        if (!require_atom(node->operand())) {
            bless_erroneous_expression(node);
        } else {
            bless(node, typing::nullity(expr->type(), common::core::Type::Nullity::NEVER_NULL));
        }
        break;
    }

    case model::expression::UnaryOperator::Kind::RELATION_ALL:
    case model::expression::UnaryOperator::Kind::RELATION_ANY:
    case model::expression::UnaryOperator::Kind::RELATION_EXISTS:
    case model::expression::UnaryOperator::Kind::RELATION_NOT_EXISTS:
    {
        report(node, Diagnostic::Code::NOT_IMPLEMENTED, "relational unary operators");
        bless_erroneous_expression(node);
        break;
    }

    case model::expression::UnaryOperator::Kind::INVALID:
        throw std::domain_error("invalid syntax");
    }
}

static bool require(bool a, bool b) {
    return a && b;
}

void Engine::visit(model::expression::BinaryOperator* node, ScopeContext& scope) {
    dispatch(node->left(), scope);
    dispatch(node->right(), scope);
    auto l_expr = extract_binding(node->left());
    auto r_expr = extract_binding(node->right());
    if (!is_valid(l_expr) || !is_valid(r_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }

    // first, we resolves overloading of some operators
    using Op = model::expression::BinaryOperator::Kind;
    switch (node->operator_kind()) {
    case Op::LOGICAL_AND:
        if (typing::is_integral(l_expr->type()) && typing::is_integral(r_expr->type())) {
            node->operator_kind(Op::BITWISE_AND);
        }
        break;
    case Op::LOGICAL_OR:
        if (typing::is_integral(l_expr->type()) && typing::is_integral(r_expr->type())) {
            node->operator_kind(Op::BITWISE_OR);
        }
        break;
    case Op::LOGICAL_XOR:
        if (typing::is_integral(l_expr->type()) && typing::is_integral(r_expr->type())) {
            node->operator_kind(Op::BITWISE_XOR);
        }
        break;
    default:
        break;
    }

    switch (node->operator_kind()) {
    case Op::ADD:
    case Op::SUBTRACT:
    case Op::MULTIPLY:
    case Op::DIVIDE:
    case Op::REMAINDER:
    {
        if (!require(require_numeric(node->left()), require_numeric(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            bless(node, apply_binary_promotion(node->left(), node->right()));
        }
        break;
    }

    case Op::BITWISE_AND:
    case Op::BITWISE_OR:
    case Op::BITWISE_XOR:
    {
        if (!require(require_integral(node->left()), require_integral(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            bless(node, apply_binary_promotion(node->left(), node->right()));
        }
        break;
    }

    case Op::SHIFT_LEFT:
    case Op::SHIFT_RIGHT_ARITHMETIC:
    case Op::SHIFT_RIGHT_LOGICAL:
    {
        if (!require(require_integral(node->left()), require_integral(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            auto t = dynamic_pointer_cast<common::core::type::Int>(l_expr->type());
            bless(node, std::make_unique<common::core::type::Int>(t->size(), t->nullity() | r_expr->type()->nullity()));
        }
        break;
    }

    case Op::EQUAL:
    case Op::NOT_EQUAL:
    {
        if (!typing::is_equality_comparable(l_expr->type(), r_expr->type())) {
            report(node->right(), Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "cannot compare equality, ",
                "left: ", l_expr->type(), ", ",
                "right: ", r_expr->type()));
            bless_erroneous_expression(node);
        } else {
            auto promoted = typing::binary_promotion(l_expr->type(), r_expr->type());
            assert(is_defined(promoted));  // NOLINT
            insert_cast(node->left(), promoted.get());
            insert_cast(node->right(), promoted.get());
            bless(node, std::make_unique<common::core::type::Bool>(common::core::Type::Nullity::NEVER_NULL));
        }
        break;
    }

    case Op::LESS_THAN:
    case Op::GREATER_THAN:
    case Op::LESS_THAN_OR_EQUAL:
    case Op::GREATER_THAN_OR_EQUAL:
    {
        if (!typing::is_order_comparable(l_expr->type(), r_expr->type())) {
            report(node->right(), Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "cannot compare order, ",
                "left: ", l_expr->type(), ", ",
                "right: ", r_expr->type()));
            bless_erroneous_expression(node);
        } else {
            auto promoted = typing::binary_promotion(l_expr->type(), r_expr->type());
            assert(is_defined(promoted));  // NOLINT
            insert_cast(node->left(), promoted.get());
            insert_cast(node->right(), promoted.get());
            bless(node, std::make_unique<common::core::type::Bool>(promoted->nullity()));
        }
        break;
    }

    case Op::LOGICAL_AND:
    case Op::LOGICAL_OR:
    case Op::LOGICAL_XOR:
    case Op::CONDITIONAL_AND:
    case Op::CONDITIONAL_OR:
    {
        if (!require(require_boolean(node->left()), require_boolean(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            auto promoted = apply_binary_promotion(node->left(), node->right());
            bless(node, std::make_unique<common::core::type::Bool>(promoted->nullity()));
        }
        break;
    }

    case Op::CONCATENATION:
    {
        if (!require(require_textual(node->left()), require_textual(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            auto target = std::make_unique<common::core::type::String>(l_expr->type()->nullity() | r_expr->type()->nullity());
            insert_cast(node->left(), target.get());
            insert_cast(node->right(), target.get());

            std::unique_ptr<common::core::value::String> value {};
            if (l_expr->constant() && r_expr->constant()) {
                auto l_str = dynamic_pointer_cast_if<common::core::value::String>(l_expr->value());
                auto r_str = dynamic_pointer_cast_if<common::core::value::String>(r_expr->value());
                if (is_defined(l_str) && is_defined(r_str)) {
                    value = std::make_unique<common::core::value::String>(l_str->get() + r_str->get());
                }
            }
            auto constant = is_defined(value);
            bless(node, std::move(target), std::move(value), constant);
        }
        break;
    }

    case Op::LIKE:
    case Op::NOT_LIKE:
    {
        if (!require(require_textual(node->left()), require_textual(node->right()))) {
            bless_erroneous_expression(node);
        } else if (!r_expr->constant()) {
            report(node->right(), Diagnostic::Code::INVALID_STRING_PATTERN,
                to_string("character pattern must be a constant expression"));
            bless_erroneous_expression(node);
        } else {
            auto target = std::make_unique<common::core::type::String>(l_expr->type()->nullity());
            insert_cast(node->left(), target.get());
            bless(node, std::make_unique<common::core::type::Bool>(l_expr->type()->nullity()));
        }
        break;
    }

    case Op::IN:
    case Op::RELATION_IN:
    {
        report(node, Diagnostic::Code::NOT_IMPLEMENTED, "IN");
        bless_erroneous_expression(node);
        break;
    }

    case Op::RELATION_UNION:
    case Op::RELATION_INTERSECTION:
    case Op::RELATION_DIFFERENCE:
    case Op::RELATION_UNION_ALL:
    case Op::RELATION_INTERSECTION_ALL:
    case Op::RELATION_DIFFERENCE_ALL:
    {
        report(node, Diagnostic::Code::NOT_IMPLEMENTED, "relational binary operators");
        bless_erroneous_expression(node);
        break;
    }

    case Op::INVALID:
        throw std::domain_error("invalid syntax");
    }
}

void Engine::visit(model::expression::relation::ScanExpression* node, ScopeContext& prev) {
    auto& storage = env_.storage_info_provider();
    auto& table_info = storage.find_table(common::core::Name(node->table()->segments()));
    if (!table_info.is_valid()) {
        report(node, Diagnostic::Code::TABLE_NOT_FOUND, to_string(node->table()));
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    std::vector<common::core::Name> qualifiers;
    qualifiers.emplace_back(*node->table());
    std::vector<common::core::type::Relation::Column> columns;
    columns.reserve(table_info.columns().size());
    for (auto& c : table_info.columns()) {
        columns.emplace_back(qualifiers, c.name(), c.type());
    }
    auto relation_type = std::make_unique<common::core::type::Relation>(std::move(columns));
    RelationScope vars { bindings(), &prev.variables(), relation_type.get(), {} }; // compute columns from type

    auto profile = vars.profile();
    profile.source_table(table_info);
    auto relation = std::make_shared<binding::RelationBinding>(binding::RelationBinding::Profile {}, std::move(profile));
    relation->scan_strategy({ table_info, binding::ScanStrategy::Kind::FULL, });
    bless(node, std::move(relation));
    bless(node, std::move(relation_type));
}

void Engine::visit(model::expression::relation::RenameExpression* node, ScopeContext& prev) {
    dispatch(node->operand(), prev);
    auto source_expr = extract_binding(node->operand());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    if (!require_relation(node->operand())) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto source_relation = extract_relation(node->operand());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    auto origin = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    if (!node->columns().empty() && node->columns().size() != origin->columns().size()) {
        report(node, Diagnostic::Code::INCOMPATIBLE_COLUMN_COUNT, to_string(
            "column list has inconsistent number of columns: "
            "listed ", node->columns().size(), " column(s)",
            ", but actual relation has ", origin->columns().size(), " column(s)"));
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    std::vector<common::core::Name> qualifiers {};
    qualifiers.reserve(1);
    qualifiers.emplace_back(*node->name());

    std::vector<common::core::type::Relation::Column> columns {};
    columns.reserve(origin->columns().size());

    std::size_t column_index = 0;
    for (auto&& column : origin->columns()) {
        std::string const* name;
        if (node->columns().empty()) {
            name = &column.name();
        } else {
            name = &node->columns()[column_index]->token();
        }
        columns.emplace_back(qualifiers, *name, make_clone(column.type()));
        ++column_index;
    }

    bless(node, std::make_unique<common::core::type::Relation>(std::move(columns)));
    bless(node, std::make_shared<binding::RelationBinding>(source_relation->output(), source_relation->output()));
}

void Engine::visit(model::expression::relation::SelectionExpression* node, ScopeContext& prev) {
    dispatch(node->operand(), prev);
    auto source_expr = extract_binding(node->operand());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    if (!require_relation(node->operand())) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto source_relation = extract_relation(node->operand());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto relation = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    RelationScope vars { bindings(), &prev.variables(), relation, source_relation->output().columns() };

    bless(node, std::make_shared<binding::RelationBinding>(source_relation->output(), source_relation->output()));
    ScopeContext scope { vars, prev.functions() };

    dispatch(node->condition(), scope);
    auto cond_expr = extract_binding(node->condition());
    if (!is_valid(cond_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }
    if (!require_boolean(node->condition())) {
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }

    bless(node, source_expr->type());
}

void Engine::visit(model::expression::relation::ProjectionExpression* node, ScopeContext& prev) {
    dispatch(node->operand(), prev);
    auto source_expr = extract_binding(node->operand());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }
    if (!require_relation(node->operand())) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }
    auto source_relation = extract_relation(node->operand());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }

    auto relation = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    RelationScope relation_scope { bindings(), &prev.variables(), relation, source_relation->output().columns() };
    ScopeContext scope { relation_scope, prev.functions() };

    std::vector<common::core::type::Relation::Column> columns;
    columns.reserve(node->columns().size());
    binding::RelationBinding::Profile output;
    output.columns().reserve(node->columns().size());
    for (auto* c : node->columns()) {
        dispatch(c->value(), scope);
        auto column_expr = extract_binding(c->value());

        // we don't care whether the column_expr is valid or not,
        // because the resulting relation type is valid only if all of column exprs are valid

        // FIXME: infer the column name from expression
        std::string simple_name;
        common::core::Name name;
        if (is_defined(c->alias())) {
            simple_name = c->alias()->token();
            name = { simple_name };
        }
        auto var = std::make_shared<binding::VariableBinding>(
            bindings().next_variable_id(),
            std::move(name),
            make_clone(column_expr->type()));
        columns.emplace_back(std::move(simple_name), make_clone(column_expr->type()));
        output.columns().emplace_back(var);
        bless(c, var);

        // FIXME: restricts non first order types like relations
    }

    // resolve aggregation
    AggregationAnalyzer aggregation { env_, node->operand() };
    for (auto* c : node->columns()) {
        aggregation.process(c->value());
    }
    if (auto aggregator = aggregation.build(); is_defined(aggregator)) {
        source_relation = extract_relation(aggregator);
    }

    bless(node, std::make_shared<binding::RelationBinding>(source_relation->output(), std::move(output)));
    bless(node, std::make_unique<common::core::type::Relation>(std::move(columns)));
}

std::vector<std::shared_ptr<binding::VariableBinding>> Engine::create_column_variables(
        common::schema::TableInfo const& table) {
    std::vector<std::shared_ptr<binding::VariableBinding>> results;
    results.reserve(table.columns().size());
    for (auto&& column : table.columns()) {
        auto variable = std::make_shared<binding::VariableBinding>(
            bindings().next_variable_id(),
            common::core::Name { column.name() },
            make_clone(column.type()));
        results.emplace_back(std::move(variable));
    }
    return results;
}

std::vector<binding::JoinStrategy::Column> Engine::compute_join_columns(
        model::expression::relation::JoinExpression const* node,
        std::vector<std::shared_ptr<binding::VariableBinding>> const& left_variables,
        std::vector<std::shared_ptr<binding::VariableBinding>> const& right_variables,
        common::core::type::Relation const* left,
        common::core::type::Relation const* right,
        bool natural,
        bool left_null,
        bool right_null) {
    std::map<std::string, std::size_t> natural_joined {};
    bool saw_error = false;
    if (natural) {
        for (auto&& left_column : left->columns()) {
            for (std::size_t i_right = 0, n_right = right->columns().size(); i_right < n_right; ++i_right) {
                auto&& right_column = right->columns()[i_right];
                if (left_column.name() == right_column.name()) {
                    if (natural_joined.find(left_column.name()) == natural_joined.end()) {
                        natural_joined.emplace(left_column.name(), i_right);
                        if (!typing::is_equality_comparable(left_column.type(), right_column.type())) {
                            saw_error = true;
                            report(node->right(), Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                                "cannot compare equality, ",
                                "left: ", left_column.type(), " (", left_column.name(), "), ",
                                "right: ", left_column.type(), " (", right_column.name(), ")"));
                        }
                    } else {
                        saw_error = true;
                        report(node->right(), Diagnostic::Code::DUPLICATE_COLUMN, to_string(
                            "duplicate natural join column: ", left_column.name()));
                    }
                }
            }
        }
        if (natural_joined.empty()) {
            saw_error = true;
            report(node->right(), Diagnostic::Code::MISSING_NATURAL_JOIN_PAIR, "no common columns");
        }
    }
    if (saw_error) {
        return {};
    }

    std::vector<binding::JoinStrategy::Column> columns {};
    for (std::size_t i = 0, n = left->columns().size(); i < n; ++i) {
        auto&& left_column = left->columns()[i];
        auto left_variable = left_variables[i];
        decltype(left_variable) right_variable {};
        bool left_nullify = false;
        bool right_nullify = false;

        std::unique_ptr<common::core::Type> type {};
        if (left_null && !typing::is_nullable(left_column.type())) {
            type = typing::nullity(left_column.type(), common::core::Type::Nullity::NULLABLE);
            left_nullify = true;
        } else {
            type = make_clone(left_column.type());
        }
        std::vector<common::core::Name> qualifiers { left_column.qualifiers() };
        if (auto iter = natural_joined.find(left_column.name()); iter != natural_joined.end()) {
            auto&& right_column = right->columns()[iter->second];
            right_variable = right_variables[iter->second];
            right_nullify = typing::is_nullable(type.get()) && !typing::is_nullable(right_column.type());
            qualifiers.reserve(left_column.qualifiers().size() + right_column.qualifiers().size());
            for (auto&& q: right_column.qualifiers()) {
                qualifiers.emplace_back(q);
            }
        }
        columns.emplace_back(
            std::move(qualifiers),
            std::make_shared<binding::VariableBinding>(
                bindings().next_variable_id(),
                common::core::Name { left_column.name() },
                std::move(type)),
            std::move(left_variable),
            left_nullify,
            std::move(right_variable),
            right_nullify
        );
    }
    for (std::size_t i = 0, n = right->columns().size(); i < n; ++i) {
        auto&& right_column = right->columns()[i];
        if (natural) {
            if (auto iter = natural_joined.find(right_column.name()); iter != natural_joined.end()) {
                // already appeared in the left relation (natural join only)
                continue;
            }
        }
        bool right_nullify { false };
        std::unique_ptr<common::core::Type> type {};
        if (right_null && !typing::is_nullable(right_column.type())) {
            type = typing::nullity(right_column.type(), common::core::Type::Nullity::NULLABLE);
            right_nullify = true;
        } else {
            type = make_clone(right_column.type());
        }
        columns.emplace_back(
            right_column.qualifiers(),
            std::make_shared<binding::VariableBinding>(
                bindings().next_variable_id(),
                common::core::Name { right_column.name() },
                std::move(type)),
            std::shared_ptr<binding::VariableBinding> {},
            false,
            right_variables[i],
            right_nullify
        );
    }
    return columns;
}

template<class T>
static inline bool is_in(T&&) {
    return false;
}

template<class T, class... Args>
static inline bool is_in(T&& value, T&& first, Args&&... rest) {
    if (value == first) {
        return true;
    }
    return is_in(std::forward<T>(value), std::forward<Args>(rest)...);
}

void Engine::visit(model::expression::relation::JoinExpression* node, ScopeContext& prev) {
    using Kind = model::expression::relation::JoinExpression::Kind;
    dispatch(node->left(), prev);
    dispatch(node->right(), prev);
    auto left_expr = extract_binding(node->left());
    auto right_expr = extract_binding(node->right());
    if (!is_valid(left_expr) || !is_valid(right_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    if (!require(require_relation(node->left()), require_relation(node->right()))) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto left_relation = extract_relation(node->left());
    auto right_relation = extract_relation(node->right());
    if (!require(is_valid(left_relation), is_valid(right_relation))) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    auto* left_type = dynamic_pointer_cast<common::core::type::Relation>(left_expr->type());
    auto* right_type = dynamic_pointer_cast<common::core::type::Relation>(right_expr->type());
    auto&& left_variables = left_relation->output().columns();
    auto&& right_variables = right_relation->output().columns();
    std::vector<binding::JoinStrategy::Column> result_join_columns ;

    auto natural = is_in(node->operator_kind(),
        Kind::NATURAL_INNER,
        Kind::NATURAL_LEFT_OUTER,
        Kind::NATURAL_RIGHT_OUTER,
        Kind::NATURAL_FULL_OUTER);
    auto left_outer = is_in(node->operator_kind(),
        Kind::LEFT_OUTER,
        Kind::FULL_OUTER,
        Kind::NATURAL_LEFT_OUTER,
        Kind::NATURAL_FULL_OUTER);
    auto right_outer = is_in(node->operator_kind(),
        Kind::RIGHT_OUTER,
        Kind::FULL_OUTER,
        Kind::NATURAL_RIGHT_OUTER,
        Kind::NATURAL_FULL_OUTER);
    auto union_join = node->operator_kind() == Kind::UNION_OUTER;

    switch (node->operator_kind()) {
        case Kind::LEFT_SEMI:
            result_join_columns.reserve(left_variables.size());
            for (std::size_t i = 0, n = left_variables.size(); i < n; ++i) {
                auto&& column = left_type->columns()[i];
                auto&& variable = left_variables[i];
                result_join_columns.emplace_back(
                    column.qualifiers(),
                    variable,
                    variable, false,
                    std::shared_ptr<binding::VariableBinding> {}, false);
            }
            break;
        case Kind::RIGHT_SEMI:
            result_join_columns.reserve(right_variables.size());
            for (std::size_t i = 0, n = right_variables.size(); i < n; ++i) {
                auto&& column = right_type->columns()[i];
                auto&& variable = right_variables[i];
                result_join_columns.emplace_back(
                    column.qualifiers(),
                    variable,
                    variable, false,
                    std::shared_ptr<binding::VariableBinding> {}, false);
            }
            break;
        case Kind::INVALID:
            std::abort();
        default:
            result_join_columns = compute_join_columns(
                node,
                left_variables, right_variables,
                left_type, right_type,
                natural,
                right_outer || union_join,
                left_outer || union_join);
            break;
    }
    if (result_join_columns.empty()) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    std::vector<std::shared_ptr<binding::VariableBinding>> process_columns;
    process_columns.reserve(left_relation->output().columns().size() + right_relation->output().columns().size());
    for (auto& column : left_relation->output().columns()) {
        process_columns.emplace_back(column);
    }
    for (auto& column : right_relation->output().columns()) {
        process_columns.emplace_back(column);
    }
    if (is_defined(node->condition())) {
        RelationScope r { bindings(), &prev.variables(), { left_type, right_type }, process_columns };
        auto vars = block_scope(r);
        ScopeContext scope { vars, prev.functions() };
        dispatch(node->condition(), scope);
        auto condition_expr = extract_binding(node->condition());
        if (!is_valid(condition_expr) || !require_boolean(node->condition())) {
            bless_undefined<binding::RelationBinding>(node);
            bless_undefined<binding::ExpressionBinding>(node);
            return;
        }
    }
    std::vector<common::core::type::Relation::Column> result_columns {};
    std::vector<std::shared_ptr<binding::VariableBinding>> result_variables {};
    std::set<std::pair<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>>> equalities {};
    result_columns.reserve(result_join_columns.size());
    result_variables.reserve(result_join_columns.size());
    for (auto&& column : result_join_columns) {
        // FIXME: naming rules
        assert(column.output()->name().segments().size() == 1);  // NOLINT
        result_variables.emplace_back(column.output());
        result_columns.emplace_back(
            column.qualifiers(),
            column.output()->name().segments()[0],
            make_clone(column.output()->type()));
        if (is_defined(column.left_source()) && is_defined(column.right_source())) {
            equalities.emplace(column.left_source(), column.right_source());
        }
    }
    auto result_type = std::make_unique<common::core::type::Relation>(std::move(result_columns));
    RelationScope relation_scope { bindings(), &prev.variables(), result_type.get(), result_variables };
    auto relation = std::make_shared<binding::RelationBinding>(
        binding::RelationBinding::Profile { process_columns },
        relation_scope.profile());
    relation->join_strategy({
        union_join ? binding::JoinStrategy::Kind::UNION : binding::JoinStrategy::Kind::NESTED_LOOP,
        left_outer,
        right_outer,
        node->operator_kind() == Kind::LEFT_SEMI,
        node->operator_kind() == Kind::RIGHT_SEMI,
        std::move(result_join_columns),
        std::move(equalities),
    });
    bless(node, std::move(relation));
    bless(node, std::move(result_type));
}

void Engine::visit(model::expression::relation::OrderExpression* node, ScopeContext& prev) {
    dispatch(node->operand(), prev);
    auto source_expr = extract_binding(node->operand());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->elements());
        return;
    }
    if (!require_relation(node->operand())) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->elements());
        return;
    }
    auto source_relation = extract_relation(node->operand());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->elements());
        return;
    }

    auto relation = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    RelationScope relation_scope { bindings(), &prev.variables(), relation, source_relation->output().columns() };
    ScopeContext scope { relation_scope, prev.functions() };

    bool saw_error = false;
    for (auto* element : node->elements()) {
        dispatch(element->key(), scope);
        auto expr = extract_binding(element->key());
        if (!is_valid(expr)) {
            bless_undefined<binding::VariableBinding>(element);
            saw_error = true;
        } else {
            if (auto column_ref = find_variable(element->key()); is_valid(column_ref)
                && source_relation->output().index_of(*column_ref).has_value()) {
                bless(element, column_ref);
            } else {
                auto id = bindings().next_variable_id();
                auto name = to_string('#', id.get());
                bless(element, std::make_shared<binding::VariableBinding>(
                    std::move(id),
                    common::core::Name {},
                    make_clone(expr->type()),
                    make_clone(expr->value())));
            }
        }
    }
    if (saw_error) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    bless(node, make_clone(relation));
    bless(node, std::make_shared<binding::RelationBinding>(source_relation->output(), source_relation->output()));
}

void Engine::visit(model::expression::relation::DistinctExpression* node, ScopeContext& prev) {
    dispatch(node->operand(), prev);
    auto source_expr = extract_binding(node->operand());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    if (!require_relation(node->operand())) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto source_relation = extract_relation(node->operand());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    auto relation = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    bless(node, make_clone(relation));
    bless(node, std::make_shared<binding::RelationBinding>(source_relation->output(), source_relation->output()));
}

void Engine::visit(model::expression::relation::AggregationExpression* node, ScopeContext& prev) {
    dispatch(node->operand(), prev);
    auto source_expr = extract_binding(node->operand());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }
    if (!require_relation(node->operand())) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }
    auto source_relation = extract_relation(node->operand());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }

    auto relation = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    RelationScope relation_scope { bindings(), &prev.variables(), relation, source_relation->output().columns() };
    ScopeContext scope { relation_scope, prev.functions() };

    std::set<std::shared_ptr<binding::VariableBinding>> group_keys {};
    for (auto* k : node->keys()) {
        dispatch(k, scope);
        auto key_expr = extract_binding(k);
        if (!is_valid(key_expr)) {
            continue;
        }
        if (auto var = find_variable(k); is_valid(var)) {
            group_keys.emplace(std::move(var));
        } else {
            report(k, Diagnostic::Code::INCOMPATIBLE_FUNCTION_QUANTIFIER, to_string(
                "group key in aggregation expression must be valid column reference"));
        }
    }

    std::vector<common::core::type::Relation::Column> columns;
    columns.reserve(node->columns().size());
    binding::RelationBinding::Profile output;
    output.columns().reserve(node->columns().size());
    for (auto* c : node->columns()) {
        std::vector<model::expression::Expression*> arguments;
        if (is_defined(c->operand())) {
            dispatch(c->operand(), scope);
            arguments.emplace_back(c->operand());
        }
        std::unique_ptr<common::core::Type> column_type {};
        if (is_defined(c->function())) {
            auto func = resolve_function(c->function(), c->quantifier(), std::move(arguments), scope);
            if (!is_valid(func)) {
                bless_undefined<binding::VariableBinding>(c);
                bless_undefined<binding::FunctionBinding>(c);
                continue;
            }
            if (!func->is_set_function()) {
                report(c->function(), Diagnostic::Code::INCOMPATIBLE_FUNCTION_QUANTIFIER, to_string(
                    "function ", func->name(), " in aggregation expression must be a set function"));
                bless_undefined<binding::VariableBinding>(c);
                bless_undefined<binding::FunctionBinding>(c);
                continue;
            }
            column_type = make_clone(func->type());
            bless(c, std::move(func));
        } else {
            // if function name is not defined, the operand must be always defined.
            assert(is_defined(c->operand()));  // NOLINT

            auto column_expr = extract_binding(c->operand());
            if (!is_valid(column_expr)) {
                bless_undefined<binding::VariableBinding>(c);
                continue;
            }

            // NOTE: if set function is undefined, the operand must be a group key or just a literal value
            if (auto* literal = dynamic_pointer_cast_if<model::expression::Literal>(c->operand()); is_defined(literal)) {
                // ok
            } else if (auto var = find_variable(c->operand()); is_valid(var) && group_keys.find(var) != group_keys.end()) {
                // ok
            } else {
                report(c->operand(), Diagnostic::Code::INVALID_COLUMN_REFERENCE, to_string(
                    "bare expression in aggregation expression must be a column reference"));
                bless_undefined<binding::VariableBinding>(c);
                continue;
            }
            column_type = make_clone(column_expr->type());
        }
        std::string simple_name;
        common::core::Name name;
        if (is_defined(c->alias())) {
            simple_name = c->alias()->token();
            name = { simple_name };
        }
        auto var = std::make_shared<binding::VariableBinding>(
            bindings().next_variable_id(),
            std::move(name),
            make_clone(column_type));
        columns.emplace_back(std::move(simple_name), std::move(column_type));
        output.columns().emplace_back(var);
        bless(c, var);
    }
    bless(node, std::make_shared<binding::RelationBinding>(source_relation->output(), std::move(output)));
    bless(node, std::make_unique<common::core::type::Relation>(std::move(columns)));
}

void Engine::visit(model::expression::relation::GroupExpression* node, ScopeContext& prev) {
    dispatch(node->operand(), prev);
    auto source_expr = extract_binding(node->operand());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    if (!require_relation(node->operand())) {
        bless_erroneous_expression(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto source_relation = extract_relation(node->operand());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::ExpressionBinding>(node);
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto relation = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    RelationScope vars { bindings(), &prev.variables(), relation, source_relation->output().columns() };

    bless(node, std::make_shared<binding::RelationBinding>(source_relation->output(), source_relation->output()));
    ScopeContext scope { vars, prev.functions() };

    bool saw_error = false;
    for (auto* key : node->keys()) {
        dispatch(key, scope);
        if (!saw_error) {
            auto key_expr = extract_binding(key);
            saw_error = !is_valid(key_expr);
        }
    }
    if (saw_error) {
        bless_undefined<binding::ExpressionBinding>(node);
        return;
    }
    bless(node, source_expr->type());
}

void Engine::visit(model::statement::dml::EmitStatement* node, ScopeContext& scope) {
    dispatch(node->source(), scope);
    auto expr = extract_binding(node->source());
    if (!is_valid(expr) || !require_relation(node->source())) {
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto source_relation = extract_relation(node->source());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    auto profile = source_relation->output();
    bless(node, std::make_shared<binding::RelationBinding>(std::move(profile)));
}

void Engine::visit(model::statement::dml::InsertValuesStatement* node, ScopeContext& prev) {
    auto& storage = env_.storage_info_provider();
    auto& table_info = storage.find_table(*node->table());
    if (!table_info.is_valid()) {
        report(node, Diagnostic::Code::TABLE_NOT_FOUND, to_string(node->table()));
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }

    // prepare column schema map
    std::map<std::string, const common::schema::TableInfo::Column*> schema_columns;
    for (auto& c : table_info.columns()) {
        schema_columns.emplace(c.name(), &c);
    }

    if (!node->columns()[0]->name()) {
        // if column names are not given - fill column names
        if (node->columns().size() != table_info.columns().size()) {
            report(node, Diagnostic::Code::INCOMPATIBLE_TABLE_SCHEMA, to_string(
                    "target table has ", table_info.columns().size(), " columns, ",
                    "but ", node->columns().size(), " columns were specified"));
            bless_undefined<binding::RelationBinding>(node);
            bless_undefined_each<binding::VariableBinding>(node->columns());
            return;
        }
        std::size_t index = 0;
        for (auto& column_info : table_info.columns()) {
            node->columns()[index]->name(ir_factory.SimpleName(column_info.name()));
            index++;
        }
    } else {
        // find duplicated or undefined columns
        bool saw_error = false;
        std::set<std::string> saw_columns;
        for (auto c : node->columns()) {
            assert(is_defined(c->name()));  // NOLINT
            if (saw_columns.find(c->name()->token()) == saw_columns.end()) {
                saw_columns.emplace(c->name()->token());
            } else {
                report(c->name(), Diagnostic::Code::DUPLICATE_COLUMN, to_string(
                    "column \"", c->name()->token(), "\" is already specified"));
                saw_error = true;
            }
            if (schema_columns.find(c->name()->token()) == schema_columns.end()) {
                report(node, Diagnostic::Code::COLUMN_NOT_FOUND, to_string(
                    "table \"", table_info.name(), "\" does not have the column ", c->name()->token()));
                saw_error = true;
            }
        }
        if (saw_error) {
            bless_undefined<binding::RelationBinding>(node);
            bless_undefined_each<binding::VariableBinding>(node->columns());
            return;
        }
        // restore omitted columns
        for (auto& c : table_info.columns()) {
            if (saw_columns.find(c.name()) != saw_columns.end()) {
                continue; // skip explicit columns
            }
            // FIXME: special expression instead of literals
            std::unique_ptr<model::expression::Expression> value;
            if (is_defined(c.default_value())) {
                if (c.default_value()->kind() == common::core::Value::Kind::PLACEHOLDER) {
                    value = ir_factory.Literal(c.type(), c.default_value());
                } else {
                    value = ir_factory.Literal(c.type(), c.default_value());
                }
            } else {
                value = ir_factory.Literal(common::core::type::Null());
            }
            node->columns().push_back(ir_factory.InsertValuesStatementColumn(
                ir_factory.SimpleName(c.name()),
                std::move(value)));
        }
        // reorder columns
        assert(table_info.columns().size() == node->columns().size());  // NOLINT
        std::size_t index = 0;
        for (auto& info : table_info.columns()) {
            for (std::size_t i = index, n = node->columns().size(); i < n; ++i) {
                auto* column = node->columns()[i];
                if (column->name()->token() == info.name()) {
                    node->columns().swap(index, i);
                    break;
                }
            }
            assert(node->columns()[index]->name()->token() == info.name());  // NOLINT
            ++index;
        }
    }
    // resolve values

    auto block = block_scope(prev.variables());
    ScopeContext scope { block, prev.functions() };
    for (auto* s : node->initialize()) {
        dispatch(s, scope);
    }

    auto destination = create_column_variables(table_info);
    assert(node->columns().size() == table_info.columns().size());  // NOLINT
    for (std::size_t i = 0, n = node->columns().size(); i < n; i++) {
        auto& info = table_info.columns()[i];
        auto* column = node->columns()[i];
        auto var = destination[i];
        assert(info.name() == column->name()->token());  // NOLINT

        // FIXME: resolve special expressions like DEFAULT, NOW, ...
        dispatch(column->value(), scope);
        auto expr = extract_binding(column->value());
        if (is_valid(expr)) {
            // FIXME: check conversion rule
            if (!typing::is_assignment_convertible(info.type(), *expr, true)) {
                if (!is_defined(column->value()) && !is_defined(info.default_value())) {
                    report(node, Diagnostic::Code::MISSING_MANDATORY_COLUMN, to_string(
                        "mandatory column \"", column->name()->token(), "\" in table \"", table_info.name(), "\"",
                        "must be specified"));
                } else {
                    report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                        table_info.name(), "::", column->name()->token(), " type: ", info.type(), ", ",
                        "expression type: ", expr->type()));
                }
                var = std::make_shared<binding::VariableBinding>();
            } else {
                insert_cast(column->value(), info.type());
            }
        } else {
            var = std::make_shared<binding::VariableBinding>();
        }
        bless(column, std::move(var));
    }

    auto meta = std::make_shared<binding::RelationBinding>();
    meta->destination_table(table_info);
    meta->destination_columns() = std::move(destination);
    bless(node, meta);
}

void Engine::visit(model::statement::dml::UpdateStatement* node, ScopeContext& prev) {
    dispatch(node->source(), prev);
    auto source_expr = extract_binding(node->source());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }
    if (!require_relation(node->source())) {
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }

    auto& storage = env_.storage_info_provider();
    auto& table_info = storage.find_table(common::core::Name(node->table()->segments()));
    if (!table_info.is_valid()) {
        report(node, Diagnostic::Code::TABLE_NOT_FOUND, to_string(node->table()));
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }

    auto source_relation = extract_relation(node->source());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }

    if (table_info.name() != source_relation->output().source_table().name()) {
        report(node->source(), Diagnostic::Code::INVALID_TABLE_EXPRESSION, to_string(node->table()));
        bless_undefined<binding::RelationBinding>(node);
        bless_undefined_each<binding::VariableBinding>(node->columns());
        return;
    }

    auto relation = dynamic_pointer_cast<common::core::type::Relation>(source_expr->type());
    RelationScope relation_scope { bindings(), &prev.variables(), relation, source_relation->output().columns() };
    auto vars = block_scope(relation_scope);
    ScopeContext scope { vars, prev.functions() };
    for (auto* s : node->initialize()) {
        dispatch(s, scope);
    }
    for (auto* c : node->columns()) {
        dispatch(c->value(), scope);
    }

    auto destination = create_column_variables(table_info);
    std::set<std::string> saw_columns {};
    for (auto* c : node->columns()) {
        auto&& name = c->name()->token();
        if (saw_columns.find(name) != saw_columns.end()) {
            report(node->source(), Diagnostic::Code::DUPLICATE_COLUMN, to_string(table_info.name(), ".", name));
            bless_undefined<binding::VariableBinding>(c);
            continue;
        }
        saw_columns.emplace(name);

        auto column_index = table_info.index_of(name);
        if (!column_index.has_value()) {
            report(node->source(), Diagnostic::Code::COLUMN_NOT_FOUND, to_string(table_info.name(), ".", name));
            bless_undefined<binding::VariableBinding>(c);
            continue;
        }

        assert(column_index.value() < table_info.columns().size());  // NOLINT
        auto column_info = table_info.columns()[column_index.value()];
        auto column_var = destination[column_index.value()];
        auto expr = extract_binding(c->value());
        if (!typing::is_assignment_convertible(column_info.type(), *expr, true)) {
            report(c->value(), Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "default value of column \"", column_info.name(), "\" is incompatible, ",
                "value type: ", expr->type(), ", ",
                "column type: ", column_info.type()));
            bless_undefined<binding::VariableBinding>(c);
            continue;
        }
        insert_cast(c->value(), column_info.type());
        bless(c, std::move(column_var));
    }

    auto meta = std::make_shared<binding::RelationBinding>(source_relation->output());
    meta->destination_table(table_info);
    meta->destination_columns() = std::move(destination);
    bless(node, meta);
}

void Engine::visit(model::statement::dml::DeleteStatement* node, ScopeContext& scope) {
    dispatch(node->source(), scope);
    auto source_expr = extract_binding(node->source());
    if (!is_valid(source_expr)) {
        bless_undefined<binding::RelationBinding>(node);
        return;
    }
    if (!require_relation(node->source())) {
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    auto& storage = env_.storage_info_provider();
    auto& table_info = storage.find_table(common::core::Name(node->table()->segments()));
    if (!table_info.is_valid()) {
        report(node, Diagnostic::Code::TABLE_NOT_FOUND, to_string(node->table()));
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    auto source_relation = extract_relation(node->source());
    if (!source_relation->output().is_valid()) {
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    if (table_info.name() != source_relation->output().source_table().name()) {
        report(node->source(), Diagnostic::Code::INVALID_TABLE_EXPRESSION, to_string(node->table()));
        bless_undefined<binding::RelationBinding>(node);
        return;
    }

    auto meta = std::make_shared<binding::RelationBinding>(source_relation->output());
    meta->destination_table(table_info);
    meta->destination_columns() = create_column_variables(table_info);
    bless(node, meta);
}

inline static common::core::Direction convert(model::statement::ddl::CreateTableStatement::PrimaryKey::Direction d) {
    using IN = model::statement::ddl::CreateTableStatement::PrimaryKey::Direction;
    using OUT = common::core::Direction;
    switch (d) {
        case IN::DONT_CARE: return OUT::ASCENDANT;
        case IN::ASCENDANT: return OUT::ASCENDANT;
        case IN::DESCENDANT: return OUT::DESCENDANT;
        default: std::abort();
    }
}

void Engine::visit(model::statement::ddl::CreateTableStatement* node, ScopeContext& scope) {
    using Column = model::statement::ddl::CreateTableStatement::Column;

    bool saw_error = false;

    // duplicate check
    std::set<std::string> saw_columns;
    for (auto* column : node->columns()) {
        if (saw_columns.find(column->name()->token()) == saw_columns.end()) {
            saw_columns.emplace(column->name()->token());
        } else {
            report(column->name(), Diagnostic::Code::DUPLICATE_COLUMN, to_string(
                "column \"", column->name(), "\" is already declared in \"", node->table(), "\""));
            saw_error = true;
        }
    }

    // resolve in-place PRIMARY KEYs
    std::vector<common::schema::IndexInfo::Column> pk_columns;
    for (auto* column : node->columns()) {
        // in-place PRIMARY KEY
        auto& attrs = column->attributes();
        if (attrs.find(Column::Attribute::PRIMARY_KEY) != attrs.end()) {
            if (!node->primary_keys().empty() || !pk_columns.empty()) {
                report(column->name(), Diagnostic::Code::DUPLICATE_PRIMARY_KEY, to_string(
                    "table \"", node->table(), "\" is already declared primary key"));
                saw_error = true;
            } else {
                pk_columns.emplace_back(column->name()->token());
            }
        }
    }
    // collect primary key elements
    assert(node->primary_keys().empty() || pk_columns.empty());  // NOLINT
    for (auto* key : node->primary_keys()) {
        bool green = true;
        for (auto&& column : pk_columns) {
            if (key->name()->token() == column.name()) {
                report(key->name(), Diagnostic::Code::DUPLICATE_PRIMARY_KEY, to_string(
                    "table \"", node->table(), "\" is already declared primary key"));
                saw_error = true;
                green = false;
                break;
            }
        }
        if (!green) {
            continue;
        }
        bool found = false;
        for (auto* column : node->columns()) {
            if (equals(column->name(), key->name())) {
                pk_columns.emplace_back(column->name()->token(), convert(key->direction()));
                found = true;
                break;
            }
        }
        if (!found) {
            report(key->name(), Diagnostic::Code::COLUMN_NOT_FOUND, to_string(
                "table \"", node->table(), "\" does not have column \"", key->name(), "\""));
            saw_error = true;
        }
    }

    // resolve columns
    std::vector<common::schema::TableInfo::Column> info_columns;
    for (auto* column : node->columns()) {
        auto& attrs = column->attributes();
        // FIXME: supported types
        std::unique_ptr<common::core::Type> type;
        std::unique_ptr<common::core::Value> default_value;
        switch (column->type()->kind()) {
            case model::type::TypeKind::TUPLE_TYPE:
            case model::type::TypeKind::RELATION_TYPE:
            case model::type::TypeKind::NULL_TYPE:
                report(column->type(), Diagnostic::Code::INVALID_COLUMN_TYPE, to_string(
                    column->type(), " is not supported type in the column \"", column->name(),
                    "\" in table \"", node->table(), "\""));
                saw_error = true;
                break;
            default:
                type = typing::convert(column->type());
                if (attrs.find(Column::Attribute::NOT_NULL) != attrs.end()
                        || attrs.find(Column::Attribute::PRIMARY_KEY) != attrs.end()) {
                    type = typing::nullity(type.get(), common::core::Type::Nullity::NEVER_NULL);
                }
                break;
        }

        // DEFAULT ...
        if (is_defined(column->value())) {
            // FIXME: resolve placeholder
            dispatch(column->value(), scope);
            auto default_expr = extract_binding(column->value());
            if (is_valid(type) && is_valid(default_expr)) {
                if (column->value()->kind() == model::expression::ExpressionKind::LITERAL) {
                    // MEMO: never use common::core::Placeholder in here,
                    //       use special expression and resolve it to the placeholder instead.
                    auto* literal = dynamic_pointer_cast<model::expression::Literal>(column->value());
                    // FIXME: check conversion rule - currently we only recognize constant expressions
                    if (typing::is_assignment_convertible(type.get(), *default_expr, false)) {
                        // fix literal type instead of casting
                        literal->type(make_clone(type));
                        default_expr->type(make_clone(type));
                        default_value = make_clone(literal->value());
                    } else {
                        report(column->value(), Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                            "default value of column \"", column->name(), "\" is incompatible, ",
                            "value type: ", default_expr->type(), ", ",
                            "column type: ", *type));
                        saw_error = true;
                    }
                } else {
                    report(column->value(), Diagnostic::Code::NOT_IMPLEMENTED, to_string(
                        "column default value: ", column->value()->kind()));
                    saw_error = true;
                }
            }
        }

        info_columns.emplace_back(column->name()->token(), std::move(type), std::move(default_value));
    }
    auto binding = std::make_shared<binding::RelationBinding>();
    if (!saw_error) {
        binding->destination_table({
            common::core::Name { node->table()->segments() },
            std::move(info_columns),
            common::schema::IndexInfo { std::move(pk_columns) },
        });
        binding->destination_columns() = create_column_variables(binding->destination_table());
    }
    bless(node, std::move(binding));
}

void Engine::visit(model::statement::ddl::DropTableStatement*, ScopeContext&) {
    // Drop table statement has nothing to analyze
}


std::unique_ptr<common::core::Type>
Engine::apply_binary_promotion(model::expression::Expression* a, model::expression::Expression* b) {
    auto a_expr = extract_binding(a);
    auto b_expr = extract_binding(b);
    assert(is_valid(a_expr));  // NOLINT
    assert(is_valid(b_expr));  // NOLINT

    auto promoted = typing::binary_promotion(a_expr->type(), b_expr->type());
    assert(is_defined(promoted));  // NOLINT
    insert_cast(a, promoted.get());
    insert_cast(b, promoted.get());
    return promoted;
}

bool Engine::require_boolean(model::expression::Expression* node) {
    assert(is_defined(node));  // NOLINT
    auto expr = extract_binding(node);
    if (!typing::is_boolean(expr->type())) {
        report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "boolean type is required, but it is ", expr->type()));
        return false;
    }
    return true;
}

bool Engine::require_integral(model::expression::Expression* node) {
    assert(is_defined(node));  // NOLINT
    auto expr = extract_binding(node);
    if (!typing::is_integral(expr->type())) {
        report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "integral type is required, but it is ", expr->type()));
        return false;
    }
    return true;
}

bool Engine::require_numeric(model::expression::Expression* node) {
    assert(is_defined(node));  // NOLINT
    auto expr = extract_binding(node);
    if (!typing::is_numeric(expr->type())) {
        report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "numeric type is required, but it is ", expr->type()));
        return false;
    }
    return true;
}

bool Engine::require_textual(model::expression::Expression* node) {
    assert(is_defined(node));  // NOLINT
    auto expr = extract_binding(node);
    if (!typing::is_textual(expr->type())) {
        report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "textual type is required, but it is ", expr->type()));
        return false;
    }
    return true;
}

bool Engine::require_atom(model::expression::Expression* node) {
    assert(is_defined(node));  // NOLINT
    auto expr = extract_binding(node);
    if (!typing::is_atom(expr->type())) {
        report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                "atom type is required, but it is ", expr->type()));
        return false;
    }
    return true;
}

bool Engine::require_tuple(model::expression::Expression* node) {
    return require_type(node, common::core::Type::Kind::TUPLE, "tuple");
}

bool Engine::require_relation(model::expression::Expression* node) {
    return require_type(node, common::core::Type::Kind::RELATION, "relation");
}

bool Engine::require_type(model::expression::Expression* node, common::core::Type::Kind kind, std::string_view kind_name) {
    assert(is_defined(node));  // NOLINT
    auto expr = extract_binding(node);
    if (!is_valid(expr) || expr->type()->kind() != kind) {
        report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                kind_name, " is required, but it is ", expr->type()));
        return false;
    }
    return true;
}

}  // namespace shakujo::analyzer::analyze
