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
#include "SelectJoin.h"

#include <memory>
#include <utility>
#include <vector>

#include <cassert>
#include <cstdlib>

#include "SimplifyPredicate.h"
#include "ComparisonTerm.h"
#include "VariableRewriter.h"

#include "shakujo/common/util/utility.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/expression/ExpressionVisitor.h"
#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"

namespace shakujo::analyzer::optimize {

using common::util::is_defined;
using common::util::make_clone;
using common::util::dynamic_pointer_cast_if;

namespace {
class ScanCollector : public model::expression::ExpressionVisitor<void> {
public:
    using ExpressionVisitor::visit;

    void visitDefault(model::expression::Expression*) override {}

    void visit(model::expression::relation::ScanExpression* node) override {
        auto relation = context_.relation_of(node);
        auto&& strategy = relation->scan_strategy();
        if (strategy.kind() == binding::ScanStrategy::Kind::FULL) {
            auto&& table = strategy.table();
            auto&& index = table.primary_index();
            if (!index) {
                return;
            }

            // normalize full-scan to empty-prefixed primary index scan
            auto&& columns = relation->output().columns();
            std::vector<std::shared_ptr<binding::VariableBinding>> key {};
            key.reserve(index.columns().size());
            for (auto&& column : index.columns()) {
                if (auto loc = table.index_of(column.name()); loc.has_value()) {
                    key.emplace_back(columns[loc.value()]);
                } else {
                    // failure
                    return;
                }
            }
            strategy_ = { table, index, key, {} };
        } else {
            strategy_ = strategy;
        }
        rewriter_.add_rule(relation->output().columns(), relation->output().columns());
    }

    void visit(model::expression::relation::SelectionExpression* node) override {
        dispatch(node->operand());
        if (!strategy_.table()) {
            return;
        }
        auto parent = context_.relation_of(node->operand());
        auto relation = context_.relation_of(node);
        VariableRewriter rewriter;
        rewriter.add_rule(parent->output().columns(), relation->output().columns());
        rewriter_.merge(rewriter);
    }

    void visit(model::expression::relation::ProjectionExpression* node) override {
        dispatch(node->operand());
        if (!strategy_.table()) {
            return;
        }
        VariableRewriter rewriter;
        for (auto* column : node->columns()) {
            if (auto* ref = dynamic_pointer_cast_if<model::expression::VariableReference>(column->value()); is_defined(ref)) {
                auto in = context_.binding_of(ref->variable_key());
                auto out = context_.binding_of(column->variable_key());
                rewriter.add_rule(in, out);
            }
        }
        rewriter_.merge(rewriter);
    }

    // FIXME: through join

    void visit(model::expression::relation::OrderExpression* node) override {
        dispatch(node->operand());
        if (!strategy_.table()) {
            return;
        }
        auto parent = context_.relation_of(node->operand());
        auto relation = context_.relation_of(node);
        VariableRewriter rewriter;
        rewriter.add_rule(parent->output().columns(), relation->output().columns());
        rewriter_.merge(rewriter);
    }

    void visit(model::expression::relation::LimitExpression* node) override {
        dispatch(node->operand());
        if (!strategy_.table()) {
            return;
        }
        auto parent = context_.relation_of(node->operand());
        auto relation = context_.relation_of(node);
        VariableRewriter rewriter;
        rewriter.add_rule(parent->output().columns(), relation->output().columns());
        rewriter_.merge(rewriter);
    }

    void visit(model::expression::relation::DistinctExpression* node) override {
        dispatch(node->operand());
        if (!strategy_.table()) {
            return;
        }
        auto parent = context_.relation_of(node->operand());
        auto relation = context_.relation_of(node);
        VariableRewriter rewriter;
        rewriter.add_rule(parent->output().columns(), relation->output().columns());
        rewriter_.merge(rewriter);
    }

    static binding::ScanStrategy collect(Context& context, model::expression::Expression* node) {
        ScanCollector collector { context };
        collector.dispatch(node);
        auto&& strategy = collector.strategy_;
        if (!strategy.table()) {
            return {};
        }
        if (strategy.kind() != binding::ScanStrategy::Kind::PREFIX
                && strategy.kind() != binding::ScanStrategy::Kind::RANGE) {
            return {};
        }
        std::vector<std::shared_ptr<binding::VariableBinding>> key_columns;
        key_columns.reserve(strategy.key_columns().size());
        for (auto&& v : strategy.key_columns()) {
            key_columns.emplace_back(collector.rewriter_.apply(v));
        }
        std::vector<std::unique_ptr<common::core::Value>> prefix;
        prefix.reserve(strategy.prefix().size());
        for (auto&& v : strategy.prefix()) {
            prefix.emplace_back(make_clone(v));
        }
        return {
            strategy.table(),
            strategy.index(),
            key_columns,
            std::move(prefix),
            strategy.lower_suffix(),
            strategy.upper_suffix(),
        };
    }

private:
    Context& context_;
    binding::ScanStrategy strategy_ {};
    VariableRewriter rewriter_ {};

    explicit ScanCollector(Context& context) : context_(context) {}
};

class Engine : public model::util::NodeWalker {
public:
    explicit Engine(Context& context) noexcept : context_(context) {}

    using NodeWalker::enter;

    void exit(model::expression::relation::JoinExpression* node) override {
        if (context_.options().join.nested_loop_seek && try_seek(node)) {
            return;
        }
    }

private:
    Context& context_;

    enum class Side {
        INVALID,
        LEFT,
        RIGHT,
    };

    using equality = std::pair<std::shared_ptr<binding::VariableBinding>, std::shared_ptr<binding::VariableBinding>>;

    bool try_seek(model::expression::relation::JoinExpression *node) {
        auto relation = context_.relation_of(node);
        auto&& strategy = relation->join_strategy();
        if (strategy.kind() != binding::JoinStrategy::Kind::NESTED_LOOP
                || strategy.equalities().empty()
                || !strategy.seek_columns().empty()) {
            return false;
        }

        auto left_scan = ScanCollector::collect(context_, node->left());
        auto right_scan = ScanCollector::collect(context_, node->right());

        // FIXME: support seek variables on middle of scan key
        auto left_seek = extract<true>(strategy, left_scan);
        auto right_seek = extract<false>(strategy, right_scan);

        auto side = select_seek(left_scan, left_seek, right_scan, right_seek);

        // seek left side - we exchange left/right operand
        if (side == Side::LEFT) {
            auto left_operand = node->release_left();
            auto right_operand = node->release_right();
            node->left(std::move(right_operand));
            node->right(std::move(left_operand));
            strategy.commute();
            for (auto&& term : left_seek) {
                std::swap(term.first, term.second);
            }
            return apply_seek(strategy, left_scan, left_seek);
        }

        // seek right side
        if (side == Side::RIGHT) {
            return apply_seek(strategy, right_scan, right_seek);
        }
        return false;
    }

    bool apply_seek(
            binding::JoinStrategy& join,
            binding::ScanStrategy const& scan,
            std::vector<equality> seek) {
        for (std::size_t i = 0, n = scan.prefix().size(); i < n; ++i) {
            join.seek_columns().emplace_back(make_clone(scan.key_columns()[i]->type()), make_clone(scan.prefix()[i]));
        }
        for (std::size_t i = 0, n = seek.size(); i < n; ++i) {
            auto&& term = seek[i];

            // seek columns may include variables from the left operand
            join.seek_columns().emplace_back(
                make_clone(scan.key_columns()[scan.prefix().size() + i]->type()),
                term.first);

            // remove duplicated condition
            auto&& equalities = join.equalities();
            if (auto it = equalities.find(term); it != equalities.end()) {
                equalities.erase(it);
            }
        }
        return true;
    }

    template<bool left>
    std::vector<equality>
    extract(binding::JoinStrategy const& join, binding::ScanStrategy const& scan) {
        if (!scan.table()) {
            return {};
        }
        if (left ? join.right_outer() : join.left_outer()) {
            return {};
        }
        auto&& columns = scan.key_columns();
        if (columns.size() == scan.prefix().size()) {
            return {};
        }
        std::vector<equality> results;
        constexpr std::size_t SELF = left ? 0 : 1;
        for (std::size_t i = scan.prefix().size(), n = columns.size(); i < n; ++i) {
            auto&& column = columns[i];
            bool found = false;
            for (auto&& pair : join.equalities()) {
                if (column == std::get<SELF>(pair)) {
                    results.emplace_back(pair);
                    found = true;
                    break;
                }
            }
            if (!found) {
                break;
            }
        }
        return results;
    }

    Side select_seek(
            binding::ScanStrategy const& left_scan,
            std::vector<equality> const& left_seek,
            binding::ScanStrategy const& right_scan,
            std::vector<equality> const& right_seek) {
        if (left_seek.empty() && right_seek.empty()) {
            return Side::INVALID;
        }
        if (right_seek.empty()) {
            return Side::LEFT;
        }
        if (left_seek.empty()) {
            return Side::RIGHT;
        }

        // FIXME: use selectivity
        bool left_primary = left_scan.index().is_primary();
        bool right_primary = right_scan.index().is_primary();
        bool left_complete = is_complete(left_scan, left_seek);
        bool right_complete = is_complete(right_scan, right_seek);
        if (right_primary && right_complete) {
            if (left_primary && left_complete && left_seek.size() > right_seek.size()) {
                return Side::LEFT;
            }
            return Side::RIGHT;
        }
        if (left_primary && left_complete) {
            return Side::LEFT;
        }
        return right_seek.size() >= left_seek.size() ? Side::RIGHT : Side::LEFT;
    }

    bool is_complete(binding::ScanStrategy const& scan, std::vector<equality> const& seek) {
        return !seek.empty() && scan.key_columns().size() == scan.prefix().size() + seek.size();
    }
};
}  // namespace

void SelectJoin::operator()(model::program::Program* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void SelectJoin::operator()(model::statement::Statement* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void SelectJoin::operator()(model::expression::Expression *node) {
    Engine engine { context_ };
    engine.walk(node);
}
}  // namespace shakujo::analyzer::optimize
