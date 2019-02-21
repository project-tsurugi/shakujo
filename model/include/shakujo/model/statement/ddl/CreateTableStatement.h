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
#ifndef SHAKUJO_MODEL_STATEMENT_DDL_CREATE_TABLE_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_DDL_CREATE_TABLE_STATEMENT_H_

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
 * @brief Represents create table statement.
 */
class CreateTableStatement
        : public Statement
        , public key::RelationKey::Provider {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit CreateTableStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents create table attribute.
     */
    enum class Attribute {
        /**
         * @brief create table only if it does not exist.
         */
        IF_NOT_EXISTS,

    };

    /**
     * @brief Represents create table columns.
     */
    class Column final {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit Column(std::unique_ptr<Impl>) noexcept;

    public:
        /**
         * @brief Represents column attribute.
         */
        enum class Attribute {
            /**
             * @brief column value must be never null.
             */
            NOT_NULL,

            /**
             * @brief column is the primary key.
             */
            PRIMARY_KEY,

        };

    public:
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

    public:
        /**
         * @brief Returns column name.
         * @return column name.
         */
        name::SimpleName* name();

        /**
         * @brief Returns column name.
         * @return column name.
         */
        inline name::SimpleName const* name() const {
            return const_cast<CreateTableStatement::Column*>(this)->name();
        }

        /**
         * @brief Sets column name.
         * @param name column name
         * @return this
         */
        CreateTableStatement::Column& name(std::unique_ptr<name::SimpleName> name);

        /**
         * @brief Releases column name from this node.
         * @return the released node
         */
        std::unique_ptr<name::SimpleName> release_name();

        /**
         * @brief Returns column type.
         * @return column type.
         */
        type::Type* type();

        /**
         * @brief Returns column type.
         * @return column type.
         */
        inline type::Type const* type() const {
            return const_cast<CreateTableStatement::Column*>(this)->type();
        }

        /**
         * @brief Sets column type.
         * @param type column type
         * @return this
         */
        CreateTableStatement::Column& type(std::unique_ptr<type::Type> type);

        /**
         * @brief Releases column type from this node.
         * @return the released node
         */
        std::unique_ptr<type::Type> release_type();

        /**
         * @brief Returns column default value.
         * @return column default value.
         */
        expression::Expression* value();

        /**
         * @brief Returns column default value.
         * @return column default value.
         */
        inline expression::Expression const* value() const {
            return const_cast<CreateTableStatement::Column*>(this)->value();
        }

        /**
         * @brief Sets column default value.
         * @param value column default value
         * @return this
         */
        CreateTableStatement::Column& value(std::unique_ptr<expression::Expression> value);

        /**
         * @brief Releases column default value from this node.
         * @return the released node
         */
        std::unique_ptr<expression::Expression> release_value();

        /**
         * @brief Returns column attributes.
         * @return column attributes.
         */
        std::set<CreateTableStatement::Column::Attribute>& attributes();

        /**
         * @brief Returns column attributes.
         * @return column attributes.
         */
        inline std::set<CreateTableStatement::Column::Attribute> const& attributes() const {
            return const_cast<CreateTableStatement::Column*>(this)->attributes();
        }
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
     * @brief Represents primary key setting.
     */
    class PrimaryKey final {
    private:
        class Impl;
        std::unique_ptr<Impl> impl_;
        explicit PrimaryKey(std::unique_ptr<Impl>) noexcept;

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

    public:
        /**
         * @brief Constructs a new empty object.
         */
        PrimaryKey();

        /**
         * @brief Destroys this object.
         */
        ~PrimaryKey() noexcept;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        PrimaryKey(PrimaryKey const& other) = delete;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        PrimaryKey& operator=(PrimaryKey const& other) = delete;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        PrimaryKey(PrimaryKey&& other) noexcept;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        PrimaryKey& operator=(PrimaryKey&& other) noexcept;

    public:
        /**
         * @brief Returns key column name.
         * @return key column name.
         */
        name::SimpleName* name();

        /**
         * @brief Returns key column name.
         * @return key column name.
         */
        inline name::SimpleName const* name() const {
            return const_cast<CreateTableStatement::PrimaryKey*>(this)->name();
        }

        /**
         * @brief Sets key column name.
         * @param name key column name
         * @return this
         */
        CreateTableStatement::PrimaryKey& name(std::unique_ptr<name::SimpleName> name);

        /**
         * @brief Releases key column name from this node.
         * @return the released node
         */
        std::unique_ptr<name::SimpleName> release_name();

        /**
         * @brief Returns column direction.
         * @return column direction.
         */
        CreateTableStatement::PrimaryKey::Direction direction() const;

        /**
         * @brief Sets column direction.
         * @param direction column direction
         * @return this
         */
        CreateTableStatement::PrimaryKey& direction(CreateTableStatement::PrimaryKey::Direction direction);

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        PrimaryKey* clone() const &;

        /**
         * @brief Returns a copy of this object.
         * @return a clone of this
         */
        PrimaryKey* clone() &&;

    };
public:
    /**
     * @brief Constructs a new empty object.
     */
    CreateTableStatement();

    /**
     * @brief Destroys this object.
     */
    ~CreateTableStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    CreateTableStatement(CreateTableStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    CreateTableStatement& operator=(CreateTableStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    CreateTableStatement(CreateTableStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    CreateTableStatement& operator=(CreateTableStatement&& other) noexcept;

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
        return const_cast<CreateTableStatement*>(this)->table();
    }

    /**
     * @brief Sets table name.
     * @param table table name
     * @return this
     */
    CreateTableStatement& table(std::unique_ptr<name::Name> table);

    /**
     * @brief Releases table name from this node.
     * @return the released node
     */
    std::unique_ptr<name::Name> release_table();

    /**
     * @brief Returns columns specification.
     * @return columns specification.
     */
    util::FragmentList<CreateTableStatement::Column>& columns();

    /**
     * @brief Returns columns specification.
     * @return columns specification.
     */
    inline util::FragmentList<CreateTableStatement::Column> const& columns() const {
        return const_cast<CreateTableStatement*>(this)->columns();
    }
    /**
     * @brief Returns create table attributes.
     * @return create table attributes.
     */
    std::set<CreateTableStatement::Attribute>& attributes();

    /**
     * @brief Returns create table attributes.
     * @return create table attributes.
     */
    inline std::set<CreateTableStatement::Attribute> const& attributes() const {
        return const_cast<CreateTableStatement*>(this)->attributes();
    }
    /**
     * @brief Returns primary key settings.
     * @return primary key settings.
     */
    util::FragmentList<CreateTableStatement::PrimaryKey>& primary_keys();

    /**
     * @brief Returns primary key settings.
     * @return primary key settings.
     */
    inline util::FragmentList<CreateTableStatement::PrimaryKey> const& primary_keys() const {
        return const_cast<CreateTableStatement*>(this)->primary_keys();
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
        return const_cast<CreateTableStatement*>(this)->relation_key();
    }

    /**
     * @brief Sets relation key.
     * @param relation_key relation key
     * @return this
     */
    CreateTableStatement& relation_key(std::unique_ptr<key::RelationKey> relation_key) override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CreateTableStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    CreateTableStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr StatementKind tag = StatementKind::CREATE_TABLE_STATEMENT;

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
 * @see CreateTableStatement::Attribute
 */
inline constexpr std::string_view to_string_view(CreateTableStatement::Attribute value) {
    switch (value) {
        case CreateTableStatement::Attribute::IF_NOT_EXISTS: return "IF_NOT_EXISTS";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see CreateTableStatement::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, CreateTableStatement::Attribute value) {
    return out << to_string_view(value);
}

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see CreateTableStatement::Column::Attribute
 */
inline constexpr std::string_view to_string_view(CreateTableStatement::Column::Attribute value) {
    switch (value) {
        case CreateTableStatement::Column::Attribute::NOT_NULL: return "NOT_NULL";
        case CreateTableStatement::Column::Attribute::PRIMARY_KEY: return "PRIMARY_KEY";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see CreateTableStatement::Column::Attribute
 */
inline std::ostream& operator<<(std::ostream& out, CreateTableStatement::Column::Attribute value) {
    return out << to_string_view(value);
}

/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see CreateTableStatement::PrimaryKey::Direction
 */
inline constexpr std::string_view to_string_view(CreateTableStatement::PrimaryKey::Direction value) {
    switch (value) {
        case CreateTableStatement::PrimaryKey::Direction::DONT_CARE: return "DONT_CARE";
        case CreateTableStatement::PrimaryKey::Direction::ASCENDANT: return "ASCENDANT";
        case CreateTableStatement::PrimaryKey::Direction::DESCENDANT: return "DESCENDANT";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see CreateTableStatement::PrimaryKey::Direction
 */
inline std::ostream& operator<<(std::ostream& out, CreateTableStatement::PrimaryKey::Direction value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::statement::ddl

#endif  // SHAKUJO_MODEL_STATEMENT_DDL_CREATE_TABLE_STATEMENT_H_
