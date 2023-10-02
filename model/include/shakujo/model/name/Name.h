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
#ifndef SHAKUJO_MODEL_NAME_NAME_H_
#define SHAKUJO_MODEL_NAME_NAME_H_

#include <memory>
#include <string>
#include <vector>

#include "../Node.h"
#include "NameKind.h"

#include "shakujo/common/core/Name.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::model::name {

/**
 * @brief represents a name in IR model.
 */
class Name : public Node, private common::util::Equivalent<Name> {
protected:
    /**
     * @brief Constructs a new object.
     */
    Name() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Name(const Name& other) = default;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Name(Name&& other) noexcept = default;

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Name& operator=(const Name& other) = default;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Name& operator=(Name&& other) noexcept = default;

public:
    /**
     * @brief Destroys this object.
     */
    ~Name() noexcept override = default;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Name* clone() const & override = 0;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    Name* clone() && override = 0;

    /**
     * @brief Returns the kind of this name.
     * @return the name kind
     */
    virtual NameKind kind() const = 0;

    /**
     * @brief Returns whether or not this is equivalent to the given name.
     * @return true if both are equivalent
     * @return false otherwise
     */
    bool operator==(const Name& other) const;

    /**
     * @brief returns common::core::Name.
     * @return the corresponded name
     */
    operator common::core::Name() const;  // NOLINT

    /**
     * @brief returns the number of name segments.
     * @return the number of name segments
     * @see tokens()
     */
    std::size_t size() const;

    /**
     * @brief returns name segments.
     * @return the name segments
     */
    std::vector<std::string> segments() const;

    /**
     * @brief Appends name into the given output stream.
     * @param out the target output stream
     * @param value the target name
     * @return the output stream
     */
    friend std::ostream& operator<<(std::ostream& out, const Name& value);
};

}  // namespace shakujo::model::name

#endif  // SHAKUJO_MODEL_NAME_NAME_H_
