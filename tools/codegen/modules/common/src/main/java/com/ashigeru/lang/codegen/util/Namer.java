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
package com.ashigeru.lang.codegen.util;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import javax.annotation.Nonnull;

import com.google.common.collect.ImmutableList;

/**
 * Converts name style.
 * @since WIP
 */
public class Namer {

    private final List<String> segments;

    /**
     * Creates a new instance.
     * @param segments the name segments
     */
    public Namer(@Nonnull List<String> segments) {
        this.segments = segments.stream()
                .map(String::toLowerCase)
                .filter(it -> !it.isEmpty())
                .collect(ImmutableList.toImmutableList());
    }

    /**
     * Returns a namer of the given name.
     * @param name the name
     * @return the namer
     */
    public static Namer parse(String name) {
        return Arrays.stream(name.split("_"))
                .filter(it -> !it.isEmpty())
                .flatMap(s -> {
                    if (s.chars().allMatch(Character::isUpperCase)) {
                        return Stream.of(s);
                    }
                    List<String> elements = new ArrayList<>();
                    int start = 0;
                    for (int i = 0, n = s.length(); i < n; i++) {
                        char c = s.charAt(i);
                        if ('A' <= c && c <= 'Z') {
                            if (start < i) {
                                elements.add(s.substring(start, i));
                                start = i;
                            }
                        }
                    }
                    if (start < s.length()) {
                        elements.add(s.substring(start));
                    }
                    return elements.stream();
                })
                .collect(Collectors.collectingAndThen(
                        ImmutableList.toImmutableList(),
                        Namer::new));
    }

    /**
     * Returns the name segments.
     * @return the name segments
     */
    public List<String> getSegments() {
        return segments;
    }

    /**
     * Returns the name as {@code UPPER_SNAKE_CASE}.
     * @return the formatted name
     */
    public String toUpperSnakeCase() {
        return segments.stream()
                .map(String::toUpperCase)
                .collect(Collectors.joining("_"));
    }

    /**
     * Returns the name as {@code lower_snake_case}.
     * @return the formatted name
     */
    public String toLowerSnakeCase() {
        return segments.stream()
                .collect(Collectors.joining("_"));
    }

    /**
     * Returns the name as {@code UpperCamelCase}.
     * @return the formatted name
     */
    public String toUpperCamelCase() {
        return segments.stream()
                .map(Namer::toCamel)
                .collect(Collectors.joining());
    }

    /**
     * Returns the name as {@code lowerCamelCase}.
     * @return the formatted name
     */
    public String toLowerCamelCase() {
        StringBuilder buf = new StringBuilder();
        buf.append(segments.get(0));
        for (int i = 1, n = segments.size(); i < n; i++) {
            buf.append(toCamel(segments.get(i)));
        }
        return buf.toString();
    }

    private static String toCamel(String s) {
        assert !s.isEmpty();
        StringBuilder buf = new StringBuilder(s);
        buf.setCharAt(0, Character.toUpperCase(s.charAt(0)));
        return buf.toString();
    }

    @Override
    public String toString() {
        return toLowerSnakeCase();
    }
}
