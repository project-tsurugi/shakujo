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
#include "shakujo/model/statement/dml/EmitStatement.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/key/RelationKey.h"

namespace shakujo::model::statement::dml {

class EmitStatement::Impl {
public:
    common::util::ManagedPtr<expression::Expression> source_;
    std::unique_ptr<key::RelationKey> relation_key_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->source_ = common::util::make_clone(source_);
        return other;
    }
};

EmitStatement::EmitStatement(std::unique_ptr<EmitStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

EmitStatement::EmitStatement() : EmitStatement(std::make_unique<EmitStatement::Impl>()) {}

EmitStatement::~EmitStatement() noexcept = default;

EmitStatement::EmitStatement(EmitStatement&&) noexcept = default;

EmitStatement& EmitStatement::operator=(EmitStatement&&) noexcept = default;

expression::Expression* EmitStatement::source() {
    return impl_->source_.get();
}

EmitStatement& EmitStatement::source(std::unique_ptr<expression::Expression> source) {
    impl_->source_ = std::move(source);
    return *this;
}

std::unique_ptr<expression::Expression> EmitStatement::release_source() {
    return impl_->source_.release();
}

key::RelationKey* EmitStatement::relation_key() {
    return impl_->relation_key_.get();
}

EmitStatement& EmitStatement::relation_key(std::unique_ptr<key::RelationKey> relation_key) {
    impl_->relation_key_ = std::move(relation_key);
    return *this;
}

EmitStatement* EmitStatement::clone() const & {
    return new EmitStatement(impl_->clone());  // NOLINT
}

EmitStatement* EmitStatement::clone() && {
    return new EmitStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement::dml

