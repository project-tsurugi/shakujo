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

/**
 * An abstract interface of declarations of the IR models.
 * @since WIP
 */
public interface Declaration extends Model {

    /**
     * Returns the qualified name of this declaration.
     * @return the qualified name
     */
    Name getName();

    /**
     * Returns the description of this declaration.
     * @return the description
     */
    default Optional<String> getDescription() {
        return Optional.empty();
    }

    /**
     * Returns whether this is a member of the specified declaration.
     * @param other the target declaration
     * @return {@code true} if this is a member of the target declaration, otherwise {@code false}
     */
    default boolean isMemberOf(@Nonnull Declaration other) {
        Name name = getName();
        if (name instanceof QualifiedName) {
            return other.getName().equals(((QualifiedName) name).getQualifier());
        }
        return false;
    }
}
