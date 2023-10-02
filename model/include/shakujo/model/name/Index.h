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
#ifndef SHAKUJO_MODEL_NAME_INDEX_H_
#define SHAKUJO_MODEL_NAME_INDEX_H_

#include <cstddef>
#include <utility>
#include <memory>
#include <optional>

#include "shakujo/model/Node.h"
#include "shakujo/model/name/SimpleName.h"

namespace shakujo::model::name {
/**
 * @brief Represents tuple index.
 */
class Index
        : public Node {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit Index(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    Index();

    /**
     * @brief Destroys this object.
     */
    ~Index() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    Index(Index const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    Index& operator=(Index const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    Index(Index&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    Index& operator=(Index&& other) noexcept;

public:
    /**
     * @brief Returns element name.
     * @return element name.
     */
    SimpleName* name();

    /**
     * @brief Returns element name.
     * @return element name.
     */
    inline SimpleName const* name() const {
        return const_cast<Index*>(this)->name();
    }

    /**
     * @brief Sets element name.
     * @param name element name
     * @return this
     */
    Index& name(std::unique_ptr<SimpleName> name);

    /**
     * @brief Releases element name from this node.
     * @return the released node
     */
    std::unique_ptr<SimpleName> release_name();

    /**
     * @brief Returns element position.
     * @return element position.
     */
    std::size_t position() const;

    /**
     * @brief Sets element position.
     * @param position element position
     * @return this
     */
    Index& position(std::size_t position);

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Index* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Index* clone() && override;

public:
    /**
     * @brief represents the invalid position.
     */
    static constexpr std::size_t INVALID_POSITION = static_cast<std::size_t>(-1);

    /**
     * @brief returns index position.
     * @return the optional index position
     */
    inline std::optional<std::size_t> position_optional() const {
        if (position() == INVALID_POSITION) {
            return {};
        }
        return std::make_optional<std::size_t>(position());
    }

    /**
     * @brief returns index position.
     * @param position the optional index position
     * @return this
     */
    inline Index& position_optional(std::optional<std::size_t> const& position) {
        this->position(position.value_or(INVALID_POSITION));
        return *this;
    }

    /**
     * @brief returns whether or not this index has either valid element name or position.
     * @return true if this has either valid element name or position
     * @return false otherwise
     */
    inline bool is_valid() const {
        return name() != nullptr || position() != INVALID_POSITION;
    }

};
}  // namespace shakujo::model::name

#endif  // SHAKUJO_MODEL_NAME_INDEX_H_
