/*
 * Copyright 2018-2023 Project Tsurugi.
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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_STATEMENT_LOG_STATEMENT_H_
#define SHAKUJO_MODEL_STATEMENT_LOG_STATEMENT_H_

#include <utility>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "shakujo/model/expression/Expression.h"
#include "shakujo/model/statement/Statement.h"
#include "shakujo/model/statement/StatementKind.h"

namespace shakujo::model::statement {
/**
 * @brief Represents logging statement.
 */
class LogStatement
        : public Statement {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit LogStatement(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Represents log level.
     */
    enum class Level {
        /**
         * @brief trace level.
         */
        TRACE,

        /**
         * @brief debug level.
         */
        DEBUG,

        /**
         * @brief information level.
         */
        INFO,

        /**
         * @brief message level.
         */
        MESSAGE,

        /**
         * @brief warning level.
         */
        WARNING,

        /**
         * @brief error level.
         */
        ERROR,

        /**
         * @brief critical level.
         */
        CRITICAL,

    };

public:
    /**
     * @brief Constructs a new empty object.
     */
    LogStatement();

    /**
     * @brief Destroys this object.
     */
    ~LogStatement() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    LogStatement(LogStatement const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    LogStatement& operator=(LogStatement const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    LogStatement(LogStatement&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    LogStatement& operator=(LogStatement&& other) noexcept;

public:
    /**
     * @brief Returns log level.
     * @return log level.
     */
    LogStatement::Level level() const;

    /**
     * @brief Sets log level.
     * @param level log level
     * @return this
     */
    LogStatement& level(LogStatement::Level level);

    /**
     * @brief Returns log message.
     * @return log message.
     */
    expression::Expression* value();

    /**
     * @brief Returns log message.
     * @return log message.
     */
    inline expression::Expression const* value() const {
        return const_cast<LogStatement*>(this)->value();
    }

    /**
     * @brief Sets log message.
     * @param value log message
     * @return this
     */
    LogStatement& value(std::unique_ptr<expression::Expression> value);

    /**
     * @brief Releases log message from this node.
     * @return the released node
     */
    std::unique_ptr<expression::Expression> release_value();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    LogStatement* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    LogStatement* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = StatementKind::LOG_STATEMENT;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */StatementKind kind() const override {
        return tag;
    }

};
/**
 * @brief returns string representation of the given value.
 * @param value the target enum constant
 * @return string representation
 * @see LogStatement::Level
 */
inline constexpr std::string_view to_string_view(LogStatement::Level value) {
    switch (value) {
        case LogStatement::Level::TRACE: return "TRACE";
        case LogStatement::Level::DEBUG: return "DEBUG";
        case LogStatement::Level::INFO: return "INFO";
        case LogStatement::Level::MESSAGE: return "MESSAGE";
        case LogStatement::Level::WARNING: return "WARNING";
        case LogStatement::Level::ERROR: return "ERROR";
        case LogStatement::Level::CRITICAL: return "CRITICAL";
    }
    return "(unknown)";
}

/**
 * @brief appends short name into the given output stream.
 * @param out the target output stream
 * @param value the target enum constant
 * @return the output stream
 * @see LogStatement::Level
 */
inline std::ostream& operator<<(std::ostream& out, LogStatement::Level value) {
    return out << to_string_view(value);
}

}  // namespace shakujo::model::statement

#endif  // SHAKUJO_MODEL_STATEMENT_LOG_STATEMENT_H_
