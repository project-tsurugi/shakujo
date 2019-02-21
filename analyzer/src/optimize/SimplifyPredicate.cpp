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
#include "SimplifyPredicate.h"

#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/util/utility.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/expression/ExpressionVisitor.h"

#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"

namespace shakujo::analyzer::optimize {

using Ternary = common::util::Ternary;
using common::util::is_defined;
using common::util::make_clone;

namespace {
class Engine
    : public model::expression::ExpressionVisitor<Ternary> {
public:
    explicit Engine(Context& context) : context_(context) {}

    using ExpressionVisitor::visit;

    bool is_bool(model::expression::Expression const* node) const {
        auto expr = context_.bindings().get(node->expression_key());
        return is_bool(expr->type());
    }

    Ternary eval(model::expression::Expression const* node) const {
        auto expr = context_.bindings().get(node->expression_key());
        if (is_bool(expr->type())) {
            return eval(expr->value());
        }
        return Ternary::UNKNOWN;
    }

    bool is_bool(common::core::Type const* type) const {
        return is_defined(dynamic_cast<common::core::type::Bool const*>(type));
    }

    Ternary eval(common::core::Value const* value) const {
        if (auto* v = dynamic_cast<common::core::value::Bool const*>(value)) {
            return v->get() ? Ternary::TRUE : Ternary::FALSE;
        }
        return Ternary::UNKNOWN;
    }

    Ternary visitDefault(model::expression::Expression*) override {
        return Ternary::UNKNOWN;
    }

    Ternary visit(model::expression::Literal* node) override {
        return eval(node);
    }

    Ternary visit(model::expression::VariableReference* node) override {
        return propagate(node, eval(node));
    }

    Ternary visit(model::expression::UnaryOperator* node) override {
        if (!is_bool(node)) {
            return Ternary::UNKNOWN;
        }
        using Kind = model::expression::UnaryOperator::Kind;
        switch (node->operator_kind()) {
            case Kind::CONDITIONAL_NOT: {
                auto term = dispatch(node->operand());
                return propagate(node, ~term);
            }
            default:
                break;
        }
        return Ternary::UNKNOWN;
    }

    Ternary visit(model::expression::BinaryOperator* node) override {
        if (!is_bool(node)) {
            return Ternary::UNKNOWN;
        }
        using Kind = model::expression::BinaryOperator::Kind;
        switch (node->operator_kind()) {
            case Kind::CONDITIONAL_AND: {
                auto left = dispatch(node->left());
                auto right = dispatch(node->right());
                if (is_const(left & right)) {
                    return propagate(node, left & right);
                }
                if (left == true) {
                    replace(node, node->release_right());
                    return right;
                }
                if (right == true) {
                    replace(node, node->release_left());
                    return left;
                }
                break;
            }
            case Kind::CONDITIONAL_OR: {
                auto left = dispatch(node->left());
                auto right = dispatch(node->right());
                if (is_const(left | right)) {
                    return propagate(node, left | right);
                }
                if (left == false) {
                    replace(node, node->release_right());
                    return right;
                }
                if (right == false) {
                    replace(node, node->release_left());
                    return left;
                }
                break;
            }
            default:
                break;
        }
        return Ternary::UNKNOWN;
    }

    Ternary visit(model::expression::ImplicitCast* node) override {
        if (!is_bool(node)) {
            return Ternary::UNKNOWN;
        }
        return dispatch(node->operand());
    }

    model::expression::Expression* replace(
            model::expression::Expression* node,
            std::unique_ptr<model::expression::Expression> replacement) {
        return node->replace_with(std::move(replacement));
    }

    static bool is_const(Ternary value) {
        return value != Ternary::UNKNOWN;
    }

    Ternary propagate(model::expression::Expression* node, Ternary value) {
        if (value == Ternary::UNKNOWN) {
            return value;
        }
        return replace_constant(node, value == true);
    }

    Ternary replace_constant(model::expression::Expression* node, bool value) {
        model::IRFactory f;
        auto* replacement = node->replace_with(f.Literal(
            std::make_unique<common::core::type::Bool>(common::core::Type::Nullity::NEVER_NULL),
            std::make_unique<common::core::value::Bool>(value)));
        replacement->expression_key(context_.bindings().create_key(std::make_shared<binding::ExpressionBinding>(
            make_clone(replacement->type()),
            make_clone(replacement->value()),
            true)));
        return value ? Ternary::TRUE : Ternary::FALSE;
    }

private:
    Context& context_;
};
}  // namespace

common::util::Ternary SimplifyPredicate::operator()(model::expression::Expression *node) {
    Engine engine { context_ };
    return engine.dispatch(node);
}
}  // namespace shakujo::analyzer::optimize
