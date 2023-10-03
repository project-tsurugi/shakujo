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
#include "shakujo/model/program/Program.h"

#include <utility>
#include <memory>

#include "shakujo/common/util/utility.h"
#include "shakujo/model/program/Comment.h"
#include "shakujo/model/program/GlobalDeclaration.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::program {

class Program::Impl {
public:
    util::NodeList<GlobalDeclaration> declarations_;
    std::unique_ptr<statement::Statement> main_;
    util::NodeList<Comment> comments_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        if (!declarations_.empty()) {
            other->declarations_.reserve(declarations_.size());
            for (auto e : declarations_) {
                other->declarations_.push_back(common::util::make_clone(e));
            }
        }
        other->main_ = common::util::make_clone(main_);
        if (!comments_.empty()) {
            other->comments_.reserve(comments_.size());
            for (auto e : comments_) {
                other->comments_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

Program::Program(std::unique_ptr<Program::Impl> impl) noexcept : impl_(std::move(impl)) {}

Program::Program() : Program(std::make_unique<Program::Impl>()) {}

Program::~Program() noexcept = default;

Program::Program(Program&&) noexcept = default;

Program& Program::operator=(Program&&) noexcept = default;

util::NodeList<GlobalDeclaration>& Program::declarations() {
    return impl_->declarations_;
}

statement::Statement* Program::main() {
    return impl_->main_.get();
}

Program& Program::main(std::unique_ptr<statement::Statement> main) {
    impl_->main_ = std::move(main);
    return *this;
}

std::unique_ptr<statement::Statement> Program::release_main() {
    std::unique_ptr<statement::Statement> ret { std::move(impl_->main_) };
    impl_->main_ = {};
    return ret;
}

util::NodeList<Comment>& Program::comments() {
    return impl_->comments_;
}

Program* Program::clone() const & {
    return new Program(impl_->clone());  // NOLINT
}

Program* Program::clone() && {
    return new Program(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::program

