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
#ifndef SHAKUJO_MODEL_TYPE_BINARY_TYPE_H_
#define SHAKUJO_MODEL_TYPE_BINARY_TYPE_H_

#include <cstddef>
#include <utility>
#include <memory>

#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief Represents fixed length octet sequence type.
 */
class BinaryType
        : public Type {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit BinaryType(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    BinaryType();

    /**
     * @brief Destroys this object.
     */
    ~BinaryType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    BinaryType(BinaryType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    BinaryType& operator=(BinaryType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    BinaryType(BinaryType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    BinaryType& operator=(BinaryType&& other) noexcept;

public:
    /**
     * @brief Returns bytes length.
     * @return bytes length.
     */
    std::size_t size() const;

    /**
     * @brief Sets bytes length.
     * @param size bytes length
     * @return this
     */
    BinaryType& size(std::size_t size);

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BinaryType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    BinaryType* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = TypeKind::BINARY_TYPE;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */TypeKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_BINARY_TYPE_H_
