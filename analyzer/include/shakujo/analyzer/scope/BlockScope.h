/*
 * Copyright 2018 shakujo project.
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
#ifndef SHAKUJO_ANALYZER_SCOPE_BLOCK_SCOPE_H_
#define SHAKUJO_ANALYZER_SCOPE_BLOCK_SCOPE_H_

#include <sstream>
#include <utility>
#include <type_traits>

#include "Scope.h"
#include "Result.h"
#include "Table.h"
#include "../Reporter.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::analyzer::scope {

/**
 * @brief a block Scope.
 * This object MUST dispose before the parent scope was disposed.
 * @tparam T binding kind
 */
template <typename T>
class BlockScope final : public Scope<T> {
public:
    /**
     * @brief the element type.
     */
    using element_type = typename Scope<T>::element_type;

private:
    Reporter* reporter_;
    Scope<T> const* parent_;
    Table<T> table_;

public:
    /**
     * @brief constructs a new object.
     * @param reporter sink of diagnostics
     * @param parent the pointer of parent scope
     */
    explicit BlockScope(
            Reporter* reporter,
            Scope<T> const* parent = nullptr)
        : reporter_(reporter)
        , parent_(parent)
    {}

    /**
     * @brief destroys this object.
     */
    ~BlockScope() override = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    BlockScope(BlockScope const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    BlockScope(BlockScope&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    BlockScope& operator=(BlockScope const& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    BlockScope& operator=(BlockScope&& other) noexcept = default;

    using Scope<T>::find;

    Result<T> find(model::name::Name const* name) const override {
        if (auto result = table_.find(name)) {
            return result;
        }
        if (parent_) {
            return parent_->find(name);
        }
        return {};
    }

    using Scope<T>::insert;

    /**
     * @brief inserts a new element into this scope.
     * @param name the element name
     * @param element the target element
     * @throw std::domain_error if not supported
     */
    void insert(model::name::Name const* name, std::shared_ptr<T> element) override {
        if (!table_.contains(name)) {
            table_.put(name->segments(), std::move(element), false);
        } else {
            reporter_->report(Diagnostic {
                name->region(),
                Diagnostic::Code::DUPLICATE_SYMBOL_IN_BLOCK,
                common::util::to_string(name),
            });
            table_.put(name->segments(), std::make_shared<T>(), true);
        }
    }
};
}  // namespace shakujo::analyzer::scope

#endif  //SHAKUJO_ANALYZER_SCOPE_BLOCK_SCOPE_H_
