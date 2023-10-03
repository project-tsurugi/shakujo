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
#ifndef SHAKUJO_MODEL_STATEMENT_DDL_CREATE_INDEX_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DDL_CREATE_INDEX_STATEMENT_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <set>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::statement::ddl {
/**
 * @brief Represents create index statement.
 */
class CreateIndexStatement
        : public Statement
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit CreateIndexStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents create index attribute.
     */
    enum class Attribute {
        /**
         * @brief create index only if it does not exist.
         */
        IF_NOT_EXISTS,

    };

    /**
     * @brief Represents create index columns.
     */
    class Column final {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Column(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Represents primary key direction.
         */
        enum class Direction {
            /**
             * @brief don't care.
             */
            DONT_CARE,

            /**
             * @brief ascendant order.
             */
            ASCENDANT,

            /**
             * @brief descendant order.
             */
            DESCENDANT,

        };

        /**
         * @brief Constructs a new empty object.
         */
        Column();

        /**
         * @brief Destroys this object.
         */
        ~Column() noexcept;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Column(Column const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Column(Column&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column&& other) noexcept;

        /**
         * @brief Returns column name.
         * @return column name.
         */
        name::SimpleName* name();

        /**
         * @brief Returns column name.
         * @return column name.
         */
        [[nodiscard]] inline name::SimpleName const* name() const {
            return const_cast<CreateIndexStatement::Column*>(this)->name();
        }

        /**
         * @brief Sets column name.
         * @param name column name
         * @return this
         */
        CreateIndexStatement::Column& name(std::unique_ptr<name::SimpleName> name);

        /**
         * @brief Releases column name from this node.
         * @return the released node
         */
        std::unique_ptr<name::SimpleName> release_name();

        /**
         * @brief Returns column direction.
         * @return column direction.
         */
        [[nodiscard]] CreateIndexStatement::Column::Direction direction() const;

        /**
         * @brief Sets column direction.
         * @param direction column direction
         * @return this
         */
        CreateIndexStatement::Column& direction(CreateIndexStatement::Column::Direction direction);

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() const &;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        Column* clone() &&;

    };

    /**
     * @brief Constructs a new empty object.
     */
    CreateIndexStatement();

    /**
     * @brief Destroys this object.
     */
    ~CreateIndexStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    CreateIndexStatement(CreateIndexStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    CreateIndexStatement& operator=(CreateIndexStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    CreateIndexStatement(CreateIndexStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    CreateIndexStatement& operator=(CreateIndexStatement&& other) noexcept;

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
        return const_cast<CreateIndexStatement*>(this)->index();
    }

    /**
     * @brief Sets index name.
     * @param index index name
     * @return this
     */
    CreateIndexStatement& index(std::unique_ptr<name::Name> index);

    /**
     * @brief Releases index name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_index();

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
        return const_cast<CreateIndexStatement*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    CreateIndexStatement& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns columns specification.
     * @return columns specification.
     */
    util::FragmentList<CreateIndexStatement::Column>& columns();

    /**
     * @brief Returns columns specification.
     * @return columns specification.
     */
    inline util::FragmentList<CreateIndexStatement::Column> const& columns() const {
        return const_cast<CreateIndexStatement*>(this)->columns();
    }
    /**
     * @brief Returns create table attributes.
     * @return create table attributes.
     */
    std::set<CreateIndexStatement::Attribute>& attributes();

    /**
     * @brief Returns create table attributes.
     * @return create table attributes.
     */
    inline std::set<CreateIndexStatement::Attribute> const& attributes() const {
        return const_cast<CreateIndexStatement*>(this)->attributes();
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
        return const_cast<CreateIndexStatement*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    CreateIndexStatement& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CreateIndexStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CreateIndexStatement* clone() && override;

    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::CREATE_INDEX_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */
    [[nodiscard]] StatementKind kind() const override {
        return tag;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see CreateIndexStatement::Attribute
 */
inline constexpr std::string_view to_string_view(CreateIndexStatement::Attribute value) {
    switch (value) {
        case CreateIndexStatement::Attribute::IF_NOT_EXISTS: return "IF_NOT_EXISTS";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see CreateIndexStatement::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, CreateIndexStatement::Attribute value) {
    return out << to_string_view(value);
}

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see CreateIndexStatement::PrimaryKey::Direction
 */
inline constexpr std::string_view to_string_view(CreateIndexStatement::Column::Direction value) {
    switch (value) {
        case CreateIndexStatement::Column::Direction::DONT_CARE: return "DONT_CARE";
        case CreateIndexStatement::Column::Direction::ASCENDANT: return "ASCENDANT";
        case CreateIndexStatement::Column::Direction::DESCENDANT: return "DESCENDANT";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see CreateIndexStatement::PrimaryKey::Direction
 */
inline std::ostream& operator<<(std::ostream& out, CreateIndexStatement::Column::Direction value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::statement::ddl

#endif  // SHAKUJO_MODEL_STATEMENT_DDL_CREATE_INDEX_STATEMENT_H_
