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
// DON'T EDIT THIS GENERATED FILE //
#include "shakujo/model/type/TupleType.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/model/util/FragmentList.h"

namespace shakujo::model::type {

class TupleType::Impl {
public:
    util::FragmentList<TupleType::Element> elements_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        if (!elements_.empty()) {
            other->elements_.reserve(elements_.size());
            for (auto e : elements_) {
                other->elements_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

class TupleType::Element::Impl {
public:
    std::unique_ptr<name::SimpleName> name_;
    std::unique_ptr<Type> type_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = common::util::make_clone(name_);
        other->type_ = common::util::make_clone(type_);
        return other;
    }
};

TupleType::TupleType(std::unique_ptr<TupleType::Impl> impl) noexcept : impl_(std::move(impl)) {}

TupleType::TupleType() : TupleType(std::make_unique<TupleType::Impl>()) {}

TupleType::~TupleType() noexcept = default;

TupleType::TupleType(TupleType&&) noexcept = default;

TupleType& TupleType::operator=(TupleType&&) noexcept = default;

util::FragmentList<TupleType::Element>& TupleType::elements() {
    return impl_->elements_;
}

TupleType* TupleType::clone() const & {
    return new TupleType(impl_->clone());  // NOLINT
}

TupleType* TupleType::clone() && {
    return new TupleType(std::move(impl_));  // NOLINT
}

TupleType::Element::Element(std::unique_ptr<TupleType::Element::Impl> impl) noexcept : impl_(std::move(impl)) {}

TupleType::Element::Element() : TupleType::Element(std::make_unique<TupleType::Element::Impl>()) {}

TupleType::Element::~Element() noexcept = default;

TupleType::Element::Element(TupleType::Element&&) noexcept = default;

TupleType::Element& TupleType::Element::operator=(TupleType::Element&&) noexcept = default;

name::SimpleName* TupleType::Element::name() {
    return impl_->name_.get();
}

TupleType::Element& TupleType::Element::name(std::unique_ptr<name::SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::SimpleName> TupleType::Element::release_name() {
    std::unique_ptr<name::SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

Type* TupleType::Element::type() {
    return impl_->type_.get();
}

TupleType::Element& TupleType::Element::type(std::unique_ptr<Type> type) {
    impl_->type_ = std::move(type);
    return *this;
}

std::unique_ptr<Type> TupleType::Element::release_type() {
    std::unique_ptr<Type> ret { std::move(impl_->type_) };
    impl_->type_ = {};
    return ret;
}

TupleType::Element* TupleType::Element::clone() const & {
    return new TupleType::Element(impl_->clone());  // NOLINT
}

TupleType::Element* TupleType::Element::clone() && {
    return new TupleType::Element(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

