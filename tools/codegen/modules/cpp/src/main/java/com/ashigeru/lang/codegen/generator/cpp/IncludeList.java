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
package com.ashigeru.lang.codegen.generator.cpp;

import java.util.EnumSet;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import javax.annotation.Nonnull;

import com.ashigeru.lang.codegen.ir.model.Name;

/**
 * Represents an include list.
 * @since WIP
 */
public class IncludeList {

    /**
     * Standard namespace.
     */
    public static final Name STD = Name.of("std");

    /**
     * Bool type.
     */
    public static final Name BOOL_TYPE = Name.of("bool");

    /**
     * Int type.
     */
    public static final Name INT_TYPE = STD.resolve("int64_t");

    /**
     * Float type.
     */
    public static final Name FLOAT_TYPE = Name.of("double");

    /**
     * Size type.
     */
    public static final Name SIZE_TYPE = STD.resolve("size_t");

    /**
     * String type.
     */
    public static final Name STRING_TYPE = STD.resolve("string");

    /**
     * Vector type.
     */
    public static final Name VECTOR_TYPE = STD.resolve("vector");

    /**
     * Initializer list type.
     */
    public static final Name INITIALIZER_LIST_TYPE = STD.resolve("initializer_list");

    /**
     * Set type.
     */
    public static final Name SET_TYPE = STD.resolve("set");

    private final Set<Standard> standards = EnumSet.noneOf(Standard.class);

    private final Set<Name> internals = new TreeSet<>();

    /**
     * Returns the standards.
     * @return the standards
     */
    public Set<Standard> getStandards() {
        return Set.copyOf(standards);
    }

    /**
     * Returns the internals.
     * @return the internals
     */
    public List<Name> getInternals() {
        return List.copyOf(internals);
    }

    /**
     * Adds a standard.
     * @param standard the standard kind
     */
    public void add(Standard standard) {
        standards.add(standard);
    }

    /**
     * Adds a name.
     * @param name the name
     */
    public void add(@Nonnull Name name) {
        internals.add(name);
    }

    /**
     * Standard include files.
     * @since WIP
     */
    public enum Standard {

        /**
         * {@code <cstddef>}.
         */
        STDDEF("cstddef"),

        /**
         * {@code <cstdint>}.
         */
        STDINT("cstdint"),

        /**
         * {@code <stdexcept>}.
         */
        STDEXCEPT("stdexcept"),

        /**
         * {@code <utility>}.
         */
        UTILITY("utility"),

        /**
         * {@code <memory>}.
         */
        MEMORY("memory"),

        /**
         * {@code <string>}.
         */
        STRING("string"),

        /**
         * {@code <vector>}.
         */
        VECTOR("vector"),

        /**
         * {@code <set>}.
         */
        SET("set"),

        /**
         * {@code <initializer_list>}.
         */
        INITIALIZER_LIST("initializer_list"),

        /**
         * {@code <iostream>}.
         */
        IOSTREAM("iostream"),

        /**
         * {@code <optional>}.
         */
        OPTIONAL("optional"),

        /**
         * {@code <sstream>}.
         */
        SSTREAM("sstream"),

        /**
         * {@code <type_traits>}.
         */
        TYPE_TRAITS("type_traits"),
        ;

        private final String name;

        Standard(String name) {
            this.name = name;
        }

        /**
         * Returns the name.
         * @return the name
         */
        public String getName() {
            return name;
        }
    }
}
