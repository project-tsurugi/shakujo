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
#include "shakujo/common/core/DocumentRegion.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace shakujo::common::core {

static const std::string EMPTY_STRING;  // NOLINT

const std::string& DocumentRegion::path() const {
    if (path_) {
        return *path_;
    }
    return EMPTY_STRING;
}

bool DocumentRegion::operator==(DocumentRegion const& other) const {
    return path_ && other.path_ && *path_ == *other.path_
        && begin_ == other.begin_ && end_ == other.end_;
}

std::ostream& operator<<(std::ostream& out, DocumentRegion const& value) {
    if (value.path().empty()) {
        out << "(region not available)";
    } else {
        out << value.path();
        if (value.begin()) {
            out << ":" << value.begin();
            if (value.end()) {
                out << "-" << value.end();
            }
        }
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, DocumentRegion::Position const& value) {
    if (!value) {
        out << "?";
    } else {
        out << value.line_number();
        if (value.column_number() != 0U) {
            out << ":" << value.column_number();
        }
    }
    return out;
}

}  // namespace shakujo::common::core

