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
#ifndef SHAKUJO_ANALYZER_OPTIMIZE_COMPARISON_TERM_H_
#define SHAKUJO_ANALYZER_OPTIMIZE_COMPARISON_TERM_H_

#include <memory>
#include <vector>

#include <cstdlib>

#include "shakujo/common/core/Value.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/utility.h"

#include "shakujo/model/expression/Expression.h"

#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/BindingContext.h"

namespace shakujo::analyzer::optimize {

class ComparisonTerm {
public:
    enum class Operator {
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
    };

    class Factor {
    public:
        Factor() = default;
        explicit Factor(std::shared_ptr<binding::VariableBinding> variable) : variable_(std::move(variable)) {}
        explicit Factor(std::unique_ptr<common::core::Value> value) : value_(std::move(value)) {}
        explicit Factor(common::core::Value const* value) : value_(common::util::make_clone(value)) {}

        std::shared_ptr<binding::VariableBinding>& variable() {
            return variable_;
        }

        std::shared_ptr<binding::VariableBinding> const& variable() const {
            return variable_;
        }

        Factor& variable(std::shared_ptr<binding::VariableBinding> variable) {
            variable_ = std::move(variable);
            return *this;
        }

        common::core::Value const* value() const {
            return value_.get();
        }

        Factor& value(std::unique_ptr<common::core::Value> value) {
            value_ = std::move(value);
            return *this;
        }

        bool is_variable() const {
            return variable_ != nullptr;
        }

        bool is_value() const {
            return value_ != nullptr;
        }

        explicit operator bool() const {
            return is_variable() || is_value();
        }

    private:
        std::shared_ptr<binding::VariableBinding> variable_ {};
        common::util::ClonablePtr<common::core::Value> value_ {};
    };

public:
    ComparisonTerm() = default;
    ComparisonTerm(model::expression::Expression* source, Operator op, Factor left, Factor right)
        : source_(source)
        , op_(op)
        , left_(std::move(left))
        , right_(std::move(right))
    {}

    model::expression::Expression* source() const {
        return source_;
    }

    Operator op() const {
        return op_;
    }

    Factor& left() {
        return left_;
    }

    Factor const& left() const {
        return left_;
    }

    Factor& right() {
        return right_;
    }

    Factor const& right() const {
        return right_;
    }

    /**
     * @brief collects comparison terms in the given expression.
     * Invariant of each term [t]:
     * - t.left().is_variable()
     * - t.right().is_variable() or t.right().is_value()
     * @param context the current context
     * @param expression the target expression
     * @return the found terms
     */
    static std::vector<ComparisonTerm> collect(
            binding::BindingContext& context,
            model::expression::Expression* expression);

private:
    model::expression::Expression* source_ {};
    Operator op_ { Operator::EQ };
    Factor left_ {};
    Factor right_ {};
};

inline ComparisonTerm::Operator operator~(ComparisonTerm::Operator op) {
    using Op = ComparisonTerm::Operator;
    switch (op) {
        case Op::EQ: return Op::NE;
        case Op::NE: return Op::EQ;
        case Op::LT: return Op::GE;
        case Op::LE: return Op::GT;
        case Op::GT: return Op::LE;
        case Op::GE: return Op::LT;
    }
    std::abort();
}

inline ComparisonTerm::Operator commute(ComparisonTerm::Operator op) {
    using Op = ComparisonTerm::Operator;
    switch (op) {
        case Op::EQ: return Op::EQ;
        case Op::NE: return Op::NE;
        case Op::LT: return Op::GT;
        case Op::LE: return Op::GE;
        case Op::GT: return Op::LT;
        case Op::GE: return Op::LE;
    }
    std::abort();
}
}  // namespace shakujo::analyzer::optimize

#endif  //SHAKUJO_ANALYZER_OPTIMIZE_COMPARISON_TERM_H_
