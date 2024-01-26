/*
 * Copyright 2018-2024 Project Tsurugi.
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

#ifndef SHAKUJO_COMMON_CORE_DOCUMENT_REGION_H_
#define SHAKUJO_COMMON_CORE_DOCUMENT_REGION_H_

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "shakujo/common/util/operators.h"

namespace shakujo::common::core {

/**
 * @brief Represents a region in documents.
 */
class DocumentRegion : private util::Equivalent<DocumentRegion>, util::Boolean<DocumentRegion> {
public:
    /**
     * @brief represents a position in a document.
     */
    class Position : private util::Equivalent<Position>, util::Boolean<Position>, util::Comparable<Position> {
    private:
        std::size_t line_number_;
        std::size_t column_number_;

    public:
        /**
         * @brief constructs a new empty object.
         */
        constexpr Position() noexcept : Position(0U, 0U) {}

        /**
         * @brief constructs a new object.
         * @param line_number 1-origin line number
         * @param column_number 1-origin column number
         */
        constexpr Position(std::size_t line_number, std::size_t column_number = 0U) noexcept  // NOLINT
            : line_number_(line_number), column_number_(line_number == 0U ? 0U : column_number)
        {}

        /**
         * @brief returns the line number.
         * @return the 1-origin line number
         * @return 0 if it is not defined
         */
        inline constexpr std::size_t line_number() const {
            return line_number_;
        }

        /**
         * @brief returns the column number.
         * @return the 1-origin column number
         * @return 0 if it is not defined
         */
        inline constexpr std::size_t column_number() const {
            return column_number_;
        }

        /**
         * @brief returns whether or not this represents a valid position.
         * @return true this represents a valid position
         * @return false otherwise
         */
        inline constexpr explicit operator bool() const noexcept {
            return line_number_ != 0U;
        }

        /**
         * @brief returns whether this is equivalent to the given position.
         * @param other the target position
         * @return true true the both are equivalent
         * @return false otherwise
         */
        inline constexpr bool operator==(Position const& other) const {
            return line_number_ == other.line_number_ && column_number_ == other.column_number_;
        }

        /**
         * @brief compares this with the given position.
         * @param other the target position
         * @return < 0 if this is before the given position
         * @return = 0 if two positions are equivalent
         * @return > 0 if this is after the given position
         */
        inline constexpr int compare(Position const& other) const {
            if (line_number_ < other.line_number_) {
                return -1;
            }
            if (line_number_ > other.line_number_) {
                return +1;
            }
            if (column_number_ < other.column_number_) {
                return -1;
            }
            if (column_number_ > other.column_number_) {
                return +1;
            }
            return 0;
        }

        /**
         * @brief Appends the object information into the given output stream.
         * @param out the target output stream
         * @param value the target object
         * @return the output stream
         */
        friend std::ostream& operator<<(std::ostream& out, DocumentRegion::Position const& value);
    };

private:
    std::shared_ptr<std::string const> path_;
    Position begin_;
    Position end_;

public:
    /**
     * @brief constructs a new empty object.
     */
    DocumentRegion() noexcept : DocumentRegion(std::shared_ptr<std::string const>()) {};

    /**
     * @brief constructs a new object
     * @param path the document path
     * @param begin the beginning position (inclusive)
     * @param end the ending position (inclusive)
     */
    explicit DocumentRegion(
            std::shared_ptr<std::string const> path,
            Position begin = {},
            Position end = {}) noexcept
        : path_(std::move(path)), begin_(begin), end_(end)
    {}

    /**
     * @brief constructs a new object
     * @param path the document path
     * @param begin the beginning position (inclusive)
     * @param end the ending position (inclusive)
     */
    explicit DocumentRegion(
            std::string_view path,
            Position begin = {},
            Position end = {}) noexcept
        : path_(std::make_shared<std::string>(path)), begin_(begin), end_(end)
    {}

    /**
     * @brief returns the document path.
     * @return the document path
     * @return empty string if it is not defined
     */
    std::string const& path() const;

    /**
     * @brief returns the shared pointer of document path.
     * @return the shared pointer of document path
     * @return empty shared pointer if it is not defined
     */
    inline std::shared_ptr<std::string const> path_shared() const {
        return path_;
    }

    /**
     * @brief returns the begining position of this region.
     * @return the beginning position (inclusive)
     */
    inline Position const& begin() const {
        return begin_;
    }

    /**
     * @brief returns the ending position of this region.
     * @return the ending position (inclusive)
     */
    inline Position const& end() const {
        return end_;
    }

    /**
     * @brief returns whether or not this represents a valid position.
     * @return true this represents a valid position
     * @return false otherwise
     */
    inline explicit operator bool() const noexcept {
        return !path().empty();
    }

    /**
     * @brief returns whether or not this is equivalent to the given region.
     * @param other the target region
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(DocumentRegion const& other) const;

    /**
     * @brief Appends the object information into the given output stream.
     * @param out the target output stream
     * @param value the target object
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, DocumentRegion const& value);
};

/**
 * @brief sets the document region into the given element.
 * The target element must have region(DocumentRegion).
 * @param node the target node
 * @param region the document region
 * @return the node
 */
template<typename T>
std::unique_ptr<T> operator<<(std::unique_ptr<T> node, common::core::DocumentRegion region) {
    if (node) {
        node->region(std::move(region));
    }
    return node;
}

}  // namespace shakujo::common::core

#endif  // SHAKUJO_COMMON_CORE_DOCUMENT_REGION_H_
