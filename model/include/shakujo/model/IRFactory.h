/*
 * Copyright 2018-2023 Project Tsurugi.
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
#ifndef SHAKUJO_MODEL_IR_FACTORY_H_
#define SHAKUJO_MODEL_IR_FACTORY_H_

#include <string>
#include <initializer_list>
#include <vector>

#include "shakujo/common/core/Name.h"
#include "shakujo/common/core/Type.h"
#include "shakujo/common/core/type/Bool.h"
#include "shakujo/common/core/type/Int.h"
#include "shakujo/common/core/type/Float.h"
#include "shakujo/common/core/type/Char.h"
#include "shakujo/common/core/type/String.h"
#include "shakujo/common/core/type/Null.h"
#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/core/value/String.h"
#include "shakujo/common/core/value/Null.h"

#include "shakujo/common/util/utility.h"

#include "IRFactoryBase.h"
#include "name/Name.h"
#include "name/SimpleName.h"
#include "name/QualifiedName.h"
#include "name/Index.h"
#include "expression/Literal.h"
#include "type/Type.h"

namespace shakujo::model {

/**
 * @brief provides IR models.
 */
class IRFactory : public IRFactoryBase {
public:
    /**
     * @brief returns a new name node.
     * @param segments the name segments
     * @return name::SimpleName if segments.size() == 1
     * @return name::QualifiedName if segments.size() > 1
     * @return name::UndefinedName if segments.empty()
     */
    std::unique_ptr<name::Name> Name(std::vector<std::string> segments);

    /**
     * @brief returns a new name node.
     * @param name the name
     * @return name::SimpleName if name.segments.size() == 1
     * @return name::QualifiedName if name.segments.size() > 1
     * @return name::UndefinedName if name.segments.empty()
     */
    std::unique_ptr<name::Name> Name(common::core::Name name) {
        return Name(std::move(name).segments());
    }

    /**
     * @brief returns a new name::SimpleName.
     * @param token name token
     * @return a created node
     * @see ::shakujo::model::name::SimpleName
     */
    std::unique_ptr<name::SimpleName> Name(std::string_view token) {
        return SimpleName(std::string { token });
    }

    /**
     * @brief returns a new name::QualifiedName.
     * @tparam token types
     * @param token the root name
     * @param rest the rest name segments
     * @return a created node
     * @see ::shakujo::model::name::QualifiedName
     */
    template<typename... Args>
    std::unique_ptr<name::QualifiedName> Name(std::string_view token, Args&&... rest) {
        return Name(Name(token), std::forward<Args>(rest)...);
    }

    /**
     * @brief returns a new name::QualifiedName.
     * @param qualifier the name qualifier
     * @param token the name token
     * @return a created node
     * @see ::shakujo::model::name::QualifiedName
     */
    std::unique_ptr<name::QualifiedName> Name(
            std::unique_ptr<name::Name> qualifier,
            std::string_view token) {
        return QualifiedName(std::move(qualifier), SimpleName(std::string { token }));
    }

    /**
     * @brief returns a new name::QualifiedName.
     * @tparam token types
     * @param qualifier the name qualifier
     * @param token the name token
     * @param rest the rest name segments
     * @return a created node
     * @see ::shakujo::model::name::QualifiedName
     */
    template<typename... Args>
    std::unique_ptr<name::QualifiedName> Name(
            std::unique_ptr<name::Name> qualifier,
            std::string_view token,
            Args&&... rest) {
        return Name(Name(std::move(qualifier), token), std::forward<Args>(rest)...);
    }

    using IRFactoryBase::Index;

    /**
     * @brief returns a new name::Index.
     * @param name element name
     * @return a created node
     * @see ::shakujo::model::name::Index
     */
    std::unique_ptr<name::Index> Index(std::unique_ptr<name::SimpleName> name) {
        return Index(std::move(name), name::Index::INVALID_POSITION);
    }

    /**
     * @brief returns a new name::Index.
     * @param position element position
     * @return a created node
     * @see ::shakujo::model::name::Index
     */
    std::unique_ptr<name::Index> Index(std::size_t position) {
        return Index({}, position);
    }

    using IRFactoryBase::Literal;

    /**
     * @brief returns a new expression::Literal.
     * @param type literal type
     * @param value literal value
     * @return a created node
     * @see ::shakujo::model::expression::Literal
     */
    std::unique_ptr<expression::Literal> Literal(
            common::core::Type const* type,
            common::core::Value const* value) {
        std::unique_ptr<expression::Literal> ret = Literal();
        ret->type(common::util::make_clone(type));
        ret->value(common::util::make_clone(value));
        return ret;
    }

    /**
     * @brief returns a new expression::Literal.
     * @param type literal type
     * @param value literal value
     * @return a created node
     * @see ::shakujo::model::expression::Literal
     */
    std::unique_ptr<expression::Literal> Literal(
            common::core::Type&& type,
            common::core::Value&& value) {
        std::unique_ptr<expression::Literal> ret = Literal();
        ret->type(common::util::make_clone(std::move(type)));
        ret->value(common::util::make_clone(std::move(value)));
        return ret;
    }

    /**
     * @brief returns a new expression::Literal from the given value.
     * @param type literal type
     * @param value the literal type
     * @return a created node
     * @see ::shakujo::model::expression::Literal
     */
    template<class T, class V = typename T::value_type>
    std::unique_ptr<expression::Literal> Literal(T const* type, typename V::type value = {}) {
        return Literal(common::util::make_clone(type), std::make_unique<V>(value));
    }

    /**
     * @brief returns a new expression::Literal from the given value.
     * @param type literal type
     * @param value the literal type
     * @return a created node
     * @see ::shakujo::model::expression::Literal
     */
    template<class T, class V = typename T::value_type>
    std::unique_ptr<expression::Literal> Literal(T&& type, typename V::type value = {}) {
        return Literal(common::util::make_clone(std::forward<T>(type)), std::make_unique<V>(value));
    }

    /**
     * @brief returns a type node from the given type info.
     * @param type the type info
     * @return the corresponded type
     */
    std::unique_ptr<type::Type> Type(common::core::Type const* type);

    /**
     * @brief returns a type node from the given type info.
     * @param type the type info
     * @return the corresponded type
     */
    inline std::unique_ptr<type::Type> Type(common::core::Type const& type) {
        return Type(&type);
    }

    using IRFactoryBase::ImplicitCast;

    /**
     * @brief returns a new expression::ImplicitCast.
     * @param type the target type
     * @param operand the operand
     * @return a created node
     * @see expression::ImplicitCast
     */
    std::unique_ptr<expression::ImplicitCast> ImplicitCast(
            common::core::Type const* type,
            std::unique_ptr<expression::Expression> operand) {
        auto ret = ImplicitCast();
        ret->type(common::util::make_clone(type));
        ret->operand(std::move(operand));
        return ret;
    }

    /**
     * @brief returns a new expression::ImplicitCast.
     * @param type the target type
     * @param operand the operand
     * @return a created node
     * @see expression::ImplicitCast
     */
    std::unique_ptr<expression::ImplicitCast> ImplicitCast(
            common::core::Type&& type,
            std::unique_ptr<expression::Expression> operand) {
        auto ret = ImplicitCast();
        ret->type(common::util::make_clone(std::move(type)));
        ret->operand(std::move(operand));
        return ret;
    }

    using IRFactoryBase::TupleTypeElement;

    /**
     * @brief returns a new type::TupleType::Element.
     * @param type element type
     * @return a created node
     * @see type::TupleType::Element
     */
    std::unique_ptr<type::TupleType::Element> TupleTypeElement(
            std::unique_ptr<type::Type> type) {
        return TupleTypeElement(std::unique_ptr<name::SimpleName>(), std::move(type));
    }

    using IRFactoryBase::InsertValuesStatement;

    /**
     * @brief returns a new statement::dml::InsertValuesStatement.
     * @param table table name
     * @param columns destination column specifications
     * @return a created node
     * @see statement::dml::InsertValuesStatement
     */
    std::unique_ptr<statement::dml::InsertValuesStatement> InsertValuesStatement(
            std::unique_ptr<name::Name> table,
            common::util::MoveInitializerList<std::unique_ptr<statement::dml::InsertValuesStatement::Column>> columns) {
        // for compatibility
        return InsertValuesStatement(std::move(table), {}, std::move(columns));
    }

    using IRFactoryBase::UpdateStatement;

    /**
     * @brief returns a new statement::dml::UpdateStatement.
     * @param source target relation
     * @param table table name
     * @param columns destination column specification
     * @return a created node
     * @see statement::dml::UpdateStatement
     */
    std::unique_ptr<statement::dml::UpdateStatement> UpdateStatement(
            std::unique_ptr<expression::Expression> source,
            std::unique_ptr<name::Name> table,
            common::util::MoveInitializerList<std::unique_ptr<statement::dml::UpdateStatement::Column>> columns) {
        return UpdateStatement(std::move(source), std::move(table), {}, std::move(columns));
    }
};
}  // namespace shakujo::model

#endif  // SHAKUJO_MODEL_IR_FACTORY_H_
