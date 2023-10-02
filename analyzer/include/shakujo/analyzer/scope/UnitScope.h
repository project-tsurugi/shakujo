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
#ifndef SHAKUJO_ANALYZER_SCOPE_UNIT_SCOPE_H_
#define SHAKUJO_ANALYZER_SCOPE_UNIT_SCOPE_H_

#include <memory>
#include <sstream>

#include "Scope.h"
#include "Result.h"
#include "../Reporter.h"
#include "shakujo/model/name/Name.h"

namespace shakujo::analyzer::scope {

/**
 * @brief a scope where declarations are restricted.
 * @tparam T the binding kind
 */
template<typename T>
class UnitScope final : public Scope<T> {
public:
    /**
     * @brief the element type.
     */
    using element_type = typename Scope<T>::element_type;

private:
    Reporter* reporter_;
    Scope<element_type> const* parent_;
    Diagnostic::Code error_code_;

public:
    /**
     * @brief constructs a new object.
     * @param reporter sink of diagnostics
     * @param parent the pointer of parent scope
     * @param error_code the error code
     */
    explicit UnitScope(
            Reporter* reporter,
            Scope<element_type> const* parent = nullptr,
            Diagnostic::Code error_code = Diagnostic::Code::UNKNOWN_ERROR)
        : reporter_(reporter)
        , parent_(parent)
        , error_code_(error_code)
    {}

    /**
     * @brief destroys this object.
     */
    ~UnitScope() override = default;

    /**
     * @brief constructs a new object.
     * @param other the copy source
     */
    UnitScope(UnitScope const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    UnitScope(UnitScope&& other) noexcept = default;

    /**
     * @brief assigns the given object into this.
     * @param other the copy source
     * @return this
     */
    UnitScope& operator=(UnitScope const& other) = delete;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    UnitScope& operator=(UnitScope&& other) noexcept = default;

    Result<T> find(model::name::Name const* name) const override {
        if (parent_) {
            return parent_->find(name);
        }
        return {};
    }

    /**
     * @brief just report an error.
     * @param name the element name
     */
    void insert(model::name::Name const* name, std::shared_ptr<T>) override {
        std::ostringstream ss;
        ss << name;
        reporter_->report(Diagnostic {
            name->region(),
            error_code_,
            ss.str(),
        });
    }
};
}  // namespace shakujo::analyzer::scope

#endif  //SHAKUJO_ANALYZER_SCOPE_UNIT_SCOPE_H_
