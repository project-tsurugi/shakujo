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
// DON'T EDIT THIS GENERATED FILE //
#include "shakujo/model/type/RelationType.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/type/TupleType.h"

namespace shakujo::model::type {

class RelationType::Impl {
public:
    std::unique_ptr<TupleType> element_type_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->element_type_ = common::util::make_clone(element_type_);
        return other;
    }
};

RelationType::RelationType(std::unique_ptr<RelationType::Impl> impl) noexcept : impl_(std::move(impl)) {}

RelationType::RelationType() : RelationType(std::make_unique<RelationType::Impl>()) {}

RelationType::~RelationType() noexcept = default;

RelationType::RelationType(RelationType&&) noexcept = default;

RelationType& RelationType::operator=(RelationType&&) noexcept = default;

TupleType* RelationType::element_type() {
    return impl_->element_type_.get();
}

RelationType& RelationType::element_type(std::unique_ptr<TupleType> element_type) {
    impl_->element_type_ = std::move(element_type);
    return *this;
}

std::unique_ptr<TupleType> RelationType::release_element_type() {
    std::unique_ptr<TupleType> ret { std::move(impl_->element_type_) };
    impl_->element_type_ = {};
    return ret;
}

RelationType* RelationType::clone() const & {
    return new RelationType(impl_->clone());  // NOLINT
}

RelationType* RelationType::clone() && {
    return new RelationType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

