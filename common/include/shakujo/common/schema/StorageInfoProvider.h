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

#ifndef SHAKUJO_COMMON_SCHEMA_STORAGE_INFO_PROVIDER_H_
#define SHAKUJO_COMMON_SCHEMA_STORAGE_INFO_PROVIDER_H_

#include <functional>
#include "TableInfo.h"

namespace shakujo::common::schema {

/**
 * @brief an interface that provides schema information of the storage.
 * This skeletal implementation always returns invalid table information.
 */
class StorageInfoProvider {
public:
    /**
     * @brief constructs a new object.
     */
    StorageInfoProvider() noexcept = default;

    /**
     * @brief destroys this object.
     */
    virtual ~StorageInfoProvider() noexcept = default;

    StorageInfoProvider(StorageInfoProvider const&) = delete;
    StorageInfoProvider(StorageInfoProvider&&) noexcept = delete;
    StorageInfoProvider& operator=(StorageInfoProvider const&) = delete;
    StorageInfoProvider& operator=(StorageInfoProvider&&) noexcept = delete;

    /**
     * @brief returns table information for the given name.
     * @param name the table name
     * @return the corresponded table information, or invalid table information if it does not exist
     */
    virtual TableInfo const& find_table(core::Name const& name) const;

    /**
     * @brief provides all tables in this provider.
     * @param consumer the destination consumer, which accepts each table info
     */
    virtual void each_table(std::function<void(TableInfo const&)>const& consumer) const;
};
}  // namespace shakujo::common::schema

#endif  // SHAKUJO_COMMON_SCHEMA_STORAGE_INFO_PROVIDER_H_
