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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_NAME_NAME_VISITOR_H_
#define SHAKUJO_MODEL_NAME_NAME_VISITOR_H_

#include <stdexcept>
#include <utility>
#include <sstream>

#include "shakujo/model/name/NameKind.h"
#include "shakujo/model/name/QualifiedName.h"
#include "shakujo/model/name/SimpleName.h"

namespace shakujo::model::name {
/**
 * @brief A visitor for Name.
 * The default implementation of visit() will be redirected to visitDefault().
 * @tparam Return the result type
 * @tparam Args the parameter types
 * @see Name
 */
template <class Return, class... Args>
class NameVisitor {
public:
    /**
     * @brief Processes the default Name.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Name
     */
    virtual Return visitDefault(Name* node, [[maybe_unused]] Args... args) {  // NOLINT
        std::ostringstream ss;
        ss << "visit for " << node->kind() << " is not implemented.";
        throw std::runtime_error(ss.str());
    }

    /**
     * @brief Processes QualifiedName.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see QualifiedName
     */
    virtual Return visit(QualifiedName* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes SimpleName.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see SimpleName
     */
    virtual Return visit(SimpleName* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Name* node, Args... args) {  // NOLINT
        switch (node->kind()) {
        case QualifiedName::tag:
            return visit(static_cast<QualifiedName*>(node), std::forward<Args>(args)...);  // NOLINT
        case SimpleName::tag:
            return visit(static_cast<SimpleName*>(node), std::forward<Args>(args)...);  // NOLINT
        }
        // may not occur
        return visitDefault(node, std::forward<Args>(args)...);
    }

};

}  // namespace shakujo::model::name

#endif  // SHAKUJO_MODEL_NAME_NAME_VISITOR_H_
