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

import java.util.Objects;

import javax.annotation.Nonnull;

/**
 * Represents a declared type.
 * @since WIP
 */
public class DeclaredTypeMirror implements TypeMirror {

    private final Name name;

    /**
     * Creates a new instance.
     * @param name the type name
     */
    public DeclaredTypeMirror(@Nonnull Name name) {
        this.name = name;
    }

    @Override
    public Kind getKind() {
        return Kind.DECLARED_TYPE;
    }

    /**
     * Returns the name.
     * @return the name
     */
    public Name getName() {
        return name;
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(name);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        DeclaredTypeMirror other = (DeclaredTypeMirror) obj;
        return Objects.equals(name, other.name);
    }

    @Override
    public String toString() {
        return name.toString();
    }
}
