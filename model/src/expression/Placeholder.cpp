/*
 * Copyright 2018-2019 shakujo project.
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
// DON'T EDIT THIS GENERATED FILE //
#include "shakujo/model/expression/Placeholder.h"

#include <utility>
#include <memory>
#include <string>

#include "shakujo/model/key/ExpressionKey.h"

namespace shakujo::model::expression {

class Placeholder::Impl {
public:
    std::string name_;
    std::unique_ptr<key::ExpressionKey> expression_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = name_;
        return other;
    }
};

Placeholder::Placeholder(std::unique_ptr<Placeholder::Impl> impl) noexcept : impl_(std::move(impl)) {}

Placeholder::Placeholder() : Placeholder(std::make_unique<Placeholder::Impl>()) {}

Placeholder::~Placeholder() noexcept = default;

Placeholder::Placeholder(Placeholder&&) noexcept = default;

Placeholder& Placeholder::operator=(Placeholder&&) noexcept = default;

std::string const& Placeholder::name() const {
    return impl_->name_;
}

Placeholder& Placeholder::name(std::string name) {
    impl_->name_ = std::move(name);
    return *this;
}

key::ExpressionKey* Placeholder::expression_key() {
    return impl_->expression_key_.get();
}

Placeholder& Placeholder::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

Placeholder* Placeholder::clone() const & {
    return new Placeholder(impl_->clone());  // NOLINT
}

Placeholder* Placeholder::clone() && {
    return new Placeholder(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression

