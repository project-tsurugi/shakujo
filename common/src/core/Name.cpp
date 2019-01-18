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
#include "shakujo/common/core/Name.h"

#include <memory>

namespace shakujo::common::core {

Name::Name(std::string_view name) {
    segments_.reserve(1);
    segments_.emplace_back(name);
}

int Name::compare(Name const& other) const {
    auto& a = segments_;
    auto& b = other.segments_;
    for (std::size_t i = 0, n = std::min(a.size(), b.size()); i < n; i++) {
        int cmp = a[i].compare(b[i]);
        if (cmp != 0) {
            return cmp;
        }
    }
    if (a.size() == b.size()) {
        return 0;
    }
    if (a.size() < b.size()) {
        return -1;
    }
    return +1;
}

std::ostream& operator<<(std::ostream& out, Name const& value) {
    bool cont = false;
    for (auto& s : value.segments()) {
        if (cont) out << "::";
        out << s;
        cont = true;
    }
    return out;
}

}  // namespace shakujo::common::core

