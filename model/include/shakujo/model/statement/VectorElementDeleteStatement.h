/*
 * Copyright 2018-2023 Project Tsurugi.
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
#ifndef SHAKUJO_MODEL_STATEMENT_VECTOR_ELEMENT_DELETE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_VECTOR_ELEMENT_DELETE_STATEMENT_H_

#include <utility>
#include <memory>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents removing elements from vectors.
 */
class VectorElementDeleteStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit VectorElementDeleteStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    VectorElementDeleteStatement();

    /**
     * @brief Destroys this object.
     */
    ~VectorElementDeleteStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    VectorElementDeleteStatement(VectorElementDeleteStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementDeleteStatement& operator=(VectorElementDeleteStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    VectorElementDeleteStatement(VectorElementDeleteStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    VectorElementDeleteStatement& operator=(VectorElementDeleteStatement&& other) noexcept;

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
        return const_cast<VectorElementDeleteStatement*>(this)->data();
    }

    /**
     * @brief Sets target vector.
     * @param data target vector
     * @return this
     */
    VectorElementDeleteStatement& data(std::unique_ptr<expression::Expression> data);

    /**
     * @brief Releases target vector from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_data();

    /**
     * @brief Returns begining index.
     * @return begining index.
     */
    expression::Expression* begin();

    /**
     * @brief Returns begining index.
     * @return begining index.
     */
    inline expression::Expression const* begin() const {
        return const_cast<VectorElementDeleteStatement*>(this)->begin();
    }

    /**
     * @brief Sets begining index.
     * @param begin begining index
     * @return this
     */
    VectorElementDeleteStatement& begin(std::unique_ptr<expression::Expression> begin);

    /**
     * @brief Releases begining index from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_begin();

    /**
     * @brief Returns ending index.
     * @return ending index.
     */
    expression::Expression* end();

    /**
     * @brief Returns ending index.
     * @return ending index.
     */
    inline expression::Expression const* end() const {
        return const_cast<VectorElementDeleteStatement*>(this)->end();
    }

    /**
     * @brief Sets ending index.
     * @param end ending index
     * @return this
     */
    VectorElementDeleteStatement& end(std::unique_ptr<expression::Expression> end);

    /**
     * @brief Releases ending index from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_end();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementDeleteStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    VectorElementDeleteStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::VECTOR_ELEMENT_DELETE_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_VECTOR_ELEMENT_DELETE_STATEMENT_H_
