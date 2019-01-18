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
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/value/Int.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/expression/ConstExpressionVisitor.h"

#include "shakujo/model/util/NodeSerializer.h"
#include "shakujo/common/util/JsonSerializer.h"

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace shakujo;
using namespace shakujo::common;
using namespace shakujo::common::core;
using namespace shakujo::model;
using namespace shakujo::model::expression;

class V : public ConstExpressionVisitor<int> {
    virtual int visit(Literal const* node) override {
        switch (node->value()->kind()) {
        case Value::Kind::INT:
            return static_cast<int>(dynamic_cast<value::Int const*>(node->value())->get());
        default:
            std::stringstream ss;
            ss << "unknown value kind: " << node->value()->kind();
            throw std::runtime_error(ss.str());
        }
    }

    virtual int visit(BinaryOperator const* node) override {
        switch (node->operator_kind()) {
        case BinaryOperator::Kind::ADD:
            return dispatch(node->left()) + dispatch(node->right());

        case BinaryOperator::Kind::MULTIPLY:
            return dispatch(node->left()) * dispatch(node->right());

        default:
            std::stringstream ss;
            ss << "unknown binary operator: " << node->operator_kind();
            throw std::runtime_error(ss.str());
        }
    }

    virtual int visit(UnaryOperator const* node) override {
        switch (node->operator_kind()) {
        case UnaryOperator::Kind::SIGN_INVERSION:
            return -dispatch(node->operand());

        default:
            std::stringstream ss;
            ss << "unknown unary operator: " << node->operator_kind();
            throw std::runtime_error(ss.str());
        }
    }
};

int main() {
    IRFactory f;
    namespace v = shakujo::common::core::value;
    namespace t = shakujo::common::core::type;
    auto expr = f.BinaryOperator(
        BinaryOperator::Kind::ADD,
        f.Literal(t::Int(64U), 100),
        f.BinaryOperator(
            BinaryOperator::Kind::MULTIPLY,
            f.Literal(t::Int(64U), 2),
            f.UnaryOperator(
                UnaryOperator::Kind::SIGN_INVERSION,
                f.Literal(t::Int(64U), 3)
            )
        )
    );

    common::util::JsonSerializer json { std::cout };
    model::util::NodeSerializer serializer {};
    serializer.serialize(json, expr.get());
    std::cout << std::endl << std::endl;

    auto result = V{}.dispatch(expr.get());
    cout << "result: " << result << endl;
    if (result == (100 + (2 * -3))) {
        return 0;
    } else {
        return 1;
    }
}
