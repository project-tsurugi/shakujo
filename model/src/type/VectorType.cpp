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
// DON'T EDIT THIS GENERATED FILE //
#include "shakujo/model/type/VectorType.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::type {

class VectorType::Impl {
public:
    std::unique_ptr<Type> element_type_;

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

VectorType::VectorType(std::unique_ptr<VectorType::Impl> impl) noexcept : impl_(std::move(impl)) {}

VectorType::VectorType() : VectorType(std::make_unique<VectorType::Impl>()) {}

VectorType::~VectorType() noexcept = default;

VectorType::VectorType(VectorType&&) noexcept = default;

VectorType& VectorType::operator=(VectorType&&) noexcept = default;

Type* VectorType::element_type() {
    return impl_->element_type_.get();
}

VectorType& VectorType::element_type(std::unique_ptr<Type> element_type) {
    impl_->element_type_ = std::move(element_type);
    return *this;
}

std::unique_ptr<Type> VectorType::release_element_type() {
    std::unique_ptr<Type> ret { std::move(impl_->element_type_) };
    impl_->element_type_ = {};
    return ret;
}

VectorType* VectorType::clone() const & {
    return new VectorType(impl_->clone());  // NOLINT
}

VectorType* VectorType::clone() && {
    return new VectorType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

