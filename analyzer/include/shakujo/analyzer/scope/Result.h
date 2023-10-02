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
#ifndef SHAKUJO_ANALYZER_SCOPE_RESULT_H_
#define SHAKUJO_ANALYZER_SCOPE_RESULT_H_

#include <memory>
#include <utility>
#include <vector>

#include "shakujo/model/name/Name.h"

namespace shakujo::analyzer::scope {

/**
 * @brief represents a result of Scope::find().
 * @see Scope::find()
 */
template<typename T>
class Result {
private:
    model::name::Name const* name_ = {};
    std::shared_ptr<T> element_ = {};
    std::vector<std::size_t> succeeding_indices_ = {};

public:
    /**
     * @brief constructs a new missing result.
     */
    Result() = default;

    /**
     * @brief constructs a new object.
     * @param name the corresponded name
     * @param element the found element
     * @param succeeding_indices the required succeeding indices
     */
    Result(
            model::name::Name const* name,
            std::shared_ptr<T> element,
            std::vector<std::size_t> succeeding_indices = {}) noexcept
        : name_(element ? name : nullptr)
        , element_(std::move(element))
        , succeeding_indices_(std::move(succeeding_indices))
    {}

    /**
     * @brief constructs a new object.
     * @param element the found element
     * @param succeeding_indices the required succeeding indices
     */
    explicit Result(
            std::shared_ptr<T> element,
            std::vector<std::size_t> succeeding_indices = {}) noexcept
        : element_(std::move(element))
        , succeeding_indices_(std::move(succeeding_indices))
    {}

    /**
     * @brief destructs this object.
     */
    ~Result() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param other the source object
     */
    Result(Result const& other) = default;

    /**
     * @brief constructs a new object.
     * @param other the source object
     */
    Result(Result&& other) noexcept = default;

    /**
     * @brief assigns to this object.
     * @param other the source object
     * @return this
     */
    Result& operator=(Result const& other) = default;

    /**
     * @brief assigns to this object.
     * @param other the source object
     * @return this
     */
    Result& operator=(Result&& other) noexcept = default; // std::map may throw
    /**
     * @brief returns the matched name.
     * @return the matched name, or an undefined name if not matched
     */
    model::name::Name const* name() const {
        return name_;
    }

    /**
     * @brief returns the found element
     * @return the found element, or null if not found
     */
    std::shared_ptr<T> element() const {
        return element_;
    }

    /**
     * @brief returns the required trailing indices.
     * Note that, the name() does not contain the trailing indices.
     * @return the required succeeding indices
     */
    std::vector<std::size_t> const& succeeding_indices() const {
        return succeeding_indices_;
    }

    /**
     * @brief returns whether or not this contains a valid result.
     * @return true if this is valid
     * @return false otherwise
     */
    explicit operator bool() const {
        return static_cast<bool>(element_);
    }
};
}  // namespace shakujo::analyzer::scope

#endif  //SHAKUJO_ANALYZER_SCOPE_RESULT_H_
