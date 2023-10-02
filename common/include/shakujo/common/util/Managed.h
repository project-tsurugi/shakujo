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
#ifndef SHAKUJO_COMMON_UTIL_MANAGED_H_
#define SHAKUJO_COMMON_UTIL_MANAGED_H_

#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

#include "ManagedPtr.h"
#include "utility.h"

namespace shakujo::common::util {

/**
 * @brief an interface of managed object by ManagedPtr.
 * @tparam E interface type
 * @see ManagedPtr
 */
template<typename E>
class Managed {
private:
    ManagedPtr<E>* container_ { nullptr };

protected:
    /**
     * @brief Constructs a new object.
     */
    Managed() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Managed(Managed const & other) noexcept {
        // never copy the container
        (void) other; // gcc cannot parse [[maybe_unused]] in the first arg of ctor
    }

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Managed(Managed&& other) noexcept {
        // never move the container
        (void) other; // gcc cannot parse [[maybe_unused]] in the first arg of ctor
    }

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Managed& operator=([[maybe_unused]] Managed const & other) noexcept {
        // never copy the container
        return *this;
    }

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Managed& operator=([[maybe_unused]] Managed && other) noexcept {
        // never move the container
        return *this;
    }

public:
    /**
     * @brief Destroys this object.
     */
    virtual ~Managed() noexcept = default;

    /**
     * @brief returns whether or not this object is managed.
     * @return true if this object is managed
     * @return false otherwise
     */
    bool is_managed() const {
        return common::util::is_defined(container_);
    }

    /**
     * @brief replaces this object with another one.
     * Note that, this operation may destroy this object if you don't escape
     * the unique_ptr of passed function argument.
     * @tparam T the replacement type
     * @param replacer the replacer function, must not return empty pointer
     * @return the replaced object
     */
    E* replace(std::function<std::unique_ptr<E>(std::unique_ptr<E>)> const& replacer) {
        ManagedPtr<E>* container = container_;
        if (!common::util::is_defined(container)) {
            throw std::runtime_error("object must be managed");
        }
        auto self = container->release();
        auto replacement = replacer(std::move(self));
        if (!common::util::is_defined(replacement)) {
            throw std::logic_error("replacement must not be null");
        }
        auto raw = replacement.get();
        container->operator=(std::move(replacement));
        return raw;
    }

    /**
     * @brief replaces this object with another one.
     * @tparam T the replacement type
     * @param replacement the replacement
     * @return the replaced object
     */
    template<class T>
    std::enable_if_t<std::is_base_of_v<E, T>, T*>
    replace_with(std::unique_ptr<T> replacement) {
        return dynamic_pointer_cast<T>(replace([&](auto){ return std::move(replacement); }));
    }

private:
    friend class ManagedPtr<E>;

    /**
     * @brief sets a container that manages this object.
     * @param container the pointer of container, or nullptr if detached
     */
    void managed_by(ManagedPtr<E>* container) {
        container_ = container;
    }
};

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_MANAGED_H_
