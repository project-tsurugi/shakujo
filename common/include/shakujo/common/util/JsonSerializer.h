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
#ifndef SHAKUJO_COMMON_UTIL_JSON_SERIALIZER_H_
#define SHAKUJO_COMMON_UTIL_JSON_SERIALIZER_H_

#include "DataSerializer.h"

#include <iostream>
#include <memory>
#include <utility>

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/Type.h"

namespace shakujo::common::util {

/**
 * @brief JSON style DataSerializer implementation.
 */
class JsonSerializer : public DataSerializer {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    /**
     * @brief constructs a new object.
     * @param output the output sink
     * @param kind_property the property name for the object kind
     * @param pretty_print whether or not pretty printing is enabled
     */
    explicit JsonSerializer(
            std::ostream& output,
            std::string_view kind_property = "class",
            bool pretty_print = true);

    ~JsonSerializer() noexcept override;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    JsonSerializer(const JsonSerializer& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    JsonSerializer(JsonSerializer&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    JsonSerializer& operator=(JsonSerializer const& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    JsonSerializer& operator=(JsonSerializer&& other) noexcept = default;

    using DataSerializer::value;

    void enter_object(std::string_view kind) override;
    void exit_object(std::string_view kind) override;
    void enter_property(std::string_view name) override;
    void exit_property(std::string_view name) override;
    void enter_array(std::size_t size) override;
    void exit_array(std::size_t size) override;
    void value(std::nullptr_t ptr) override;
    void value(std::size_t value) override;
    void value(core::value::Bool::type value) override;
    void value(core::value::Int::type value) override;
    void value(core::value::Float::type value) override;
    void value(std::string_view value) override;
    void value(core::Name const& value) override;
};
}  // namespace shakujo::common::util

#endif  // SHAKUJO_COMMON_UTIL_JSON_SERIALIZER_H_
