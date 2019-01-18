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
#ifndef SHAKUJO_MODEL_UTIL_NODE_LIST_BASE_H_
#define SHAKUJO_MODEL_UTIL_NODE_LIST_BASE_H_

#include <memory>

#include "shakujo/common/util/PtrList.h"
#include "shakujo/common/util/MoveInitializerList.h"

namespace shakujo::model::util {
    /**
     * @brief A list of IR nodes.
     * @tparam P the pointer type
     */
    template<typename P>
    class NodeListBase : public common::util::PtrList<P> {
    public:
        /**
         * @brief the parent type.
         */
        using parent_type = common::util::PtrList<P>;

        /**
         * @brief the element type of list.
         */
        using element_type = typename parent_type::element_type;

        /**
         * @brief the smart pointer type of list.
         */
        using container_type = typename parent_type::container_type;

        /**
         * @brief the size type of this list.
         */
        using size_type = typename parent_type::size_type;

        using parent_type::size;
        using parent_type::clear;

    protected:
        using parent_type::get_internal;
        using parent_type::insert_internal;
        using parent_type::remove_internal;

    public:
        /**
         * @brief Construct a new object.
         */
        NodeListBase() noexcept = default;

        /**
         * @brief constructs a new object from element vector.
         * @param elements the initial elements
         */
        explicit NodeListBase(std::vector<container_type> elements)
            : parent_type(std::move(elements))
        {}

        /**
         * @brief Destroys this object.
         */
        ~NodeListBase() noexcept override = default;

    protected:
        /**
         * @brief Copy-constructs a new object.
         * @param other the source object
         */
        NodeListBase(const NodeListBase<P>& other) : parent_type() {
            for (auto& v : other) {
                insert_internal(size()).reset(v.clone());
            }
        }

        /**
         * @brief Copy-assigns to this object.
         * @param other the source object
         * @return this
         */
        NodeListBase<P>& operator=(const NodeListBase<P>& other) {
            clear();
            for (auto& v : other) {
                insert_internal(size()).reset(v.clone());
            }
            return *this;
        }

        /**
         * @brief Move-constructs a new object.
         * @param other the source object
         */
        NodeListBase(NodeListBase<P>&& other) noexcept = default;

        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        NodeListBase<P>& operator=(NodeListBase<P>&& other) noexcept = default;

    public:
        /**
         * @brief Move-assigns to this object.
         * @param other the source object
         * @return this
         */
        NodeListBase<P>& operator=(common::util::MoveInitializerList<std::unique_ptr<element_type>>&& other) noexcept {
            parent_type::operator=(std::move(other).build());
            return *this;
        }

        /**
         * @brief adds an element to tail of this.
         * @param ptr the smart pointer of element
         */
        void push_back(std::unique_ptr<element_type> ptr) {
            insert_internal(size()) = std::move(ptr);
        }

        /**
         * @brief Adds an element into the given position.
         * @param index the insert position (0-origin)
         * @param ptr the smart pointer of element
         */
        void insert(size_type index, std::unique_ptr<element_type> ptr) {
            insert_internal(index) = std::move(ptr);
        }

        /**
         * @brief Replaces the element on the given position.
         * @param index the target position (0-origin)
         * @param ptr the smart pointer of replacement element
         */
        void assign(size_type index, std::unique_ptr<element_type> ptr) {
            get_internal(index) = std::move(ptr);
        }

        /**
         * @brief swaps between two elements.
         * @param a the first element position
         * @param b the second element position
         */
        void swap(size_type a, size_type b) {
            if (a == b) {
                return;
            }
            get_internal(a).swap(get_internal(b));
        }

        /**
         * @brief Removes the element on the given position.
         * @param index the target position (0-origin)
         */
        void remove(size_type index) {
            remove_internal(index);
        }

        /**
         * @brief Removes the element on the given position and returns it.
         * @param index the remove position (0-origin)
         * @return the removed element
         */
        container_type release(size_type index) {
            return std::move(remove_internal(index));
        }
    };
}  // namespace shakujo::model::util

#endif  // SHAKUJO_MODEL_UTIL_NODE_LIST_BASE_H_
