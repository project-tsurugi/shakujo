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
#ifndef SHAKUJO_MODEL_NAME_SIMPLE_NAME_H_
#define SHAKUJO_MODEL_NAME_SIMPLE_NAME_H_

#include <utility>
#include <memory>
#include <string>

#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/NameKind.h"

namespace shakujo::model::name {
/**
 * @brief Represents simple name.
 */
class SimpleName
        : public Name {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit SimpleName(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    SimpleName();

    /**
     * @brief Destroys this object.
     */
    ~SimpleName() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    SimpleName(SimpleName const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    SimpleName& operator=(SimpleName const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    SimpleName(SimpleName&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    SimpleName& operator=(SimpleName&& other) noexcept;

public:
    /**
     * @brief Returns name token.
     * @return name token.
     */
    std::string const& token() const;
    /**
     * @brief Sets name token.
     * @param token name token
     * @return this
     */
    SimpleName& token(std::string token);

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    SimpleName* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    SimpleName* clone() && override;

public:
    /**
     * @brief Returns the node kind.
     * @return the node kind
     */
    NameKind kind() const override {
        return NameKind::SIMPLE_NAME;
    }

};
}  // namespace shakujo::model::name

#endif  // SHAKUJO_MODEL_NAME_SIMPLE_NAME_H_
