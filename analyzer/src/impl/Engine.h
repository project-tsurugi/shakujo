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
#ifndef SHAKUJO_ANALYZER_IMPL_ENGINE_H_
#define SHAKUJO_ANALYZER_IMPL_ENGINE_H_

#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "ScopeContext.h"

#include "shakujo/analyzer/AnalyzerContext.h"
#include "shakujo/analyzer/binding/ExpressionBinding.h"
#include "shakujo/analyzer/binding/VariableBinding.h"
#include "shakujo/analyzer/binding/FunctionBinding.h"
#include "shakujo/analyzer/scope/Scope.h"
#include "shakujo/analyzer/scope/BlockScope.h"
#include "shakujo/analyzer/scope/UnitScope.h"

#include "shakujo/model/IRFactory.h"
#include "shakujo/model/Node.h"
#include "shakujo/model/program/Program.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/program/GlobalDeclarationVisitor.h"
#include "shakujo/model/statement/StatementVisitor.h"
#include "shakujo/model/expression/ExpressionVisitor.h"

#include "shakujo/common/core/value/Error.h"
#include "shakujo/common/core/type/Relation.h"
#include "shakujo/common/core/type/Error.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::impl {

class Engine final  // NOLINT
        : protected model::program::GlobalDeclarationVisitor<void, ScopeContext&>
        , protected model::statement::StatementVisitor<void, ScopeContext&>
        , protected model::expression::ExpressionVisitor<void, ScopeContext&>
        {
private:
    AnalyzerContext& env_;
    model::IRFactory ir_factory;

public:
    explicit Engine(AnalyzerContext& env) : env_(env) {}

    void process(model::program::Program* node, ScopeContext& prev) {
        auto vars = block_scope(prev.variables());
        auto funcs = block_scope(prev.functions());
        ScopeContext scope { &vars, &funcs };
        for (auto e : node->declarations()) {
            dispatch(e, scope);
        }
        dispatch(node->main(), scope);
    }

    void process(model::statement::Statement* node, ScopeContext& scope) {
        dispatch(node, scope);
    }

    void process(model::expression::Expression* node, ScopeContext& scope) {
        dispatch(node, scope);
    }

    template<typename T>
    inline scope::BlockScope<T> block_scope(scope::Scope<T> const& parent) {
        return scope::BlockScope<T> { &env_.reporter(), &parent };
    }

    template<typename T>
    inline scope::UnitScope<T> unit_scope(scope::Scope<T> const& parent) {
        return scope::UnitScope<T> { &env_.reporter(), &parent };
    }

protected:
    using model::program::GlobalDeclarationVisitor<void, ScopeContext&>::dispatch;
    using model::program::GlobalDeclarationVisitor<void, ScopeContext&>::visit;
    using model::statement::StatementVisitor<void, ScopeContext&>::dispatch;
    using model::statement::StatementVisitor<void, ScopeContext&>::visit;
    using model::expression::ExpressionVisitor<void, ScopeContext&>::dispatch;
    using model::expression::ExpressionVisitor<void, ScopeContext&>::visit;

    // program
    void visit(model::program::GlobalVariableDeclaration*, ScopeContext&) override;

    // statements
    void visit(model::statement::EmptyStatement*, ScopeContext&) override;
    void visit(model::statement::BlockStatement*, ScopeContext&) override;
    void visit(model::statement::LocalVariableDeclaration*, ScopeContext&) override;
    void visit(model::statement::ExpressionStatement*, ScopeContext&) override;

    void visit(model::statement::BranchStatement*, ScopeContext&) override;
    void visit(model::statement::LogStatement*, ScopeContext&) override;

    // expressions
    void visit(model::expression::Literal*, ScopeContext&) override;
    void visit(model::expression::UnaryOperator*, ScopeContext&) override;
    void visit(model::expression::BinaryOperator*, ScopeContext&) override;
    void visit(model::expression::VariableReference*, ScopeContext&) override;
    void visit(model::expression::TupleElementLoadExpression*, ScopeContext&) override;
    void visit(model::expression::AssignExpression*, ScopeContext&) override;
    void visit(model::expression::FunctionCall*, ScopeContext&) override;

    // DML
    void visit(model::statement::dml::EmitStatement*, ScopeContext&) override;
    void visit(model::expression::relation::ScanExpression*, ScopeContext&) override;
    void visit(model::expression::relation::SelectionExpression*, ScopeContext&) override;
    void visit(model::expression::relation::ProjectionExpression*, ScopeContext&) override;
    void visit(model::expression::relation::JoinExpression*, ScopeContext&) override;

    void visit(model::statement::dml::InsertValuesStatement*, ScopeContext&) override;
    void visit(model::statement::dml::UpdateStatement*, ScopeContext&) override;
    void visit(model::statement::dml::DeleteStatement*, ScopeContext&) override;

    // DDL
    void visit(model::statement::ddl::CreateTableStatement*, ScopeContext&) override;

private:
    template<class K>
    auto extract(K const* key) -> decltype(std::declval<binding::BindingContext>().get(key)) {
        auto ptr = bindings().get(key);
        assert(common::util::is_defined(ptr));  // NOLINT
        return ptr;
    }

    std::shared_ptr<binding::ExpressionBinding> extract_binding(model::key::ExpressionKey::Provider const* node) {
        return extract(node->expression_key());
    }

    std::shared_ptr<binding::RelationBinding> extract_relation(model::expression::Expression const* node) {
        auto* provider = common::util::dynamic_pointer_cast<model::key::RelationKey::Provider>(node);
        return extract(provider->relation_key());
    }

    void insert_cast(model::expression::Expression*, common::core::Type const*);

    std::unique_ptr<common::core::Type> apply_binary_promotion(model::expression::Expression*, model::expression::Expression*);
    bool require_boolean(model::expression::Expression*);
    bool require_integral(model::expression::Expression*);
    bool require_numeric(model::expression::Expression*);
    bool require_textual(model::expression::Expression*);
    bool require_atom(model::expression::Expression*);
    bool require_tuple(model::expression::Expression*);
    bool require_relation(model::expression::Expression*);
    bool require_type(model::expression::Expression*, common::core::Type::Kind, std::string_view);

    void bless(model::key::ExpressionKey::Provider* node, std::shared_ptr<binding::ExpressionBinding> binding);
    void bless(model::key::VariableKey::Provider* node, std::shared_ptr<binding::VariableBinding> binding);
    void bless(model::key::FunctionKey::Provider* node, std::shared_ptr<binding::FunctionBinding> binding);
    void bless(model::key::RelationKey::Provider* node, std::shared_ptr<binding::RelationBinding> binding);

    template<class Binding>
    void bless_undefined(typename Binding::key_type::Provider* node) {
        bless(node, std::make_shared<Binding>());
    }

    template<class Binding, class List>
    void bless_undefined_each(List& elements) {
        for (auto* element : elements) {
            bless(element, std::make_shared<Binding>());
        }
    }

    void bless(
            model::key::ExpressionKey::Provider* node,
            std::unique_ptr<common::core::Type> type = {},
            std::unique_ptr<common::core::Value> value = {},
            bool constant = false) {
        bless(node, std::make_shared<binding::ExpressionBinding>(std::move(type), std::move(value), constant));
    }
    inline void bless(
            model::key::ExpressionKey::Provider* node,
            common::core::Type const* type,
            common::core::Value const* value = nullptr,
            bool constant = false) {
        bless(node, common::util::make_clone(type), common::util::make_clone(value), constant);
    }
    inline void bless(
            model::key::ExpressionKey::Provider* node,
            common::core::Type&& type,
            bool constant = false) {
        bless(node, common::util::make_clone(std::move(type)), {}, constant);
    }
    inline void bless(
            model::key::ExpressionKey::Provider* node,
            common::core::Type&& type,
            common::core::Value&& value,
            bool constant = false) {
        bless(node, common::util::make_clone(std::move(type)), common::util::make_clone(std::move(value)), constant);
    }
    void bless_erroneous_expression(model::key::ExpressionKey::Provider* node) {
        bless(node, std::make_unique<common::core::type::Error>());
    }

    std::tuple<std::unique_ptr<model::name::Name>, std::vector<std::unique_ptr<model::name::SimpleName>>>
    split_name(std::unique_ptr<model::name::Name>, std::size_t);

    void process_tuple_element(model::expression::TupleElementLoadExpression*);

    std::unique_ptr<common::core::Type> resolve_index(model::name::Index*, common::core::type::Tuple const*);

    void enrich_relation_profile(
            model::Node*, binding::RelationBinding::Profile&,
            common::schema::TableInfo const&, common::schema::IndexInfo const&);

    std::vector<binding::RelationBinding::JoinColumn> compute_join_columns(
            model::expression::relation::JoinExpression const* node,
            std::vector<std::shared_ptr<binding::VariableBinding>> const& left_variables,
            std::vector<std::shared_ptr<binding::VariableBinding>> const& right_variables,
            common::core::type::Relation const* left,
            common::core::type::Relation const* right,
            bool natural,
            bool left_null,
            bool right_null);

    inline void report(common::core::DocumentRegion region, Diagnostic::Code code, std::string message) {
        env_.reporter().report(Diagnostic(std::move(region), code, std::move(message)));
    }

    inline void report(model::Node const* node, Diagnostic::Code code, std::string message) {
        report(node->region(), code, std::move(message));
    }

    inline binding::BindingContext& bindings() {
        return env_.binding_context();
    }
};

}  // namespace shakujo::analyzer::impl

#endif  //SHAKUJO_ANALYZER_IMPL_ENGINE_H_
