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
#ifndef SHAKUJO_MODEL_NAME_NAME_KIND_H_
#define SHAKUJO_MODEL_NAME_NAME_KIND_H_

#include <iostream>


namespace shakujo::model::name {
/**
 * @brief Represents the catalog of Name.
 * @see Name::kind()
 */
enum class NameKind {
    /**
     * @brief tag for QualifiedName.
     * @see QualifiedName
     */
    QUALIFIED_NAME,
    /**
     * @brief tag for SimpleName.
     * @see SimpleName
     */
    SIMPLE_NAME,
};

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see NameKind
 */
std::ostream& operator<<(std::ostream& out, NameKind value);

}  // namespace shakujo::model::name

#endif  // SHAKUJO_MODEL_NAME_NAME_KIND_H_
