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
#include "shakujo/model/type/TypeKind.h"

#include <iostream>


namespace shakujo::model::type {
std::ostream& operator<<(std::ostream& out, TypeKind value) {
    switch (value) {
    case TypeKind::ARRAY_TYPE:
        out << "ARRAY_TYPE";
        break;
    case TypeKind::BOOLEAN_TYPE:
        out << "BOOLEAN_TYPE";
        break;
    case TypeKind::CHAR_TYPE:
        out << "CHAR_TYPE";
        break;
    case TypeKind::FLOAT32_TYPE:
        out << "FLOAT32_TYPE";
        break;
    case TypeKind::FLOAT64_TYPE:
        out << "FLOAT64_TYPE";
        break;
    case TypeKind::INT32_TYPE:
        out << "INT32_TYPE";
        break;
    case TypeKind::INT64_TYPE:
        out << "INT64_TYPE";
        break;
    case TypeKind::NULL_TYPE:
        out << "NULL_TYPE";
        break;
    case TypeKind::RELATION_TYPE:
        out << "RELATION_TYPE";
        break;
    case TypeKind::STRING_TYPE:
        out << "STRING_TYPE";
        break;
    case TypeKind::TUPLE_TYPE:
        out << "TUPLE_TYPE";
        break;
    case TypeKind::VECTOR_TYPE:
        out << "VECTOR_TYPE";
        break;
    }
    return out;
}

}  // namespace shakujo::model::type

