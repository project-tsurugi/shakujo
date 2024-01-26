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
#ifndef SHAKUJO_MODEL_UTIL_PLACEHOLDER_RESOLVER_H_
#define SHAKUJO_MODEL_UTIL_PLACEHOLDER_RESOLVER_H_

#include <memory>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include "shakujo/model/Node.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/expression/Placeholder.h"

namespace shakujo::model::util {

/**
 * @brief resolves expression placeholders.
 */
class PlaceholderResolver {
public:
    /**
     * @brief replacement result of PlaceholderResolver.
     */
    class Result {
    private:
        std::vector<std::unique_ptr<expression::Placeholder>> replaced_;
        std::vector<expression::Placeholder*> left_;

    public:
        /**
         * @brief constructs a new object.
         * @param replaced the actually replaced placeholders
         * @param left the left placeholders
         */
        Result(
                std::vector<std::unique_ptr<expression::Placeholder>> replaced,
                std::vector<expression::Placeholder*> left)
            : replaced_(std::move(replaced)), left_(std::move(left))
        {}

        /**
         * @brief returns the replaced placeholders.
         * @return the replaced placeholders
         */
        inline std::vector<std::unique_ptr<expression::Placeholder>>& replaced() & {
            return replaced_;
        }

        /**
         * @brief returns the replaced placeholders.
         * @return the replaced placeholders
         */
        inline std::vector<std::unique_ptr<expression::Placeholder>> const& replaced() const& {
            return replaced_;
        }

        /**
         * @brief returns the replaced placeholders.
         * @return the replaced placeholders
         */
        inline std::vector<std::unique_ptr<expression::Placeholder>>&& replaced() && {
            return std::move(replaced_);
        }

        /**
         * @brief returns the left placeholders.
         * @return the left placeholders
         */
        inline std::vector<expression::Placeholder*>& left() & {
            return left_;
        }

        /**
         * @brief returns the left placeholders.
         * @return the left placeholders
         */
        inline std::vector<expression::Placeholder*> const& left() const& {
            return left_;
        }

        /**
         * @brief returns the left placeholders.
         * @return the left placeholders
         */
        inline std::vector<expression::Placeholder*>&& left() && {
            return std::move(left_);
        }

        /**
         * @brief returns whether or not there are no left placeholders.
         * @return true if there are no left placeholders
         * @return false otherwise
         */
        inline explicit operator bool() const {
            return left_.empty();
        }
    };

    /**
     * @brief constructs a new object.
     * @param keywords keyword based replacements
     */
    explicit PlaceholderResolver(std::map<std::string, std::unique_ptr<expression::Expression>> keywords)
        : keywords_(std::move(keywords))
    {}

    /**
     * @brief replaces placeholders.
     * @param node the starting node
     * @return the replacement result
     */
    Result operator()(Node* node) const;

    /**
     * @brief collects placeholders.
     * @param node the starting node
     * @return the collected placeholders
     */
    static std::vector<expression::Placeholder*> collect(Node* node);

    /**
     * @brief collects placeholders.
     * @param node the starting node
     * @return the collected placeholders
     */
    inline static std::vector<expression::Placeholder const*> collect(Node const* node) {
        auto result = collect(const_cast<Node*>(node));
        return { result.begin(), result.end() };
    }

private:
    std::map<std::string, std::unique_ptr<expression::Expression>> keywords_;

    std::unique_ptr<expression::Placeholder> replace(expression::Placeholder* node) const;
};

}  // namespace shakujo::model::util

#endif  // SHAKUJO_MODEL_UTIL_PLACEHOLDER_RESOLVER_H_
