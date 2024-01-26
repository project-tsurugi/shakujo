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
#include "shakujo/common/schema/StorageInfoProvider.h"

#include "shakujo/common/schema/TableInfo.h"

namespace shakujo::common::schema {

static const TableInfo INVALID;  // NOLINT

TableInfo const& StorageInfoProvider::find_table(core::Name const&) const {
    return INVALID;
}

void StorageInfoProvider::each_table(std::function<void(TableInfo const&)> const&) const {}

}  // namespace shakujo::common::schema

