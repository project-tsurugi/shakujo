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
#ifndef SHAKUJO_MODEL_KEY_EXPRESSION_KEY_H_
#define SHAKUJO_MODEL_KEY_EXPRESSION_KEY_H_

#include <memory>

#include "shakujo/common/util/Clonable.h"

namespace shakujo::model::key {

/**
 * @brief Represents expression key.
 */
class ExpressionKey final {
public:
    /**
     * @brief an interface of ExpressionKey entity.
     * The default implementation is always evaluated as false in bool context.
     */
    class Entity {
    protected:
        /**
         * @brief constructs a new object.
         */
        Entity() noexcept = default;

    public:
        /**
         * @brief Destroys this object.
         */
        virtual ~Entity() noexcept = default;

        /**
         * @brief constructs a new object.
         * @param other the copy source
         */
        Entity(Entity const& other) = delete;

        /**
         * @brief constructs a new object.
         * @param other the move source
         */
        Entity(Entity&& other) noexcept = delete;

        /**
         * @brief assigns the given object into this.
         * @param other the copy source
         * @return this
         */
        Entity& operator=(Entity const& other) = delete;

        /**
         * @brief assigns the given object into this.
         * @param other the move source
         * @return this
         */
        Entity& operator=(Entity&& other) noexcept = delete;
    };

    /**
     * @brief Represents mix-in of key provider.
     */
    class Provider : public common::util::Clonable {
    protected:
        /**
         * @brief constructs a new object.
         */
        Provider() noexcept = default;

        /**
         * @brief constructs a new object.
         * @param other the copy source
         */
        Provider(Provider const& other) = default;

        /**
         * @brief constructs a new object.
         * @param other the move source
         */
        Provider(Provider&& other) noexcept = default;

        /**
         * @brief assigns the given object into this.
         * @param other the copy source
         * @return this
         */
        Provider& operator=(Provider const& other) = default;

        /**
         * @brief assigns the given object into this.
         * @param other the move source
         * @return this
         */
        Provider& operator=(Provider&& other) noexcept = default;

    public:
        /**
         * @brief destructs this object.
         */
        ~Provider() noexcept override = default;

        /**
         * @brief returns a clone of this object.
         * @return a clone
         */
        Provider* clone() const & override = 0;

        /**
         * @brief returns a clone of this object.
         * @return a clone
         */
        Provider* clone() && override = 0;

        /**
         * @brief Returns the expression key.
         * @return the expression key.
         */
        virtual ExpressionKey* expression_key() = 0;

        /**
         * @brief Returns expression key.
         * @return expression key.
         */
        virtual ExpressionKey const* expression_key() const = 0;

        /**
         * @brief Sets expression key.
         * @param expression_key expression key
         * @return this
         */
        virtual Provider& expression_key(std::unique_ptr<ExpressionKey> expression_key) = 0;
    };

private:
    std::unique_ptr<Entity> entity_;

public:
    /**
     * @brief Constructs a new empty object.
     */
    ExpressionKey() : entity_(nullptr) {}

    /**
     * @brief Constructs a new object with the given entity.
     * @param entity the key entity
     */
    explicit ExpressionKey(std::unique_ptr<Entity> entity) : entity_(std::move(entity)) {}

    /**
     * @brief Destroys this object.
     */
    ~ExpressionKey() noexcept = default;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    ExpressionKey(ExpressionKey const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ExpressionKey& operator=(ExpressionKey const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    ExpressionKey(ExpressionKey&& other) noexcept = default;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ExpressionKey& operator=(ExpressionKey&& other) noexcept = default;

    /**
     * @brief returns whether or not this key has the given type of entity.
     * @tparam T the entity type
     * @return true if this has the given type of entity
     * @return false otherwise
     */
    template<typename T>
    bool has_entity() const {
        if (!entity_) {
            return false;
        }
        return dynamic_cast<T*>(entity_.get()) != nullptr;
    }

    /**
     * @brief returns the entity of this key.
     * @return the entity
     * @throws std::bad_cast
     * @see has_entity()
     */
    template<typename T>
    T* entity() {
        return dynamic_cast<T*>(entity_.get());
    }

    /**
     * @brief returns the entity of this key.
     * @return the entity
     * @see has_entity()
     */
    template<typename T>
    inline T const* entity() const {
        return const_cast<ExpressionKey*>(this)->entity<T>();
    }
};
}  // namespace shakujo::model::key

#endif  // SHAKUJO_MODEL_KEY_EXPRESSION_KEY_H_
