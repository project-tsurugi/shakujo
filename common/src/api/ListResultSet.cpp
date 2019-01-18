/*
 * Copyright 2018-2018 shakujo project.
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
#include "shakujo/common/api/ListResultSet.h"

#include "shakujo/common/core/type/Char.h"

#include "shakujo/common/core/value/Bool.h"
#include "shakujo/common/core/value/Int.h"
#include "shakujo/common/core/value/Float.h"
#include "shakujo/common/core/value/String.h"
#include "shakujo/common/core/value/Null.h"

namespace shakujo::common::api {

template<typename T>
static T validate_range(const core::value::Int* column) {
    if (!column) {
        return {};
    }
    using limits = std::numeric_limits<T>;
    auto v = column->get();
    if (v > limits::max()) {
        throw std::domain_error(util::to_string("exceed numeric limits: ", column, " > ", limits::max()));
    }
    if (v < limits::min()) {
        throw std::domain_error(util::to_string("exceed numeric limits: ", column, " < ", limits::min()));
    }
    return static_cast<T>(v);
}

static bool is_consistent(
        core::Value::Kind value_kind,
        core::Type::Kind type_kind,
        core::Type::Nullity type_nullity) {
    if (value_kind == core::Value::Kind::NULL_) {
        return type_nullity == core::Type::Nullity::NULLABLE;
    }
    switch (value_kind) {
    case core::Value::Kind::BOOL:
        return type_kind == core::Type::Kind::BOOL;
    case core::Value::Kind::INT:
        return type_kind == core::Type::Kind::INT;
    case core::Value::Kind::FLOAT:
        return type_kind == core::Type::Kind::FLOAT;
    case core::Value::Kind::STRING:
        return type_kind == core::Type::Kind::STRING || type_kind == core::Type::Kind::CHAR;
    default:
        return false;
    }
}

ListResultSet::ListResultSet(std::unique_ptr<core::type::Relation> type, std::list<core::value::Tuple> rows)
    : type_(std::move(type)), rows_(std::move(rows))
{
    std::size_t row_number = 0;
    for (auto& row : rows_) {
        if (row.size() > type_->columns().size()) {
            throw std::invalid_argument(util::to_string("too many columns at ", row_number));
        }
        std::size_t column_number = 0;
        for (auto element : row.elements()) {
            auto column_type = type_->at(column_number).type();
            if (!is_consistent(element->kind(), column_type->kind(), column_type->nullity())) {
                throw std::invalid_argument(util::to_string("inconsistent type: ",
                        "row=", row_number, ", ",
                        "column=", column_number, ", ",
                        "value=", *element, ", ",
                        "type=", *column_type));
            }
            ++column_number;
        }
        ++row_number;
    }
}

bool ListResultSet::next() {
    if (rows_.empty()) {
        current_row_ = {};
        return false;
    }
    current_row_ = std::move(rows_.front());
    rows_.pop_front();
    return true;
}

bool ListResultSet::is_null(std::size_t position) const {
    return get_row<core::Value>(position) == nullptr;
}

bool ListResultSet::get_bool(std::size_t position) const {
    auto column = get_row<core::value::Bool>(position);
    return column ? column->get() : false;
}

int32_t ListResultSet::get_int32(std::size_t position) const {
    auto column = get_row<core::value::Int>(position);
    return validate_range<std::int32_t>(column);
}

int64_t ListResultSet::get_int64(std::size_t position) const {
    auto column = get_row<core::value::Int>(position);
    return validate_range<std::int64_t>(column);
}

float ListResultSet::get_float32(std::size_t position) const {
    auto column = get_row<core::value::Float>(position);
    return static_cast<float>(column->get());
}

double ListResultSet::get_float64(std::size_t position) const {
    auto column = get_row<core::value::Float>(position);
    return static_cast<double>(column->get());
}

std::string_view ListResultSet::get_text(std::size_t position) const {
    auto column = get_row<core::value::String>(position);
    if (!column) {
        return {};
    }
    return column->get();
}

}  // namespace shakujo::common::api

