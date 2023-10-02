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
#ifndef SHAKUJO_MODEL_STATEMENT_ANCHOR_DECLARATION_H_
#define SHAKUJO_MODEL_STATEMENT_ANCHOR_DECLARATION_H_

#include <utility>
#include <memory>

#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents declaring control anchor.
 */
class AnchorDeclaration
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit AnchorDeclaration(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    AnchorDeclaration();

    /**
     * @brief Destroys this object.
     */
    ~AnchorDeclaration() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    AnchorDeclaration(AnchorDeclaration const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    AnchorDeclaration& operator=(AnchorDeclaration const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    AnchorDeclaration(AnchorDeclaration&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    AnchorDeclaration& operator=(AnchorDeclaration&& other) noexcept;

public:
    /**
     * @brief Returns anchor name.
     * @return anchor name.
     */
    name::Name* name();

    /**
     * @brief Returns anchor name.
     * @return anchor name.
     */
    inline name::Name const* name() const {
        return const_cast<AnchorDeclaration*>(this)->name();
    }

    /**
     * @brief Sets anchor name.
     * @param name anchor name
     * @return this
     */
    AnchorDeclaration& name(std::unique_ptr<name::Name> name);

    /**
     * @brief Releases anchor name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_name();

    /**
     * @brief Returns body statement.
     * @return body statement.
     */
    Statement* body();

    /**
     * @brief Returns body statement.
     * @return body statement.
     */
    inline Statement const* body() const {
        return const_cast<AnchorDeclaration*>(this)->body();
    }

    /**
     * @brief Sets body statement.
     * @param body body statement
     * @return this
     */
    AnchorDeclaration& body(std::unique_ptr<Statement> body);

    /**
     * @brief Releases body statement from this node.
     * @return the released node
     */
    std::unique_ptr<Statement> release_body();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    AnchorDeclaration* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    AnchorDeclaration* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::ANCHOR_DECLARATION;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_ANCHOR_DECLARATION_H_
