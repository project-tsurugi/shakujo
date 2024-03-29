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
#include "shakujo/model/type/ArrayType.h"

#include <cstddef>
#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/type/Type.h"

namespace shakujo::model::type {

class ArrayType::Impl {
public:
    std::unique_ptr<Type> element_type_;
    std::size_t size_ { static_cast<std::size_t>(-1) };

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->element_type_ = common::util::make_clone(element_type_);
        other->size_ = size_;
        return other;
    }
};

ArrayType::ArrayType(std::unique_ptr<ArrayType::Impl> impl) noexcept : impl_(std::move(impl)) {}

ArrayType::ArrayType() : ArrayType(std::make_unique<ArrayType::Impl>()) {}

ArrayType::~ArrayType() noexcept = default;

ArrayType::ArrayType(ArrayType&&) noexcept = default;

ArrayType& ArrayType::operator=(ArrayType&&) noexcept = default;

Type* ArrayType::element_type() {
    return impl_->element_type_.get();
}

ArrayType& ArrayType::element_type(std::unique_ptr<Type> element_type) {
    impl_->element_type_ = std::move(element_type);
    return *this;
}

std::unique_ptr<Type> ArrayType::release_element_type() {
    std::unique_ptr<Type> ret { std::move(impl_->element_type_) };
    impl_->element_type_ = {};
    return ret;
}

std::size_t ArrayType::size() const {
    return impl_->size_;
}

ArrayType& ArrayType::size(std::size_t size) {
    impl_->size_ = size;
    return *this;
}

ArrayType* ArrayType::clone() const & {
    return new ArrayType(impl_->clone());  // NOLINT
}

ArrayType* ArrayType::clone() && {
    return new ArrayType(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::type

