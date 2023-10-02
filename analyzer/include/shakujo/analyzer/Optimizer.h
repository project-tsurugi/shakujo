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
#ifndef SHAKUJO_ANALYZER_OPTIMIZER_H_
#define SHAKUJO_ANALYZER_OPTIMIZER_H_

#include <memory>

#include "binding/BindingContext.h"
#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/expression/Expression.h"

namespace shakujo::analyzer {

/**
 * @brief optimizes analyzed IR models.
 */
class Optimizer {
public:
    /**
     * @brief constructs a new object.
     * @param bindings the binding context
     */
    explicit Optimizer(binding::BindingContext& bindings);

    /**
     * @brief destructs this object.
     */
    ~Optimizer() noexcept;

    Optimizer(Optimizer const& other) = delete;
    Optimizer& operator=(Optimizer const& other) = delete;

    /**
     * @brief constructs a new object.
     * @param other the move source
     */
    Optimizer(Optimizer&& other) noexcept;

    /**
     * @brief assigns the given object into this.
     * @param other the move source
     * @return this
     */
    Optimizer& operator=(Optimizer&& other) noexcept;

    /**
     * @brief optimizes the given program.
     * The given node must be analyzed by Analyzer.
     * @param node the target node
     */
    void operator()(model::program::Program* node);

    /**
     * @brief optimizes the given statement.
     * The given node must be analyzed by Analyzer.
     * @param node the target node
     */
    void operator()(model::statement::Statement* node);

    /**
     * @brief optimizes the given expression.
     * The given node must be analyzed by Analyzer.
     * The expression must be managed because it may be rewrite to another expression.
     * @param node the target node
     */
    void operator()(model::expression::Expression* node);

    /**
     * @brief optimizer options.
     */
    struct Options {

        /**
         * @brief options about scan operation optimization.
         */
        struct Scan {

            /**
             * @brief whether or not primary index scan is enabled.
             */
            bool primary_index = true;

            /**
             * @brief whether or not secondary index scan (with primary index) is enabled.
             */
            bool secondary_index = true;

            /**
             * @brief a penalty ratio of using secondary index instead of primary index.
             * 0.0: no penalty .. 1.0: never use secondary indices.
             */
            double secondary_index_penalty = 0.5;

            /**
             * @brief returns whether or not this optimization is enabled.
             * @return true if enabled
             * @return false otherwise
             */
            explicit operator bool() const {
                return primary_index || secondary_index;
            }
        };

        /**
         * @brief options about join operation optimization.
         */
        struct Join {
            /**
             * @brief whether or not seek operations in nested loop join is enabled.
             */
            bool nested_loop_seek = true;

            /**
             * @brief returns whether or not this optimization is enabled.
             * @return true if enabled
             * @return false otherwise
             */
            explicit operator bool() const {
                return nested_loop_seek;
            }
        };

        /**
         * @brief options about predicate push down optimization.
         */
        struct PredicatePushDown {

            /**
             * @brief whether or not enable this optimization.
             */
            bool enabled = true;

            /**
             * @brief returns whether or not this optimization is enabled.
             * @return true if enabled
             * @return false otherwise
             */
            explicit operator bool() const {
                return enabled;
            }
        };

        /**
         * @brief options about projection push down optimization.
         */
        struct ProjectionPushDown {

            /**
             * @brief whether or not enable this optimization.
             */
            bool enabled = true;

            /**
             * @brief returns whether or not this optimization is enabled.
             * @return true if enabled
             * @return false otherwise
             */
            explicit operator bool() const {
                return enabled;
            }
        };

        /**
         * @brief options about redundant relational operators.
         */
        struct Redundancy {

            /**
             * @brief whether or not remove redundant sort operations.
             */
            bool sort = true;

            /**
             * @brief whether or not remove redundant distinct operations.
             */
            bool distinct = true;
        };

        /**
         * @brief options about scan operation optimization.
         */
        Scan scan {};

        /**
         * @brief options about scan operation optimization.
         */
        Join join {};

        /**
         * @brief options about predicate push down optimization.
         */
        PredicatePushDown predicate_push_down {};

        /**
         * @brief options about projection push down optimization.
         */
        ProjectionPushDown projection_push_down {};

        /**
         * @brief options about redundant relational operators.
         */
        Redundancy redundancy {};
    };

    /**
     * @brief returns options of this optimizer.
     * @return the options
     */
    Options& options();

    /**
     * @brief returns options of this optimizer.
     * @return the options
     */
    Options const& options() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
}  // namespace shakujo::analyzer

#endif  //SHAKUJO_ANALYZER_OPTIMIZER_H_
