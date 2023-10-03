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
#ifndef SHAKUJO_ANALYZER_SCOPE_SCOPE_H_
#define SHAKUJO_ANALYZER_SCOPE_SCOPE_H_

#include <memory>
#include <stdexcept>

#include "Result.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::analyzer::scope {

/**
 * @brief an interface that provides shared elements.
 * @tparam T binding kind
 */
template <typename T>
class Scope {
protected:
    /**
     * @brief constructs a new object.
     */
    Scope() = default;

public:
    /**
     * @brief the element type.
     */
    using element_type = T;

    /**
     * @brief destroys this object.
     */
    virtual ~Scope() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    Scope(Scope const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Scope(Scope&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Scope& operator=(Scope const& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Scope& operator=(Scope&& other) noexcept = default;

    /**
     * @brief returns an element for the given name.
     * @param name the element name
     * @return the find result
     */
    virtual Result<T> find(model::name::Name const* name) const = 0;

    /**
     * @brief returns an element for the given name.
     * @param name the element name
     * @return the find result
     */
    Result<T> find(std::unique_ptr<model::name::Name> const& name) const {
        return find(name.get());
    }

    /**
     * @brief inserts a new element into this scope.
     * This implementation always raises an error.
     * @param name the element name
     * @param element the target element
     * @throw std::domain_error if not supported
     */
    virtual void insert([[maybe_unused]] model::name::Name const* name, [[maybe_unused]] std::shared_ptr<T> element) {
        throw std::domain_error("this scope does not support adding elements");
    }

    /**
     * @brief inserts a new element into this scope.
     * @param name the element name
     * @param element the target element
     * @throw std::domain_error if not supported
     */
    void insert(std::unique_ptr<model::name::Name> const& name, std::shared_ptr<T> element) {
        insert(name.get(), std::move(element));
    }
};
}  // namespace shakujo::analyzer::scope

#endif  //SHAKUJO_ANALYZER_SCOPE_SCOPE_H_
