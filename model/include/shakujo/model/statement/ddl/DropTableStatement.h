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
#ifndef SHAKUJO_MODEL_STATEMENT_DDL_DROP_TABLE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DDL_DROP_TABLE_STATEMENT_H_

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
 * @brief Represents drop table statement.
 */
class DropTableStatement
        : public Statement
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit DropTableStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents drop table attribute.
     */
    enum class Attribute {
        /**
         * @brief drop table only if it exists.
         */
        IF_EXISTS,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    DropTableStatement();

    /**
     * @brief Destroys this object.
     */
    ~DropTableStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    DropTableStatement(DropTableStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    DropTableStatement& operator=(DropTableStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    DropTableStatement(DropTableStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    DropTableStatement& operator=(DropTableStatement&& other) noexcept;

public:
    /**
     * @brief Returns table name.
     * @return table name.
     */
    name::Name* table();

    /**
     * @brief Returns table name.
     * @return table name.
     */
    inline name::Name const* table() const {
        return const_cast<DropTableStatement*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    DropTableStatement& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns drop table attributes.
     * @return drop table attributes.
     */
    std::set<DropTableStatement::Attribute>& attributes();

    /**
     * @brief Returns drop table attributes.
     * @return drop table attributes.
     */
    inline std::set<DropTableStatement::Attribute> const& attributes() const {
        return const_cast<DropTableStatement*>(this)->attributes();
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
        return const_cast<DropTableStatement*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    DropTableStatement& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DropTableStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    DropTableStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::DROP_TABLE_STATEMENT;

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
 * @see DropTableStatement::Attribute
 */
inline constexpr std::string_view to_string_view(DropTableStatement::Attribute value) {
    switch (value) {
        case DropTableStatement::Attribute::IF_EXISTS: return "IF_EXISTS";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see DropTableStatement::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, DropTableStatement::Attribute value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::statement::ddl

#endif  // SHAKUJO_MODEL_STATEMENT_DDL_DROP_TABLE_STATEMENT_H_
