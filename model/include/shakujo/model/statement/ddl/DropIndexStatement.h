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
#ifndef SHAKUJO_MODEL_STATEMENT_DDL_DROP_INDEX_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DDL_DROP_INDEX_STATEMENT_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <set>
#include <iostream>

#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement::ddl {
/**
 * @brief Represents drop index statement.
 */
class DropIndexStatement
        : public Statement
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit DropIndexStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents drop index attribute.
     */
    enum class Attribute {
        /**
         * @brief drop index only if it exists.
         */
        IF_EXISTS,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    DropIndexStatement();

    /**
     * @brief Destroys this object.
     */
    ~DropIndexStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    DropIndexStatement(DropIndexStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    DropIndexStatement& operator=(DropIndexStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    DropIndexStatement(DropIndexStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    DropIndexStatement& operator=(DropIndexStatement&& other) noexcept;

public:
    /**
     * @brief Returns index name.
     * @return index name.
     */
    name::Name* index();

    /**
     * @brief Returns index name.
     * @return index name.
     */
    inline name::Name const* index() const {
        return const_cast<DropIndexStatement*>(this)->index();
    }

    /**
     * @brief Sets index name.
     * @param index index name
     * @return this
     */
    DropIndexStatement& index(std::unique_ptr<name::Name> index);

    /**
     * @brief Releases index name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_index();

    /**
     * @brief Returns drop index attributes.
     * @return drop index attributes.
     */
    std::set<DropIndexStatement::Attribute>& attributes();

    /**
     * @brief Returns drop index attributes.
     * @return drop index attributes.
     */
    inline std::set<DropIndexStatement::Attribute> const& attributes() const {
        return const_cast<DropIndexStatement*>(this)->attributes();
    }
    /**
     * @brief Returns relation key.
     * @return relation key.
     */
    key::RelationKey* relation_key() override;

    /**
     * @brief Returns relation key.
     * @return relation key.
     */
    inline key::RelationKey const* relation_key() const override {
        return const_cast<DropIndexStatement*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    DropIndexStatement& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DropIndexStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DropIndexStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::DROP_INDEX_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see DropIndexStatement::Attribute
 */
inline constexpr std::string_view to_string_view(DropIndexStatement::Attribute value) {
    switch (value) {
        case DropIndexStatement::Attribute::IF_EXISTS: return "IF_EXISTS";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see DropIndexStatement::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, DropIndexStatement::Attribute value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::statement::ddl

#endif  // SHAKUJO_MODEL_STATEMENT_DDL_DROP_INDEX_STATEMENT_H_
