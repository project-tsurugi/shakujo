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
#include "shakujo/model/name/Name.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <shakujo/model/name/Name.h>


#include "shakujo/common/core/Name.h"
#include "shakujo/model/name/ConstNameVisitor.h"

namespace shakujo::model::name {

bool Name::operator==(Name const& other) const {
    if (kind() != other.kind()) {
        return false;
    }
    class Eq : public ConstNameVisitor<bool, Name const*> {
    public:
        bool visit(SimpleName const* node, Name const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<SimpleName const*>(other);
            return node->token() == that->token();
        }

        bool visit(QualifiedName const* node, Name const* other) override {
            if (node->kind() != other->kind()) {
                return false;
            }
            auto that = dynamic_cast<QualifiedName const*>(other);
            return dispatch(node->name(), that->name())
                && dispatch(node->qualifier(), that->qualifier());
        }
    };
    return Eq{}.dispatch(this, &other);
}

std::size_t Name::size() const {
    std::size_t ret = 0;
    Name const* current = this;
    while (current) {
        switch (current->kind()) {
        case NameKind::SIMPLE_NAME:
            return ret + 1;
        case NameKind::QUALIFIED_NAME:
            ret++;
            current = dynamic_cast<const QualifiedName*>(current)->qualifier();
            break;
        }
    }
    return 0;
}

std::vector<std::string> Name::segments() const {
    class Collector : public ConstNameVisitor<void, std::vector<std::string>&> {
    public:
        void visit(SimpleName const* node, std::vector<std::string>& out) override {
            out.push_back(node->token());
        }

        void visit(QualifiedName const* node, std::vector<std::string>& out) override {
            dispatch(node->qualifier(), out);
            dispatch(node->name(), out);
        }
    };
    std::vector<std::string> ret;
    Collector{}.dispatch(this, ret);
    return ret;
}

Name::operator common::core::Name() const {
    return common::core::Name(segments());
}

std::ostream& operator<<(std::ostream& out, Name const& value) {
    class Printer : public ConstNameVisitor<void, std::ostream&> {
    public:
        void visit(SimpleName const* node, std::ostream& out) override {
            out << node->token();
        }

        void visit(QualifiedName const* node, std::ostream& out) override {
            dispatch(node->qualifier(), out);
            out << "::";
            dispatch(node->name(), out);
        }
    };
    Printer{}.dispatch(&value, out);
    return out;
}

}  // namespace shakujo::model::name
