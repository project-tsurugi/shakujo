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
#ifndef SHAKUJO_COMMON_UTIL_PTR_LIST_H_
#define SHAKUJO_COMMON_UTIL_PTR_LIST_H_

#include <vector>
#include <memory>
#include <functional>
#include <utility>
#include <type_traits>

#include "operators.h"

namespace shakujo::common::util {

/**
 * @brief A default implementation of smart pointer list.
 * @tparam P the smart pointer of element type, should have operator=(std::unique_ptr<E>).
 * @tparam E the element type
 */
template<class P, class E = typename P::element_type>
class PtrList {
public:
    /**
     * @brief the element type of list.
     */
    using element_type = E;

    /**
     * @brief the smart pointer type of list.
     */
    using container_type = P;

    /**
     * @brief the value type of list.
     */
    using value_type = std::add_pointer_t<E>;

    /**
     * @brief the const value type of list.
     */
    using const_value_type = std::add_pointer_t<std::add_const_t<E>>;

    /**
     * @brief the size type of this list.
     */
    using size_type = typename std::vector<std::unique_ptr<E>>::size_type;

private:
    std::vector<container_type> entity_;

protected:
    /**
     * @brief Construct a new object.
     */
    PtrList() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param elements the source elements
     */
    explicit PtrList(std::vector<std::unique_ptr<E>> elements) {
        entity_.reserve(elements.size());
        for (auto& element : elements) {
            entity_.emplace_back(std::move(element));
        }
    }

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    PtrList(PtrList const& other) = default;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    PtrList(PtrList&& other) noexcept = default;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    PtrList& operator=(PtrList const& other) = default;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    PtrList& operator=(PtrList&& other) noexcept = default;

    /**
     * @brief sets element into this object.
     * @param elements the source object
     * @return this
     */
    PtrList& operator=(std::vector<std::unique_ptr<E>> elements) {
        entity_.clear();
        operator+=(std::move(elements));
        return *this;
    }

    /**
     * @brief sets element into this object.
     * @param elements the source object
     * @return this
     */
    PtrList& operator+=(std::vector<std::unique_ptr<E>> elements) {
        entity_.reserve(entity_.size() + elements.size());
        for (auto& ptr : elements) {
            entity_.emplace_back(std::move(ptr));
        }
        return *this;
    }

public:
    /**
     * @brief Destroy this object.
     */
    virtual ~PtrList() noexcept = default;

public:
    /**
     * @brief Returns the number of elements in this list.
     * @return the number of elements
     */
    size_type size() const {
        return entity_.size();
    }

    /**
     * @brief returns whether or not this list is empty.
     * @return true if this is empty list
     * @return false otherwise
     */
    bool empty() const {
        return entity_.empty();
    }

    /**
     * @brief ensures the capacity of this list.
     * @param capacity the capacity
     */
    void reserve(size_type capacity) {
        entity_.reserve(capacity);
    }

    /**
     * @brief Returns an element on the given position.
     * @param index the element position
     * @return the corresponded element
     * @throws std::out_of_range if the position is out-of-bounds
     */
    value_type at(size_type index) {
        return entity_.at(index).get();
    }

    /**
     * @brief Returns an element on the given position.
     * @param index the element position
     * @return the corresponded element
     * @throws std::out_of_range if the position is out-of-bounds
     */
    inline const_value_type at(size_type index) const {
        return const_cast<PtrList*>(this)->at(index);
    }

    /**
     * @brief Returns an element on the given position.
     * @param index the element position
     * @return the corresponded element
     * @throws std::out_of_range if the position is out-of-bounds
     */
    value_type operator[](size_type index) {
        return entity_[index].get();
    }

    /**
     * @brief Returns an element on the given position.
     * @param index the element position
     * @return the corresponded element
     * @throws std::out_of_range if the position is out-of-bounds
     */
    inline const_value_type operator[](size_type index) const {
        return const_cast<PtrList*>(this)->operator[](index);
    }

    /**
     * @brief Returns the first element.
     * @return the first element on this list
     * @throws std::out_of_range if this is empty
     */
    value_type front() {
        return entity_.front().get();
    }

    /**
     * @brief Returns the first element.
     * @return the first element on this list
     * @throws std::out_of_range if this is empty
     */
    const_value_type front() const {
        return const_cast<PtrList*>(this)->front();
    }

    /**
     * @brief Returns the last element.
     * @return the last element on this list
     * @throws std::out_of_range if this is empty
     */
    value_type back() {
        return *entity_.back();
    }

    /**
     * @brief Returns the last element.
     * @return the last element on this list
     * @throws std::out_of_range if this is empty
     */
    const_value_type back() const {
        return const_cast<PtrList*>(this)->back();
    }

protected:
    /**
     * @brief Returns the smart pointer on the given position.
     * @param index the element position (0-origin)
     * @return reference of the corresponded smart pointer
     */
    inline container_type& get_internal(size_type index) {
        return entity_[index];
    }

    /**
     * @brief Inserts an empty smart pointer to the given position.
     * @param index the element position (0-origin)
     * @return reference of the created smart pointer
     */
    inline container_type& insert_internal(size_type index) {
        entity_.emplace(entity_.begin() + index, nullptr);
        return entity_[index];
    }

    /**
     * @brief Removes the smart pointer on the given position.
     * @param index the element position (0-origin)
     * @return the removed smart pointer
     */
    inline container_type remove_internal(size_type index) {
        container_type ret = std::move(entity_[index]);
        entity_.erase(entity_.begin() + index);
        return ret;
    }

    /**
     * @brief Removes all elements in this list.
     */
    inline void clear() {
        entity_.clear();
    }

public:
    /**
     * @brief A wrapper of std::vector::iterator.
     * @tparam T the source iterator type
     * @tparam U the element type
     */
    template<class T, class U>
    class iterator_wrapper : private Equivalent<iterator_wrapper<T, U>> {
    public:
        /**
         * @brief The wrapped iterator type.
         */
        using iterator_type = T;

        /**
         * @brief The value type.
         */
        using value_type = U;

        /**
         * @brief The difference type.
         */
        using difference_type = typename iterator_type::difference_type;

        /**
         * @brief The value pointer type.
         */
        using pointer = U*;

        /**
         * @brief The value reference type.
         */
        using reference = U&;

        /**
         * @brief The iterator category tag.
         */
        using iterator_category = std::random_access_iterator_tag;

    private:
        iterator_type entity_;

    public:
        /**
         * @brief construct a new object.
         * @param entity the entity iterator
         */
        explicit iterator_wrapper(iterator_type entity)
            : entity_(std::move(entity)) {}

        /**
         * @brief Advances this iterator.
         * @return this
         */
        inline iterator_wrapper& operator++() {
            ++entity_;
            return *this;
        }

        /**
         * @brief Advances this iterator and returns the original one.
         * @return a copy of iterator which points the original position
         */
        inline const iterator_wrapper operator++(int) {
            iterator_wrapper copy { entity_++ };
            return copy;
        }

        /**
         * @brief Returns an advanced iterator.
         * @param difference the difference to advance
         * @return the advanced iterator
         */
        inline iterator_wrapper operator+(difference_type difference) const {
            iterator_wrapper copy { entity_ + difference };
            return copy;
        }

        /**
         * @brief Advances this iterator.
         * @param difference the difference to advance
         * @return this
         */
        inline iterator_wrapper& operator+=(difference_type difference) {
            entity_ += difference;
            return *this;
        }

        /**
         * @brief Returns whether or not this iterator is equivalent to the given one.
         * @param other the target iterator
         * @return true if the two iterators are equivalent
         * @return false otherwise
         */
        inline bool operator==(const iterator_wrapper& other) const {
            return entity_ == other.entity_;
        }

        /**
         * @brief Returns the element where this iterator points.
         * @return the pointing element
         */
        inline value_type operator*() {
            return entity_->get();
        }
    };

    /**
     * @brief The iterator type.
     */
    using iterator = iterator_wrapper<typename std::vector<container_type>::iterator, value_type>;

    /**
     * @brief The iterator type for constant list.
     */
    using const_iterator = iterator_wrapper<typename std::vector<container_type>::const_iterator, const_value_type>;

    /**
     * @brief The reverse iterator type.
     */
    using reverse_iterator = iterator_wrapper<typename std::vector<container_type>::reverse_iterator, value_type>;

    /**
     * @brief The reverse iterator type for constant list.
     */
    using const_reverse_iterator = iterator_wrapper<typename std::vector<container_type>::const_reverse_iterator, const_value_type>;

    /**
     * @brief Returns a new iterator which indicates the beginning of this list.
     * @return the created iterator
     */
    iterator begin() {
        return iterator(entity_.begin());
    }

    /**
     * @brief Returns a new iterator which indicates the beginning of this list.
     * @return the created iterator
     */
    const_iterator begin() const {
        return const_iterator(entity_.cbegin());
    }

    /**
     * @brief Returns a new iterator which indicates the beginning of this list.
     * @return the created iterator
     */
    const_iterator cbegin() const {
        return const_iterator(entity_.cbegin());
    }

    /**
     * @brief Returns a new iterator which indicates the ending of this list.
     * @return the created iterator
     */
    iterator end() {
        return iterator(entity_.end());
    }

    /**
     * @brief Returns a new iterator which indicates the ending of this list.
     * @return the created iterator
     */
    const_iterator end() const {
        return const_iterator(entity_.cend());
    }

    /**
     * @brief Returns a new iterator which indicates the ending of this list.
     * @return the created iterator
     */
    const_iterator cend() const {
        return const_iterator(entity_.cend());
    }

    /**
     * @brief Returns a new reverse iterator which indicates the ending of this list.
     * @return the created reverse iterator
     */
    reverse_iterator rbegin() {
        return reverse_iterator(entity_.rbegin());
    }

    /**
     * @brief Returns a new reverse iterator which indicates the ending of this list.
     * @return the created reverse iterator
     */
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(entity_.crbegin());
    }

    /**
     * @brief Returns a new reverse iterator which indicates the ending of this list.
     * @return the created reverse iterator
     */
    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(entity_.crbegin());
    }

    /**
     * @brief Returns a new reverse iterator which indicates the beginning of this list.
     * @return the created reverse iterator
     */
    reverse_iterator rend() {
        return reverse_iterator(entity_.rend());
    }

    /**
     * @brief Returns a new reverse iterator which indicates the beginning of this list.
     * @return the created reverse iterator
     */
    const_reverse_iterator rend() const {
        return const_reverse_iterator(entity_.crend());
    }

    /**
     * @brief Returns a new reverse iterator which indicates the beginning of this list.
     * @return the created reverse iterator
     */
    const_reverse_iterator crend() const {
        return const_reverse_iterator(entity_.crend());
    }
};

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_PTR_LIST_H_
