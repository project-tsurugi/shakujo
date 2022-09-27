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
#ifndef SHAKUJO_MODEL_TYPE_VAR_CHAR_TYPE_H_
#define SHAKUJO_MODEL_TYPE_VAR_CHAR_TYPE_H_

#include <cstddef>
#include <utility>
#include <memory>

#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"

namespace shakujo::model::type {
/**
 * @brief Represents variant length character sequence type.
 */
class VarCharType
        : public Type {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VarCharType(std::unique_ptr<Impl>) noexcept;

public:
    /// @brief represents don't care values.
    static constexpr std::size_t dont_care = static_cast<std::size_t>(-1);

    /**
     * @brief Constructs a new empty object.
     */
    VarCharType();

    /**
     * @brief Destroys this object.
     */
    ~VarCharType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VarCharType(VarCharType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VarCharType& operator=(VarCharType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VarCharType(VarCharType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VarCharType& operator=(VarCharType&& other) noexcept;

public:
    /**
     * @brief Returns bytes length.
     * @return bytes length.
     * @return dont_care if bytes are flexible
     */
    std::size_t size() const;

    /**
     * @brief Sets bytes length.
     * @param size bytes length
     * @return this
     */
    VarCharType& size(std::size_t size);

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VarCharType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VarCharType* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = TypeKind::VAR_CHAR_TYPE;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */TypeKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_VAR_CHAR_TYPE_H_
