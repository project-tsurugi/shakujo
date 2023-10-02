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
#include "shakujo/model/statement/AnchorDeclaration.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/statement/Statement.h"

namespace shakujo::model::statement {

class AnchorDeclaration::Impl {
public:
    std::unique_ptr<name::Name> name_;
    std::unique_ptr<Statement> body_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->name_ = common::util::make_clone(name_);
        other->body_ = common::util::make_clone(body_);
        return other;
    }
};

AnchorDeclaration::AnchorDeclaration(std::unique_ptr<AnchorDeclaration::Impl> impl) noexcept : impl_(std::move(impl)) {}

AnchorDeclaration::AnchorDeclaration() : AnchorDeclaration(std::make_unique<AnchorDeclaration::Impl>()) {}

AnchorDeclaration::~AnchorDeclaration() noexcept = default;

AnchorDeclaration::AnchorDeclaration(AnchorDeclaration&&) noexcept = default;

AnchorDeclaration& AnchorDeclaration::operator=(AnchorDeclaration&&) noexcept = default;

name::Name* AnchorDeclaration::name() {
    return impl_->name_.get();
}

AnchorDeclaration& AnchorDeclaration::name(std::unique_ptr<name::Name> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<name::Name> AnchorDeclaration::release_name() {
    std::unique_ptr<name::Name> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

Statement* AnchorDeclaration::body() {
    return impl_->body_.get();
}

AnchorDeclaration& AnchorDeclaration::body(std::unique_ptr<Statement> body) {
    impl_->body_ = std::move(body);
    return *this;
}

std::unique_ptr<Statement> AnchorDeclaration::release_body() {
    std::unique_ptr<Statement> ret { std::move(impl_->body_) };
    impl_->body_ = {};
    return ret;
}

AnchorDeclaration* AnchorDeclaration::clone() const & {
    return new AnchorDeclaration(impl_->clone());  // NOLINT
}

AnchorDeclaration* AnchorDeclaration::clone() && {
    return new AnchorDeclaration(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

