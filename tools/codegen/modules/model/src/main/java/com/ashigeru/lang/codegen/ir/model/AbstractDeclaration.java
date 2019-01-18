/**
 * Copyright 2011-2018 Asakusa Framework Team.
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
package com.ashigeru.lang.codegen.ir.model;

import java.util.Optional;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;

/**
 * An abstract implementation of {@link Declaration}.
 * @since WIP
 */
public abstract class AbstractDeclaration extends AbstractModel implements Declaration {

    private final Name name;

    private String description;

    /**
     * Creates a new instance.
     * @param name the declaration name
     */
    public AbstractDeclaration(@Nonnull Name name) {
        this.name = name;
    }

    @Override
    public Name getName() {
        return name;
    }

    /**
     * Sets the description.
     * @param description the description
     */
    public void setDescription(@Nullable String description) {
        this.description = description;
    }

    @Override
    public Optional<String> getDescription() {
        return Optional.ofNullable(description)
                .or(() -> Declaration.super.getDescription());
    }

    @Override
    public String toString() {
        return String.format("%s:%s", getClass().getSimpleName(), name);
    }
}
