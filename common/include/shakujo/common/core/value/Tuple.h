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
#ifndef SHAKUJO_COMMON_CORE_VALUE_TUPLE_H_
#define SHAKUJO_COMMON_CORE_VALUE_TUPLE_H_

#include <utility>
#include <vector>

#include "../Value.h"
#include "shakujo/common/util/ClonablePtr.h"
#include "shakujo/common/util/PtrList.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::common::core::value {

/**
 * @brief Tuple type binding.
 */
class Tuple final : public Value {
public:
    /**
     * @brief the tuple elements.
     */
    class List : public common::util::PtrList<common::util::ClonablePtr<Value>> {
    public:
        /**
         * @brief the parent type.
         */
        using parent_type = common::util::PtrList<common::util::ClonablePtr<Value>>;

        /**
         * @brief the element type of list.
         */
        using element_type = typename parent_type::element_type;

        /**
         * @brief the size type of this list.
         */
        using size_type = typename parent_type::size_type;

        using parent_type::size;

        /**
         * @brief Construct a new object.
         * @param args elements
         */
        template<typename... Args>
        explicit List(Args&&... args) {
            append(std::forward<Args>(args)...);
        }

        /**
         * @brief Construct a new object.
         * @param elements elements
         */
        explicit List(std::vector<std::unique_ptr<Value>> elements) : parent_type(std::move(elements)) {}

    protected:
        using parent_type::clear;
        using parent_type::get_internal;
        using parent_type::insert_internal;
        using parent_type::remove_internal;

    private:
        inline void append() {}

        template<typename ...Rest>
        inline void append(element_type const* first, Rest&&... rest) {
            insert_internal(size()) = util::make_clone(first);
            append(std::forward<Rest>(rest)...);
        }

        template<typename ...Rest>
        inline void append(element_type&& first, Rest&&... rest) {
            insert_internal(size()) = util::make_clone(std::move(first));
            append(std::forward<Rest>(rest)...);
        }

        template<typename ...Rest>
        inline void append(std::unique_ptr<element_type> first, Rest&&... rest) {
            insert_internal(size()) = std::move(first);
            append(std::forward<Rest>(rest)...);
        }
    };

private:
    List elements_;

public:
    /**
     * @brief the kind of this value.
     */
    static inline constexpr auto tag = Kind::TUPLE;

    /**
     * @brief constructs a new object.
     */
    Tuple() = default;

    /**
     * @brief constructs a new object.
     * @param elements elements
     */
    explicit Tuple(std::unique_ptr<Value> elements) : elements_(std::move(elements)) {}

    /**
     * @brief constructs a new object.
     * @tparam Args element types
     * @param args elements
     */
    template <typename... Args>
    explicit Tuple(Args&&... args) : elements_(std::forward<Args>(args)...) {}

    /**
     * @brief Destroys this object.
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
     * @param other the copy source
     * @return this
     */
    Tuple& operator=(Tuple&& other) = default;

    Kind kind() const override {
        return tag;
    }

    Tuple* clone() const & override {
        return new Tuple(*this);  // NOLINT(cppcoreguidelines-owning-memory)
    }

    Tuple* clone() && override {
        return new Tuple(std::move(*this));  // NOLINT(cppcoreguidelines-owning-memory)
    }

    /**
     * @brief returns the element list of this binding.
     * @return the element list
     */
    List& elements() noexcept {
        return elements_;
    }

    /**
     * @brief returns the element list of this binding.
     * @return the element list
     */
    List const& elements() const noexcept {
        return elements_;
    }

    /**
     * @brief returns the number of elements.
     * @return the number of elements
     */
    List::size_type size() const noexcept {
        return elements_.size();
    }

    /**
     * @brief returns an element.
     * @param position the element position (0-origin)
     * @return the element
     */
    Value* operator[](List::size_type position) {  // FIXME
        return elements_[position];
    }

    /**
     * @brief returns an element.
     * @param position the element position (0-origin)
     * @return the element
     */
    inline const Value* operator[](List::size_type position) const {
        return const_cast<Tuple*>(this)->operator[](position);
    }
};

}  // namespace shakujo::common::core::value


#endif  // SHAKUJO_COMMON_CORE_VALUE_TUPLE_H_
