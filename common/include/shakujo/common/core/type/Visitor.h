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
#ifndef SHAKUJO_COMMON_CORE_TYPE_VISITOR_H_
#define SHAKUJO_COMMON_CORE_TYPE_VISITOR_H_

#include <stdexcept>
#include <utility>
#include <sstream>

#include "../Type.h"
#include "Array.h"
#include "Bool.h"
#include "Char.h"
#include "Cursor.h"
#include "Error.h"
#include "Float.h"
#include "Int.h"
#include "Null.h"
#include "Relation.h"
#include "String.h"
#include "Tuple.h"
#include "Vector.h"

namespace shakujo::common::core::type {

/**
 * @brief A visitor for Type.
 * The default implementation of visit() will be redirected to visitDefault().
 * @tparam Return the result type
 * @tparam Args the parameter types
 * @see Type
 */
template <class Return, class... Args>
class Visitor {
public:
    /**
     * @brief Processes the default Type.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Type
     */
    virtual Return visitDefault(Type* node, Args... args) {  // NOLINT
        (void) sizeof...(args); // ignore unused
        std::ostringstream ss;
        ss << "visit for " << node->kind() << " is not implemented.";
        throw std::runtime_error(ss.str());
    }

    /**
     * @brief Processes Int.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Int
     */
    virtual Return visit(Int* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Float.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Float
     */
    virtual Return visit(Float* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Char.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Char
     */
    virtual Return visit(Char* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes String.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see String
     */
    virtual Return visit(String* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Bool.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Bool
     */
    virtual Return visit(Bool* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Null.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Null
     */
    virtual Return visit(Null* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Tuple.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Tuple
     */
    virtual Return visit(Tuple* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Array.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Array
     */
    virtual Return visit(Array* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Vector.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Vector
     */
    virtual Return visit(Vector* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Relation.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Relation
     */
    virtual Return visit(Relation* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Cursor.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Cursor
     */
    virtual Return visit(Cursor* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Processes Error.
     * @param node the processing target
     * @param args the processing arguments
     * @return the result
     * @see Error
     */
    virtual Return visit(Error* node, Args... args) {  // NOLINT
        return visitDefault(node, args...);
    }

    /**
     * @brief Dispatches visitor action for individual element types.
     * @param node the processing target element
     * @param args the processing arguments
     * @return the result
     */
    Return dispatch(Type* node, Args... args) {  // NOLINT
        switch (node->kind()) {
        case Type::Kind::INT:
            return visit(dynamic_cast<Int*>(node), args...);
        case Type::Kind::FLOAT:
            return visit(dynamic_cast<Float*>(node), args...);
        case Type::Kind::CHAR:
            return visit(dynamic_cast<Char*>(node), args...);
        case Type::Kind::STRING:
            return visit(dynamic_cast<String*>(node), args...);
        case Type::Kind::BOOL:
            return visit(dynamic_cast<Bool*>(node), args...);
        case Type::Kind::NULL_:
            return visit(dynamic_cast<Null*>(node), args...);
        case Type::Kind::TUPLE:
            return visit(dynamic_cast<Tuple*>(node), args...);
        case Type::Kind::ARRAY:
            return visit(dynamic_cast<Array*>(node), args...);
        case Type::Kind::VECTOR:
            return visit(dynamic_cast<Vector*>(node), args...);
        case Type::Kind::RELATION:
            return visit(dynamic_cast<Relation*>(node), args...);
        case Type::Kind::CURSOR:
            return visit(dynamic_cast<Cursor*>(node), args...);
        case Type::Kind::ERROR:
            return visit(dynamic_cast<Error*>(node), args...);
        }
        // may not occur
        return visitDefault(node, args...);
    }
};

}  // namespace shakujo::common::core::type

#endif  // SHAKUJO_COMMON_CORE_TYPE_VISITOR_H_
