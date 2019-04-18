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
#ifndef SHAKUJO_MODEL_PROGRAM_COMMENT_H_
#define SHAKUJO_MODEL_PROGRAM_COMMENT_H_

#include <utility>
#include <memory>
#include <string>

#include "shakujo/model/Node.h"

namespace shakujo::model::program {
/**
 * @brief Represents comment in program.
 */
class Comment
        : public Node {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit Comment(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    Comment();

    /**
     * @brief Destroys this object.
     */
    ~Comment() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    Comment(Comment const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    Comment& operator=(Comment const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    Comment(Comment&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    Comment& operator=(Comment&& other) noexcept;

public:
    /**
     * @brief Returns comment text.
     * @return comment text.
     */
    std::string const& contents() const;
    /**
     * @brief Sets comment text.
     * @param contents comment text
     * @return this
     */
    Comment& contents(std::string contents);

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Comment* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Comment* clone() && override;

};
}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_COMMENT_H_
