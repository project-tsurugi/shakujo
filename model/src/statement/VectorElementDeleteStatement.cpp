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
#include "shakujo/model/statement/VectorElementDeleteStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::model::statement {

class VectorElementDeleteStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> data_;
    common::util::ManagedPtr<expression::Expression> begin_;
    common::util::ManagedPtr<expression::Expression> end_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->data_ = common::util::make_clone(data_);
        other->begin_ = common::util::make_clone(begin_);
        other->end_ = common::util::make_clone(end_);
        return other;
    }
};

VectorElementDeleteStatement::VectorElementDeleteStatement(std::unique_ptr<VectorElementDeleteStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

VectorElementDeleteStatement::VectorElementDeleteStatement() : VectorElementDeleteStatement(std::make_unique<VectorElementDeleteStatement::Impl>()) {}

VectorElementDeleteStatement::~VectorElementDeleteStatement() noexcept = default;

VectorElementDeleteStatement::VectorElementDeleteStatement(VectorElementDeleteStatement&&) noexcept = default;

VectorElementDeleteStatement& VectorElementDeleteStatement::operator=(VectorElementDeleteStatement&&) noexcept = default;

expression::Expression* VectorElementDeleteStatement::data() {
    return impl_->data_.get();
}

VectorElementDeleteStatement& VectorElementDeleteStatement::data(std::unique_ptr<expression::Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<expression::Expression> VectorElementDeleteStatement::release_data() {
    return impl_->data_.release();
}

expression::Expression* VectorElementDeleteStatement::begin() {
    return impl_->begin_.get();
}

VectorElementDeleteStatement& VectorElementDeleteStatement::begin(std::unique_ptr<expression::Expression> begin) {
    impl_->begin_ = std::move(begin);
    return *this;
}

std::unique_ptr<expression::Expression> VectorElementDeleteStatement::release_begin() {
    return impl_->begin_.release();
}

expression::Expression* VectorElementDeleteStatement::end() {
    return impl_->end_.get();
}

VectorElementDeleteStatement& VectorElementDeleteStatement::end(std::unique_ptr<expression::Expression> end) {
    impl_->end_ = std::move(end);
    return *this;
}

std::unique_ptr<expression::Expression> VectorElementDeleteStatement::release_end() {
    return impl_->end_.release();
}

VectorElementDeleteStatement* VectorElementDeleteStatement::clone() const & {
    return new VectorElementDeleteStatement(impl_->clone());  // NOLINT
}

VectorElementDeleteStatement* VectorElementDeleteStatement::clone() && {
    return new VectorElementDeleteStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

