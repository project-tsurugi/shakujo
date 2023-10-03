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
#include "shakujo/model/util/PlaceholderResolver.h"

#include <cassert>
#include <stdexcept>

#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::model::util {

using common::util::is_defined;
using common::util::make_clone;
using common::util::dynamic_pointer_cast;
using common::util::dynamic_pointer_cast_if;

PlaceholderResolver::Result PlaceholderResolver::operator()(Node *node) const {
    auto placeholders = collect(node);
    std::vector<std::unique_ptr<expression::Placeholder>> replaced;
    std::vector<expression::Placeholder*> left;
    for (auto* ph : placeholders) {
        auto origin = replace(ph);
        if (is_defined(origin)) {
            replaced.emplace_back(std::move(origin));
        } else {
            left.emplace_back(ph);
        }
    }
    return { std::move(replaced), std::move(left) };
}

std::unique_ptr<expression::Placeholder> PlaceholderResolver::replace(expression::Placeholder* node) const {
    assert(node->is_managed());  // NOLINT
    auto& name = node->name();
    if (name.empty()) {
        // FIXME: positional placeholder
        return {};
    }
    if (auto it = keywords_.find(name); it != keywords_.end()) {
        std::unique_ptr<expression::Placeholder> result;
        node->replace([&](auto origin) {
            result = dynamic_pointer_cast<expression::Placeholder>(std::move(origin));
            auto replacement = make_clone(it->second);
            if (!replacement->region() && result->region()) {
                replacement->region(result->region());
            }
            return replacement;
        });
        return result;
    }
    return {};
}

std::vector<expression::Placeholder *> PlaceholderResolver::collect(Node *node) {
    if (auto ph = dynamic_pointer_cast_if<expression::Placeholder>(node); is_defined(ph) && !ph->is_managed()) {
        throw std::invalid_argument("replacement target placeholder must be managed");
    }
    class Collector : public NodeWalker {
    public:
        std::vector<expression::Placeholder*> results {};

        using NodeWalker::enter;

        bool enter(expression::Placeholder* node) override {
            results.emplace_back(node);
            return false;
        }
    } collector;
    collector.walk(node);
    return std::move(collector.results);
}

}  // namespace shakujo::model::util
