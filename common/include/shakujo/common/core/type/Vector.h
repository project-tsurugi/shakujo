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
#ifndef SHAKUJO_COMMON_CORE_TYPE_VECTOR_H_
#define SHAKUJO_COMMON_CORE_TYPE_VECTOR_H_

#include <memory>

#include "../Type.h"
#include "shakujo/common/util/ClonablePtr.h"

namespace shakujo::common::core::type {

/**
 * @brief flexible length vector type binding.
 */
class Vector final : public Type {
private:
    common::util::ClonablePtr<Type> element_type_;
    Type::Nullity nullity_;

public:
    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     * @param nullity nullity of the type
     */
    explicit Vector(
            std::unique_ptr<Type> element_type,
            Type::Nullity nullity = Type::Nullity::NULLABLE)
        : element_type_(std::move(element_type))
        , nullity_(nullity)
    {}

    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     * @param nullity nullity of the type
     */
    explicit Vector(
            Type const* element_type,
            Type::Nullity nullity = Type::Nullity::NULLABLE)
        : Vector(util::make_clone(element_type), nullity)
    {}

    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     * @param nullity nullity of the type
     */
    explicit Vector(
            Type&& element_type,
            Type::Nullity nullity = Type::Nullity::NULLABLE)
        : Vector(util::make_clone(std::move(element_type)), nullity)
    {}

    /**
     * @brief destructs this object.
     */
    ~Vector() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Vector(Vector const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Vector(Vector&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Vector& operator=(Vector const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Vector& operator=(Vector&& other) noexcept = default;

    Type::Kind kind() const override {
        return Type::Kind::VECTOR;
    }

    bool is_valid() const override {
        return util::is_valid(element_type_);
    }

    Nullity nullity() const override {
        return nullity_;
    }

    Vector* clone() const & override {
        return new Vector(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Vector* clone() && override {
        return new Vector(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
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
};

}  // namespace shakujo::common::core::type

#endif  // SHAKUJO_COMMON_CORE_TYPE_VECTOR_H_
