/*
 * Copyright 2018-2019 shakujo project.
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
#include "shakujo/model/program/Comment.h"

#include <utility>
#include <string>


namespace shakujo::model::program {

class Comment::Impl {
public:
    std::string contents_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        other->contents_ = contents_;
        return other;
    }
};

Comment::Comment(std::unique_ptr<Comment::Impl> impl) noexcept : impl_(std::move(impl)) {}

Comment::Comment() : Comment(std::make_unique<Comment::Impl>()) {}

Comment::~Comment() noexcept = default;

Comment::Comment(Comment&&) noexcept = default;

Comment& Comment::operator=(Comment&&) noexcept = default;

std::string const& Comment::contents() const {
    return impl_->contents_;
}

Comment& Comment::contents(std::string contents) {
    impl_->contents_ = std::move(contents);
    return *this;
}

Comment* Comment::clone() const & {
    return new Comment(impl_->clone());  // NOLINT
}

Comment* Comment::clone() && {
    return new Comment(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::program

