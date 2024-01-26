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
#ifndef SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_H_
#define SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_H_

#include "shakujo/model/Node.h"
#include "shakujo/model/program/GlobalDeclarationKind.h"

namespace shakujo::model::program {
/**
 * @brief Represents top level declaration.
 */
class GlobalDeclaration
        : public Node {
public:
    /**
     * @brief Constructs a new empty object.
     */
    GlobalDeclaration() noexcept;

    /**
     * @brief Destroys this object.
     */
    ~GlobalDeclaration() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    GlobalDeclaration(GlobalDeclaration const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    GlobalDeclaration& operator=(GlobalDeclaration const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    GlobalDeclaration(GlobalDeclaration&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    GlobalDeclaration& operator=(GlobalDeclaration&& other) noexcept;

public:
    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    GlobalDeclaration* clone() const & override = 0;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    GlobalDeclaration* clone() && override = 0;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */virtual GlobalDeclarationKind kind() const = 0;

};
}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_GLOBAL_DECLARATION_H_
