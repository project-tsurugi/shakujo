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
#ifndef SHAKUJO_COMMON_CORE_TYPE_BOOL_H_
#define SHAKUJO_COMMON_CORE_TYPE_BOOL_H_

#include <utility>

#include "../Type.h"
#include "../value/Bool.h"

namespace shakujo::common::core::type {

/**
 * @brief boolean type binding.
 */
class Bool final : public Type {
private:
    Type::Nullity nullity_;

public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::BOOL;

    /**
     * @brief the corresponded value type.
     */
    using value_type = value::Bool;

    /**
     * @brief Constructs a new object.
     * @param nullity the nullity
     */
    constexpr explicit Bool(Type::Nullity nullity = Type::Nullity::NULLABLE) noexcept
        : nullity_(nullity)
    {}

    /**
     * @brief destructs this object.
     */
    ~Bool() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Bool(Bool const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Bool(Bool&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Bool& operator=(Bool const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Bool& operator=(Bool&& other) noexcept = default;

    Type::Kind kind() const override {
        return tag;
    }

    Nullity nullity() const override {
        return nullity_;
    }

    Bool* clone() const & override {
        return new Bool(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Bool* clone() && override {
        return new Bool(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_BOOL_H_
