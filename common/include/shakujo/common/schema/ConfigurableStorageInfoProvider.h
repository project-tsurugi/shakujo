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

#ifndef SHAKUJO_COMMON_SCHEMA_CONFIGURABLE_STORAGE_INFO_PROVIDER_H_
#define SHAKUJO_COMMON_SCHEMA_CONFIGURABLE_STORAGE_INFO_PROVIDER_H_

#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <utility>
#include <stdexcept>

#include <cstring>

#include "StorageInfoProvider.h"
#include "TableInfo.h"
#include "IndexInfo.h"

#include "../util/utility.h"

namespace shakujo::common::schema {

/**
 * @brief configurable StorageInfoProvider.
 */
class ConfigurableStorageInfoProvider : public StorageInfoProvider {
public:
    /**
     * @brief constructs a new object.
     * @param case_sensitive compare name with case sensitive
     */
    ConfigurableStorageInfoProvider(bool case_sensitive = true) noexcept  // NOLINT
        : entries_(Comparator(case_sensitive))
    {}

    /**
     * @brief adds an table.
     * if the table was overwritten, a reference to existing table will be also changed.
     * @param table the target table information
     * @param overwrite overwrite the existing table
     * @return this
     * @throw std::invalid_argument on overwrite is disabled, if a table with same name already exists in this provider
     */
    ConfigurableStorageInfoProvider& add(TableInfo table, bool overwrite = true) {
        std::unique_lock lock { mutex_ };
        if (auto it = entries_.find(table.name()); it != entries_.end()) {
            if (overwrite) {
                *it->second = std::move(table);
            } else {
                throw std::invalid_argument(util::to_string("table \"", table.name(), "\" already exists"));
            }
        } else {
            auto name = table.name();
            entries_.emplace(std::move(name), std::make_unique<TableInfo>(std::move(table)));
        }
        return *this;
    }

    /**
     * @brief remove a table.
     * @param name the target table name
     * @return true if the table entry is successfully removed
     * @return false if the table entry with the given name doesn't exist
     */
    bool remove(core::Name const& name) {
        std::unique_lock lock { mutex_ };
        if (auto it = entries_.find(name); it != entries_.end()) {
            entries_.erase(it);
            return true;
        }
        return false;
    }

    TableInfo const& find_table(core::Name const& name) const override {
        std::unique_lock lock { mutex_ };
        if (auto it = entries_.find(name); it != entries_.end()) {
            return *it->second;
        }
        return StorageInfoProvider::find_table(name);
    }

    void each_table(std::function<void(TableInfo const&)>const& consumer) const override {
        std::unique_lock lock { mutex_ };
        for(auto& p: entries_) {
            consumer(*p.second);
        }
    }
private:
    class Comparator {
    public:
        explicit constexpr Comparator(bool case_sensitive = true) noexcept : case_sensitive_(case_sensitive) {}
        static constexpr char to_upper(char c) {
            if ('a' <= c && c <= 'z') {
                return static_cast<char>(c - 0x20);
            }
            return c;
        }

        inline bool operator()(core::Name const& a, core::Name const& b) const {
            auto&& as = a.segments();
            auto&& bs = b.segments();
            for (std::size_t i = 0, n = std::min(as.size(), bs.size()); i < n; ++i) {
                int cmp = compare(as[i], bs[i]);
                if(cmp != 0) {
                    return cmp < 0;
                }
            }
            return as.size() < bs.size();
        }

        int compare(std::string_view a, std::string_view b) const {
            if (case_sensitive_) {
                return a.compare(b);
            }
            for (std::size_t i = 0, n = std::min(a.size(), b.size()); i < n; ++i) {
                auto ac = to_upper(a[i]);
                auto bc = to_upper(b[i]);
                if (ac < bc) {
                    return -1;
                }
                if (ac > bc) {
                    return +1;
                }
            }
            if (a.size() < b.size()) {
                return -1;
            }
            if (a.size() > b.size()) {
                return +1;
            }
            return 0;
        }

    private:
        bool case_sensitive_;
    };

    std::map<common::core::Name, std::unique_ptr<TableInfo>, Comparator> entries_;
    mutable std::mutex mutex_ {};
};

}  // namespace shakujo::common::schema

#endif  // SHAKUJO_COMMON_SCHEMA_CONFIGURABLE_STORAGE_INFO_PROVIDER_H_
