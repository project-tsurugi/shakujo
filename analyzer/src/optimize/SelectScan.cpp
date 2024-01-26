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
#include "SelectScan.h"

#include <array>
#include <functional>
#include <limits>
#include <map>
#include <vector>

#include <cassert>
#include <cstdlib>

#include "SimplifyPredicate.h"
#include "ComparisonTerm.h"

#include "shakujo/common/util/utility.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/util/NodeWalker.h"

#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/RelationBinding.h"

namespace shakujo::analyzer::optimize {

using common::util::is_defined;
using common::util::make_clone;
using common::util::dynamic_pointer_cast_if;

namespace {

struct Suffix {
    ComparisonTerm* term;
    bool inclusive;
};

struct StrategyInfo {
    common::schema::IndexInfo const& index;
    std::vector<ComparisonTerm*> prefix;
    Suffix lower {};
    Suffix upper {};
};

class Engine : public model::util::NodeWalker {
public:
    explicit Engine(Context& context) noexcept : context_(context) {}

    using NodeWalker::enter;

    bool enter(model::expression::relation::SelectionExpression* node) override {
        if (auto* scan = dynamic_pointer_cast_if<model::expression::relation::ScanExpression>(node->operand()); is_defined(scan)) {
            if (optimize(scan, node->condition())) {
                SimplifyPredicate simplifier { context_ };
                if (simplifier(node->condition()) == true) {
                    node->replace_with(node->release_operand());
                }
            }
            return false;
        }
        return true;
    }

    bool optimize(model::expression::relation::ScanExpression* node, model::expression::Expression* condition) {
        auto relation = context_.bindings().get(node->relation_key());
        auto&& strategy = relation->scan_strategy();
        if (strategy.kind() != binding::ScanStrategy::Kind::FULL) {
            return false;
        }
        auto&& table = strategy.table();
        auto columns = create_column_map(*relation);
        auto all_terms = ComparisonTerm::collect(context_.bindings(), condition);
        auto terms = grouping(*relation, all_terms);

        std::vector<StrategyInfo> candidates {};
        if (context_.options().scan.primary_index) {
            if (auto&& index = table.primary_index()) {
                auto candidate = extract_candidate(index, columns, terms);
                // primary key single row
                if (candidate.prefix.size() == index.columns().size()) {
                    relation->scan_strategy(create_strategy(candidate, table, columns));
                    consume_terms(candidate);
                    return true;
                }
                candidates.emplace_back(std::move(candidate));
            }
        }
        if (context_.options().scan.secondary_index) {
            for (auto&& index : table.secondary_indices()) {
                auto candidate = extract_candidate(index, columns, terms);
                candidates.emplace_back(std::move(candidate));
            }
        }
        if (!candidates.empty()) {
            if (auto* candidate = select_strategy(candidates); is_defined(candidate)) {
                relation->scan_strategy(create_strategy(*candidate, table, columns));
                consume_terms(*candidate);
                return true;
            }
        }
        return false;
    }

    StrategyInfo const* select_strategy(std::vector<StrategyInfo> const& candidates) {
        assert(!candidates.empty());  // NOLINT
        /*
         * FIXME: use cost info
         * temporary scoring rule:
         * primary index = x 1.0
         * secondary index = x 1.0 - secondary_index_penalty
         * columns = x (prefix-length + suffix-count / 3) / (key-size + 1)
         */
        double current_max = -std::numeric_limits<double>::infinity();
        StrategyInfo const* current_candidate = nullptr;
        for (auto&& candidate : candidates) {
            if (candidate.prefix.empty()
                    && !is_defined(candidate.lower.term)
                    && !is_defined(candidate.upper.term)) {
                continue;
            }
            double score = 1.0;
            if (!candidate.index.is_primary()) {
                score *= 1.0 - context_.options().scan.secondary_index_penalty;
            }
            std::size_t suffix_count = 0;
            if (is_defined(candidate.lower.term)) {
                ++suffix_count;
            }
            if (is_defined(candidate.upper.term)) {
                ++suffix_count;
            }
            std::divides<double> div {};
            score *= div(candidate.prefix.size() + div(suffix_count, 3), candidate.index.columns().size() + 1);
            if (score > current_max) {
                current_max = score;
                current_candidate = &candidate;
            }
        }
        return current_candidate;
    }

    binding::ScanStrategy create_strategy(
            StrategyInfo const& info,
            common::schema::TableInfo const& table,
            std::map<std::string, std::shared_ptr<binding::VariableBinding>> const& column_map) {
        std::vector<std::shared_ptr<binding::VariableBinding>> key_columns {};
        key_columns.reserve(info.index.columns().size());
        for (auto&& column : info.index.columns()) {
            auto it = column_map.find(column.name());
            assert(it != column_map.end());  // NOLINT
            key_columns.emplace_back(it->second);
        }
        std::vector<std::unique_ptr<common::core::Value>> prefix {};
        prefix.reserve(info.prefix.size());
        for (auto* term : info.prefix) {
            prefix.emplace_back(make_clone(term->right().constant()));
        }

        binding::ScanStrategy::Suffix lower = create_suffix(info.lower);
        binding::ScanStrategy::Suffix upper = create_suffix(info.upper);

        // remove nulls
        if (prefix.size() < key_columns.size() && (lower.is_valid() || upper.is_valid())) {
            bool asc = info.index.columns()[prefix.size()].direction() == common::core::Direction::ASCENDANT;
            auto&& suffix = asc ? lower : upper;
            auto&& var = key_columns[prefix.size()];
            if (!suffix.is_valid() && var->type()->nullable()) {
                suffix = { std::make_unique<common::core::value::Null>(), false };
            }
        }

        return {
            table,
            info.index,
            std::move(key_columns),
            std::move(prefix),
            std::move(lower),
            std::move(upper),
        };
    }

    binding::ScanStrategy::Suffix create_suffix(Suffix const& info) {
        if (!is_defined(info.term)) {
            return {};
        }
        return { make_clone(info.term->right().constant()), info.inclusive };
    }

    void consume_terms(StrategyInfo const& info) {
        for (auto* term : info.prefix) {
            consume_term(term);
        }
        if (auto* term = info.lower.term; is_defined(term)) {
            consume_term(term);
        }
        if (auto* term = info.upper.term; is_defined(term)) {
            consume_term(term);
        }
    }

    void consume_term(ComparisonTerm* term) {
        model::IRFactory f;
        auto* replacement = term->source()->replace_with(f.Literal(
            std::make_unique<common::core::type::Bool>(common::core::Type::Nullity::NEVER_NULL),
            std::make_unique<common::core::value::Bool>(true)));
        replacement->expression_key(context_.bindings().create_key<binding::ExpressionBinding>(
            make_clone(replacement->type()),
            make_clone(replacement->value()),
            true));
    }

    std::map<std::string, std::shared_ptr<binding::VariableBinding>> create_column_map(
            binding::RelationBinding& relation) {
        std::map<std::string, std::shared_ptr<binding::VariableBinding>> results {};
        auto&& profile = relation.output();
        auto&& table = relation.scan_strategy().table();
        assert(table.columns().size() == profile.columns().size());  // NOLINT
        for (std::size_t i = 0, n = table.columns().size(); i < n; ++i) {
            results.emplace(table.columns()[i].name(), profile.columns()[i]);
        }
        return results;
    }

    std::map<std::shared_ptr<binding::VariableBinding>, std::vector<ComparisonTerm*>> grouping(
            binding::RelationBinding& relation,
            std::vector<ComparisonTerm>& terms) {
        auto&& profile = relation.output();
        std::map<std::shared_ptr<binding::VariableBinding>, std::vector<ComparisonTerm*>> results {};
        for (auto&& column : profile.columns()) {
            results.emplace(column, std::vector<ComparisonTerm*> {});
        }
        for (auto&& term : terms) {
            assert(term.left().is_variable());  // NOLINT
            if (auto it = results.find(term.left().variable()); it != results.end()) {
                auto&& group = it->second;
                group.emplace_back(&term);
            }
        }
        return results;
    }

    StrategyInfo extract_candidate(
            common::schema::IndexInfo const& index,
            std::map<std::string, std::shared_ptr<binding::VariableBinding>> const& column_map,
            std::map<std::shared_ptr<binding::VariableBinding>, std::vector<ComparisonTerm*>> const& term_map) {
        auto&& prefix = extract_prefix(index, column_map, term_map);
        if (prefix.size() == index.columns().size()) {
            return StrategyInfo { index, std::move(prefix) };
        }
        auto suffixes = extract_suffix(index, prefix.size(), column_map, term_map);
        return StrategyInfo { index, std::move(prefix), suffixes[0], suffixes[1] };
    }

    std::vector<ComparisonTerm*> extract_prefix(
            common::schema::IndexInfo const& index,
            std::map<std::string, std::shared_ptr<binding::VariableBinding>> const& column_map,
            std::map<std::shared_ptr<binding::VariableBinding>, std::vector<ComparisonTerm*>> const& term_map) {
        std::vector<ComparisonTerm*> results {};
        for (auto&& column : index.columns()) {
            auto itc = column_map.find(column.name());
            assert(itc != column_map.end());  // NOLINT
            auto itt = term_map.find(itc->second);
            assert(itt != term_map.end());  // NOLINT

            auto&& terms = itt->second;
            bool found = false;
            for (auto* term : terms) {
                if (term->op() == ComparisonTerm::Operator::EQ) {
                    results.emplace_back(term);
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

    std::array<Suffix, 2> extract_suffix(
            common::schema::IndexInfo const& index,
            std::size_t prefix_size,
            std::map<std::string, std::shared_ptr<binding::VariableBinding>> const& column_map,
            std::map<std::shared_ptr<binding::VariableBinding>, std::vector<ComparisonTerm*>> const& term_map) {
        std::array<Suffix, 2> results { Suffix { nullptr, true }, Suffix { nullptr, true } };
        if (prefix_size >= index.columns().size()) {
            return results;
        }
        auto&& column = index.columns()[prefix_size];
        auto itc = column_map.find(column.name());
        assert(itc != column_map.end());  // NOLINT
        auto itt = term_map.find(itc->second);
        assert(itt != term_map.end());  // NOLINT

        auto&& terms = itt->second;
        bool asc = column.direction() == common::core::Direction::ASCENDANT;
        for (auto* term : terms) {
            bool lower;
            bool inclusive;
            using Op = ComparisonTerm::Operator;
            switch (term->op()) {
                case Op::EQ:
                case Op::NE:
                    continue;
                case Op::LT: // var < val
                    lower = false;
                    inclusive = false;
                    break;
                case Op::LE:
                    lower = false;
                    inclusive = true;
                    break;
                case Op::GT: // var > val
                    lower = true;
                    inclusive = false;
                    break;
                case Op::GE:
                    lower = true;
                    inclusive = true;
                    break;
                default:
                    std::abort();
            }
            if (!asc) {
                lower = !lower;
            }
            auto&& suffix = results[lower ? 0 : 1];  // NOLINT

            // FIXME: also update if range becomes more smaller
            if (suffix.term == nullptr) {
                suffix.term = term;
                suffix.inclusive = inclusive;
            }
        }
        return results;
    }

private:
    Context& context_;
};
}  // namespace

void SelectScan::operator()(model::program::Program* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void SelectScan::operator()(model::statement::Statement* node) {
    Engine engine { context_ };
    engine.walk(node);
}

void SelectScan::operator()(model::expression::Expression *node) {
    Engine engine { context_ };
    engine.walk(node);
}
}  // namespace shakujo::analyzer::optimize
