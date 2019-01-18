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
#ifndef SHAKUJO_COMMON_API_LIST_RESULT_SET_H_
#define SHAKUJO_COMMON_API_LIST_RESULT_SET_H_

#include <memory>
#include <initializer_list>
#include <limits>
#include <list>
#include <vector>
#include <type_traits>

#include "ResultSet.h"

#include "shakujo/common/core/Value.h"
#include "shakujo/common/core/value/Tuple.h"
#include "shakujo/common/util/utility.h"

namespace shakujo::common::api {

/**
 * @brief a list based implementation of ResultSet.
 */
class ListResultSet : public ResultSet {
private:
    std::unique_ptr<core::type::Relation> type_;
    std::list<core::value::Tuple> rows_;
    core::value::Tuple current_row_;

public:
    /**
     * @brief constructs a new object.
     * @param type the type of relation
     * @param rows the actual values of each row
     * @throws std::domain_error if the rows contains inconsistent value
     */
    ListResultSet(std::unique_ptr<core::type::Relation> type, std::list<core::value::Tuple> rows);

    /**
     * @brief constructs a new object.
     * @param type the type of relation
     * @param rows the actual values of each row
     * @throws std::domain_error if the rows contains inconsistent value
     */
    ListResultSet(core::type::Relation const* type, std::list<core::value::Tuple> rows)
        : ListResultSet(util::make_clone(type), std::move(rows))
    {}

    /**
     * @brief constructs a new object.
     * @param type the type of relation
     * @param rows the actual values of each row
     * @throws std::domain_error if the rows contains inconsistent value
     */
    ListResultSet(core::type::Relation&& type, std::list<core::value::Tuple> rows)
        : ListResultSet(util::make_clone(type), std::move(rows))
    {}

    inline core::type::Relation const* type() override {
        return type_.get();
    }

    inline std::size_t size() const override {
        return type_->columns().size();
    }

    bool next() override;
    bool is_null(std::size_t position) const override;
    bool get_bool(std::size_t position) const override;
    int32_t get_int32(std::size_t position) const override;
    int64_t get_int64(std::size_t position) const override;
    float get_float32(std::size_t position) const override;
    double get_float64(std::size_t position) const override;
    using ResultSet::get_text;
    std::string_view get_text(std::size_t position) const override;

private:
    template<typename T>
    const T* get_row(std::size_t position) const {
        if (!current_row_.is_valid()) {
            throw std::logic_error("current row is not initialized");
        }
        if (position >= size()) {
            throw std::out_of_range(util::to_string("position=", position, ", columns=", rows_.size()));
        }
        if (position >= current_row_.size()) {
            return nullptr;
        }
        auto column = current_row_[position];
        if (column->kind() == core::Value::Kind::NULL_) {
            return nullptr;
        }
        if (auto ptr = dynamic_cast<const T*>(column)) {
            return ptr;
        }
        throw std::domain_error(util::to_string("cannot cast position=", position));
    }
};

}  // namespace shakujo::common::api

#endif  // SHAKUJO_COMMON_API_LIST_RESULT_SET_H_
