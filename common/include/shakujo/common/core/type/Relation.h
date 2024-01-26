/*
 * Copyright 2018-2024 Project Tsurugi.
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
#ifndef SHAKUJO_COMMON_CORE_TYPE_RELATION_H_
#define SHAKUJO_COMMON_CORE_TYPE_RELATION_H_

#include <cstddef>
#include <memory>
#include <string>
#include <utility>

#include "../Type.h"
#include "../Name.h"
#include "Tuple.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::common::core::type {

/**
 * @brief relation type binding.
 */
class Relation final : public Type {
public:
    /**
     * @brief represents a element of Tuple.
     */
    class Column final {
    private:
        std::vector<Name> qualifiers_;
        std::string name_;
        common::util::ClonablePtr<Type> type_;

    public:

        /**
         * @brief Construct a new object.
         * @param qualifiers the column qualifier candidates
         * @param name the element name
         * @param type the element type
         */
        Column(std::vector<Name> qualifiers, std::string_view name, std::unique_ptr<Type> type)
            : qualifiers_(std::move(qualifiers)), name_(name), type_(std::move(type))
        {}

        /**
         * @brief Construct a new object.
         * @param qualifiers the column qualifier candidates
         * @param name the element name
         * @param type the element type
         */
        Column(std::vector<Name> qualifiers, std::string_view name, Type const* type)
            : Column(std::move(qualifiers), name, util::make_clone(type))
        {}

        /**
         * @brief Construct a new object.
         * @param qualifiers the column qualifier candidates
         * @param name the element name
         * @param type the element type
         */
        Column(std::vector<Name> qualifiers, std::string_view name, Type&& type)
            : Column(std::move(qualifiers), name, util::make_clone(type))
        {}

        /**
         * @brief Construct a new object.
         * @param name the element name
         * @param type the element type
         */
        Column(std::string_view name, std::unique_ptr<Type> type) : name_(name), type_(std::move(type)) {}

        /**
         * @brief Construct a new object.
         * @param name the element name
         * @param type the element type
         */
        Column(std::string_view name, Type const* type) : Column(name, util::make_clone(type)) {}

        /**
         * @brief Construct a new object.
         * @param name the element name
         * @param type the element type
         */
        Column(std::string_view name, Type&& type) : Column(name, util::make_clone(std::move(type))) {}

        /**
         * @brief Construct a new object.
         * @param type the element type
         */
        Column(std::unique_ptr<Type> type)  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            : type_(std::move(type))
        {}

        /**
         * @brief Construct a new object.
         * @param type the element type
         */
        Column(Type const* type)  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            : Column(util::make_clone(type))
        {}

        /**
         * @brief Construct a new object.
         * @param type the element type
         */
        Column(Type&& type)  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            : Column(util::make_clone(type))
        {}

        /**
         * @brief destructs this object.
         */
        ~Column() noexcept = default;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Column(Column const& other) = default;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Column(Column&& other) noexcept = default;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column const& other) = default;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Column& operator=(Column&& other) noexcept = default;

        /**
         * @brief returns the name qualifiers.
         * @return the name qualifiers
         */
        std::vector<Name> const& qualifiers() const {
            return qualifiers_;
        }

        /**
         * @brief returns the name of this element.
         * @return the element name, or an empty string if it is not defined
         */
        std::string const& name() const {
            return name_;
        }

        /**
         * @brief returns the element type.
         * @return the type
         */
        Type const* type() const {
            return type_.get();
        }
    };

private:
    std::vector<Column> columns_;

public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::RELATION;

    /**
     * @brief Constructs a new object.
     * @param elements the tuple elements
     */
    explicit Relation(std::vector<Column> elements) : columns_(std::move(elements)) {}

    /**
     * @brief Constructs a new object.
     * @param elements the tuple elements
     */
    Relation(std::initializer_list<Column> elements) : columns_(elements) {}

    /**
     * @brief constructs a new object from a tuple.
     * @param tuple the source tuple
     * @param qualifiers the column qualifier candidates
     */
    explicit Relation(Tuple const* tuple, std::vector<Name> const & qualifiers = {}) {
        columns_.reserve(tuple->elements().size());
        for (auto& e : tuple->elements()) {
            columns_.emplace_back(qualifiers, e.name(), util::make_clone(e.type()));
        }
    }

    /**
     * @brief destructs this object.
     */
    ~Relation() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Relation(Relation const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Relation(Relation&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Relation& operator=(Relation const& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Relation& operator=(Relation&& other) noexcept = default;

    Type::Kind kind() const override {
        return tag;
    }

    bool is_valid() const override {
        for (auto& column : columns_) {
            if (!util::is_valid(column.type())) {
                return false;
            }
        }
        return true;
    }

    Nullity nullity() const override {
        return Type::Nullity::NEVER_NULL;
    }

    Relation* clone() const & override {
        return new Relation(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Relation* clone() && override {
        return new Relation(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the element type.
     * @return the element type
     */
    std::vector<Column> const& columns() const {
        return columns_;
    }

    /**
     * @brief returns a column.
     * @param position the column position (0-origin)
     * @return the column
     */
    inline Column& at(std::size_t position) {
        return columns_[position];
    }

    /**
     * @brief returns a column.
     * @param position the column position (0-origin)
     * @return the column
     */
    inline Column const& at(std::size_t position) const {
        return const_cast<Relation*>(this)->at(position);
    }

    /**
     * @brief returns a column.
     * @param position the column position (0-origin)
     * @return the column
     */
    inline Column& operator[](std::size_t position) {
        return at(position);
    }

    /**
     * @brief returns a column.
     * @param position the column position (0-origin)
     * @return the column
     */
    inline Column const& operator[](std::size_t position) const {
        return at(position);
    }

    /**
     * @brief returns the row type of this relation.
     * @return the row type
     */
    std::unique_ptr<Tuple> row_type() const {
        std::vector<Tuple::Element> elements;
        elements.reserve(columns_.size());
        for (auto& column : columns_) {
            elements.emplace_back(column.name(), util::make_clone(column.type()));
        }
        return std::make_unique<Tuple>(std::move(elements));
    }
};

}  // namespace shakujo::common::core::type


#endif  // SHAKUJO_COMMON_CORE_TYPE_RELATION_H_
