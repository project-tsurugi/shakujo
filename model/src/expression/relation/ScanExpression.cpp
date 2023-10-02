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
#include "shakujo/model/expression/relation/ScanExpression.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/key/ExpressionKey.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::expression::relation {

class ScanExpression::Impl {
public:
    std::unique_ptr<name::Name> table_;
    std::unique_ptr<key::ExpressionKey> expression_key_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->table_ = common::util::make_clone(table_);
        return other;
    }
};

ScanExpression::ScanExpression(std::unique_ptr<ScanExpression::Impl> impl) noexcept : impl_(std::move(impl)) {}

ScanExpression::ScanExpression() : ScanExpression(std::make_unique<ScanExpression::Impl>()) {}

ScanExpression::~ScanExpression() noexcept = default;

ScanExpression::ScanExpression(ScanExpression&&) noexcept = default;

ScanExpression& ScanExpression::operator=(ScanExpression&&) noexcept = default;

name::Name* ScanExpression::table() {
    return impl_->table_.get();
}

ScanExpression& ScanExpression::table(std::unique_ptr<name::Name> table) {
    impl_->table_ = std::move(table);
    return *this;
}

std::unique_ptr<name::Name> ScanExpression::release_table() {
    std::unique_ptr<name::Name> ret { std::move(impl_->table_) };
    impl_->table_ = {};
    return ret;
}

key::ExpressionKey* ScanExpression::expression_key() {
    return impl_->expression_key_.get();
}

ScanExpression& ScanExpression::expression_key(std::unique_ptr<key::ExpressionKey> expression_key) {
    impl_->expression_key_ = std::move(expression_key);
    return *this;
}

key::RelationKey* ScanExpression::relation_key() {
    return impl_->relation_key_.get();
}

ScanExpression& ScanExpression::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

ScanExpression* ScanExpression::clone() const & {
    return new ScanExpression(impl_->clone());  // NOLINT
}

ScanExpression* ScanExpression::clone() && {
    return new ScanExpression(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::expression::relation

