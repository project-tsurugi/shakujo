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
#include "shakujo/analyzer/binding/RelationBinding.h"

namespace shakujo::analyzer::binding {

static const std::any EMPTY_ANY;  // NOLINT

class RelationBinding::Impl {
public:
    std::vector<std::shared_ptr<VariableBinding>> columns_;
    std::map<std::string, std::any> attributes_;
    Impl(std::vector<std::shared_ptr<VariableBinding>> columns)
        : columns_(std::move(columns))
    {}
};

RelationBinding::RelationBinding(std::vector<std::shared_ptr<VariableBinding>> columns)
    : impl_(new Impl(std::move(columns)))
{}

RelationBinding::~RelationBinding() noexcept = default;

std::vector<std::shared_ptr<VariableBinding>>& RelationBinding::columns() {
    return impl_->columns_;
}

std::map<std::string, std::any>& RelationBinding::attributes() {
    return impl_->attributes_;
}

std::any const& RelationBinding::find_attribute(std::string const& key) const {
    if (auto it = attributes().find(key); it != attributes().end()) {
        return it->second;
    }
    return EMPTY_ANY;
}
}  // namespace shakujo::analyzer::binding
