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
#ifndef SHAKUJO_COMMON_API_RESULT_SET_H_
#define SHAKUJO_COMMON_API_RESULT_SET_H_

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

#include "shakujo/common/core/type/Relation.h"

namespace shakujo::common::api {

/**
 * @brief represents a result set of queries.
 */
class ResultSet {
protected:
    /**
     * @brief constructs a new object.
     */
    ResultSet() = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    ResultSet(ResultSet const& other) = default;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    ResultSet(ResultSet&& other) noexcept = default;

    /**
     * @brief assigns the given object into this object.
     * @param other the copy source
     * @return this
     */
    ResultSet& operator=(ResultSet const& other) = default;

    /**
     * @brief assigns the given object into this object.
     * @param other the move source
     * @return this
     */
    ResultSet& operator=(ResultSet&& other) noexcept = default;

public:
    /**
     * @brief destroys this object.
     */
    virtual ~ResultSet() = default;

    /**
     * @brief returns the type information of this result set.
     * It may not have any columns if it is not sure or this result set is not valid.
     * @return the type information, may not have any columns
     */
    virtual core::type::Relation const* type() = 0;

    /**
     * @brief returns the number of columns in this results.
     * @return the number of columns
     * @return 0 if this result set is not valid
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief advances the pointing row to the next one and returns whether or not such the row exists.
     * The ResultSet initially does not point any rows until call this function.
     * @return true the next row exists
     * @return false otherwise
     */
    virtual bool next() = 0;

    /**
     * @brief returns whether or not the target column was NULL in the current row.
     * @param position the target column position (0-origin)
     * @return true if the target column was NULL
     * @return false otherwise
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column nullity is not compatible (optional behavior)
     */
    virtual bool is_null(std::size_t position) const = 0;

    /**
     * @brief returns the boolean column value in the current row.
     * @param position the target column position (0-origin)
     * @return the column value
     * @return undefined if the target column was NULL
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     */
    virtual bool get_bool(std::size_t position) const = 0;

    /**
     * @brief returns the 32-bit int column value in the current row.
     * @param position the target column position (0-origin)
     * @return the column value
     * @return undefined if the target column was NULL
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     */
    virtual std::int32_t get_int32(std::size_t position) const = 0;

    /**
     * @brief returns the 64-bit int column value in the current row.
     * @param position the target column position (0-origin)
     * @return the column value
     * @return undefined if the target column was NULL
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     */
    virtual std::int64_t get_int64(std::size_t position) const = 0;

    /**
     * @brief returns the 32-bit float column value in the current row.
     * @param position the target column position (0-origin)
     * @return the column value
     * @return undefined if the target column was NULL
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     */
    virtual float get_float32(std::size_t position) const = 0;

    /**
     * @brief returns the 64-bit float column value in the current row.
     * @param position the target column position (0-origin)
     * @return the column value
     * @return undefined if the target column was NULL
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     */
    virtual double get_float64(std::size_t position) const = 0;

    /**
     * @brief returns the pointer to a buffer contains text-kind column value in the current row.
     * The returned view may be disabled after @link next() @endlink was called or this result set was disposed.
     * @param position the target column position (0-origin)
     * @return a buffer view that contains the column value
     * @return undefined if the target column was NULL
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     * @see util::strnlen()
     */
    virtual std::string_view get_text(std::size_t position) const = 0;

    /**
     * @brief copies the text-kind column value in the current row into the given buffer.
     * The filled buffer MAY NOT terminated by null character.
     * This operation may set undefined value to the buffer if the target column was NULL.
     * @param position the target column position (0-origin)
     * @param buffer pointer of the destination buffer
     * @param buffer_size the destination buffer size
     * @return a buffer view that contains the column value
     * @return undefined if the target column was NULL
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     * @see util::strnlen()
     */
    virtual std::string_view get_text(std::size_t position, void* buffer, std::size_t buffer_size) const;

    /**
     * @brief copies the text-kind column value in the current row into the given buffer.
     * If the column value contains a null character, the filled buffer MAY only have before the null character.
     * This operation may set undefined value to the buffer if the target column was NULL.
     * @param position the target column position (0-origin)
     * @param buffer the destination buffer
     * @throws std::out_of_range if the column position is out of range
     * @throws std::logic_error if this result set does not points any rows
     * @throws std::domain_error if the target column type is not compatible
     * @see is_null()
     */
    virtual void get_text(std::size_t position, std::string& buffer) const;
};
}  // namespace shakujo::common::api

#endif  // SHAKUJO_COMMON_API_RESULT_SET_H_
