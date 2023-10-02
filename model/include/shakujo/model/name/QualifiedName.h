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
// DON'T EDIT THIS GENERATED FILE //
#ifndef SHAKUJO_MODEL_NAME_QUALIFIED_NAME_H_
#define SHAKUJO_MODEL_NAME_QUALIFIED_NAME_H_

#include <utility>
#include <memory>

#include "shakujo/model/name/Name.h"
#include "shakujo/model/name/NameKind.h"
#include "shakujo/model/name/SimpleName.h"

namespace shakujo::model::name {
/**
 * @brief Represents qualified name.
 */
class QualifiedName
        : public Name {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    explicit QualifiedName(std::unique_ptr<Impl>) noexcept;

public:
    /**
     * @brief Constructs a new empty object.
     */
    QualifiedName();

    /**
     * @brief Destroys this object.
     */
    ~QualifiedName() noexcept override;

    /**
     * @brief Copy-constructs a new object.
     * @param other the source object
     */
    QualifiedName(QualifiedName const& other) = delete;

    /**
     * @brief Copy-assigns to this object.
     * @param other the source object
     * @return this
     */
    QualifiedName& operator=(QualifiedName const& other) = delete;

    /**
     * @brief Move-constructs a new object.
     * @param other the source object
     */
    QualifiedName(QualifiedName&& other) noexcept;

    /**
     * @brief Move-assigns to this object.
     * @param other the source object
     * @return this
     */
    QualifiedName& operator=(QualifiedName&& other) noexcept;

public:
    /**
     * @brief Returns qualifier.
     * @return qualifier.
     */
    Name* qualifier();

    /**
     * @brief Returns qualifier.
     * @return qualifier.
     */
    inline Name const* qualifier() const {
        return const_cast<QualifiedName*>(this)->qualifier();
    }

    /**
     * @brief Sets qualifier.
     * @param qualifier qualifier
     * @return this
     */
    QualifiedName& qualifier(std::unique_ptr<Name> qualifier);

    /**
     * @brief Releases qualifier from this node.
     * @return the released node
     */
    std::unique_ptr<Name> release_qualifier();

    /**
     * @brief Returns name.
     * @return name.
     */
    SimpleName* name();

    /**
     * @brief Returns name.
     * @return name.
     */
    inline SimpleName const* name() const {
        return const_cast<QualifiedName*>(this)->name();
    }

    /**
     * @brief Sets name.
     * @param name name
     * @return this
     */
    QualifiedName& name(std::unique_ptr<SimpleName> name);

    /**
     * @brief Releases name from this node.
     * @return the released node
     */
    std::unique_ptr<SimpleName> release_name();

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    QualifiedName* clone() const & override;

    /**
     * @brief Returns a copy of this object.
     * @return a clone of this
     */
    QualifiedName* clone() && override;

public:
    /**
     * @brief the node kind.
     */
    static inline constexpr auto tag = NameKind::QUALIFIED_NAME;

    /**
     * @brief Returns the node kind.
     * @return the node kind
     * @see tag
     */NameKind kind() const override {
        return tag;
    }

};
}  // namespace shakujo::model::name

#endif  // SHAKUJO_MODEL_NAME_QUALIFIED_NAME_H_
