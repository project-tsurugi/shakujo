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
#include "shakujo/analyzer/Diagnostic.h"

namespace shakujo::analyzer {

Diagnostic::Severity Diagnostic::severity() const {
    using severity_t = std::underlying_type_t<Severity>;
    using code_t = std::underlying_type_t<Code>;
    static_assert(
            std::is_same<severity_t, code_t>::value,
            "underlying type of Severity and Code must be same"
    );
    auto v = static_cast<code_t>(code_);
    if (v >= static_cast<severity_t>(Severity::CRITICAL)) {
        return Severity::CRITICAL;
    }
    if (v >= static_cast<severity_t>(Severity::ERROR)) {
        return Severity::ERROR;
    }
    if (v >= static_cast<severity_t>(Severity::WARNING)) {
        return Severity::WARNING;
    }
    return Severity::INFORMATION;
}

std::ostream& operator<<(std::ostream& out, const Diagnostic& value) {
    out << "[" << value.severity() << ": " << value.code() << "]"
        << " " << value.message();
    if (value.region()) {
        out << " (" << value.region() << ")";
    }
    return out;
}
}  // namespace shakujo::analyzer
