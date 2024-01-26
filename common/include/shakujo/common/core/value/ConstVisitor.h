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
#ifndef SHAKUJO_COMMON_CORE_VALUE_CONST_VISITOR_H_
#define SHAKUJO_COMMON_CORE_VALUE_CONST_VISITOR_H_

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
class ConstVisitor {
public:
    /**
     * @brief Processes the default Value.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Value
     */
    virtual Return visitDefault(Value const* node, Args... args) {
        (void) sizeof...(args); // ignore unused
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
    virtual Return visit(Bool const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Int.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Int
     */
    virtual Return visit(Int const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Float.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Float
     */
    virtual Return visit(Float const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Placeholder.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Null
     */
    virtual Return visit(Placeholder const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes String.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see String
     */
    virtual Return visit(String const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Tuple.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Tuple
     */
    virtual Return visit(Tuple const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Null.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Null
     */
    virtual Return visit(Null const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Error.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Error
     */
    virtual Return visit(Error const* node, Args... args) {
        return visitDefault(node, args...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Value const* node, Args... args) {
        switch (node->kind()) {
        case Bool::tag:
            return visit(static_cast<Bool const*>(node), args...);  // NOLINT
        case Int::tag:
            return visit(static_cast<Int const*>(node), args...);  // NOLINT
        case Float::tag:
            return visit(static_cast<Float const*>(node), args...);  // NOLINT
        case Placeholder::tag:
            return visit(static_cast<Placeholder const*>(node), args...);  // NOLINT
        case String::tag:
            return visit(static_cast<String const*>(node), args...);  // NOLINT
        case Tuple::tag:
            return visit(static_cast<Tuple const*>(node), args...);  // NOLINT
        case Null::tag:
            return visit(static_cast<Null const*>(node), args...);  // NOLINT
        case Error::tag:
            return visit(static_cast<Error const*>(node), args...);  // NOLINT
        }
        // may not occur
        return visitDefault(node, args...);
    }
};

}  // namespace shakujo::common::core::value

#endif  // SHAKUJO_COMMON_CORE_VALUE_CONST_VISITOR_H_
