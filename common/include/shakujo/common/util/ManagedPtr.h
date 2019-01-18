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
#ifndef SHAKUJO_COMMON_UTIL_MANAGED_PTR_H_
#define SHAKUJO_COMMON_UTIL_MANAGED_PTR_H_

#include <memory>
#include <utility>

namespace shakujo::common::util {

/**
 * @brief smart pointer for managed object.
 * The element class E must have E::managed_by(ManagedPtr<E>*).
 * @tparam E the element type
 * @see Managed
 */
template<typename E>
class ManagedPtr final {
private:
    std::unique_ptr<E> element_;

public:
    /**
     * @brief the element type.
     */
    using element_type = E;

    /**
     * @brief Constructs a new object.
     */
    ManagedPtr() noexcept = default;

    /**
     * @brief Destroys this object.
     */
    ~ManagedPtr() noexcept = default;

    /**
     * @brief Constructs a new object.
     * @param element the source pointer
     */
    explicit ManagedPtr(std::unique_ptr<E> element) : element_(std::move(element)) {
        attach();
    }

    /**
     * @brief Constructs a new object.
     * @param element the source pointer
     */
    explicit ManagedPtr(E* element) : ManagedPtr(std::unique_ptr<E>(element)) {
        attach();
    }

    /**
     * @brief Copy-constructs a new object.
     * @param other a copy source
     */
    ManagedPtr(ManagedPtr const & other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other a move source
     */
    ManagedPtr(ManagedPtr&& other) noexcept : element_(std::move(other.element_)) {
        attach();
    }

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    ManagedPtr& operator=(ManagedPtr const& other) = delete;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    ManagedPtr& operator=(ManagedPtr&& other) noexcept {
        detach();
        element_ = std::move(other.element_);
        attach();
        return *this;
    }

    /**
     * @brief remplaces this object.
     * @param other the source object
     * @return this
     */
    ManagedPtr& operator=(std::unique_ptr<E> other) noexcept {
        detach();
        element_ = std::move(other);
        attach();
        return *this;
    }

    /**
     * @brief resets this object.
     * @return this
     */
    ManagedPtr& operator=(std::nullptr_t) noexcept {
        detach();
        element_.reset();
        attach();
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
    E const * get() const {
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
    const E& operator*() const {
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
     * @brief sets the element pointer.
     * @param ptr the target pointer
     */
    void reset(E* ptr) {
        *this = std::unique_ptr<E>(ptr);
    }

    /**
     * @brief releases the pointing target.
     * @return released element
     */
    std::unique_ptr<E> release() {
        detach();
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

private:
    void attach() {
        if (element_) {
            element_->managed_by(this);
        }
    }

    void detach() {
        if (element_) {
            element_->managed_by(nullptr);
        }
    }
};

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_MANAGED_PTR_H_
