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
#ifndef SHAKUJO_COMMON_UTIL_DATA_SERIALIZER_H_
#define SHAKUJO_COMMON_UTIL_DATA_SERIALIZER_H_

#include <string_view>

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"

namespace shakujo::common::util {

/**
 * @brief prints IR node tree.
 */
class DataSerializer {
protected:
    /**
     * @brief constructs a new object.
     */
    DataSerializer() noexcept = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    DataSerializer(const DataSerializer& other) = default;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    DataSerializer(DataSerializer&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    DataSerializer& operator=(const DataSerializer& other) = default;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    DataSerializer& operator=(DataSerializer&& other) noexcept = default;

public:
    /**
     * @brief destructs this object.
     */
    virtual ~DataSerializer() noexcept = default;

    /**
     * @brief callback on entering into an object.
     * @param kind the object kind, or empty to omit
     */
    virtual void enter_object(std::string_view kind) = 0;

    /**
     * @brief callback on exiting from an object.
     * @param kind the object kind, or empty to omit
     */
    virtual void exit_object(std::string_view kind) = 0;

    /**
     * @brief callback on entering into a property.
     * @param name the property name
     */
    virtual void enter_property(std::string_view name) = 0;

    /**
     * @brief callback on exiting from a property.
     * @param name the property name
     */
    virtual void exit_property(std::string_view name) = 0;

    /**
     * @brief callback on entering into an array.
     * @param size the array size
     */
    virtual void enter_array(std::size_t size) = 0;

    /**
     * @brief callback on exiting from an array.
     * @param size the array size
     */
    virtual void exit_array(std::size_t size) = 0;

    /**
     * @brief callback on a null value.
     */
    inline void value() {
        value(nullptr);
    }

    /**
     * @brief callback on a null value.
     * @param ptr null pointer
     */
    virtual void value(std::nullptr_t ptr) = 0;

    /**
     * @brief callback on a value.
     * @param value the value
     */
    virtual void value(std::size_t value) {
        this->value(std::to_string(value));
    }

    /**
     * @brief callback on a value.
     * @param value the value
     */
    virtual void value(core::value::Bool::type value) {
        this->value(value ? "TRUE" : "FALSE");
    }

    /**
     * @brief callback on a value.
     * @param value the value
     */
    virtual void value(core::value::Int::type value) {
        this->value(std::to_string(value));
    }

    /**
     * @brief callback on a value.
     * @param value the value
     */
    virtual void value(core::value::Float::type value) {
        this->value(std::to_string(value));
    }

    /**
     * @brief callback on a value.
     * @param value the value
     */
    virtual void value(std::string_view value) = 0;

    /**
     * @brief callback on a value.
     * @param value the value
     */
    inline void value(char const* value) {
        this->value(std::string_view { value });
    }

    /**
     * @brief callback on a value.
     * @param value the value
     */
    virtual void value(core::Name const& value) = 0;
};

}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_DATA_SERIALIZER_H_
