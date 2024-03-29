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
#ifndef SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_KIND_H_
#define SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_KIND_H_

#include <string>
#include <string_view>
#include <iostream>


namespace shakujo::model::program {
/**
 * @brief Represents the catalog of GlobalDeclaration.
 * @see GlobalDeclaration::kind()
 */
enum class GlobalDeclarationKind {
    /**
     * @brief tag for GlobalFunctionDeclaration.
     * @see GlobalFunctionDeclaration
     */
    GLOBAL_FUNCTION_DECLARATION,
    /**
     * @brief tag for GlobalVariableDeclaration.
     * @see GlobalVariableDeclaration
     */
    GLOBAL_VARIABLE_DECLARATION,
};

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see GlobalDeclarationKind
 */
inline constexpr std::string_view to_string_view(GlobalDeclarationKind value) {
    switch (value) {
        case GlobalDeclarationKind::GLOBAL_FUNCTION_DECLARATION: return "GLOBAL_FUNCTION_DECLARATION";
        case GlobalDeclarationKind::GLOBAL_VARIABLE_DECLARATION: return "GLOBAL_VARIABLE_DECLARATION";
    }
    return "(unknown)";
}

/**
 * @brief Appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see GlobalDeclarationKind
 */
inline std::ostream& operator<<(std::ostream& out, GlobalDeclarationKind value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_KIND_H_
