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
#ifndef SHAKUJO_MODEL_TYPE_VECTOR_TYPE_H_
#define SHAKUJO_MODEL_TYPE_VECTOR_TYPE_H_

#include <utility>
#include <memory>

#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief Represents vector type.
 */
class VectorType
        : public Type {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VectorType(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    VectorType();

    /**
     * @brief Destroys this object.
     */
    ~VectorType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VectorType(VectorType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorType& operator=(VectorType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VectorType(VectorType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorType& operator=(VectorType&& other) noexcept;

public:
    /**
     * @brief Returns element type.
     * @return element type.
     */
    Type* element_type();

    /**
     * @brief Returns element type.
     * @return element type.
     */
    inline Type const* element_type() const {
        return const_cast<VectorType*>(this)->element_type();
    }

    /**
     * @brief Sets element type.
     * @param element_type element type
     * @return this
     */
    VectorType& element_type(std::unique_ptr<Type> element_type);

    /**
     * @brief Releases element type from this node.
     * @return the released node
     */
    std::unique_ptr<Type> release_element_type();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorType* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    TypeKind kind() const override {
        return TypeKind::VECTOR_TYPE;
    }

};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_VECTOR_TYPE_H_
