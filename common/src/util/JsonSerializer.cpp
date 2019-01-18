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
#include "shakujo/common/util/JsonSerializer.h"

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "shakujo/common/util/utility.h"

namespace shakujo::common::util {

namespace {
enum class Scope {
    TOP,
    OBJECT,
    ARRAY,
    PROPERTY,
};
struct Context {
    Scope scope;
    bool first;
    explicit Context(Scope s) : scope(s), first(true) {}
};
}  // namespace

class JsonSerializer::Impl {
public:
    std::ostream& output_;
    std::string kind_property_;
    bool pretty_print_;
    std::vector<Context> context_;
    int indent_level_ = 0;
    bool line_head_ = true;
    std::ostringstream str_buffer_;

    Impl(std::ostream& output,
            std::string_view kind_property,
            bool pretty_print)
        : output_(output)
        , kind_property_(kind_property)
        , pretty_print_(pretty_print)
    {
        context_.emplace_back(Scope::TOP);
    }

    inline bool pretty() {
        return pretty_print_;
    }

    inline Context& context() {
        return context_.back();
    }

    void enter(Scope next_scope) {
        delimit();
        auto& last = context();
        switch (next_scope) {
        case Scope::TOP:
            error("nested top scope");
        case Scope::OBJECT:
            if (last.scope == Scope::OBJECT) {
                error("bare object in object");
            }
            put_raw("{");
            indent_level(+1);
            break;
        case Scope::ARRAY:
            if (last.scope == Scope::OBJECT) {
                error("bare array in object");
            }
            put_raw("[");
            indent_level(+1);
            break;
        case Scope::PROPERTY:
            if (last.scope != Scope::OBJECT) {
                error("orphan property");
            }
            break;
        }
        context_.emplace_back(next_scope);
    }

    void exit() {
        auto& last = context();
        switch (last.scope) {
        case Scope::TOP:
            error("exit from top");
        case Scope::OBJECT:
            if (!last.first && pretty_print_) {
                line_break();
            }
            indent_level(-1);
            put_raw("}");
            break;
        case Scope::ARRAY:
            if (!last.first && pretty_print_) {
                line_break();
            }
            indent_level(-1);
            put_raw("]");
            break;
        case Scope::PROPERTY:
            if (last.first) {
                error("absent property");
            }
            break;
        }
       context_.pop_back();
    }

    void next_value() {
        auto& current = context();
        switch (current.scope) {
        case Scope::TOP:
        case Scope::ARRAY:
            break;
        case Scope::OBJECT:
            error("bare value on object");
        case Scope::PROPERTY:
            if (!current.first) {
                error("multivalue property");
            }
            break;
        }
        delimit();
    }

    template<typename T>
    inline void put_raw(T const& value) {
        indent();
        append(output_, value);
    }

    void put_string(std::string_view value) {
        indent();
        output_ << '"';
        for (auto c : value) {
            switch (c) {
                case '\0': output_ << '\\' << '0'; break;
                case '"': output_ << '\\' << '"'; break;
                case '\\': output_ << '\\' << '\\'; break;
                case '\r': output_ << '\\' << 'r'; break;
                case '\n': output_ << '\\' << 'n'; break;
                case '\t': output_ << '\\' << 't'; break;
                default: output_ << c; break;
            }
        }
        output_ << '"';
    }

    template<typename T>
    inline void put_string_any(const T& value) {
        str_buffer_.str("");
        str_buffer_.clear(std::stringstream::goodbit);
        append(str_buffer_, value);
        put_string(str_buffer_.str());
    }

    void indent() {
        if (pretty_print_ && line_head_) {
            for (int i = 0; i < indent_level_; i++) {
                output_ << "  ";
            }
            line_head_ = false;
        }
    }

    inline void delimit() {
        auto& current = context_.back();
        switch (current.scope) {
        case Scope::TOP:
            if (!current.first) {
                line_break();
            }
            current.first = false;
            break;
        case Scope::OBJECT:
        case Scope::ARRAY:
            if (!current.first) {
                put_raw(",");
            }
            current.first = false;
            if (pretty()) line_break();
            break;
        case Scope::PROPERTY:
            current.first = false;
            padding();
            break;
        }
    }

    inline void line_break() {
        output_ << std::endl;
        line_head_ = true;
    }

    void padding() {
        if (pretty()) put_raw(" ");
    }

    void indent_level(int offset) {
        if (indent_level_ + offset < 0) error("invalid indent level");
        indent_level_ += offset;
    }

    [[noreturn]] void error(std::string_view message) {
        throw std::logic_error(message.data());
    }
};

JsonSerializer::JsonSerializer(
        std::ostream& output,
        std::string_view kind_property,
        bool pretty_print)
    : impl_(new Impl(output, kind_property, pretty_print))
{}

JsonSerializer::~JsonSerializer() noexcept = default;

void JsonSerializer::enter_object(std::string_view kind) {
    impl_->enter(Scope::OBJECT);
    if (!kind.empty() && !impl_->kind_property_.empty()) {
        enter_property(impl_->kind_property_.c_str());
        value(kind);
        exit_property(impl_->kind_property_.c_str());
    }
}

void JsonSerializer::exit_object(std::string_view) {
    impl_->exit();
}

void JsonSerializer::enter_property(std::string_view name) {
    impl_->enter(Scope::PROPERTY);
    impl_->put_string(name);
    impl_->put_raw(":");
}

void JsonSerializer::exit_property(std::string_view) {
    impl_->exit();
}

void JsonSerializer::enter_array(std::size_t) {
    impl_->enter(Scope::ARRAY);
}

void JsonSerializer::exit_array(std::size_t) {
    impl_->exit();
}

void JsonSerializer::value(std::nullptr_t) {
    impl_->next_value();
    impl_->put_raw("null");
}

void JsonSerializer::value(std::size_t value) {
    impl_->next_value();
    impl_->put_raw(value);
}

void JsonSerializer::value(core::value::Bool::type value) {
    impl_->next_value();
    impl_->put_raw(value);
}

void JsonSerializer::value(core::value::Int::type value) {
    impl_->next_value();
    impl_->put_raw(value);
}

void JsonSerializer::value(core::value::Float::type value) {
    impl_->next_value();
    impl_->put_raw(value);
}

void JsonSerializer::value(std::string_view value) {
    impl_->next_value();
    impl_->put_string(value);
}

void JsonSerializer::value(core::Name const& value) {
    impl_->next_value();
    impl_->put_string_any(value);
}

}  // namespace shakujo::common::util
