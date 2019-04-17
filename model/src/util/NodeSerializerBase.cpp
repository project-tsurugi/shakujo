/*
 * Copyright 2018-2019 shakujo project.
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
// DON'T EDIT THIS GENERATED FILE //
#include "shakujo/model/util/NodeSerializerBase.h"

#include <stdexcept>
#include <sstream>

#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/util/DataSerializer.h"
#include "shakujo/common/util/utility.h"
#include "shakujo/model/Node.h"
#include "shakujo/model/expression/ArrayCreationExpression.h"
#include "shakujo/model/expression/ArrayElementLoadExpression.h"
#include "shakujo/model/expression/ArrayElementStoreExpression.h"
#include "shakujo/model/expression/AssignExpression.h"
#include "shakujo/model/expression/BinaryOperator.h"
#include "shakujo/model/expression/BlockExpression.h"
#include "shakujo/model/expression/CaseExpression.h"
#include "shakujo/model/expression/CursorAdvanceExpression.h"
#include "shakujo/model/expression/CursorCreationExpression.h"
#include "shakujo/model/expression/CursorElementLoadExpression.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/ExpressionKind.h"
#include "shakujo/model/expression/FunctionCall.h"
#include "shakujo/model/expression/ImplicitCast.h"
#include "shakujo/model/expression/Literal.h"
#include "shakujo/model/expression/Placeholder.h"
#include "shakujo/model/expression/StringOperator.h"
#include "shakujo/model/expression/TupleCreationExpression.h"
#include "shakujo/model/expression/TupleElementLoadExpression.h"
#include "shakujo/model/expression/TupleElementStoreExpression.h"
#include "shakujo/model/expression/TypeOperator.h"
#include "shakujo/model/expression/UnaryOperator.h"
#include "shakujo/model/expression/VariableReference.h"
#include "shakujo/model/expression/VectorCreationExpression.h"
#include "shakujo/model/expression/VectorElementLoadExpression.h"
#include "shakujo/model/expression/VectorElementStoreExpression.h"
#include "shakujo/model/expression/VectorLengthExpression.h"
#include "shakujo/model/expression/relation/AggregationExpression.h"
#include "shakujo/model/expression/relation/DistinctExpression.h"
#include "shakujo/model/expression/relation/GroupExpression.h"
#include "shakujo/model/expression/relation/JoinExpression.h"
#include "shakujo/model/expression/relation/LimitExpression.h"
#include "shakujo/model/expression/relation/OrderExpression.h"
#include "shakujo/model/expression/relation/ProjectionExpression.h"
#include "shakujo/model/expression/relation/ScanExpression.h"
#include "shakujo/model/expression/relation/SelectionExpression.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/FunctionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/key/VariableKey.h"
#include "shakujo/model/name/Index.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/NameKind.h"
#include "shakujo/model/name/QualifiedName.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/program/GlobalDeclaration.h"
#include "shakujo/model/program/GlobalDeclarationKind.h"
#include "shakujo/model/program/GlobalFunctionDeclaration.h"
#include "shakujo/model/program/GlobalVariableDeclaration.h"
#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/AnchorDeclaration.h"
#include "shakujo/model/statement/BlockStatement.h"
#include "shakujo/model/statement/BranchStatement.h"
#include "shakujo/model/statement/BreakStatement.h"
#include "shakujo/model/statement/ContinueStatement.h"
#include "shakujo/model/statement/EmptyStatement.h"
#include "shakujo/model/statement/ExpressionStatement.h"
#include "shakujo/model/statement/ForEachStatement.h"
#include "shakujo/model/statement/ForStatement.h"
#include "shakujo/model/statement/LocalVariableDeclaration.h"
#include "shakujo/model/statement/LogStatement.h"
#include "shakujo/model/statement/RaiseStatement.h"
#include "shakujo/model/statement/ReturnStatement.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/statement/VectorElementDeleteStatement.h"
#include "shakujo/model/statement/VectorElementInsertStatement.h"
#include "shakujo/model/statement/WhileStatement.h"
#include "shakujo/model/statement/ddl/CreateTableStatement.h"
#include "shakujo/model/statement/ddl/DropTableStatement.h"
#include "shakujo/model/statement/dml/DeleteStatement.h"
#include "shakujo/model/statement/dml/EmitStatement.h"
#include "shakujo/model/statement/dml/InsertRelationStatement.h"
#include "shakujo/model/statement/dml/InsertValuesStatement.h"
#include "shakujo/model/statement/dml/UpdateStatement.h"
#include "shakujo/model/statement/transaction/TransactionBlockStatement.h"
#include "shakujo/model/type/ArrayType.h"
#include "shakujo/model/type/BooleanType.h"
#include "shakujo/model/type/CharType.h"
#include "shakujo/model/type/Float32Type.h"
#include "shakujo/model/type/Float64Type.h"
#include "shakujo/model/type/Int32Type.h"
#include "shakujo/model/type/Int64Type.h"
#include "shakujo/model/type/NullType.h"
#include "shakujo/model/type/RelationType.h"
#include "shakujo/model/type/StringType.h"
#include "shakujo/model/type/TupleType.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"
#include "shakujo/model/type/VarCharType.h"
#include "shakujo/model/type/VectorType.h"

namespace shakujo::model::util {
class NodeSerializerBase::Impl {
private:
    std::ostringstream str_buffer_;

public:
    bool qualified_kind_ { false };
    bool enum_kind_ { false };
    bool fragment_kind_ { false };

    template<typename T>
    inline std::string to_string(const T& value) {
        str_buffer_.str("");
        str_buffer_.clear(std::stringstream::goodbit);
        common::util::append(str_buffer_, value);
        return str_buffer_.str();
    }
};

NodeSerializerBase::NodeSerializerBase() : impl_(new Impl()) {}

NodeSerializerBase::~NodeSerializerBase() noexcept = default;

bool NodeSerializerBase::show_qualified_kind() const {
    return impl_->qualified_kind_;
}

NodeSerializerBase& NodeSerializerBase::show_qualified_kind(bool on) {
    impl_->qualified_kind_ = on;
    return *this;
}

bool NodeSerializerBase::show_enum_kind() const {
    return impl_->enum_kind_;
}

NodeSerializerBase& NodeSerializerBase::show_enum_kind(bool on) {
    impl_->enum_kind_ = on;
    return *this;
}

bool NodeSerializerBase::show_fragment_kind() const {
    return impl_->fragment_kind_;
}

NodeSerializerBase& NodeSerializerBase::show_fragment_kind(bool on) {
    impl_->fragment_kind_ = on;
    return *this;
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] common::core::Type const* value) {
    // external object
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Type");
    printer.exit_object("Type");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] common::core::Value const* value) {
    // external object
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Value");
    printer.exit_object("Value");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] Node const* value) {
    // external object
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Node");
    printer.exit_object("Node");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::ArrayCreationExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ArrayCreationExpression");
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("values");
        auto& list = value->values();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("values");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("ArrayCreationExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::ArrayElementLoadExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ArrayElementLoadExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("index");
        serialize(printer, value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("ArrayElementLoadExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::ArrayElementStoreExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ArrayElementStoreExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("index");
        serialize(printer, value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("ArrayElementStoreExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::AssignExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("AssignExpression");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    printer.exit_object("AssignExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::BinaryOperator const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("BinaryOperator");
    {
        printer.enter_property("operator_kind");
        serialize(printer, value->operator_kind());
        printer.exit_property("operator_kind");
    }
    {
        printer.enter_property("left");
        serialize(printer, value->left());
        printer.exit_property("left");
    }
    {
        printer.enter_property("right");
        serialize(printer, value->right());
        printer.exit_property("right");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("BinaryOperator");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::BinaryOperator::Kind value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Kind");
        } else {
            printer.enter_object("BinaryOperator::Kind");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Kind");
        } else {
            printer.exit_object("BinaryOperator::Kind");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::BlockExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("BlockExpression");
    {
        printer.enter_property("head");
        auto& list = value->head();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("head");
    }
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("BlockExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::CaseExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("CaseExpression");
    {
        printer.enter_property("cases");
        auto& list = value->cases();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("cases");
    }
    {
        printer.enter_property("default_expression");
        serialize(printer, value->default_expression());
        printer.exit_property("default_expression");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("CaseExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::CaseExpression::Clause const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Clause");
    } else {
        printer.enter_object("CaseExpression::Clause");
    }
    {
        printer.enter_property("condition");
        serialize(printer, value->condition());
        printer.exit_property("condition");
    }
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Clause");
    } else {
        printer.exit_object("CaseExpression::Clause");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::CursorAdvanceExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("CursorAdvanceExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("CursorAdvanceExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::CursorCreationExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("CursorCreationExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("CursorCreationExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::CursorElementLoadExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("CursorElementLoadExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("CursorElementLoadExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::Expression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    switch (value->kind()) {
    case expression::ArrayCreationExpression::tag:
        serialize(printer, static_cast<expression::ArrayCreationExpression const*>(value));  // NOLINT
        return;
    case expression::ArrayElementLoadExpression::tag:
        serialize(printer, static_cast<expression::ArrayElementLoadExpression const*>(value));  // NOLINT
        return;
    case expression::ArrayElementStoreExpression::tag:
        serialize(printer, static_cast<expression::ArrayElementStoreExpression const*>(value));  // NOLINT
        return;
    case expression::AssignExpression::tag:
        serialize(printer, static_cast<expression::AssignExpression const*>(value));  // NOLINT
        return;
    case expression::BinaryOperator::tag:
        serialize(printer, static_cast<expression::BinaryOperator const*>(value));  // NOLINT
        return;
    case expression::BlockExpression::tag:
        serialize(printer, static_cast<expression::BlockExpression const*>(value));  // NOLINT
        return;
    case expression::CaseExpression::tag:
        serialize(printer, static_cast<expression::CaseExpression const*>(value));  // NOLINT
        return;
    case expression::CursorAdvanceExpression::tag:
        serialize(printer, static_cast<expression::CursorAdvanceExpression const*>(value));  // NOLINT
        return;
    case expression::CursorCreationExpression::tag:
        serialize(printer, static_cast<expression::CursorCreationExpression const*>(value));  // NOLINT
        return;
    case expression::CursorElementLoadExpression::tag:
        serialize(printer, static_cast<expression::CursorElementLoadExpression const*>(value));  // NOLINT
        return;
    case expression::FunctionCall::tag:
        serialize(printer, static_cast<expression::FunctionCall const*>(value));  // NOLINT
        return;
    case expression::ImplicitCast::tag:
        serialize(printer, static_cast<expression::ImplicitCast const*>(value));  // NOLINT
        return;
    case expression::Literal::tag:
        serialize(printer, static_cast<expression::Literal const*>(value));  // NOLINT
        return;
    case expression::Placeholder::tag:
        serialize(printer, static_cast<expression::Placeholder const*>(value));  // NOLINT
        return;
    case expression::StringOperator::tag:
        serialize(printer, static_cast<expression::StringOperator const*>(value));  // NOLINT
        return;
    case expression::TupleCreationExpression::tag:
        serialize(printer, static_cast<expression::TupleCreationExpression const*>(value));  // NOLINT
        return;
    case expression::TupleElementLoadExpression::tag:
        serialize(printer, static_cast<expression::TupleElementLoadExpression const*>(value));  // NOLINT
        return;
    case expression::TupleElementStoreExpression::tag:
        serialize(printer, static_cast<expression::TupleElementStoreExpression const*>(value));  // NOLINT
        return;
    case expression::TypeOperator::tag:
        serialize(printer, static_cast<expression::TypeOperator const*>(value));  // NOLINT
        return;
    case expression::UnaryOperator::tag:
        serialize(printer, static_cast<expression::UnaryOperator const*>(value));  // NOLINT
        return;
    case expression::VariableReference::tag:
        serialize(printer, static_cast<expression::VariableReference const*>(value));  // NOLINT
        return;
    case expression::VectorCreationExpression::tag:
        serialize(printer, static_cast<expression::VectorCreationExpression const*>(value));  // NOLINT
        return;
    case expression::VectorElementLoadExpression::tag:
        serialize(printer, static_cast<expression::VectorElementLoadExpression const*>(value));  // NOLINT
        return;
    case expression::VectorElementStoreExpression::tag:
        serialize(printer, static_cast<expression::VectorElementStoreExpression const*>(value));  // NOLINT
        return;
    case expression::VectorLengthExpression::tag:
        serialize(printer, static_cast<expression::VectorLengthExpression const*>(value));  // NOLINT
        return;
    case expression::relation::AggregationExpression::tag:
        serialize(printer, static_cast<expression::relation::AggregationExpression const*>(value));  // NOLINT
        return;
    case expression::relation::DistinctExpression::tag:
        serialize(printer, static_cast<expression::relation::DistinctExpression const*>(value));  // NOLINT
        return;
    case expression::relation::GroupExpression::tag:
        serialize(printer, static_cast<expression::relation::GroupExpression const*>(value));  // NOLINT
        return;
    case expression::relation::JoinExpression::tag:
        serialize(printer, static_cast<expression::relation::JoinExpression const*>(value));  // NOLINT
        return;
    case expression::relation::LimitExpression::tag:
        serialize(printer, static_cast<expression::relation::LimitExpression const*>(value));  // NOLINT
        return;
    case expression::relation::OrderExpression::tag:
        serialize(printer, static_cast<expression::relation::OrderExpression const*>(value));  // NOLINT
        return;
    case expression::relation::ProjectionExpression::tag:
        serialize(printer, static_cast<expression::relation::ProjectionExpression const*>(value));  // NOLINT
        return;
    case expression::relation::ScanExpression::tag:
        serialize(printer, static_cast<expression::relation::ScanExpression const*>(value));  // NOLINT
        return;
    case expression::relation::SelectionExpression::tag:
        serialize(printer, static_cast<expression::relation::SelectionExpression const*>(value));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << value->kind();
    throw std::invalid_argument(ss.str());
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::FunctionCall const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("FunctionCall");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("arguments");
        auto& list = value->arguments();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("arguments");
    }
    {
        printer.enter_property("quantifier");
        serialize(printer, value->quantifier());
        printer.exit_property("quantifier");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("function_key");
        serialize(printer, value->function_key());
        printer.exit_property("function_key");
    }
    printer.exit_object("FunctionCall");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::FunctionCall::Quantifier value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Quantifier");
        } else {
            printer.enter_object("FunctionCall::Quantifier");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Quantifier");
        } else {
            printer.exit_object("FunctionCall::Quantifier");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::ImplicitCast const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ImplicitCast");
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("ImplicitCast");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::Literal const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Literal");
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("Literal");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::Placeholder const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Placeholder");
    {
        printer.enter_property("name");
        printer.value(value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("Placeholder");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::StringOperator const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("StringOperator");
    {
        printer.enter_property("operator_kind");
        serialize(printer, value->operator_kind());
        printer.exit_property("operator_kind");
    }
    {
        printer.enter_property("string");
        printer.value(value->string());
        printer.exit_property("string");
    }
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("StringOperator");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::StringOperator::Kind value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Kind");
        } else {
            printer.enter_object("StringOperator::Kind");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Kind");
        } else {
            printer.exit_object("StringOperator::Kind");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::TupleCreationExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("TupleCreationExpression");
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("values");
        auto& list = value->values();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("values");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("TupleCreationExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::TupleElementLoadExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("TupleElementLoadExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("index");
        serialize(printer, value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("TupleElementLoadExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::TupleElementStoreExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("TupleElementStoreExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("index");
        serialize(printer, value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("TupleElementStoreExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::TypeOperator const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("TypeOperator");
    {
        printer.enter_property("operator_kind");
        serialize(printer, value->operator_kind());
        printer.exit_property("operator_kind");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("TypeOperator");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::TypeOperator::Kind value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Kind");
        } else {
            printer.enter_object("TypeOperator::Kind");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Kind");
        } else {
            printer.exit_object("TypeOperator::Kind");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::UnaryOperator const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("UnaryOperator");
    {
        printer.enter_property("operator_kind");
        serialize(printer, value->operator_kind());
        printer.exit_property("operator_kind");
    }
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("UnaryOperator");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::UnaryOperator::Kind value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Kind");
        } else {
            printer.enter_object("UnaryOperator::Kind");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Kind");
        } else {
            printer.exit_object("UnaryOperator::Kind");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::VariableReference const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VariableReference");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    printer.exit_object("VariableReference");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::VectorCreationExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VectorCreationExpression");
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("values");
        auto& list = value->values();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("values");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("VectorCreationExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::VectorElementLoadExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VectorElementLoadExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("index");
        serialize(printer, value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("VectorElementLoadExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::VectorElementStoreExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VectorElementStoreExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("index");
        serialize(printer, value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("VectorElementStoreExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::VectorLengthExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VectorLengthExpression");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    printer.exit_object("VectorLengthExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::AggregationExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("AggregationExpression");
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("keys");
        auto& list = value->keys();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("keys");
    }
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    {
        printer.enter_property("alias");
        serialize(printer, value->alias());
        printer.exit_property("alias");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("AggregationExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::AggregationExpression::Column const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Column");
    } else {
        printer.enter_object("AggregationExpression::Column");
    }
    {
        printer.enter_property("function");
        serialize(printer, value->function());
        printer.exit_property("function");
    }
    {
        printer.enter_property("quantifier");
        serialize(printer, value->quantifier());
        printer.exit_property("quantifier");
    }
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("alias");
        serialize(printer, value->alias());
        printer.exit_property("alias");
    }
    {
        printer.enter_property("function_key");
        serialize(printer, value->function_key());
        printer.exit_property("function_key");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Column");
    } else {
        printer.exit_object("AggregationExpression::Column");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::DistinctExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("DistinctExpression");
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("DistinctExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::GroupExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("GroupExpression");
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("keys");
        auto& list = value->keys();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("keys");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("GroupExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::JoinExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("JoinExpression");
    {
        printer.enter_property("operator_kind");
        serialize(printer, value->operator_kind());
        printer.exit_property("operator_kind");
    }
    {
        printer.enter_property("left");
        serialize(printer, value->left());
        printer.exit_property("left");
    }
    {
        printer.enter_property("right");
        serialize(printer, value->right());
        printer.exit_property("right");
    }
    {
        printer.enter_property("condition");
        serialize(printer, value->condition());
        printer.exit_property("condition");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("JoinExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::JoinExpression::Kind value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Kind");
        } else {
            printer.enter_object("JoinExpression::Kind");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Kind");
        } else {
            printer.exit_object("JoinExpression::Kind");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::LimitExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("LimitExpression");
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("count");
        printer.value(value->count());
        printer.exit_property("count");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("LimitExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::OrderExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("OrderExpression");
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("elements");
        auto& list = value->elements();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("elements");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("OrderExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::OrderExpression::Direction value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Direction");
        } else {
            printer.enter_object("OrderExpression::Direction");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Direction");
        } else {
            printer.exit_object("OrderExpression::Direction");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::OrderExpression::Element const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Element");
    } else {
        printer.enter_object("OrderExpression::Element");
    }
    {
        printer.enter_property("key");
        serialize(printer, value->key());
        printer.exit_property("key");
    }
    {
        printer.enter_property("direction");
        serialize(printer, value->direction());
        printer.exit_property("direction");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Element");
    } else {
        printer.exit_object("OrderExpression::Element");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::ProjectionExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ProjectionExpression");
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    {
        printer.enter_property("alias");
        serialize(printer, value->alias());
        printer.exit_property("alias");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("ProjectionExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::ProjectionExpression::Column const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Column");
    } else {
        printer.enter_object("ProjectionExpression::Column");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("alias");
        serialize(printer, value->alias());
        printer.exit_property("alias");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Column");
    } else {
        printer.exit_object("ProjectionExpression::Column");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::ScanExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ScanExpression");
    {
        printer.enter_property("table");
        serialize(printer, value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("alias");
        serialize(printer, value->alias());
        printer.exit_property("alias");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("ScanExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, expression::relation::SelectionExpression const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("SelectionExpression");
    {
        printer.enter_property("operand");
        serialize(printer, value->operand());
        printer.exit_property("operand");
    }
    {
        printer.enter_property("condition");
        serialize(printer, value->condition());
        printer.exit_property("condition");
    }
    {
        printer.enter_property("expression_key");
        serialize(printer, value->expression_key());
        printer.exit_property("expression_key");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("SelectionExpression");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] key::ExpressionKey const* value) {
    // external object
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ExpressionKey");
    printer.exit_object("ExpressionKey");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] key::FunctionKey const* value) {
    // external object
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("FunctionKey");
    printer.exit_object("FunctionKey");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] key::RelationKey const* value) {
    // external object
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("RelationKey");
    printer.exit_object("RelationKey");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] key::VariableKey const* value) {
    // external object
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VariableKey");
    printer.exit_object("VariableKey");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, name::Index const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Index");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("position");
        printer.value(value->position());
        printer.exit_property("position");
    }
    printer.exit_object("Index");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, name::Name const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    switch (value->kind()) {
    case name::QualifiedName::tag:
        serialize(printer, static_cast<name::QualifiedName const*>(value));  // NOLINT
        return;
    case name::SimpleName::tag:
        serialize(printer, static_cast<name::SimpleName const*>(value));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << value->kind();
    throw std::invalid_argument(ss.str());
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, name::QualifiedName const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("QualifiedName");
    {
        printer.enter_property("qualifier");
        serialize(printer, value->qualifier());
        printer.exit_property("qualifier");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    printer.exit_object("QualifiedName");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, name::SimpleName const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("SimpleName");
    {
        printer.enter_property("token");
        printer.value(value->token());
        printer.exit_property("token");
    }
    printer.exit_object("SimpleName");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::GlobalDeclaration const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    switch (value->kind()) {
    case program::GlobalFunctionDeclaration::tag:
        serialize(printer, static_cast<program::GlobalFunctionDeclaration const*>(value));  // NOLINT
        return;
    case program::GlobalVariableDeclaration::tag:
        serialize(printer, static_cast<program::GlobalVariableDeclaration const*>(value));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << value->kind();
    throw std::invalid_argument(ss.str());
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("GlobalFunctionDeclaration");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("parameters");
        auto& list = value->parameters();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("parameters");
    }
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    {
        printer.enter_property("attributes");
        auto& list = value->attributes();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("attributes");
    }
    {
        printer.enter_property("function_key");
        serialize(printer, value->function_key());
        printer.exit_property("function_key");
    }
    printer.exit_object("GlobalFunctionDeclaration");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration::Attribute value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Attribute");
        } else {
            printer.enter_object("GlobalFunctionDeclaration::Attribute");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Attribute");
        } else {
            printer.exit_object("GlobalFunctionDeclaration::Attribute");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration::Parameter const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Parameter");
    } else {
        printer.enter_object("GlobalFunctionDeclaration::Parameter");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("attributes");
        auto& list = value->attributes();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("attributes");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Parameter");
    } else {
        printer.exit_object("GlobalFunctionDeclaration::Parameter");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::GlobalFunctionDeclaration::Parameter::Attribute value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Attribute");
        } else {
            printer.enter_object("GlobalFunctionDeclaration::Parameter::Attribute");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Attribute");
        } else {
            printer.exit_object("GlobalFunctionDeclaration::Parameter::Attribute");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::GlobalVariableDeclaration const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("GlobalVariableDeclaration");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("attributes");
        auto& list = value->attributes();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("attributes");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    printer.exit_object("GlobalVariableDeclaration");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::GlobalVariableDeclaration::Attribute value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Attribute");
        } else {
            printer.enter_object("GlobalVariableDeclaration::Attribute");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Attribute");
        } else {
            printer.exit_object("GlobalVariableDeclaration::Attribute");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, program::Program const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Program");
    {
        printer.enter_property("declarations");
        auto& list = value->declarations();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("declarations");
    }
    {
        printer.enter_property("main");
        serialize(printer, value->main());
        printer.exit_property("main");
    }
    printer.exit_object("Program");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::AnchorDeclaration const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("AnchorDeclaration");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    printer.exit_object("AnchorDeclaration");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::BlockStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("BlockStatement");
    {
        printer.enter_property("elements");
        auto& list = value->elements();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("elements");
    }
    printer.exit_object("BlockStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::BranchStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("BranchStatement");
    {
        printer.enter_property("condition");
        serialize(printer, value->condition());
        printer.exit_property("condition");
    }
    {
        printer.enter_property("then_statement");
        serialize(printer, value->then_statement());
        printer.exit_property("then_statement");
    }
    {
        printer.enter_property("else_statement");
        serialize(printer, value->else_statement());
        printer.exit_property("else_statement");
    }
    printer.exit_object("BranchStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::BreakStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("BreakStatement");
    {
        printer.enter_property("anchor");
        serialize(printer, value->anchor());
        printer.exit_property("anchor");
    }
    printer.exit_object("BreakStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ContinueStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ContinueStatement");
    {
        printer.enter_property("anchor");
        serialize(printer, value->anchor());
        printer.exit_property("anchor");
    }
    printer.exit_object("ContinueStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] statement::EmptyStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("EmptyStatement");
    printer.exit_object("EmptyStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ExpressionStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ExpressionStatement");
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    printer.exit_object("ExpressionStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ForEachStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ForEachStatement");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("enumeration");
        serialize(printer, value->enumeration());
        printer.exit_property("enumeration");
    }
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    printer.exit_object("ForEachStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ForStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ForStatement");
    {
        printer.enter_property("initialize");
        auto& list = value->initialize();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("initialize");
    }
    {
        printer.enter_property("condition");
        serialize(printer, value->condition());
        printer.exit_property("condition");
    }
    {
        printer.enter_property("update");
        auto& list = value->update();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("update");
    }
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    printer.exit_object("ForStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::LocalVariableDeclaration const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("LocalVariableDeclaration");
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("attributes");
        auto& list = value->attributes();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("attributes");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    printer.exit_object("LocalVariableDeclaration");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::LocalVariableDeclaration::Attribute value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Attribute");
        } else {
            printer.enter_object("LocalVariableDeclaration::Attribute");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Attribute");
        } else {
            printer.exit_object("LocalVariableDeclaration::Attribute");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::LogStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("LogStatement");
    {
        printer.enter_property("level");
        serialize(printer, value->level());
        printer.exit_property("level");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    printer.exit_object("LogStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::LogStatement::Level value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Level");
        } else {
            printer.enter_object("LogStatement::Level");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Level");
        } else {
            printer.exit_object("LogStatement::Level");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::RaiseStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("RaiseStatement");
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    printer.exit_object("RaiseStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ReturnStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ReturnStatement");
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    printer.exit_object("ReturnStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::Statement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    switch (value->kind()) {
    case statement::AnchorDeclaration::tag:
        serialize(printer, static_cast<statement::AnchorDeclaration const*>(value));  // NOLINT
        return;
    case statement::BlockStatement::tag:
        serialize(printer, static_cast<statement::BlockStatement const*>(value));  // NOLINT
        return;
    case statement::BranchStatement::tag:
        serialize(printer, static_cast<statement::BranchStatement const*>(value));  // NOLINT
        return;
    case statement::BreakStatement::tag:
        serialize(printer, static_cast<statement::BreakStatement const*>(value));  // NOLINT
        return;
    case statement::ContinueStatement::tag:
        serialize(printer, static_cast<statement::ContinueStatement const*>(value));  // NOLINT
        return;
    case statement::EmptyStatement::tag:
        serialize(printer, static_cast<statement::EmptyStatement const*>(value));  // NOLINT
        return;
    case statement::ExpressionStatement::tag:
        serialize(printer, static_cast<statement::ExpressionStatement const*>(value));  // NOLINT
        return;
    case statement::ForEachStatement::tag:
        serialize(printer, static_cast<statement::ForEachStatement const*>(value));  // NOLINT
        return;
    case statement::ForStatement::tag:
        serialize(printer, static_cast<statement::ForStatement const*>(value));  // NOLINT
        return;
    case statement::LocalVariableDeclaration::tag:
        serialize(printer, static_cast<statement::LocalVariableDeclaration const*>(value));  // NOLINT
        return;
    case statement::LogStatement::tag:
        serialize(printer, static_cast<statement::LogStatement const*>(value));  // NOLINT
        return;
    case statement::RaiseStatement::tag:
        serialize(printer, static_cast<statement::RaiseStatement const*>(value));  // NOLINT
        return;
    case statement::ReturnStatement::tag:
        serialize(printer, static_cast<statement::ReturnStatement const*>(value));  // NOLINT
        return;
    case statement::VectorElementDeleteStatement::tag:
        serialize(printer, static_cast<statement::VectorElementDeleteStatement const*>(value));  // NOLINT
        return;
    case statement::VectorElementInsertStatement::tag:
        serialize(printer, static_cast<statement::VectorElementInsertStatement const*>(value));  // NOLINT
        return;
    case statement::WhileStatement::tag:
        serialize(printer, static_cast<statement::WhileStatement const*>(value));  // NOLINT
        return;
    case statement::ddl::CreateTableStatement::tag:
        serialize(printer, static_cast<statement::ddl::CreateTableStatement const*>(value));  // NOLINT
        return;
    case statement::ddl::DropTableStatement::tag:
        serialize(printer, static_cast<statement::ddl::DropTableStatement const*>(value));  // NOLINT
        return;
    case statement::dml::DeleteStatement::tag:
        serialize(printer, static_cast<statement::dml::DeleteStatement const*>(value));  // NOLINT
        return;
    case statement::dml::EmitStatement::tag:
        serialize(printer, static_cast<statement::dml::EmitStatement const*>(value));  // NOLINT
        return;
    case statement::dml::InsertRelationStatement::tag:
        serialize(printer, static_cast<statement::dml::InsertRelationStatement const*>(value));  // NOLINT
        return;
    case statement::dml::InsertValuesStatement::tag:
        serialize(printer, static_cast<statement::dml::InsertValuesStatement const*>(value));  // NOLINT
        return;
    case statement::dml::UpdateStatement::tag:
        serialize(printer, static_cast<statement::dml::UpdateStatement const*>(value));  // NOLINT
        return;
    case statement::transaction::TransactionBlockStatement::tag:
        serialize(printer, static_cast<statement::transaction::TransactionBlockStatement const*>(value));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << value->kind();
    throw std::invalid_argument(ss.str());
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::VectorElementDeleteStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VectorElementDeleteStatement");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("begin");
        serialize(printer, value->begin());
        printer.exit_property("begin");
    }
    {
        printer.enter_property("end");
        serialize(printer, value->end());
        printer.exit_property("end");
    }
    printer.exit_object("VectorElementDeleteStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::VectorElementInsertStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VectorElementInsertStatement");
    {
        printer.enter_property("data");
        serialize(printer, value->data());
        printer.exit_property("data");
    }
    {
        printer.enter_property("index");
        serialize(printer, value->index());
        printer.exit_property("index");
    }
    {
        printer.enter_property("values");
        auto& list = value->values();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("values");
    }
    printer.exit_object("VectorElementInsertStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::WhileStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("WhileStatement");
    {
        printer.enter_property("condition");
        serialize(printer, value->condition());
        printer.exit_property("condition");
    }
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    printer.exit_object("WhileStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("CreateTableStatement");
    {
        printer.enter_property("table");
        serialize(printer, value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    {
        printer.enter_property("attributes");
        auto& list = value->attributes();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("attributes");
    }
    {
        printer.enter_property("primary_keys");
        auto& list = value->primary_keys();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("primary_keys");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("CreateTableStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::Attribute value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Attribute");
        } else {
            printer.enter_object("CreateTableStatement::Attribute");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Attribute");
        } else {
            printer.exit_object("CreateTableStatement::Attribute");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::Column const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Column");
    } else {
        printer.enter_object("CreateTableStatement::Column");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("attributes");
        auto& list = value->attributes();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("attributes");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Column");
    } else {
        printer.exit_object("CreateTableStatement::Column");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::Column::Attribute value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Attribute");
        } else {
            printer.enter_object("CreateTableStatement::Column::Attribute");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Attribute");
        } else {
            printer.exit_object("CreateTableStatement::Column::Attribute");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::PrimaryKey const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("PrimaryKey");
    } else {
        printer.enter_object("CreateTableStatement::PrimaryKey");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("direction");
        serialize(printer, value->direction());
        printer.exit_property("direction");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("PrimaryKey");
    } else {
        printer.exit_object("CreateTableStatement::PrimaryKey");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::CreateTableStatement::PrimaryKey::Direction value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Direction");
        } else {
            printer.enter_object("CreateTableStatement::PrimaryKey::Direction");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Direction");
        } else {
            printer.exit_object("CreateTableStatement::PrimaryKey::Direction");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::DropTableStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("DropTableStatement");
    {
        printer.enter_property("table");
        serialize(printer, value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("attributes");
        auto& list = value->attributes();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("attributes");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("DropTableStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::ddl::DropTableStatement::Attribute value) {
    if (show_enum_kind()) {
        if (show_qualified_kind()) {
            printer.enter_object("Attribute");
        } else {
            printer.enter_object("DropTableStatement::Attribute");
        }
        printer.enter_property("value");
        printer.value(impl_->to_string(value));
        printer.exit_property("value");
        if (show_qualified_kind()) {
            printer.exit_object("Attribute");
        } else {
            printer.exit_object("DropTableStatement::Attribute");
        }
    } else {
        printer.value(impl_->to_string(value));
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::dml::DeleteStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("DeleteStatement");
    {
        printer.enter_property("source");
        serialize(printer, value->source());
        printer.exit_property("source");
    }
    {
        printer.enter_property("table");
        serialize(printer, value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("DeleteStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::dml::EmitStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("EmitStatement");
    {
        printer.enter_property("source");
        serialize(printer, value->source());
        printer.exit_property("source");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("EmitStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::dml::InsertRelationStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("InsertRelationStatement");
    {
        printer.enter_property("table");
        serialize(printer, value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    {
        printer.enter_property("source");
        serialize(printer, value->source());
        printer.exit_property("source");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("InsertRelationStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::dml::InsertValuesStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("InsertValuesStatement");
    {
        printer.enter_property("table");
        serialize(printer, value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("initialize");
        auto& list = value->initialize();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("initialize");
    }
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("InsertValuesStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::dml::InsertValuesStatement::Column const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Column");
    } else {
        printer.enter_object("InsertValuesStatement::Column");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Column");
    } else {
        printer.exit_object("InsertValuesStatement::Column");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::dml::UpdateStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("UpdateStatement");
    {
        printer.enter_property("source");
        serialize(printer, value->source());
        printer.exit_property("source");
    }
    {
        printer.enter_property("table");
        serialize(printer, value->table());
        printer.exit_property("table");
    }
    {
        printer.enter_property("initialize");
        auto& list = value->initialize();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("initialize");
    }
    {
        printer.enter_property("columns");
        auto& list = value->columns();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("columns");
    }
    {
        printer.enter_property("relation_key");
        serialize(printer, value->relation_key());
        printer.exit_property("relation_key");
    }
    printer.exit_object("UpdateStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::dml::UpdateStatement::Column const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Column");
    } else {
        printer.enter_object("UpdateStatement::Column");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("value");
        serialize(printer, value->value());
        printer.exit_property("value");
    }
    {
        printer.enter_property("variable_key");
        serialize(printer, value->variable_key());
        printer.exit_property("variable_key");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Column");
    } else {
        printer.exit_object("UpdateStatement::Column");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, statement::transaction::TransactionBlockStatement const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("TransactionBlockStatement");
    {
        printer.enter_property("body");
        serialize(printer, value->body());
        printer.exit_property("body");
    }
    printer.exit_object("TransactionBlockStatement");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::ArrayType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("ArrayType");
    {
        printer.enter_property("element_type");
        serialize(printer, value->element_type());
        printer.exit_property("element_type");
    }
    {
        printer.enter_property("size");
        printer.value(value->size());
        printer.exit_property("size");
    }
    printer.exit_object("ArrayType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] type::BooleanType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("BooleanType");
    printer.exit_object("BooleanType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::CharType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("CharType");
    {
        printer.enter_property("size");
        printer.value(value->size());
        printer.exit_property("size");
    }
    printer.exit_object("CharType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] type::Float32Type const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Float32Type");
    printer.exit_object("Float32Type");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] type::Float64Type const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Float64Type");
    printer.exit_object("Float64Type");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] type::Int32Type const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Int32Type");
    printer.exit_object("Int32Type");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] type::Int64Type const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("Int64Type");
    printer.exit_object("Int64Type");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] type::NullType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("NullType");
    printer.exit_object("NullType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::RelationType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("RelationType");
    {
        printer.enter_property("element_type");
        serialize(printer, value->element_type());
        printer.exit_property("element_type");
    }
    printer.exit_object("RelationType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, [[maybe_unused]] type::StringType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("StringType");
    printer.exit_object("StringType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::TupleType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("TupleType");
    {
        printer.enter_property("elements");
        auto& list = value->elements();
        auto size = list.size();
        printer.enter_array(size);
        for (auto element : list) {
            serialize(printer, element);
        }
        printer.exit_array(size);
        printer.exit_property("elements");
    }
    printer.exit_object("TupleType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::TupleType::Element const* value) {
    if (!show_fragment_kind()) {
        printer.enter_object({});
    } else if (show_qualified_kind()) {
        printer.enter_object("Element");
    } else {
        printer.enter_object("TupleType::Element");
    }
    {
        printer.enter_property("name");
        serialize(printer, value->name());
        printer.exit_property("name");
    }
    {
        printer.enter_property("type");
        serialize(printer, value->type());
        printer.exit_property("type");
    }
    if (!show_fragment_kind()) {
        printer.exit_object({});
    } else if (show_qualified_kind()) {
        printer.exit_object("Element");
    } else {
        printer.exit_object("TupleType::Element");
    }
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::Type const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    switch (value->kind()) {
    case type::ArrayType::tag:
        serialize(printer, static_cast<type::ArrayType const*>(value));  // NOLINT
        return;
    case type::BooleanType::tag:
        serialize(printer, static_cast<type::BooleanType const*>(value));  // NOLINT
        return;
    case type::CharType::tag:
        serialize(printer, static_cast<type::CharType const*>(value));  // NOLINT
        return;
    case type::Float32Type::tag:
        serialize(printer, static_cast<type::Float32Type const*>(value));  // NOLINT
        return;
    case type::Float64Type::tag:
        serialize(printer, static_cast<type::Float64Type const*>(value));  // NOLINT
        return;
    case type::Int32Type::tag:
        serialize(printer, static_cast<type::Int32Type const*>(value));  // NOLINT
        return;
    case type::Int64Type::tag:
        serialize(printer, static_cast<type::Int64Type const*>(value));  // NOLINT
        return;
    case type::NullType::tag:
        serialize(printer, static_cast<type::NullType const*>(value));  // NOLINT
        return;
    case type::RelationType::tag:
        serialize(printer, static_cast<type::RelationType const*>(value));  // NOLINT
        return;
    case type::StringType::tag:
        serialize(printer, static_cast<type::StringType const*>(value));  // NOLINT
        return;
    case type::TupleType::tag:
        serialize(printer, static_cast<type::TupleType const*>(value));  // NOLINT
        return;
    case type::VarCharType::tag:
        serialize(printer, static_cast<type::VarCharType const*>(value));  // NOLINT
        return;
    case type::VectorType::tag:
        serialize(printer, static_cast<type::VectorType const*>(value));  // NOLINT
        return;
    }
    std::ostringstream ss;
    ss << "unknown node kind: " << value->kind();
    throw std::invalid_argument(ss.str());
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::VarCharType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VarCharType");
    {
        printer.enter_property("size");
        printer.value(value->size());
        printer.exit_property("size");
    }
    printer.exit_object("VarCharType");
}

void NodeSerializerBase::serialize(common::util::DataSerializer& printer, type::VectorType const* value) {
    if (value == nullptr) {
        printer.value(nullptr);
        return;
    }
    printer.enter_object("VectorType");
    {
        printer.enter_property("element_type");
        serialize(printer, value->element_type());
        printer.exit_property("element_type");
    }
    printer.exit_object("VectorType");
}

}  // namespace shakujo::model::util

