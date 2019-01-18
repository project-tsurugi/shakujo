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
#include "shakujo/model/name/Index.h"

#include <cstddef>
#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/name/SimpleName.h"

namespace shakujo::model::name {

class Index::Impl {
public:
    std::unique_ptr<SimpleName> name_;
    std::size_t position_ { static_cast<std::size_t>(-1) };

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = common::util::make_clone(name_);
        other->position_ = position_;
        return other;
    }
};

Index::Index(std::unique_ptr<Index::Impl> impl) noexcept : impl_(std::move(impl)) {}

Index::Index() : Index(std::make_unique<Index::Impl>()) {}

Index::~Index() noexcept = default;

Index::Index(Index&&) noexcept = default;

Index& Index::operator=(Index&&) noexcept = default;

SimpleName* Index::name() {
    return impl_->name_.get();
}

Index& Index::name(std::unique_ptr<SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<SimpleName> Index::release_name() {
    std::unique_ptr<SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

std::size_t Index::position() const {
    return impl_->position_;
}

Index& Index::position(std::size_t position) {
    impl_->position_ = position;
    return *this;
}

Index* Index::clone() const & {
    return new Index(impl_->clone());  // NOLINT
}

Index* Index::clone() && {
    return new Index(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::name

