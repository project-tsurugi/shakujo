/*
 * Copyright 2018-2018 shakujo project.
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
#ifndef SHAKUJO_COMMON_CORE_TYPE_TUPLE_H_
#define SHAKUJO_COMMON_CORE_TYPE_TUPLE_H_

#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "../Type.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::common::core::type {

/**
 * @brief tuple type binding.
 */
class Tuple final : public Type {
public:
    /**
     * @brief represents a element of Tuple.
     */
    class Element final {
    private:
        std::string name_;
        common::util::ClonablePtr<Type> type_;

    public:
        /**
         * @brief Construct a new object.
         * @param name the element name
         * @param type the element type
         */
        Element(std::string_view name, std::unique_ptr<Type> type)
            : name_(name), type_(std::move(type))
        {}

        /**
         * @brief Construct a new object.
         * @param name the element name
         * @param type the element type
         */
        Element(std::string_view name, Type const* type)
            : Element(name, util::make_clone(type))
        {}

        /**
         * @brief Construct a new object.
         * @param name the element name
         * @param type the element type
         */
        Element(std::string_view name, Type&& type)
            : Element(name, util::make_clone(type))
        {}

        /**
         * @brief Construct a new object.
         * @param type the element type
         */
        Element(std::unique_ptr<Type> type)  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            : type_(std::move(type))
        {}

        /**
         * @brief Construct a new object.
         * @param type the element type
         */
        Element(Type const* type)   // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            : Element(util::make_clone(type))
        {}

        /**
         * @brief Construct a new object.
         * @param type the element type
         */
        Element(Type&& type)   // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            : Element(util::make_clone(type))
        {}

        /**
         * @brief Destroys this object.
         */
        ~Element() noexcept = default;

        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        Element(Element const& other) = default;

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        Element(Element&& other) noexcept = default;

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        Element& operator=(Element const& other) = default;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        Element& operator=(Element&& other) noexcept = default;

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
    std::vector<Element> elements_;
    Type::Nullity nullity_;

public:
    /**
     * @brief the kind of this type.
     */
    static inline constexpr Kind tag = Kind::TUPLE;

    /**
     * @brief Constructs a new object.
     * @param elements the tuple elements
     * @param nullity nullity of the type
     */
    explicit Tuple(
        std::vector<Element> elements,
        Type::Nullity nullity = Type::Nullity::NULLABLE)
        : elements_(std::move(elements))
        , nullity_(nullity)
    {}

    /**
     * @brief Constructs a new object.
     * @param elements the tuple elements
     * @param nullity nullity of the type
     */
    Tuple(
        std::initializer_list<Element> elements,
        Type::Nullity nullity = Type::Nullity::NULLABLE)
        : elements_(elements)
        , nullity_(nullity)
    {}

    /**
     * @brief destructs this object.
     */
    ~Tuple() noexcept override = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Tuple(Tuple const& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Tuple(Tuple&& other) noexcept = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Tuple& operator=(Tuple const& other) = default;
    
    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Tuple& operator=(Tuple&& other) noexcept = default;

    Type::Kind kind() const override {
        return tag;
    }

    bool is_valid() const override {
        for (auto& element : elements_) {
            if (!util::is_valid(element.type())) {
                return false;
            }
        }
        return true;
    }

    Nullity nullity() const override {
        return nullity_;
    }

    Tuple* clone() const & override {
        return new Tuple(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Tuple* clone() && override {
        return new Tuple(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the tuple elements.
     * @return the tuple elements
     */
    std::vector<Element> const& elements() const {
        return elements_;
    }

    /**
     * @brief returns an element.
     * @param position the element position (0-origin)
     * @return the element
     */
    Element& operator[](std::size_t position) {
        return elements_[position];
    }

    /**
     * @brief returns an element.
     * @param position the element position (0-origin)
     * @return the element
     */
    inline Element const& operator[](std::size_t position) const {
        return const_cast<Tuple*>(this)->operator[](position);
    }
};

}  // namespace shakujo::common::core::type

#endif  // SHAKUJO_COMMON_CORE_TYPE_TUPLE_H_
