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
#ifndef SHAKUJO_MODEL_TYPE_TUPLE_TYPE_H_
#define SHAKUJO_MODEL_TYPE_TUPLE_TYPE_H_

#include <utility>
#include <memory>

#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/type/TypeKind.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::type {
/**
 * @brief Represents named tuple type.
 */
class TupleType
        : public Type {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit TupleType(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents tuple element.
     */
    class Element final {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Element(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Constructs a new empty object.
         */
        Element();

        /**
         * @brief Destroys this object.
         */
        ~Element() noexcept;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Element(Element const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Element& operator=(Element const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Element(Element&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Element& operator=(Element&& other) noexcept;

    public:
        /**
         * @brief Returns element name.
         * @return element name.
         */
        name::SimpleName* name();

        /**
         * @brief Returns element name.
         * @return element name.
         */
        inline name::SimpleName const* name() const {
            return const_cast<TupleType::Element*>(this)->name();
        }

        /**
         * @brief Sets element name.
         * @param name element name
         * @return this
         */
        TupleType::Element& name(std::unique_ptr<name::SimpleName> name);

        /**
         * @brief Releases element name from this node.
         * @return the released node
         */
        std::unique_ptr<name::SimpleName> release_name();

        /**
         * @brief Returns element type.
         * @return element type.
         */
        Type* type();

        /**
         * @brief Returns element type.
         * @return element type.
         */
        inline Type const* type() const {
            return const_cast<TupleType::Element*>(this)->type();
        }

        /**
         * @brief Sets element type.
         * @param type element type
         * @return this
         */
        TupleType::Element& type(std::unique_ptr<Type> type);

        /**
         * @brief Releases element type from this node.
         * @return the released node
         */
        std::unique_ptr<Type> release_type();

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Element* clone() const &;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Element* clone() &&;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    TupleType();

    /**
     * @brief Destroys this object.
     */
    ~TupleType() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    TupleType(TupleType const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    TupleType& operator=(TupleType const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    TupleType(TupleType&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    TupleType& operator=(TupleType&& other) noexcept;

public:
    /**
     * @brief Returns tuple elements.
     * @return tuple elements.
     */
    util::FragmentList<TupleType::Element>& elements();

    /**
     * @brief Returns tuple elements.
     * @return tuple elements.
     */
    inline util::FragmentList<TupleType::Element> const& elements() const {
        return const_cast<TupleType*>(this)->elements();
    }
    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TupleType* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    TupleType* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr TypeKind tag = TypeKind::TUPLE_TYPE;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */TypeKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::type

#endif  // SHAKUJO_MODEL_TYPE_TUPLE_TYPE_H_
