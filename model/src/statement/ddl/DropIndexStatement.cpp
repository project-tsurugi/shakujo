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
#include "shakujo/model/statement/ddl/DropIndexStatement.h"

#include <utility>
#include <memory>
#include <set>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/key/RelationKey.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::model::statement::ddl {

class DropIndexStatement::Impl {
public:
    std::unique_ptr<name::Name> index_;
    std::set<DropIndexStatement::Attribute> attributes_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->index_ = common::util::make_clone(index_);
        other->attributes_ = attributes_;
        return other;
    }
};

DropIndexStatement::DropIndexStatement(std::unique_ptr<DropIndexStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

DropIndexStatement::DropIndexStatement() : DropIndexStatement(std::make_unique<DropIndexStatement::Impl>()) {}

DropIndexStatement::~DropIndexStatement() noexcept = default;

DropIndexStatement::DropIndexStatement(DropIndexStatement&&) noexcept = default;

DropIndexStatement& DropIndexStatement::operator=(DropIndexStatement&&) noexcept = default;

name::Name* DropIndexStatement::index() {
    return impl_->index_.get();
}

DropIndexStatement& DropIndexStatement::index(std::unique_ptr<name::Name> index) {
    impl_->index_ = std::move(index);
    return *this;
}

std::unique_ptr<name::Name> DropIndexStatement::release_index() {
    std::unique_ptr<name::Name> ret { std::move(impl_->index_) };
    impl_->index_ = {};
    return ret;
}

std::set<DropIndexStatement::Attribute>& DropIndexStatement::attributes() {
    return impl_->attributes_;
}

key::RelationKey* DropIndexStatement::relation_key() {
    return impl_->relation_key_.get();
}

DropIndexStatement& DropIndexStatement::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

DropIndexStatement* DropIndexStatement::clone() const & {
    return new DropIndexStatement(impl_->clone());  // NOLINT
}

DropIndexStatement* DropIndexStatement::clone() && {
    return new DropIndexStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::ddl

