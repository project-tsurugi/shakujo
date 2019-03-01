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
#include "VariableRewriter.h"

#include <stdexcept>

#include "shakujo/model/util/NodeWalker.h"

namespace shakujo::analyzer::optimize {

std::vector<model::expression::VariableReference*> VariableRewriter::collect(model::expression::Expression* node) {
    class Engine : public model::util::NodeWalker {
    public:
        bool enter(model::expression::VariableReference* node) override {
            results_.push_back(node);
            return true;
        }
        std::vector<model::expression::VariableReference*> results_ {};
    } engine {};
    engine.walk(node);
    return std::move(engine.results_);
}

std::shared_ptr<binding::VariableBinding> VariableRewriter::apply(
        std::shared_ptr<binding::VariableBinding> const& variable) {
    if (auto it = mapping_.find(variable); it != mapping_.end()) {
        return it->second;
    }
    return variable;
}

void VariableRewriter::add_rule(
        std::shared_ptr<binding::VariableBinding> from,
        std::shared_ptr<binding::VariableBinding> to) {
    if (!from->is_valid()) {
        throw std::invalid_argument("from must be valid");
    }
    mapping_.emplace(std::move(from), std::move(to));
}

void VariableRewriter::add_rule(
        std::vector<std::shared_ptr<binding::VariableBinding>> const &from,
        std::vector<std::shared_ptr<binding::VariableBinding>> const &to) {
    if (from.size() != to.size()) {
        throw std::invalid_argument("inconsistent mapping");
    }
    for (std::size_t i = 0, n = from.size(); i < n; ++i) {
        add_rule(from[i], to[i]);
    }
}

void VariableRewriter::deny(std::shared_ptr<binding::VariableBinding> from) {
    add_rule(std::move(from), std::make_shared<binding::VariableBinding>());
}

void VariableRewriter::deny(std::vector<std::shared_ptr<binding::VariableBinding>> const& from) {
    for (auto&& entry : from) {
        deny(entry);
    }
}

VariableRewriter& VariableRewriter::merge(VariableRewriter const& other) {
    for (auto&& [k, v] : mapping_) {
        (void) k;  // NOTE: avoid compile error on GCC
        if (auto it = other.mapping_.find(v); it != other.mapping_.end()) {
            v = it->second;
        } else {
            v = std::make_shared<binding::VariableBinding>();
        }
    }
    for (auto&& [k, v] : other.mapping_) {
        if (auto it = mapping_.find(k); it == mapping_.end()) {
            mapping_.emplace(k, v);
        }
    }
    return *this;
}

}  // namespace shakujo::analyzer::optimize
