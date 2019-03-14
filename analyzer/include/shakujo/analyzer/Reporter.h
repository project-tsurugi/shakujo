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
#ifndef SHAKUJO_ANALYZER_REPORTER_H_
#define SHAKUJO_ANALYZER_REPORTER_H_

#include <utility>
#include <vector>

#include "Diagnostic.h"

namespace shakujo::analyzer {

/**
 * @brief a sink of diagnostics in semantic analysis phase.
 */
class Reporter {
private:
    std::vector<Diagnostic> diagnostics_;

public:
    /**
     * @brief constructs a new object.
     */
    Reporter() = default;

    /**
     * @brief destroys this object.
     */
    ~Reporter() noexcept = default;

    Reporter(Reporter const&) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Reporter(Reporter&& other) noexcept = default;

    Reporter& operator=(Reporter const&) = delete;

    /**
     * @brief assigns another object into this object.
     * @param other the move source
     * @return this
     */
    Reporter& operator=(Reporter&& other) noexcept = default;

    /**
     * @brief returns whether or not this has been saw any erroneous diagnostics.
     * @return true if
     * @return false otherwise
     * @see diagnostics()
     */
    bool saw_error() const {
        for (auto& d : diagnostics_) {
            if (d.severity() >= Diagnostic::Severity::ERROR) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief returns the all diagnostics.
     * @return the all diagnostics
     */
    std::vector<Diagnostic> const& diagnostics() const {
        return diagnostics_;
    }

    /**
     * @brief adds a diagnostic into this context.
     * @param diagnostic the target diagnostic
     * @return this
     */
    Reporter& report(Diagnostic diagnostic) {
        diagnostics_.push_back(std::move(diagnostic));
        return *this;
    }

    /**
     * @brief removes the all reported diagnostics.
     * @return this
     */
    Reporter& clear() {
        diagnostics_.clear();
        return *this;
    }
};
}  // namespace shakujo::analyzer

#endif  //SHAKUJO_ANALYZER_ANALYZER_CONTEXT_H_
