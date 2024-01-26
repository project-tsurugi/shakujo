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
#include "shakujo/model/statement/VectorElementInsertStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/util/ManagedNodeList.h"

namespace shakujo::model::statement {

class VectorElementInsertStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> data_;
    common::util::ManagedPtr<expression::Expression> index_;
    util::ManagedNodeList<expression::Expression> values_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->data_ = common::util::make_clone(data_);
        other->index_ = common::util::make_clone(index_);
        if (!values_.empty()) {
            other->values_.reserve(values_.size());
            for (auto e : values_) {
                other->values_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

VectorElementInsertStatement::VectorElementInsertStatement(std::unique_ptr<VectorElementInsertStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

VectorElementInsertStatement::VectorElementInsertStatement() : VectorElementInsertStatement(std::make_unique<VectorElementInsertStatement::Impl>()) {}

VectorElementInsertStatement::~VectorElementInsertStatement() noexcept = default;

VectorElementInsertStatement::VectorElementInsertStatement(VectorElementInsertStatement&&) noexcept = default;

VectorElementInsertStatement& VectorElementInsertStatement::operator=(VectorElementInsertStatement&&) noexcept = default;

expression::Expression* VectorElementInsertStatement::data() {
    return impl_->data_.get();
}

VectorElementInsertStatement& VectorElementInsertStatement::data(std::unique_ptr<expression::Expression> data) {
    impl_->data_ = std::move(data);
    return *this;
}

std::unique_ptr<expression::Expression> VectorElementInsertStatement::release_data() {
    return impl_->data_.release();
}

expression::Expression* VectorElementInsertStatement::index() {
    return impl_->index_.get();
}

VectorElementInsertStatement& VectorElementInsertStatement::index(std::unique_ptr<expression::Expression> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<expression::Expression> VectorElementInsertStatement::release_index() {
    return impl_->index_.release();
}

util::ManagedNodeList<expression::Expression>& VectorElementInsertStatement::values() {
    return impl_->values_;
}

VectorElementInsertStatement* VectorElementInsertStatement::clone() const & {
    return new VectorElementInsertStatement(impl_->clone());  // NOLINT
}

VectorElementInsertStatement* VectorElementInsertStatement::clone() && {
    return new VectorElementInsertStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

