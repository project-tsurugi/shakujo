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
#ifndef SHAKUJO_COMMON_CORE_TYPE_CURSOR_H_
#define SHAKUJO_COMMON_CORE_TYPE_CURSOR_H_

#include <memory>
#include <utility>

#include "../Type.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "Relation.h"

namespace shakujo::common::core::type {

/**
 * @brief relation cursor type binding.
 */
class Cursor final : public Type {
private:
    common::util::ClonablePtr<Relation> element_type_;

public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::CURSOR;

    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     */
    explicit Cursor(std::unique_ptr<Relation> element_type) : element_type_(std::move(element_type)) {}

    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     */
    explicit Cursor(Relation const* element_type) : Cursor(util::make_clone(element_type)) {}

    /**
     * @brief Constructs a new object.
     * @param element_type the element type
     */
    explicit Cursor(Relation&& element_type) : Cursor(util::make_clone(element_type)) {}

    /**
     * @brief destructs this object.
     */
    ~Cursor() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Cursor(Cursor const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Cursor(Cursor&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Cursor& operator=(Cursor const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Cursor& operator=(Cursor&& other) noexcept = default;

    Type::Kind kind() const override {
        return tag;
    }

    bool is_valid() const override {
        return util::is_valid(element_type_);
    }

    Nullity nullity() const override {
        return Type::Nullity::NEVER_NULL;
    }

    Cursor* clone() const & override {
        return new Cursor(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Cursor* clone() && override {
        return new Cursor(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the element type.
     * @return the element type
     */
    Relation* element_type() {
        return element_type_.get();
    }

    /**
     * @brief returns the element type.
     * @return the element type
     */
    Relation const* element_type() const {
        return element_type_.get();
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_CURSOR_H_
