/*
 * Copyright 2018-2023 Project Tsurugi.
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
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/value/Int.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/expression/ConstExpressionVisitor.h"

#include "shakujo/model/util/NodeSerializer.h"
#include "shakujo/common/util/JsonSerializer.h"

namespace shakujo::example::visitor {

using namespace shakujo::model::expression;

namespace v = shakujo::common::core::value;
namespace t = shakujo::common::core::type;

class V : public model::expression::ConstExpressionVisitor<int> {
    int visit(Literal const* node) override {
        switch (node->value()->kind()) {
        case common::core::Value::Kind::INT:
            return static_cast<int>(dynamic_cast<v::Int const*>(node->value())->get());
        default:
            std::stringstream ss;
            ss << "unknown value kind: " << node->value()->kind();
            throw std::runtime_error(ss.str());
        }
    }

    int visit(BinaryOperator const* node) override {
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

    int visit(UnaryOperator const* node) override {
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

static int run() {
    model::IRFactory f;
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
    std::cout << "result: " << result << std::endl;
    if (result == (100 + (2 * -3))) {
        return 0;
    }
    return 1;
}

}  // namespace shakujo::example::visitor

int main() {
    try {
        return shakujo::example::visitor::run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
