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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_TYPE_TYPE_KIND_H_
#define SHAKUJO_MODEL_TYPE_TYPE_KIND_H_

#include <string>
#include <string_view>
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
     * @brief tag for BinaryType.
     * @see BinaryType
     */
    BINARY_TYPE,
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
     * @brief tag for DateType.
     * @see DateType
     */
    DATE_TYPE,
    /**
     * @brief tag for DecimalType.
     * @see DecimalType
     */
    DECIMAL_TYPE,
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
     * @brief tag for TimeType.
     * @see TimeType
     */
    TIME_TYPE,
    /**
     * @brief tag for TimestampType.
     * @see TimestampType
     */
    TIMESTAMP_TYPE,
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
     * @brief tag for VarBinaryType.
     * @see VarBinaryType
     */
    VAR_BINARY_TYPE,
    /**
     * @brief tag for VectorType.
     * @see VectorType
     */
    VECTOR_TYPE,
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see TypeKind
 */
inline constexpr std::string_view to_string_view(TypeKind value) {
    switch (value) {
        case TypeKind::ARRAY_TYPE: return "ARRAY_TYPE";
        case TypeKind::BINARY_TYPE: return "BINARY_TYPE";
        case TypeKind::BOOLEAN_TYPE: return "BOOLEAN_TYPE";
        case TypeKind::CHAR_TYPE: return "CHAR_TYPE";
        case TypeKind::DATE_TYPE: return "DATE_TYPE";
        case TypeKind::DECIMAL_TYPE: return "DECIMAL_TYPE";
        case TypeKind::FLOAT32_TYPE: return "FLOAT32_TYPE";
        case TypeKind::FLOAT64_TYPE: return "FLOAT64_TYPE";
        case TypeKind::INT32_TYPE: return "INT32_TYPE";
        case TypeKind::INT64_TYPE: return "INT64_TYPE";
        case TypeKind::NULL_TYPE: return "NULL_TYPE";
        case TypeKind::RELATION_TYPE: return "RELATION_TYPE";
        case TypeKind::STRING_TYPE: return "STRING_TYPE";
        case TypeKind::TIME_TYPE: return "TIME_TYPE";
        case TypeKind::TIMESTAMP_TYPE: return "TIMESTAMP_TYPE";
        case TypeKind::TUPLE_TYPE: return "TUPLE_TYPE";
        case TypeKind::VAR_BINARY_TYPE: return "VAR_BINARY_TYPE";
        case TypeKind::VAR_CHAR_TYPE: return "VAR_CHAR_TYPE";
        case TypeKind::VECTOR_TYPE: return "VECTOR_TYPE";
    }
    return "(unknown)";
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see TypeKind
 */
inline std::ostream& operator<<(std::ostream& out, TypeKind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_TYPE_KIND_H_
