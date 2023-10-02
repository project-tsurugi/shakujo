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
#include "shakujo/model/name/QualifiedName.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"

namespace shakujo::model::name {

class QualifiedName::Impl {
public:
    std::unique_ptr<Name> qualifier_;
    std::unique_ptr<SimpleName> name_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->qualifier_ = common::util::make_clone(qualifier_);
        other->name_ = common::util::make_clone(name_);
        return other;
    }
};

QualifiedName::QualifiedName(std::unique_ptr<QualifiedName::Impl> impl) noexcept : impl_(std::move(impl)) {}

QualifiedName::QualifiedName() : QualifiedName(std::make_unique<QualifiedName::Impl>()) {}

QualifiedName::~QualifiedName() noexcept = default;

QualifiedName::QualifiedName(QualifiedName&&) noexcept = default;

QualifiedName& QualifiedName::operator=(QualifiedName&&) noexcept = default;

Name* QualifiedName::qualifier() {
    return impl_->qualifier_.get();
}

QualifiedName& QualifiedName::qualifier(std::unique_ptr<Name> qualifier) {
    impl_->qualifier_ = std::move(qualifier);
    return *this;
}

std::unique_ptr<Name> QualifiedName::release_qualifier() {
    std::unique_ptr<Name> ret { std::move(impl_->qualifier_) };
    impl_->qualifier_ = {};
    return ret;
}

SimpleName* QualifiedName::name() {
    return impl_->name_.get();
}

QualifiedName& QualifiedName::name(std::unique_ptr<SimpleName> name) {
    impl_->name_ = std::move(name);
    return *this;
}

std::unique_ptr<SimpleName> QualifiedName::release_name() {
    std::unique_ptr<SimpleName> ret { std::move(impl_->name_) };
    impl_->name_ = {};
    return ret;
}

QualifiedName* QualifiedName::clone() const & {
    return new QualifiedName(impl_->clone());  // NOLINT
}

QualifiedName* QualifiedName::clone() && {
    return new QualifiedName(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::name

