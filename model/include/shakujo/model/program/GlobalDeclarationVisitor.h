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
#ifndef SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_VISITOR_H_
#define SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_VISITOR_H_

#include <stdexcept>
#include <utility>
#include <sstream>

#include "shakujo/model/program/GlobalDeclarationKind.h"
#include "shakujo/model/program/GlobalFunctionDeclaration.h"
#include "shakujo/model/program/GlobalVariableDeclaration.h"

namespace shakujo::model::program {
/**
 * @brief A visitor for GlobalDeclaration.
 * The default implementation of visit() will be redirected to visitDefault().
 * @tparam Return the result type
 * @tparam Args the parameter types
 * @see GlobalDeclaration
 */
template <class Return, class... Args>
class GlobalDeclarationVisitor {
public:
    /**
     * @brief Processes the default GlobalDeclaration.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see GlobalDeclaration
     */
    virtual Return visitDefault(GlobalDeclaration* node, [[maybe_unused]] Args... args) {  // NOLINT
        std::ostringstream ss;
        ss << "visit for " << node->kind() << " is not implemented.";
        throw std::runtime_error(ss.str());
    }

    /**
     * @brief Processes GlobalFunctionDeclaration.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see GlobalFunctionDeclaration
     */
    virtual Return visit(GlobalFunctionDeclaration* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes GlobalVariableDeclaration.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see GlobalVariableDeclaration
     */
    virtual Return visit(GlobalVariableDeclaration* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(GlobalDeclaration* node, Args... args) {  // NOLINT
        switch (node->kind()) {
        case GlobalDeclarationKind::GLOBAL_FUNCTION_DECLARATION:
            return visit(dynamic_cast<GlobalFunctionDeclaration*>(node), args...);
        case GlobalDeclarationKind::GLOBAL_VARIABLE_DECLARATION:
            return visit(dynamic_cast<GlobalVariableDeclaration*>(node), args...);
        }
        // may not occur
        return visitDefault(node, args...);
    }

};

}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_VISITOR_H_
