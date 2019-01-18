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
package com.ashigeru.lang.codegen.ir.reflect;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

/**
 * Represents a meta type.
 * @since WIP
 */
public final class MetaType {

    private final Kind kind;

    private final Class<?> raw;

    private final List<MetaType> arguments;

    private MetaType(Kind kind, Class<?> raw, List<MetaType> arguments) {
        this.kind = kind;
        this.raw = raw;
        this.arguments = arguments;
    }

    /**
     * Converts a reflect type into meta type.
     * @param type the reflect type
     * @return the corresponded meta type
     */
    public static MetaType of(Type type) {
        if (type instanceof Class<?>) {
            return new MetaType(Kind.SCALAR, (Class<?>) type, List.of());
        } else if (type instanceof ParameterizedType) {
            ParameterizedType p = (ParameterizedType) type;
            if (p.getRawType() == List.class && p.getActualTypeArguments().length == 1) {
                return listOf(p.getActualTypeArguments()[0]);
            }
        }
        throw new IllegalArgumentException(type.toString());
    }

    /**
     * Converts a reflect type into meta list type.
     * @param componentType the list component type
     * @return the corresponded meta type
     */
    public static MetaType listOf(Type componentType) {
        return new MetaType(Kind.LIST, List.class, List.of(MetaType.of(componentType)));
    }

    /**
     * Returns the kind of this type.
     * @return the type kind
     */
    public Kind getKind() {
        return kind;
    }

    /**
     * Returns the raw type
     * @return the raw type
     */
    public Class<?> getRaw() {
        return raw;
    }

    /**
     * Returns the type argument.
     * @param index the argument index
     * @return the type argument
     */
    public MetaType getArgument(int index) {
        return arguments.get(index);
    }

    @Override
    public int hashCode() {
        return Objects.hash(kind, raw, arguments);
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
        MetaType other = (MetaType) obj;
        return Objects.equals(kind, other.kind)
                && Objects.equals(raw, other.raw)
                && Objects.equals(arguments, other.arguments);
    }

    @Override
    public String toString() {
        if (arguments.isEmpty()) {
            return raw.getName();
        } else {
            return String.format("%s[%s]",
                    raw.getName(),
                    arguments.stream().map(MetaType::toString).collect(Collectors.joining(", ")));
        }
    }

    /**
     * Represents a type kind.
     * @since WIP
     */
    public enum Kind {

        /**
         * scalar type.
         */
        SCALAR,

        /**
         * 1-D list type.
         */
        LIST,
    }
}
