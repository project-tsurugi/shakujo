/*
 * Copyright 2018 shakujo project.
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
#ifndef PARSER_TEST_BASE_H_
#define PARSER_TEST_BASE_H_

#include "shakujo/parser/Parser.h"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>

#include "shakujo/model/IRFactory.h"

#include "shakujo/model/util/NodeSerializer.h"
#include "shakujo/common/util/JsonSerializer.h"

namespace shakujo::parser {

auto NON_NULL = shakujo::common::core::Type::Nullity::NEVER_NULL;

class ParserTestBase {
public:
    parser::Parser parser;
    model::IRFactory f;

    template<typename T>
    std::unique_ptr<T> parse_program_main(std::string const& text, std::string_view message = "invalid main statement") {
        auto program = parser.parse_program("<testing>", text);
        auto result = cast_ptr(program->release_main(), message).to<T>();
        return result;
    }

    template<typename T>
    std::unique_ptr<T> parse_expression(std::string const& text, std::string_view message = "invalid expression type") {
        std::istringstream input { text };
        auto node = parser.parse_expression("<testing>", input);
        auto result = cast_ptr(std::move(node), message).to<T>();
        return result;
    }

    template<typename T>
    struct RawCaster {
        T* v;
        std::function<void(T*)> thrower;

        template<typename U>
        U* to() {
            if (auto u = dynamic_cast<U*>(v)) {
                return u;
            }
            thrower(v);
            throw std::bad_cast();
        }
    };

    template<typename T>
    struct PtrCaster {
        std::unique_ptr<T> ptr;
        std::function<void(std::unique_ptr<T>)> thrower;

        template<typename U>
        std::unique_ptr<U> to() {
            auto raw = dynamic_cast<U*>(ptr.get());
            if (!raw) {
                thrower(std::move(ptr));
                throw std::bad_cast();
            }
            std::unique_ptr<U> result { raw };
            ptr.release();
            return result;
        }
    };

    template<typename T>
    RawCaster<T> cast_node(T* node, std::string_view message = "cast failure") {
        return RawCaster<T> { node, [=](T* node) { raise(message, *node); }};
    };

    template<typename T>
    PtrCaster<T> cast_ptr(std::unique_ptr<T> ptr, std::string_view message = "cast failure") {
        return PtrCaster<T> { std::move(ptr), [&](std::unique_ptr<T> ptr) { raise(message, *ptr); }};
    };

    template<typename T>
    [[noreturn]] void raise(std::string_view message, const T& node) {
        std::ostringstream ss;
        ss << message << ": ";
        shakujo::common::util::JsonSerializer json { ss };
        shakujo::model::util::NodeSerializer serializer {};
        serializer.show_undefined_value(true);

        serializer.serialize(json, &node);
        throw std::logic_error(ss.str());
    }

    template<typename T>
    std::string to_string(const T& node) {
        std::ostringstream ss;
        shakujo::common::util::JsonSerializer json { ss };
        shakujo::model::util::NodeSerializer serializer {};
        serializer.serialize(json, node);
        return ss.str();
    }
};
}  // namespace shakujo::parser

#endif  // PARSER_TEST_BASE_H_