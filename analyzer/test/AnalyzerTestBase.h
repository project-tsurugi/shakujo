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
#ifndef ANALYZER_TEST_BASE_H_
#define ANALYZER_TEST_BASE_H_

#include "shakujo/analyzer/Analyzer.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include "shakujo/model/IRFactory.h"
#include "shakujo/common/schema/StorageInfoProvider.h"
#include "shakujo/common/schema/ConfigurableStorageInfoProvider.h"
#include "shakujo/common/util/JsonSerializer.h"
#include "shakujo/analyzer/binding/BindingContext.h"
#include "shakujo/analyzer/binding/BindingSerializer.h"

#include "shakujo/common/core/type/Relation.h"

namespace shakujo::analyzer {

static const common::core::Type::Nullity NULLABLE = common::core::Type::Nullity::NULLABLE;
static const common::core::Type::Nullity NON_NULL = common::core::Type::Nullity::NEVER_NULL;
static const common::core::type::Error ERROR_TYPE;

class AnalyzerTestBase {
public:
    shakujo::model::IRFactory f;
    std::shared_ptr<common::schema::ConfigurableStorageInfoProvider> tables =
            std::make_shared<common::schema::ConfigurableStorageInfoProvider>();
    AnalyzerContext env { tables };

    bool error() {
        return env.reporter().saw_error();
    }

    bool contains(Diagnostic::Code code) {
        for (auto& e : env.reporter().diagnostics()) {
            if (e.code() == code) return true;
        }
        return false;
    }

    bool contains(Diagnostic::Severity severity) {
        for (auto& e : env.reporter().diagnostics()) {
            if (e.severity() == severity) return true;
        }
        return false;
    }

    void add(common::schema::TableInfo table) {
        tables->add(std::move(table));
    }

    std::vector<common::core::Name> names(std::initializer_list<const char*> ss) {
        std::vector<common::core::Name> ret;
        for (auto& s : ss) {
            ret.emplace_back(s);
        }
        return ret;
    }

    std::shared_ptr<binding::ExpressionBinding> extract(model::key::ExpressionKey::Provider* provider, bool force = false) {
        auto ptr = env.binding_context().get(provider->expression_key());
        if (!ptr || (!force && !ptr->is_valid())) {
            throw std::runtime_error("yet not resolved");
        }
        return ptr;
    }

    std::shared_ptr<binding::VariableBinding> extract_var(model::key::VariableKey::Provider* provider, bool force = false) {
        auto ptr = env.binding_context().get(provider->variable_key());
        if (!ptr || (!force && !ptr->is_valid())) {
            throw std::runtime_error("yet not resolved");
        }
        return ptr;
    }

    common::core::type::Relation const* extract_relation(shakujo::model::key::ExpressionKey::Provider* provider) {
        auto binding = extract(provider);
        if (binding->type()->kind() != common::core::Type::Kind::RELATION) {
            std::ostringstream ss;
            ss << "require relation: " << binding->type();
            throw std::runtime_error(ss.str());
        }
        return dynamic_cast<common::core::type::Relation const*>(binding->type());
    }

    binding::Id<binding::VariableBinding> const& add_variable(
            std::unique_ptr<shakujo::model::name::Name> name,
            common::core::Type const* type,
            common::core::Value const* value = nullptr) {
        auto element = std::make_shared<binding::VariableBinding>(
                env.binding_context().next_variable_id(),
                static_cast<common::core::Name>(*name),
                type,
                value);
        env.variable_scope().insert(name.get(), element);
        return element->id();
    }

    binding::Id<binding::VariableBinding> const& add_variable(
            std::string_view name,
            common::core::Type const* type,
            common::core::Value const* value = nullptr) {
        return add_variable(f.Name(name), type, value);
    }

    binding::Id<binding::VariableBinding> const& add_variable(
            std::unique_ptr<shakujo::model::name::Name> name,
            common::core::Type const& type) {
        return add_variable(std::move(name), &type);
    }

    binding::Id<binding::VariableBinding> const& add_variable(
            std::string_view name,
            common::core::Type const& type) {
        return add_variable(name, &type);
    }

    std::string diagnostics() {
        std::ostringstream ss;
        ss << "{";
        bool cont = false;
        for (auto& e : env.reporter().diagnostics()) {
            if (cont) ss << ", ";
            cont = true;
            ss << e;
        }
        ss << "}";
        return ss.str();
    }

    template<typename T, typename M>
    std::unique_ptr<T> analyze_managed(std::unique_ptr<T> node) {
        shakujo::common::util::ManagedPtr<M> manager { std::move(node) };
        Analyzer {}.analyze(env, manager.get());
        auto ptr = common::util::dynamic_pointer_cast<T>(manager.release());
        if (ptr) {
            env.reporter().report(Diagnostic { Diagnostic::Code::MESSAGE, to_string(ptr.get()) });
            return ptr;
        }
        throw std::logic_error("type changed");
    }

    template<typename T>
    std::unique_ptr<T> analyze_unmanaged(std::unique_ptr<T> node) {
        Analyzer {}.analyze(env, node.get());
        env.reporter().report(Diagnostic { Diagnostic::Code::MESSAGE, to_string(node.get()) });
        return std::move(node);
    }

    template<typename T>
    std::string to_string(T const* value) {
        std::ostringstream ss;
        common::util::JsonSerializer json { ss };
        binding::BindingSerializer serializer { env.binding_context() };
        serializer.serialize(json, value);
        return ss.str();
    }

    template<typename T>
    bool is(model::Node const* node) {
        return dynamic_cast<T const*>(node) != nullptr;
    }

    std::unique_ptr<model::expression::Expression> var(std::string_view name) {
        return f.VariableReference(f.Name(name));
    }

    std::unique_ptr<model::expression::Expression> literal(
            int value,
            std::size_t size = 64U,
            common::core::Type::Nullity nullity = NON_NULL) {
        return f.Literal(common::core::type::Int(size, nullity), value);
    }

    std::unique_ptr<model::expression::Expression> literal(int value, common::core::Type::Nullity nullity) {
        return literal(value, 64U, nullity);
    }

    std::unique_ptr<model::expression::Expression> literal(
            double value,
            std::size_t size = 64U,
            common::core::Type::Nullity nullity = NON_NULL) {
        return f.Literal(common::core::type::Float(size, nullity), value);
    }

    std::unique_ptr<model::expression::Expression> literal(double value, common::core::Type::Nullity nullity) {
        return literal(value, 64U, nullity);
    }

    std::unique_ptr<model::expression::Expression> literal(bool value, common::core::Type::Nullity nullity = NON_NULL) {
        return f.Literal(common::core::type::Bool(nullity), value);
    }

    std::unique_ptr<model::expression::Expression> literal(const char* value, common::core::Type::Nullity nullity = NON_NULL) {
        return f.Literal(common::core::type::String(nullity), value);
    }

    std::unique_ptr<model::expression::Expression> literal(std::nullptr_t) {
        return f.Literal(common::core::type::Null());
    }

    template<typename T>
    typename T::type get(model::expression::Expression const* literal) {
        if (auto lp = dynamic_cast<model::expression::Literal const*>(literal)) {
            if (auto lv = dynamic_cast<const T*>(lp->value())) {
                return lv->get();
            }
            EXPECT_TRUE(false) << "inconsistent type: " << lp->value();
            throw std::bad_cast();
        }
        EXPECT_TRUE(false) << "not a literal: " << literal->kind();
        throw std::bad_cast();
    }

    template<typename T>
    T* as(model::Node* node) {
        auto p = node;
        if (auto tp = dynamic_cast<T*>(p)) {
            return tp;
        }
        if (auto esp = dynamic_cast<model::statement::ExpressionStatement*>(p)) {
            return as<T>(esp->body());
        }
        throw std::bad_cast();
    }

    common::core::Type const& type(model::expression::Expression* expr) {
        return *extract(expr)->type();
    }

    common::core::Type const& cast_type(model::expression::Expression const* cast) {
        if (auto cp = dynamic_cast<model::expression::ImplicitCast const*>(cast)) {
            return *cp->type();
        }
        return ERROR_TYPE;
    }

    bool is_propagated_error(model::expression::Expression const* expr) {
        auto ptr = env.binding_context().get(expr->expression_key());
        if (!ptr) {
            return false;
        }
        return !ptr->type();
    }

    void success(bool ok = true) {
        if (ok) {
            EXPECT_FALSE(error()) << diagnostics();
        } else {
            EXPECT_TRUE(error()) << diagnostics();
        }
    }
};
}  // namespace shakujo::analyzer

#endif  // ANALYZER_TEST_BASE_H_