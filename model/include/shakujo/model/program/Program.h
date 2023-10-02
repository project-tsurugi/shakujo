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
#ifndef SHAKUJO_MODEL_PROGRAM_PROGRAM_H_
#define SHAKUJO_MODEL_PROGRAM_PROGRAM_H_

#include <utility>
#include <memory>

#include "shakujo/model/Node.h"
#include "shakujo/model/program/Comment.h"
#include "shakujo/model/program/GlobalDeclaration.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::program {
/**
 * @brief Represents compilation unit.
 */
class Program
        : public Node {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit Program(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    Program();

    /**
     * @brief Destroys this object.
     */
    ~Program() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    Program(Program const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    Program& operator=(Program const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    Program(Program&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    Program& operator=(Program&& other) noexcept;

public:
    /**
     * @brief Returns global declarations.
     * @return global declarations.
     */
    util::NodeList<GlobalDeclaration>& declarations();

    /**
     * @brief Returns global declarations.
     * @return global declarations.
     */
    inline util::NodeList<GlobalDeclaration> const& declarations() const {
        return const_cast<Program*>(this)->declarations();
    }
    /**
     * @brief Returns program entry.
     * @return program entry.
     */
    statement::Statement* main();

    /**
     * @brief Returns program entry.
     * @return program entry.
     */
    inline statement::Statement const* main() const {
        return const_cast<Program*>(this)->main();
    }

    /**
     * @brief Sets program entry.
     * @param main program entry
     * @return this
     */
    Program& main(std::unique_ptr<statement::Statement> main);

    /**
     * @brief Releases program entry from this node.
     * @return the released node
     */
    std::unique_ptr<statement::Statement> release_main();

    /**
     * @brief Returns comments.
     * @return comments.
     */
    util::NodeList<Comment>& comments();

    /**
     * @brief Returns comments.
     * @return comments.
     */
    inline util::NodeList<Comment> const& comments() const {
        return const_cast<Program*>(this)->comments();
    }
    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Program* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Program* clone() && override;

};
}  // namespace shakujo::model::program

#endif  // SHAKUJO_MODEL_PROGRAM_PROGRAM_H_
