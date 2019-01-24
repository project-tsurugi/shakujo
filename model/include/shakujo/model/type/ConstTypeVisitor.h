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
#ifndef SHAKUJO_MODEL_TYPE_CONST_TYPE_VISITOR_H_
#define SHAKUJO_MODEL_TYPE_CONST_TYPE_VISITOR_H_

#include <stdexcept>
#include <utility>
#include <sstream>

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
#include "shakujo/model/type/TypeKind.h"
#include "shakujo/model/type/VarCharType.h"
#include "shakujo/model/type/VectorType.h"

namespace shakujo::model::type {
/**
 * @brief A visitor for Type.
 * The default implementation of visit() will be redirected to visitDefault().
 * @tparam Return the result type
 * @tparam Args the parameter types
 * @see Type
 */
template <class Return, class... Args>
class ConstTypeVisitor {
public:
    /**
     * @brief Processes the default Type.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Type
     */
    virtual Return visitDefault(Type const* node, [[maybe_unused]] Args... args) {
        std::ostringstream ss;
        ss << "visit for " << node->kind() << " is not implemented.";
        throw std::runtime_error(ss.str());
    }

    /**
     * @brief Processes ArrayType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see ArrayType
     */
    virtual Return visit(ArrayType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes BooleanType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see BooleanType
     */
    virtual Return visit(BooleanType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes CharType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see CharType
     */
    virtual Return visit(CharType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Float32Type.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Float32Type
     */
    virtual Return visit(Float32Type const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Float64Type.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Float64Type
     */
    virtual Return visit(Float64Type const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Int32Type.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Int32Type
     */
    virtual Return visit(Int32Type const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Int64Type.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Int64Type
     */
    virtual Return visit(Int64Type const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes NullType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see NullType
     */
    virtual Return visit(NullType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes RelationType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see RelationType
     */
    virtual Return visit(RelationType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes StringType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see StringType
     */
    virtual Return visit(StringType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes TupleType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see TupleType
     */
    virtual Return visit(TupleType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VarCharType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VarCharType
     */
    virtual Return visit(VarCharType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes VectorType.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see VectorType
     */
    virtual Return visit(VectorType const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Type const* node, Args... args) {
        switch (node->kind()) {
        case TypeKind::ARRAY_TYPE:
            return visit(dynamic_cast<ArrayType const*>(node), args...);
        case TypeKind::BOOLEAN_TYPE:
            return visit(dynamic_cast<BooleanType const*>(node), args...);
        case TypeKind::CHAR_TYPE:
            return visit(dynamic_cast<CharType const*>(node), args...);
        case TypeKind::FLOAT32_TYPE:
            return visit(dynamic_cast<Float32Type const*>(node), args...);
        case TypeKind::FLOAT64_TYPE:
            return visit(dynamic_cast<Float64Type const*>(node), args...);
        case TypeKind::INT32_TYPE:
            return visit(dynamic_cast<Int32Type const*>(node), args...);
        case TypeKind::INT64_TYPE:
            return visit(dynamic_cast<Int64Type const*>(node), args...);
        case TypeKind::NULL_TYPE:
            return visit(dynamic_cast<NullType const*>(node), args...);
        case TypeKind::RELATION_TYPE:
            return visit(dynamic_cast<RelationType const*>(node), args...);
        case TypeKind::STRING_TYPE:
            return visit(dynamic_cast<StringType const*>(node), args...);
        case TypeKind::TUPLE_TYPE:
            return visit(dynamic_cast<TupleType const*>(node), args...);
        case TypeKind::VAR_CHAR_TYPE:
            return visit(dynamic_cast<VarCharType const*>(node), args...);
        case TypeKind::VECTOR_TYPE:
            return visit(dynamic_cast<VectorType const*>(node), args...);
        }
        // may not occur
        return visitDefault(node, args...);
    }

};

}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_CONST_TYPE_VISITOR_H_
