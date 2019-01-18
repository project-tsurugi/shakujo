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
#ifndef SHAKUJO_COMMON_CORE_TYPE_TEXTUAL_H_
#define SHAKUJO_COMMON_CORE_TYPE_TEXTUAL_H_

#include "../Type.h"

namespace shakujo::common::core::type {

/**
 * @brief textual types.
 */
class Textual : public Type {
private:
    Type::Nullity nullity_;

protected:
    /**
     * @brief Constructs a new object.
     * @param nullity the nullity
     */
    constexpr explicit Textual(Type::Nullity nullity) noexcept : nullity_(nullity) {}

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Textual(Textual const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Textual(Textual&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Textual& operator=(Textual const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Textual& operator=(Textual&& other) noexcept = default;

public:
    ~Textual() noexcept override = default;

    Textual* clone() const & override = 0;

    Textual* clone() && override = 0;

    Nullity nullity() const final {
        return nullity_;
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_TEXTUAL_H_
