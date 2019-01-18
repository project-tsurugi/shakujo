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
 * Represents a list type.
 * @since WIP
 */
public class ListTypeMirror implements TypeMirror {

    private final TypeMirror componentType;

    /**
     * Creates a new instance.
     * @param componentType the component type
     */
    public ListTypeMirror(@Nonnull TypeMirror componentType) {
        this.componentType = componentType;
    }

    @Override
    public Kind getKind() {
        return Kind.LIST_TYPE;
    }

    /**
     * Returns the component type.
     * @return the component type
     */
    public TypeMirror getComponentType() {
        return componentType;
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(componentType);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        ListTypeMirror other = (ListTypeMirror) obj;
        return Objects.equals(componentType, other.componentType);
    }

    @Override
    public String toString() {
        return String.format("List[%s]", componentType);
    }
}
