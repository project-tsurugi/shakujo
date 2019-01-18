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

#ifndef SHAKUJO_COMMON_SCHEMA_CONFIGURABLE_STORAGE_INFO_PROVIDER_H_
#define SHAKUJO_COMMON_SCHEMA_CONFIGURABLE_STORAGE_INFO_PROVIDER_H_

#include <utility>
#include <vector>

#include "StorageInfoProvider.h"
#include "TableInfo.h"

namespace shakujo::common::schema {

/**
 * @brief configurable StorageInfoProvider.
 */
class ConfigurableStorageInfoProvider : public StorageInfoProvider {
private:
    std::vector<TableInfo> entries_;

public:
    /**
     * @brief adds an table.
     * @param table the target table information
     * @return this
     */
    ConfigurableStorageInfoProvider& add(TableInfo table) {
        entries_.push_back(std::move(table));
        return *this;
    }

    TableInfo const& find_table(core::Name const& name) const override {
        for (auto& table : entries_) {
            if (name == table.name()) {
                return table;
            }
        }
        return StorageInfoProvider::find_table(name);
    }
};

}  // namespace shakujo::common::schema

#endif  // SHAKUJO_COMMON_SCHEMA_CONFIGURABLE_STORAGE_INFO_PROVIDER_H_
