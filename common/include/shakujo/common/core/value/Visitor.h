/*
 * Copyright 2018-2018 shakujo project.
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
#ifndef SHAKUJO_COMMON_CORE_VALUE_VISITOR_H_
#define SHAKUJO_COMMON_CORE_VALUE_VISITOR_H_

#include <stdexcept>
#include <utility>
#include <sstream>

#include "../Value.h"
#include "Bool.h"
#include "Error.h"
#include "Float.h"
#include "Int.h"
#include "Null.h"
#include "Placeholder.h"
#include "String.h"
#include "Tuple.h"

namespace shakujo::common::core::value {

/**
 * @brief A visitor for Value.
 * The default implementation of visit() will be redirected to visitDefault().
 * @tparam Return the result type
 * @tparam Args the parameter types
 * @see Value
 */
template <class Return, class... Args>
class Visitor {
public:
    /**
     * @brief Processes the default Value.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Value
     */
    virtual Return visitDefault(Value* node, [[maybe_unused]] Args... args) {  // NOLINT
        std::ostringstream ss;
        ss << "visit for " << node->kind() << " is not implemented.";
        throw std::runtime_error(ss.str());
    }

    /**
     * @brief Processes Bool.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Bool
     */
    virtual Return visit(Bool* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Int.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Int
     */
    virtual Return visit(Int* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Float.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Float
     */
    virtual Return visit(Float* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Placeholder.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Null
     */
    virtual Return visit(Placeholder* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes String.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see String
     */
    virtual Return visit(String* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Tuple.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Tuple
     */
    virtual Return visit(Tuple* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Null.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Null
     */
    virtual Return visit(Null* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Processes Error.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Error
     */
    virtual Return visit(Error* node, Args... args) {  // NOLINT
        return visitDefault(node, std::forward<Args>(args)...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Value* node, Args... args) {  // NOLINT
        switch (node->kind()) {
        case Bool::tag:
            return visit(static_cast<Bool*>(node), std::forward<Args>(args)...);  // NOLINT
        case Int::tag:
            return visit(static_cast<Int*>(node), std::forward<Args>(args)...);  // NOLINT
        case Float::tag:
            return visit(static_cast<Float*>(node), std::forward<Args>(args)...);  // NOLINT
        case Placeholder::tag:
            return visit(static_cast<Placeholder*>(node), std::forward<Args>(args)...);  // NOLINT
        case String::tag:
            return visit(static_cast<String*>(node), std::forward<Args>(args)...);  // NOLINT
        case Tuple::tag:
            return visit(static_cast<Tuple*>(node), std::forward<Args>(args)...);  // NOLINT
        case Null::tag:
            return visit(static_cast<Null*>(node), std::forward<Args>(args)...);  // NOLINT
        case Error::tag:
            return visit(static_cast<Error*>(node), std::forward<Args>(args)...);  // NOLINT
        }
        // may not occur
        return visitDefault(node, std::forward<Args>(args)...);
    }
};

}  // namespace shakujo::common::core::value

#endif  // SHAKUJO_COMMON_CORE_VALUE_VISITOR_H_
