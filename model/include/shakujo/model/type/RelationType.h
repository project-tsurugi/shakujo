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
#ifndef SHAKUJO_MODEL_TYPE_RELATION_TYPE_H_
#define SHAKUJO_MODEL_TYPE_RELATION_TYPE_H_

#include <utility>
#include <memory>

#include "shakujo/model/type/TupleType.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief Represents relation type.
 */
class RelationType
        : public Type {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit RelationType(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    RelationType();

    /**
     * @brief Destroys this object.
     */
    ~RelationType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    RelationType(RelationType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    RelationType& operator=(RelationType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    RelationType(RelationType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    RelationType& operator=(RelationType&& other) noexcept;

public:
    /**
     * @brief Returns element type.
     * @return element type.
     */
    TupleType* element_type();

    /**
     * @brief Returns element type.
     * @return element type.
     */
    inline TupleType const* element_type() const {
        return const_cast<RelationType*>(this)->element_type();
    }

    /**
     * @brief Sets element type.
     * @param element_type element type
     * @return this
     */
    RelationType& element_type(std::unique_ptr<TupleType> element_type);

    /**
     * @brief Releases element type from this node.
     * @return the released node
     */
    std::unique_ptr<TupleType> release_element_type();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    RelationType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    RelationType* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = TypeKind::RELATION_TYPE;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */TypeKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_RELATION_TYPE_H_
