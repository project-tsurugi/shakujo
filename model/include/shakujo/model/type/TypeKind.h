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
#ifndef SHAKUJO_MODEL_TYPE_TYPE_KIND_H_
#define SHAKUJO_MODEL_TYPE_TYPE_KIND_H_

#include <iostream>


namespace shakujo::model::type {
/**
 * @brief Represents the catalog of Type.
 * @see Type::kind()
 */
enum class TypeKind {
    /**
     * @brief tag for ArrayType.
     * @see ArrayType
     */
    ARRAY_TYPE,
    /**
     * @brief tag for BooleanType.
     * @see BooleanType
     */
    BOOLEAN_TYPE,
    /**
     * @brief tag for CharType.
     * @see CharType
     */
    CHAR_TYPE,
    /**
     * @brief tag for Float32Type.
     * @see Float32Type
     */
    FLOAT32_TYPE,
    /**
     * @brief tag for Float64Type.
     * @see Float64Type
     */
    FLOAT64_TYPE,
    /**
     * @brief tag for Int32Type.
     * @see Int32Type
     */
    INT32_TYPE,
    /**
     * @brief tag for Int64Type.
     * @see Int64Type
     */
    INT64_TYPE,
    /**
     * @brief tag for NullType.
     * @see NullType
     */
    NULL_TYPE,
    /**
     * @brief tag for RelationType.
     * @see RelationType
     */
    RELATION_TYPE,
    /**
     * @brief tag for StringType.
     * @see StringType
     */
    STRING_TYPE,
    /**
     * @brief tag for TupleType.
     * @see TupleType
     */
    TUPLE_TYPE,
    /**
     * @brief tag for VarCharType.
     * @see VarCharType
     */
    VAR_CHAR_TYPE,
    /**
     * @brief tag for VectorType.
     * @see VectorType
     */
    VECTOR_TYPE,
};

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see TypeKind
 */
std::ostream& operator<<(std::ostream& out, TypeKind value);

}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_TYPE_KIND_H_
