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
#include "shakujo/analyzer/Diagnostic.h"

#include <gtest/gtest.h>
#include <sstream>
#include <type_traits>

namespace shakujo::analyzer {

using namespace shakujo::analyzer;
using Code = Diagnostic::Code;
using Severity = Diagnostic::Severity;

using Region = shakujo::common::core::DocumentRegion;

class DiagnosticTest : public ::testing::Test {
public:
    void output_test(const Diagnostic& v) {
        std::ostringstream ss;
        ss << v;
        auto str = ss.str();
        EXPECT_FALSE(str.empty()) << str;
    }
};

TEST_F(DiagnosticTest, simple) {
    Diagnostic d { Code::UNKNOWN_INFORMATION, "OK" };

    EXPECT_FALSE(d.region());
    EXPECT_EQ(Severity::INFORMATION, d.severity());
    EXPECT_EQ(Code::UNKNOWN_INFORMATION, d.code());
    EXPECT_EQ("OK", d.message());
}

TEST_F(DiagnosticTest, severity_info) {
    Diagnostic d { Code::UNKNOWN_INFORMATION, "OK" };
    EXPECT_EQ(Severity::INFORMATION, d.severity());
    output_test(d);
}

TEST_F(DiagnosticTest, severity_warn) {
    Diagnostic d { Code::UNKNOWN_WARNING, "OK" };
    EXPECT_EQ(Severity::WARNING, d.severity());
    output_test(d);
}

TEST_F(DiagnosticTest, severity_error) {
    Diagnostic d { Code::UNKNOWN_ERROR, "OK" };
    EXPECT_EQ(Severity::ERROR, d.severity());
    output_test(d);
}

TEST_F(DiagnosticTest, severity_crit) {
    Diagnostic d { Code::UNKNOWN_CRITICAL, "OK" };
    EXPECT_EQ(Severity::CRITICAL, d.severity());
    output_test(d);
}

TEST_F(DiagnosticTest, region) {
    Diagnostic d { Region("testing"), Code::UNKNOWN_INFORMATION, "OK" };

    EXPECT_EQ("testing", d.region().path());
    EXPECT_EQ(Severity::INFORMATION, d.severity());
    EXPECT_EQ(Code::UNKNOWN_INFORMATION, d.code());
    EXPECT_EQ("OK", d.message());
}
}  // namespace shakujo::analyzer
