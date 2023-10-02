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
#ifndef SHAKUJO_ANALYZER_SCOPE_TABLE_H_
#define SHAKUJO_ANALYZER_SCOPE_TABLE_H_

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <stdexcept>
#include <sstream>
#include <utility>
#include <vector>

#include "Result.h"
#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/SimpleName.h"
#include "shakujo/model/name/QualifiedName.h"

#include "shakujo/common/util/utility.h"

namespace shakujo::analyzer::scope {

using common::util::dynamic_pointer_cast;

/**
 * @brief represents a symbol table.
 * @tparam T element kind
 */
template <typename T>
class Table final {
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
        inline bool operator()(std::string const& a, std::string const& b) const {
            if (case_sensitive_) {
                return a < b;
            }
            for (std::size_t i = 0, n = std::min(a.size(), b.size()); i < n; ++i) {
                auto ac = to_upper(a[i]);
                auto bc = to_upper(b[i]);
                if (ac < bc) {
                    return true;
                }
                if (ac > bc) {
                    return false;
                }
            }
            return a.size() < b.size();
        }

    private:
        bool case_sensitive_;
    };

    class VectorComparator {
    public:
        explicit constexpr VectorComparator(bool case_sensitive = true) noexcept : element_(Comparator(case_sensitive)) {}
        inline bool operator()(std::vector<std::string> const& a, std::vector<std::string> const& b) const {
            for (std::size_t i = 0, n = std::min(a.size(), b.size()); i < n; ++i) {
                auto&& as = a[i];
                auto&& bs = b[i];
                if (element_(as, bs)) {
                    return true;
                }
                if (element_(bs, as)) {
                    return false;
                }
            }
            return a.size() < b.size();
        }

    private:
        Comparator element_;
    };

    std::map<std::string, std::shared_ptr<T>, Comparator> simple_elements_;
    std::map<std::vector<std::string>, std::shared_ptr<T>, VectorComparator> qualified_elements_;

    Result<T> find_simple(model::name::SimpleName const* name) const {
        auto it = simple_elements_.find(name->token());
        if (it != simple_elements_.end()) {
            return Result<T>(name, it->second);
        }
        return {};
    }

    Result<T> find_qualified(model::name::QualifiedName const* name) const {
        auto segments = name->segments();
        model::name::QualifiedName const* current = name;
        while (true) {
            auto it = qualified_elements_.find(segments);
            if (it != qualified_elements_.end()) {
                return Result<T>(current, it->second);
            }
            model::name::Name const* qualifier = current->qualifier();
            switch (qualifier->kind()) {
                case model::name::SimpleName::tag:
                    return find_simple(dynamic_pointer_cast<model::name::SimpleName>(qualifier));
                case model::name::QualifiedName::tag:
                    current = dynamic_pointer_cast<model::name::QualifiedName>(qualifier);
                    break;
                default:
                    return {};
            }
            segments.pop_back();
        }
    }

public:

    /**
     * @brief constructs a new object.
     * @param case_sensitive compare name with case sensitive
     */
    explicit Table(bool case_sensitive = true) noexcept
        : simple_elements_(Comparator(case_sensitive))
        , qualified_elements_(VectorComparator(case_sensitive))
    {}

    /**
     * @brief destructs this object.
     */
    ~Table() noexcept = default;

    /**
     * @brief Copy constructs a new object.
     * @param other the source object
     */
    Table(const Table& other) = delete;

    /**
     * @brief Move constructs a new object.
     * @param other the source object
     */
    Table(Table&& other) noexcept = default;  // NOLINT

    /**
     * @brief Copy assigns to this object.
     * @param other the source object
     * @return this
     */
    Table& operator=(Table const& other) = delete;

    /**
     * @brief Move assigns to this object.
     * @param other the source object
     * @return this
     */
    Table& operator=(Table&& other) noexcept = default;  // NOLINT

    /**
     * @brief returns an element.
     * @param name the element name
     * @return the corresponded element, or null if it is not in this table
     */
    Result<T> find(model::name::Name const* name) const {
        switch (name->kind()) {
        case model::name::SimpleName::tag:
            return find_simple(dynamic_pointer_cast<model::name::SimpleName>(name));
        case model::name::QualifiedName::tag:
            return find_qualified(dynamic_pointer_cast<model::name::QualifiedName>(name));
        default:
            return {};
        }
    }

    /**
     * @brief returns whether or not this table contains an entry for the given name.
     * @param name the element name
     * @return true if this contains entry for the name
     * @return false otherwise
     */
    bool contains(model::name::Name const* name) const {
        switch (name->kind()) {
        case model::name::SimpleName::tag:
            return contains(dynamic_pointer_cast<model::name::SimpleName>(name)->token());
        case model::name::QualifiedName::tag:
            return contains(name->segments());
        default:
            return {};
        }
    }

    /**
     * @brief returns whether or not this table contains an entry for the given name.
     * @param name the element name
     * @return true if this contains entry for the name
     * @return false otherwise
     */
    bool contains(std::string const& name) const {
        return simple_elements_.find(name) != simple_elements_.end();
    }

    /**
     * @brief returns whether or not this table contains an entry for the given name.
     * @param name the element name
     * @return true if this contains entry for the name
     * @return false otherwise
     */
    bool contains(std::vector<std::string> const& name) const {
        if (name.size() == 1) {
            return contains(name[0]);
        }
        return qualified_elements_.find(name) != qualified_elements_.end();
    }

    /**
     * @brief returns an element.
     * @param name the element name
     * @return the corresponded element, or empty pointer if it does not exist
     */
    std::shared_ptr<T> get(std::string const& name) const {
        auto it = simple_elements_.find(name);
        if (it == simple_elements_.end()) {
            return {};
        }
        return it->second;
    }

    /**
     * @brief returns an element.
     * @param name the element name
     * @return the corresponded element, or empty pointer if it does not exist
     */
    std::shared_ptr<T> get(std::vector<std::string> const& name) const {
        if (name.size() == 1) {
            return get(name[0]);
        }
        auto it = qualified_elements_.find(name);
        if (it == qualified_elements_.end()) {
            return {};
        }
        return it->second;
    }

    /**
     * @brief adds an element
     * @param name the element name
     * @param element the target element
     * @param overwrite true to overwrite duplicated entry, otherwise raise if the name was conflict
     * @return this
     * @throws if the given element name already exists in this table
     */
    Table& put(std::string name, std::shared_ptr<T> element, bool overwrite = false) {
        auto it = simple_elements_.find(name);
        if (it == simple_elements_.end()) {
            simple_elements_.emplace(std::move(name), std::move(element));
        } else {
            if (overwrite) {
                it->second = std::move(element);
            } else {
                std::ostringstream ss;
                ss << "conflict element name \"" << name << "\"";
                throw std::runtime_error(ss.str());
            }
        }
        return *this;
    }

    /**
     * @brief adds an element
     * @param name the element name
     * @param element the target element
     * @param overwrite true to overwrite duplicated entry, otherwise raise if the name was conflict
     * @return this
     * @throws if the given element name already exists in this table
     */
    Table& put(std::vector<std::string> name, std::shared_ptr<T> element, bool overwrite = false) {
        if (name.size() == 1) {
            return put(std::move(name[0]), element, overwrite);
        }
        auto it = qualified_elements_.find(name);
        if (it == qualified_elements_.end()) {
            qualified_elements_.emplace(std::move(name), std::move(element));
        } else {
            if (overwrite) {
                it->second = std::move(element);
            } else {
                std::ostringstream ss;
                ss << "conflict element name \"";
                bool cont = false;
                for (auto &s : name) {
                    if (cont) ss << "::";
                    ss << s;
                    cont = true;
                }
                ss << "\"";
                throw std::runtime_error(ss.str());
            }
        }
        return *this;
    }
};
}  // namespace shakujo::analyzer::scope

#endif  //SHAKUJO_ANALYZER_SCOPE_TABLE_H_
