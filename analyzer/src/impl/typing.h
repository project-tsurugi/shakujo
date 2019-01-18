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
#ifndef SHAKUJO_ANALYZER_IMPL_TYPING_H_
#define SHAKUJO_ANALYZER_IMPL_TYPING_H_

#include <memory>
#include <string>

#include "shakujo/common/core/Type.h"
#include "shakujo/model/type/Type.h"
#include "shakujo/analyzer/binding/ExpressionBinding.h"

namespace shakujo::analyzer::impl::typing {

std::string to_string(common::core::Type const*);

inline bool is_nullable(common::core::Type const* type) {
    return type->nullity() == common::core::Type::Nullity::NULLABLE;
}

bool is_atom(common::core::Type const*);
bool is_boolean(common::core::Type const*);
bool is_numeric(common::core::Type const*);
bool is_integral(common::core::Type const*);
bool is_textual(common::core::Type const*);

bool is_boolean_convertible(common::core::Type const*);

bool is_equality_comparable(common::core::Type const*, common::core::Type const*);
bool is_order_comparable(common::core::Type const*, common::core::Type const*);

std::unique_ptr<common::core::Type> binary_promotion(common::core::Type const*, common::core::Type const*);
std::unique_ptr<common::core::Type> nullity(common::core::Type const*, common::core::Type::Nullity);

bool is_assignment_convertible(common::core::Type const*, binding::ExpressionBinding const&);

std::unique_ptr<common::core::Type> convert(model::type::Type const*);

}  // namespace shakujo::analyzer::impl::typing

#endif  //SHAKUJO_ANALYZER_IMPL_TYPING_H_
