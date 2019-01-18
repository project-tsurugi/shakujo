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
#ifndef SHAKUJO_COMMON_CORE_TYPE_ARRAY_H_
#define SHAKUJO_COMMON_CORE_TYPE_ARRAY_H_

#include <memory>
#include <utility>

#include "../Type.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::common::core::type {

/**
 * @brief fixed length array type binding.
 */
class Array final : public Type {
private:
    common::util::ClonablePtr<Type> element_type_;
    std::size_t size_;
    Type::Nullity nullity_;

public:
    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     * @param size the element count
     * @param nullity nullity of the type
     */
    Array(
            std::unique_ptr<Type> element_type,
            std::size_t size,
            Type::Nullity nullity = Type::Nullity::NULLABLE)
        : element_type_(std::move(element_type))
        , size_(size)
        , nullity_(nullity)
    {}

    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     * @param size the element count
     * @param nullity nullity of the type
     */
    Array(
            Type const* element_type,
            std::size_t size,
            Type::Nullity nullity = Type::Nullity::NULLABLE)
        : Array(util::make_clone(element_type), size, nullity)
    {}

    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     * @param size the element count
     * @param nullity nullity of the type
     */
    Array(
            Type&& element_type,
            std::size_t size,
            Type::Nullity nullity = Type::Nullity::NULLABLE)
        : Array(util::make_clone(element_type), size, nullity)
    {}

    /**
     * @brief destructs this object.
     */
    ~Array() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Array(Array const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Array(Array&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Array& operator=(Array const& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Array& operator=(Array&& other) noexcept = default;

    Type::Kind kind() const override {
        return Type::Kind::ARRAY;
    }

    bool is_valid() const override {
        return util::is_valid(element_type_);
    }

    Nullity nullity() const override {
        return nullity_;
    }

    Array* clone() const & override {
        return new Array(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Array* clone() && override {
        return new Array(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the element type.
     * @return the element type
     */
    Type* element_type() {
        return element_type_.get();
    }

    /**
     * @brief returns the element type.
     * @return the element type
     */
    Type const* element_type() const {
        return element_type_.get();
    }

    /**
     * @brief returns the element count of this type.
     * @return the number of elements
     */
    std::size_t size() const {
        return size_;
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_ARRAY_H_
