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

import java.lang.reflect.Field;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.WildcardType;
import java.util.ArrayList;
import java.util.List;

import com.google.common.base.Preconditions;

/**
 * A meta class for declaring IR classes.
 * @since WIP
 * @see Extends
 * @see Description
 */
@SuppressWarnings("hiding")
public abstract class MetaClass {

    private boolean resolved = false;

    private final List<Property<?>> properties = new ArrayList<>();

    /**
     * Returns the properties.
     * @return the properties
     */
    public List<Property<?>> getProperties() {
        resolve();
        return List.copyOf(properties);
    }

    /**
     * Defines a new property.
     * @param <T> the property type
     * @return the created property stub
     */
    protected final <T> Property<T> property() {
        Property<T> result = new Property<>();
        properties.add(result);
        return result;
    }

    /**
     * Resolves this meta-class.
     */
    public void resolve() {
        if (resolved) {
            return;
        }
        for (Field field : getClass().getDeclaredFields()) {
            resolveField(field);
        }
        for (Property<?> property : properties) {
            if (property.name == null || property.type == null) {
                throw new IllegalStateException();
            }
        }
        resolved = true;
    }

    private void resolveField(Field field) {
        Type type = extractPropertyType(field.getGenericType());
        if (type == null) {
            return;
        }
        Property<?> def = extractProperty(field);
        def.reflect = field;
        if (def.name == null) {
            def.withName(field.getName());
        }
        if (def.type == null && !(type instanceof WildcardType)) {
            def.withType(MetaType.of(type));
        }
        if (def.description == null) {
            Util.description(field).ifPresent(def::withDescription);
        }
    }

    private static Type extractPropertyType(Type type) {
        if (type instanceof ParameterizedType) {
            ParameterizedType pType = (ParameterizedType) type;
            Type[] args = pType.getActualTypeArguments();
            if (pType.getRawType() == Property.class && args.length == 1) {
                return pType.getActualTypeArguments()[0];
            }
        }
        return null;
    }

    private Property<?> extractProperty(Field field) {
        Property<?> def = null;
        try {
            field.setAccessible(true);
            def = (Property<?>) field.get(this);
        } catch (ReflectiveOperationException e) {
            throw new IllegalStateException(field.toString(), e);
        }
        Preconditions.checkNotNull(def, field);
        return def;
    }

    /**
     * Represents a meta property.
     * @param <T> the property type
     * @since WIP
     */
    public static class Property<T> {

        String name;

        MetaType type;

        String description;

        Field reflect;

        /**
         * Sets the name.
         * @param <U> the property type
         * @param name the name
         * @return this
         */
        public <U> Property<U> withName(String name) {
            this.name = name;
            return adopt();
        }

        /**
         * Sets the type.
         * @param <U> the property type
         * @param type the property type
         * @return this
         */
        public <U> Property<U>  withType(MetaType type) {
            this.type = type;
            return adopt();
        }

        /**
         * Sets the description.
         * @param <U> the property type
         * @param description the description
         * @return this
         */
        public <U> Property<U> withDescription(String description) {
            this.description = description;
            return adopt();
        }

        @SuppressWarnings("unchecked")
        private <U> Property<U> adopt() {
            return (Property<U>) this;
        }
    }
}
