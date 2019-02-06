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
#ifndef SHAKUJO_COMMON_UTIL_PTR_H_
#define SHAKUJO_COMMON_UTIL_PTR_H_

#include <memory>

#include "operators.h"
#include "utility.h"

namespace shakujo::common::util {

/**
 * @brief copyable and movable pointer container.
 * The element type must be support polymorphic clone like Cloneable.
 * @tparam E the element type
 * @see Cloneable
 */
template<typename E>
class ClonablePtr final : private Equivalent<ClonablePtr<E>> {
private:
    template<typename T>
    friend class ClonablePtr;

    std::unique_ptr<E> element_;

public:
    /**
     * @brief the element type.
     */
    using element_type = E;

    /**
     * @brief Constructs a new object.
     */
    ClonablePtr() noexcept = default;

    /**
     * @brief Constructs a new object.
     */
    explicit ClonablePtr(std::unique_ptr<E> element) : element_(std::move(element)) {}

    /**
     * @brief Destroys this object.
     */
    ~ClonablePtr() noexcept = default;

    /**
     * @brief Copy-constructs a new object.
     * @param other a copy source
     */
    template<typename T>
    ClonablePtr(ClonablePtr<T> const& other)  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : element_(make_clone(other))
    {}

    /**
     * @brief Copy-constructs a new object.
     * @param other a copy source
     */
    template<typename T>
    ClonablePtr(ClonablePtr<T> && other)  // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
        : element_(std::move(other.element_))
    {}

    /**
     * @brief Copy-constructs a new object.
     * @param other a copy source
     */
    ClonablePtr(ClonablePtr const& other) : element_(make_clone(other)) {}

    /**
     * @brief Move-constructs a new object.
     * @param other a move source
     */
    ClonablePtr(ClonablePtr && other) noexcept = default;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ClonablePtr& operator=(ClonablePtr const& other) {
        element_ = make_clone(other);
        return *this;
    }

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ClonablePtr& operator=(ClonablePtr && other) noexcept = default;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ClonablePtr& operator=(std::unique_ptr<E> other) noexcept {
        element_ = std::move(other);
        return *this;
    };

    /**
     * @brief resets this object.
     * @return this
     */
    ClonablePtr& operator=(std::nullptr_t) noexcept {
        element_.reset();
        return *this;
    }

    /**
     * @brief returns the element pointer.
     * @return the element pointer
     */
    E* get() {
        return element_.get();
    }

    /**
     * @brief returns the element pointer.
     * @return the element pointer
     */
    const E* get() const {
        return element_.get();
    }

    /**
     * @brief returns the element.
     * @return the element
     */
    E& operator*() {
        return *element_;
    }

    /**
     * @brief returns the element.
     * @return the element
     */
    E const& operator*() const {
        return *element_;
    }

    /**
     * @brief returns the element.
     * @return the element
     */
    E* operator->() {
        return element_.get();
    }

    /**
     * @brief returns the element.
     * @return the element
     */
    E const * operator->() const {
        return element_.get();
    }

    /**
     * @brief releases the pointing target.
     * @return released element
     */
    std::unique_ptr<E> release() {
        return std::move(element_);
    }

    /**
     * @brief returns whther or not the target element is present.
     * @return true if the target element is not null
     * @return false if the target element is null
     */
    explicit operator bool() const {
        return element_.operator bool();
    }

    /**
     * @brief returns whether or not this container holds the same pointer of the given one.
     * @param other the target container
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(ClonablePtr const& other) const {
        return element_ == other.element_;
    }
};
}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_PTR_H_
