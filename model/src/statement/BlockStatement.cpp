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
#include "shakujo/model/statement/BlockStatement.h"

#include "shakujo/common/util/utility.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/util/NodeList.h"

namespace shakujo::model::statement {

class BlockStatement::Impl {
public:
    util::NodeList<Statement> elements_;

    Impl() = default;
    ~Impl() noexcept = default;
    Impl(Impl const&) = delete;
    Impl& operator=(Impl const&) = delete;
    Impl(Impl&&) = delete;
    Impl& operator=(Impl&&) = delete;

    std::unique_ptr<Impl> clone() const {
        auto other = std::make_unique<Impl>();
        if (!elements_.empty()) {
            other->elements_.reserve(elements_.size());
            for (auto e : elements_) {
                other->elements_.push_back(common::util::make_clone(e));
            }
        }
        return other;
    }
};

BlockStatement::BlockStatement(std::unique_ptr<BlockStatement::Impl> impl) noexcept : impl_(std::move(impl)) {}

BlockStatement::BlockStatement() : BlockStatement(std::make_unique<BlockStatement::Impl>()) {}

BlockStatement::~BlockStatement() noexcept = default;

BlockStatement::BlockStatement(BlockStatement&&) noexcept = default;

BlockStatement& BlockStatement::operator=(BlockStatement&&) noexcept = default;

util::NodeList<Statement>& BlockStatement::elements() {
    return impl_->elements_;
}

BlockStatement* BlockStatement::clone() const & {
    return new BlockStatement(impl_->clone());  // NOLINT
}

BlockStatement* BlockStatement::clone() && {
    return new BlockStatement(std::move(impl_));  // NOLINT
}

}  // namespace shakujo::model::statement

