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
#include "ComparisonTerm.h"

#include <stdexcept>

#include <cstdlib>

#include "shakujo/model/expression/ExpressionVisitor.h"

#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::optimize {

using common::util::is_valid;

namespace {
class Collector : public model::expression::ExpressionVisitor<ComparisonTerm::Factor> {
public:
    using ExpressionVisitor::visit;

    ComparisonTerm::Factor visit(model::expression::VariableReference* node) override {
        if (auto variable = bindings_.find(node->variable_key()); is_valid(variable)) {
            return ComparisonTerm::Factor { std::move(variable) };
        }
        return {};
    }

    ComparisonTerm::Factor visit(model::expression::Literal* node) override {
        using Kind = common::core::Value::Kind;
        switch (node->value()->kind()) {
            case Kind::INT:
            case Kind::FLOAT:
            case Kind::STRING:
                return ComparisonTerm::Factor { node->value() };
            default:
                return {};
        }
    }

    ComparisonTerm::Factor visit(model::expression::ImplicitCast* node) override {
        return dispatch(node->operand());
    }

    ComparisonTerm::Factor visit(model::expression::UnaryOperator* node) override {
        using Op = model::expression::UnaryOperator::Kind ;
        switch (node->operator_kind()) {
            case Op::SIGN_INVERSION: {
                auto factor = dispatch(node->operand());
                // FIXME: -var ?
                if (!factor || !factor.is_value()) {
                    return {};
                }
                auto value = factor.value();
                using Kind = common::core::Value::Kind;
                switch (value->kind()) {
                    case Kind::INT:
                        // FIXME: MIN_VALUE
                        return factor.value(std::make_unique<common::core::value::Int>(
                            -dynamic_cast<common::core::value::Int const*>(value)->get()));
                    case Kind::FLOAT:
                        return factor.value(std::make_unique<common::core::value::Float>(
                            -dynamic_cast<common::core::value::Float const*>(value)->get()));
                    default:
                        return {};
                }
            }

            default:
                // FIXME impl more operators
                break;
        }
        return {};
    }

    ComparisonTerm::Factor visit(model::expression::BinaryOperator* node) override {
        using Op = model::expression::BinaryOperator::Kind;
        switch (node->operator_kind()) {
            case Op::EQUAL:
            case Op::NOT_EQUAL:
            case Op::LESS_THAN:
            case Op::GREATER_THAN:
            case Op::LESS_THAN_OR_EQUAL:
            case Op::GREATER_THAN_OR_EQUAL:
                process_comparison(node);
                break;

            case Op::CONDITIONAL_AND:
                dispatch(node->left());
                dispatch(node->right());
                break;

            default:
                // FIXME impl more operators
                break;
        }
        return {};
    }

private:
    void process_comparison(model::expression::BinaryOperator* node) {
        auto op = convert(node->operator_kind());
        auto left = dispatch(node->left());
        if (!left) {
            return;
        }
        auto right = dispatch(node->right());
        if (!right) {
            return;
        }
        if (left.is_variable()) {
            terms_.emplace_back(node, op, std::move(left), std::move(right));
            return;
        }
        if (right.is_variable()) {
            terms_.emplace_back(node, commute(op), std::move(right), std::move(left));
            return;
        }
    }

    static ComparisonTerm::Operator convert(model::expression::BinaryOperator::Kind op) {
        using IN = model::expression::BinaryOperator::Kind;
        using OUT = ComparisonTerm::Operator;
        switch (op) {
            case IN::EQUAL: return OUT::EQ;
            case IN::NOT_EQUAL: return OUT::EQ;
            case IN::LESS_THAN: return OUT::LT;
            case IN::GREATER_THAN: return OUT::GT;
            case IN::LESS_THAN_OR_EQUAL: return OUT::LE;
            case IN::GREATER_THAN_OR_EQUAL: return OUT::GE;
            default: std::abort();
        }
    }

public:
    explicit Collector(binding::BindingContext& bindings) : bindings_(bindings) {}
    binding::BindingContext& bindings_;
    std::vector<ComparisonTerm> terms_ {};
};
} // namespace

std::vector<ComparisonTerm> ComparisonTerm::collect(
        binding::BindingContext& context,
        model::expression::Expression *expression) {
    Collector collector { context };
    collector.dispatch(expression);
    return std::move(collector.terms_);
}

}  // namespace shakujo::analyzer::optimize