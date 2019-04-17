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
#ifndef SHAKUJO_MODEL_STATEMENT_VECTOR_ELEMENT_INSERT_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_VECTOR_ELEMENT_INSERT_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::statement {
/**
 * @brief Represents inserting elements into vectors.
 */
class VectorElementInsertStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VectorElementInsertStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    VectorElementInsertStatement();

    /**
     * @brief Destroys this object.
     */
    ~VectorElementInsertStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VectorElementInsertStatement(VectorElementInsertStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementInsertStatement& operator=(VectorElementInsertStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VectorElementInsertStatement(VectorElementInsertStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementInsertStatement& operator=(VectorElementInsertStatement&& other) noexcept;

public:
    /**
     * @brief Returns target vector.
     * @return target vector.
     */
    expression::Expression* data();

    /**
     * @brief Returns target vector.
     * @return target vector.
     */
    inline expression::Expression const* data() const {
        return const_cast<VectorElementInsertStatement*>(this)->data();
    }

    /**
     * @brief Sets target vector.
     * @param data target vector
     * @return this
     */
    VectorElementInsertStatement& data(std::unique_ptr<expression::Expression> data);

    /**
     * @brief Releases target vector from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_data();

    /**
     * @brief Returns insertion position.
     * @return insertion position.
     */
    expression::Expression* index();

    /**
     * @brief Returns insertion position.
     * @return insertion position.
     */
    inline expression::Expression const* index() const {
        return const_cast<VectorElementInsertStatement*>(this)->index();
    }

    /**
     * @brief Sets insertion position.
     * @param index insertion position
     * @return this
     */
    VectorElementInsertStatement& index(std::unique_ptr<expression::Expression> index);

    /**
     * @brief Releases insertion position from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_index();

    /**
     * @brief Returns inserting values.
     * @return inserting values.
     */
    util::ManagedNodeList<expression::Expression>& values();

    /**
     * @brief Returns inserting values.
     * @return inserting values.
     */
    inline util::ManagedNodeList<expression::Expression> const& values() const {
        return const_cast<VectorElementInsertStatement*>(this)->values();
    }
    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementInsertStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementInsertStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::VECTOR_ELEMENT_INSERT_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_VECTOR_ELEMENT_INSERT_STATEMENT_H_
