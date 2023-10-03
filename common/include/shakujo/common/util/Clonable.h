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
#ifndef SHAKUJO_COMMON_UTIL_CLONABLE_H_
#define SHAKUJO_COMMON_UTIL_CLONABLE_H_

namespace shakujo::common::util {

/**
 * @brief an interface of polymorphic clonable object.
 */
class Clonable {
protected:
    /**
     * @brief constructs a new object.
     */
    Clonable() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    Clonable(Clonable const & other) = default;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Clonable(Clonable&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    Clonable& operator=(Clonable const & other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Clonable& operator=(Clonable&& other) noexcept = default;

public:
    /**
     * @brief Destroys this object.
     */
    virtual ~Clonable() noexcept = default;

    /**
     * @brief returns a clone of this object.
     * @return a clone
     */
    virtual Clonable* clone() const& = 0;

    /**
     * @brief returns a clone of this object.
     * @return  a clone
     */
    virtual Clonable* clone() && = 0;
};

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_CLONABLE_H_
