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
#ifndef SYNTAX_VALIDATOR_TEST_BASE_H_
#define SYNTAX_VALIDATOR_TEST_BASE_H_

#include "shakujo/analyzer/SyntaxValidator.h"

#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include "shakujo/model/IRFactory.h"
#include "shakujo/common/util/JsonSerializer.h"
#include "shakujo/model/util/NodeSerializer.h"

namespace shakujo::analyzer {

static const common::core::Type::Nullity NULLABLE = common::core::Type::Nullity::NULLABLE;
static const common::core::Type::Nullity NON_NULL = common::core::Type::Nullity::NEVER_NULL;

class SyntaxValidatorTestBase {
public:
    shakujo::model::IRFactory f;

    std::string diagnostics(Reporter const& reporter) {
        std::ostringstream ss;
        ss << "{";
        bool cont = false;
        for (auto& e : reporter.diagnostics()) {
            if (cont) ss << ", ";
            cont = true;
            ss << e;
        }
        ss << "}";
        return ss.str();
    }

    template<typename T>
    void analyze(Reporter& reporter, T const* node) {
        reporter.report(Diagnostic { Diagnostic::Code::MESSAGE, to_string(node) });
        SyntaxValidator validator;
        validator.analyze(reporter, node);
    }

    template<typename T>
    std::unique_ptr<T> edit(std::unique_ptr<T> ptr, std::function<void(T&)> editor) {
        editor(*ptr);
        return std::move(ptr);
    }

    template<typename T>
    std::string to_string(const T& value) {
        std::ostringstream ss;
        common::util::JsonSerializer json { ss };
        model::util::NodeSerializer serializer;
        serializer.show_undefined_value(true);
        serializer.serialize(json, value);
        return ss.str();
    }
};
}  // namespace shakujo::analyzer

#endif  // SYNTAX_VALIDATOR_TEST_BASE_H_