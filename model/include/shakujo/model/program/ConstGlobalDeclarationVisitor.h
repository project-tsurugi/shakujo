/*
 * Copyright 2018-2023 tsurugi project..
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
#ifndef SHAKUJO_MODEL_PROGRAM_CONST_GLOBAL_DECLARATION_VISITOR_H_
#define SHAKUJO_MODEL_PROGRAM_CONST_GLOBAL_DECLARATION_VISITOR_H_

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
class ConstGlobalDeclarationVisitor {
public:
    /**
     * @brief Processes the default GlobalDeclaration.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see GlobalDeclaration
     */
    virtual Return visitDefault(GlobalDeclaration const* node, [[maybe_unused]] Args... args) {
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
    virtual Return visit(GlobalFunctionDeclaration const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes GlobalVariableDeclaration.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see GlobalVariableDeclaration
     */
    virtual Return visit(GlobalVariableDeclaration const* node, Args... args) {
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(GlobalDeclaration const* node, Args... args) {
        switch (node->kind()) {
        case GlobalFunctionDeclaration::tag:
            return visit(static_cast<GlobalFunctionDeclaration const*>(node), std::forward<Args>(args)...);  // NOLINT
        case GlobalVariableDeclaration::tag:
            return visit(static_cast<GlobalVariableDeclaration const*>(node), std::forward<Args>(args)...);  // NOLINT
        }
        // may not occur
        return visitDefault(node, std::forward<Args>(args)...);
    }

};

}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_CONST_GLOBAL_DECLARATION_VISITOR_H_
