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

#include "shakujo/common/util/utility.h"

#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/value/Null.h"

namespace shakujo::analyzer::impl {

using common::util::make_clone;
using common::util::equals;
using common::util::is_defined;
using common::util::is_valid;
using common::util::to_string;

std::shared_ptr<binding::ExpressionBinding> Engine::extract_binding(model::key::ExpressionKey::Provider const* node) {
    auto ptr = bindings().get(node->expression_key());
    assert(is_defined(ptr));  // NOLINT
    return ptr;
}

void Engine::bless(model::key::ExpressionKey::Provider* node, std::shared_ptr<binding::ExpressionBinding> binding) {
    auto key = bindings().create_key(std::move(binding));
    node->expression_key(std::move(key));
}

void Engine::bless(model::key::VariableKey::Provider* node, std::shared_ptr<binding::VariableBinding> binding) {
    auto key = bindings().create_key(std::move(binding));
    node->variable_key(std::move(key));
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
                    expr->type());
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
            if (!typing::is_assignment_convertible(type.get(), *expr)) {
                report(node, Diagnostic::Code::INVALID_VARIABLE_TYPE, to_string(
                        "type of variable \"", name, "\" is not compatible to its initializer, ",
                        "variable type: ", typing::to_string(type.get()), ", ",
                        "initializer type: ", typing::to_string(expr->type())));
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
                    expr->type());
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
            if (!typing::is_assignment_convertible(type.get(), *expr)) {
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
            report(node, Diagnostic::Code::LITERAL_INVALID_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            bless(node, type, value, true);
        }
        break;
    case common::core::Type::Kind::INT:
        if (value->kind() != common::core::Value::Kind::INT) {
            report(node, Diagnostic::Code::LITERAL_INVALID_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            auto t = dynamic_cast<common::core::type::Int const*>(type);
            auto v = dynamic_cast<common::core::value::Int const*>(value);
            if (v->get() < t->min_value()) {
                report(node, Diagnostic::Code::LITERAL_INVALID_VALUE,
                       to_string(value, " is too small for ", t));
                bless_erroneous_expression(node);
            } else if (v->get() > t->max_value()) {
                report(node, Diagnostic::Code::LITERAL_INVALID_VALUE,
                       to_string(value, " is too large for ", t));
                bless_erroneous_expression(node);
            } else {
                bless(node, type, value, true);
            }
        }
        break;
    case common::core::Type::Kind::FLOAT:
        if (value->kind() != common::core::Value::Kind::FLOAT) {
            report(node, Diagnostic::Code::LITERAL_INVALID_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            bless(node, type, value, true);
        }
        break;
    case common::core::Type::Kind::CHAR:
        if (value->kind() != common::core::Value::Kind::STRING) {
            report(node, Diagnostic::Code::LITERAL_INVALID_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            auto t = dynamic_cast<common::core::type::Char const*>(type);
            auto v = dynamic_cast<common::core::value::String const*>(value);
            if (v->get().length() > t->size()) {
                report(node, Diagnostic::Code::LITERAL_INVALID_VALUE,
                       to_string(value, " is too long for ", t));
                bless_erroneous_expression(node);
            } else {
                bless(node, type, value, true);
            }
        }
        break;
    case common::core::Type::Kind::STRING:
        if (value->kind() != common::core::Value::Kind::STRING) {
            report(node, Diagnostic::Code::LITERAL_INVALID_VALUE, to_string(value));
            bless_erroneous_expression(node);
        } else {
            bless(node, type, value, true);
        }
        break;
    case common::core::Type::Kind::NULL_:
        if (value->kind() != common::core::Value::Kind::NULL_) {
            report(node, Diagnostic::Code::LITERAL_INVALID_VALUE, to_string(value));
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
            process_tuple_element(dynamic_cast<model::expression::TupleElementLoadExpression*>(current));
            ++position_index;
        }
    }
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
        if (!typing::is_assignment_convertible(var->type(), *expr)) {
            std::string message = to_string(
                    "variable type: ", var->type(), ", ",
                    "expression type: ", expr->type());
            report(node, Diagnostic::Code::INCOMPATIBLE_VARIABLE_TYPE, std::move(message));
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
    auto* tuple = dynamic_cast<common::core::type::Tuple const*>(expr->type());
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
    switch (node->operator_kind()) {
    case model::expression::BinaryOperator::Kind::LOGICAL_AND:
        if (typing::is_integral(l_expr->type()) && typing::is_integral(r_expr->type())) {
            node->operator_kind(model::expression::BinaryOperator::Kind::BITWISE_AND);
        }
        break;
    case model::expression::BinaryOperator::Kind::LOGICAL_OR:
        if (typing::is_integral(l_expr->type()) && typing::is_integral(r_expr->type())) {
            node->operator_kind(model::expression::BinaryOperator::Kind::BITWISE_OR);
        }
        break;
    case model::expression::BinaryOperator::Kind::LOGICAL_XOR:
        if (typing::is_integral(l_expr->type()) && typing::is_integral(r_expr->type())) {
            node->operator_kind(model::expression::BinaryOperator::Kind::BITWISE_XOR);
        }
        break;
    default:
        break;
    }

    switch (node->operator_kind()) {
    case model::expression::BinaryOperator::Kind::ADD:
    case model::expression::BinaryOperator::Kind::SUBTRACT:
    case model::expression::BinaryOperator::Kind::MULTIPLY:
    case model::expression::BinaryOperator::Kind::DIVIDE:
    case model::expression::BinaryOperator::Kind::REMAINDER:
    {
        if (!require(require_numeric(node->left()), require_numeric(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            bless(node, apply_binary_promotion(node->left(), node->right()));
        }
        break;
    }

    case model::expression::BinaryOperator::Kind::BITWISE_AND:
    case model::expression::BinaryOperator::Kind::BITWISE_OR:
    case model::expression::BinaryOperator::Kind::BITWISE_XOR:
    {
        if (!require(require_integral(node->left()), require_integral(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            bless(node, apply_binary_promotion(node->left(), node->right()));
        }
        break;
    }

    case model::expression::BinaryOperator::Kind::SHIFT_LEFT:
    case model::expression::BinaryOperator::Kind::SHIFT_RIGHT_ARITHMETIC:
    case model::expression::BinaryOperator::Kind::SHIFT_RIGHT_LOGICAL:
    {
        if (!require(require_integral(node->left()), require_integral(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            auto t = dynamic_cast<common::core::type::Int const*>(l_expr->type());
            bless(node, common::core::type::Int(t->size(), t->nullity() | r_expr->type()->nullity()));
        }
        break;
    }

    case model::expression::BinaryOperator::Kind::EQUAL:
    case model::expression::BinaryOperator::Kind::NOT_EQUAL:
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
            bless(node, common::core::type::Bool(common::core::Type::Nullity::NEVER_NULL));
        }
        break;
    }

    case model::expression::BinaryOperator::Kind::LESS_THAN:
    case model::expression::BinaryOperator::Kind::GREATER_THAN:
    case model::expression::BinaryOperator::Kind::LESS_THAN_OR_EQUAL:
    case model::expression::BinaryOperator::Kind::GREATER_THAN_OR_EQUAL:
    {
        if (!typing::is_equality_comparable(l_expr->type(), r_expr->type())) {
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
            bless(node, common::core::type::Bool(promoted->nullity()));
        }
        break;
    }

    case model::expression::BinaryOperator::Kind::LOGICAL_AND:
    case model::expression::BinaryOperator::Kind::LOGICAL_OR:
    case model::expression::BinaryOperator::Kind::LOGICAL_XOR:
    case model::expression::BinaryOperator::Kind::CONDITIONAL_AND:
    case model::expression::BinaryOperator::Kind::CONDITIONAL_OR:
    {
        if (!require(require_boolean(node->left()), require_boolean(node->right()))) {
            bless_erroneous_expression(node);
        } else {
            auto promoted = apply_binary_promotion(node->left(), node->right());
            bless(node, common::core::type::Bool(promoted->nullity()));
        }
        break;
    }

    case model::expression::BinaryOperator::Kind::IN:
    case model::expression::BinaryOperator::Kind::RELATION_IN:
    {
        report(node, Diagnostic::Code::NOT_IMPLEMENTED, "IN");
        bless_erroneous_expression(node);
        break;
    }

    case model::expression::BinaryOperator::Kind::RELATION_UNION:
    case model::expression::BinaryOperator::Kind::RELATION_INTERSECTION:
    case model::expression::BinaryOperator::Kind::RELATION_DIFFERENCE:
    case model::expression::BinaryOperator::Kind::RELATION_UNION_ALL:
    case model::expression::BinaryOperator::Kind::RELATION_INTERSECTION_ALL:
    case model::expression::BinaryOperator::Kind::RELATION_DIFFERENCE_ALL:
    {
        report(node, Diagnostic::Code::NOT_IMPLEMENTED, "relational binary operators");
        bless_erroneous_expression(node);
        break;
    }

    case model::expression::BinaryOperator::Kind::INVALID:
        throw std::domain_error("invalid syntax");
    }
}

void Engine::visit(model::statement::dml::EmitStatement* node, ScopeContext& scope) {
    dispatch(node->source(), scope);
    auto expr = extract_binding(node->source());
    if (is_valid(expr)) {
        require_relation(node->source());
    }
}

void Engine::enrich_relation_binding(
        model::Node* node, binding::RelationBinding& relation,
        common::schema::TableInfo const& table, common::schema::IndexInfo const& index) {
    relation.source_table(table);
    if (table.primary_index().is_valid()) {
        // FIXME: or index was unique
        relation.distinct(true);
    }
    if (index.is_valid()) {
        auto& order = relation.order();
        order.reserve(index.columns().size());
        for (auto& column: index.columns()) {
            auto column_at = table.index_of(column.name());
            std::shared_ptr<binding::VariableBinding> column_binding;
            if (!column_at.has_value()) {
                report(node, Diagnostic::Code::COLUMN_NOT_FOUND, to_string(column.name()));
                column_binding = std::make_shared<binding::VariableBinding>();
            } else {
                column_binding = relation.columns()[column_at.value()];
            }
            order.emplace_back(std::move(column_binding), column.direction());
        }
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
    // FIXME use table name as qualifier even if explicit alias is present
    if (is_defined(node->alias())) {
        qualifiers.emplace_back(*node->alias());
    } else {
        qualifiers.emplace_back(*node->table());
    }
    std::vector<common::core::type::Relation::Column> columns;
    columns.reserve(table_info.columns().size());
    for (auto& c : table_info.columns()) {
        columns.emplace_back(qualifiers, c.name(), c.type());
    }
    auto relation = std::make_unique<common::core::type::Relation>(std::move(columns));
    RelationScope vars { bindings(), &prev.variables(), relation.get() };

    auto relation_binding = vars.binding();
    enrich_relation_binding(node, *relation_binding, table_info, table_info.primary_index());
    bless(node, std::move(relation_binding));

    if (is_defined(node->condition())) {
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
    }
    bless(node, std::move(relation));
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

    auto relation = dynamic_cast<common::core::type::Relation const*>(source_expr->type());
    RelationScope vars { bindings(), &prev.variables(), relation };
    bless(node, vars.binding());
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

    auto relation = dynamic_cast<common::core::type::Relation const*>(source_expr->type());
    RelationScope relation_scope { bindings(), &prev.variables(), relation };
    bless(node, relation_scope.binding());
    auto vars = block_scope(relation_scope);
    ScopeContext scope { vars, prev.functions() };
    for (auto* s : node->initialize()) {
        dispatch(s, scope);
    }

    std::vector<common::core::Name> qualifiers;
    if (is_defined(node->alias())) {
        qualifiers.emplace_back(*node->alias());
    }
    std::vector<common::core::type::Relation::Column> columns;
    columns.reserve(node->columns().size());
    for (auto c : node->columns()) {
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
            column_expr->type());
        columns.emplace_back(qualifiers, std::move(simple_name), make_clone(column_expr->type()));
        bless(c, var);

        // FIXME: restricts non first order types like relations
    }
    bless(node, std::make_unique<common::core::type::Relation>(std::move(columns)));
}

static inline bool is_natural_join(model::expression::relation::JoinExpression::Kind kind) {
    using Kind = model::expression::relation::JoinExpression::Kind;
    switch (kind) {
    case Kind::NATURAL_INNER:
    case Kind::NATURAL_LEFT_OUTER:
    case Kind::NATURAL_RIGHT_OUTER:
    case Kind::NATURAL_FULL_OUTER:
        return true;
    case Kind::CROSS:
    case Kind::INNER:
    case Kind::LEFT_OUTER:
    case Kind::RIGHT_OUTER:
    case Kind::FULL_OUTER:
    case Kind::UNION_OUTER:
    case Kind::LEFT_SEMI:
    case Kind::RIGHT_SEMI:
    case Kind::INVALID:
        return false;
    }
    abort();
}

static inline std::unique_ptr<common::core::type::Relation> compute_join_type(
        common::core::type::Relation const* left,
        common::core::type::Relation const* right,
        bool natural,
        bool left_null,
        bool right_null) {
    std::map<std::string, std::vector<common::core::Name>> unified_qualifiers {};
    if (natural) {
        for (auto& left_column : left->columns()) {
            for (auto& right_column : right->columns()) {
                if (left_column.name() == right_column.name()) {
                    std::vector<common::core::Name> qualifier {};
                    for (auto& q : left_column.qualifiers()) {
                        qualifier.emplace_back(q);
                    }
                    for (auto& q : right_column.qualifiers()) {
                        qualifier.emplace_back(q);
                    }
                    unified_qualifiers.emplace(left_column.name(), std::move(qualifier));
                }
            }
        }
    }

    std::vector<common::core::type::Relation::Column> columns {};
    for (auto& column : left->columns()) {
        std::vector<common::core::Name> qualifiers {};
        auto iter = unified_qualifiers.find(column.name());
        if (iter == unified_qualifiers.end()) {
            qualifiers = column.qualifiers();
        } else {
            qualifiers = std::move(iter->second);
        }
        std::unique_ptr<common::core::Type> type {};
        if (left_null && !typing::is_nullable(column.type())) {
            type = typing::nullity(column.type(), common::core::Type::Nullity::NULLABLE);
        } else {
            type = make_clone(column.type());
        }
        columns.emplace_back(std::move(qualifiers), column.name(), std::move(type));
    }
    for (auto& column : right->columns()) {
        std::unique_ptr<common::core::Type> type {};
        if (right_null && !typing::is_nullable(column.type())) {
            type = typing::nullity(column.type(), common::core::Type::Nullity::NULLABLE);
        } else {
            type = make_clone(column.type());
        }
        if (auto iter = unified_qualifiers.find(column.name()); iter == unified_qualifiers.end()) {
            columns.emplace_back(column.qualifiers(), column.name(), std::move(type));
        }
    }

    return std::make_unique<common::core::type::Relation>(std::move(columns));
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
    auto* left_type = dynamic_cast<common::core::type::Relation const*>(left_expr->type());
    auto* right_type = dynamic_cast<common::core::type::Relation const*>(right_expr->type());
    if (is_natural_join(node->operator_kind())) {
        std::set<std::string> saw {};
        bool saw_error = false;
        for (auto& left_column : left_type->columns()) {
            for (auto& right_column : right_type->columns()) {
                if (left_column.name() == right_column.name()) {
                    if (saw.find(left_column.name()) == saw.end()) {
                        saw.emplace(left_column.name());
                        if (!typing::is_equality_comparable(left_column.type(), right_column.type())) {
                            saw_error = true;
                            report(node->right(), Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                                "cannot compare equality, ",
                                "left: ", left_expr->type(), " (", left_column.name(), "), ",
                                "right: ", right_expr->type(), " (", right_column.name(), ")"));
                        }
                    } else {
                        saw_error = true;
                        report(node->right(), Diagnostic::Code::DUPLICATE_COLUMN, to_string(
                            "duplicate natural join column: ", left_column.name()));
                    }
                }
            }
        }
        if (saw.empty()) {
            saw_error = true;
            report(node->right(), Diagnostic::Code::MISSING_NATURAL_JOIN_PAIR, "no common columns");
        }
        if (saw_error) {
            bless_erroneous_expression(node);
            bless_undefined<binding::RelationBinding>(node);
            return;
        }
    }
    std::unique_ptr<common::core::type::Relation> type;
    switch (node->operator_kind()) {
        case Kind::CROSS:
        case Kind::INNER:
            type = compute_join_type(left_type, right_type, false, false, false);
            break;
        case Kind::LEFT_OUTER:
            type = compute_join_type(left_type, right_type, false, false, true);
            break;
        case Kind::RIGHT_OUTER:
            type = compute_join_type(left_type, right_type, false, true, false);
            break;
        case Kind::FULL_OUTER:
        case Kind::UNION_OUTER:
            type = compute_join_type(left_type, right_type, false, true, true);
            break;
        case Kind::NATURAL_INNER:
            type = compute_join_type(left_type, right_type, true, false, false);
            break;
        case Kind::NATURAL_LEFT_OUTER:
            type = compute_join_type(left_type, right_type, true, false, true);
            break;
        case Kind::NATURAL_RIGHT_OUTER:
            type = compute_join_type(left_type, right_type, true, true, false);
            break;
        case Kind::NATURAL_FULL_OUTER:
            type = compute_join_type(left_type, right_type, true, true, true);
            break;
        case Kind::LEFT_SEMI:
            type = make_clone(left_type);
            break;
        case Kind::RIGHT_SEMI:
            type = make_clone(right_type);
            break;
        case Kind::INVALID:
            abort();
    }
    RelationScope relation_scope { bindings(), &prev.variables(), type.get() };
    bless(node, relation_scope.binding());
    if (is_defined(node->condition())) {
        auto vars = block_scope(relation_scope);
        ScopeContext scope { vars, prev.functions() };
        dispatch(node->condition(), scope);
        auto condition_expr = extract_binding(node->condition());
        if (!is_valid(condition_expr) || !require_boolean(node->condition())) {
            bless_undefined<binding::ExpressionBinding>(node);
            return;
        }
    }
    bless(node, std::move(type));
}

void Engine::visit(model::statement::dml::InsertValuesStatement* node, ScopeContext& scope) {
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
    assert(node->columns().size() == table_info.columns().size());  // NOLINT
    for (std::size_t i = 0, n = node->columns().size(); i < n; i++) {
        auto& info = table_info.columns()[i];
        auto* column = node->columns()[i];
        assert(info.name() == column->name()->token());  // NOLINT

        // FIXME: resolve special expressions like DEFAULT, NOW, ...
        dispatch(column->value(), scope);
        auto expr = extract_binding(column->value());
        if (is_valid(expr)) {
            if (!typing::is_assignment_convertible(info.type(), *expr)) {
                if (!is_defined(column->value()) && !is_defined(info.default_value())) {
                    report(node, Diagnostic::Code::MISSING_MANDATORY_COLUMN, to_string(
                        "mandatory column \"", column->name()->token(), "\" in table \"", table_info.name(), "\"",
                        "must be specified"));
                } else {
                    report(node, Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                        table_info.name(), "::", column->name()->token(), " type: ", info.type(), ", ",
                        "expression type: ", expr->type()));
                }
                bless_undefined<binding::VariableBinding>(column);
            } else {
                insert_cast(column->value(), info.type());
                auto var = std::make_shared<binding::VariableBinding>(
                    bindings().next_variable_id(),
                    common::core::Name { info.name() },
                    make_clone(info.type()));
                bless(column, var);
            }
        }
    }
}

void Engine::visit(model::statement::ddl::CreateTableStatement* node, ScopeContext& scope) {
    using Column = model::statement::ddl::CreateTableStatement::Column;
    using PrimaryKey = model::statement::ddl::CreateTableStatement::PrimaryKey;

    // duplicate check
    std::set<std::string> saw_columns;
    for (auto* column : node->columns()) {
        if (saw_columns.find(column->name()->token()) == saw_columns.end()) {
            saw_columns.emplace(column->name()->token());
        } else {
            report(column->name(), Diagnostic::Code::DUPLICATE_COLUMN, to_string(
                "column \"", column->name(), "\" is already declared in \"", node->table(), "\""));
        }
    }
    // resolve in-place PRIMARY KEYs
    for (auto* column : node->columns()) {
        // in-place PRIMARY KEY
        auto& attrs = column->attributes();
        auto it = attrs.find(Column::Attribute::PRIMARY_KEY);
        if (it != attrs.end()) {
            if (!node->primary_keys().empty()) {
                report(column->name(), Diagnostic::Code::DUPLICATE_PRIMARY_KEY, to_string(
                    "table \"", node->table(), "\" is already declared primary key"));
            } else {
                node->primary_keys().push_back(ir_factory.CreateTableStatementPrimaryKey(
                    make_clone(column->name()),
                    PrimaryKey::Direction::DONT_CARE));
                attrs.erase(it);
            }
        }
    }
    // mark NOT NULLs for primary key elements
    for (auto* key : node->primary_keys()) {
        bool saw = false;
        for (auto* column : node->columns()) {
            if (equals(column->name(), key->name())) {
                column->attributes().emplace(Column::Attribute::NOT_NULL);
                saw = true;
                break;
            }
        }
        if (!saw) {
            report(key->name(), Diagnostic::Code::COLUMN_NOT_FOUND, to_string(
                "table \"", node->table(), "\" does not have column \"", key->name(), "\""));
        }
    }
    // resolve columns
    for (auto* column : node->columns()) {
        auto& attrs = column->attributes();
        // FIXME: supported types
        std::unique_ptr<common::core::Type> type;
        switch (column->type()->kind()) {
            case model::type::TypeKind::TUPLE_TYPE:
            case model::type::TypeKind::RELATION_TYPE:
            case model::type::TypeKind::NULL_TYPE:
                report(column->type(), Diagnostic::Code::INVALID_COLUMN_TYPE, to_string(
                    column->type(), " is not supported type in the column \"", column->name(),
                    "\" in table \"", node->table(), "\""));
                break;
            default:
                // FIXME: introduce type binding
                type = typing::convert(column->type());
                if (attrs.find(Column::Attribute::NOT_NULL) != attrs.end()) {
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
                    auto* literal = dynamic_cast<model::expression::Literal*>(column->value());
                    if (typing::is_assignment_convertible(type.get(), *default_expr)) {
                        // fix literal type instead of casting
                        literal->type(make_clone(type));
                        default_expr->type(std::move(type));
                    } else {
                        report(column->value(), Diagnostic::Code::INCOMPATIBLE_EXPRESSION_TYPE, to_string(
                            "default value of column \"", column->name(), "\" is incompatible, ",
                            "value type: ", default_expr->type(), ", ",
                            "column type: ", *type));
                    }
                } else {
                    report(column->value(), Diagnostic::Code::NOT_IMPLEMENTED, to_string(
                        "column default value: ", column->value()->kind()));
                }
            }
        }
    }
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

}  // namespace shakujo::analyzer::impl
