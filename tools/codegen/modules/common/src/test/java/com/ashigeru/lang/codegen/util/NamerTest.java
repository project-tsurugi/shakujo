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

import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.Test;

import com.ashigeru.lang.codegen.util.Namer;

class NamerTest {

    @Test
    void simple() {
        Namer namer = Namer.parse("hello");
        assertAll(
                () -> assertEquals("hello", namer.toLowerSnakeCase()),
                () -> assertEquals("HELLO", namer.toUpperSnakeCase()),
                () -> assertEquals("hello", namer.toLowerCamelCase()),
                () -> assertEquals("Hello", namer.toUpperCamelCase()));
    }

    @Test
    void multiple_segments() {
        Namer namer = Namer.parse("hello_world");
        assertAll(
                () -> assertEquals("hello_world", namer.toLowerSnakeCase()),
                () -> assertEquals("HELLO_WORLD", namer.toUpperSnakeCase()),
                () -> assertEquals("helloWorld", namer.toLowerCamelCase()),
                () -> assertEquals("HelloWorld", namer.toUpperCamelCase()));
    }

    @Test
    void parse_upper_camel() {
        Namer namer = Namer.parse("HelloWorld");
        assertEquals("hello_world", namer.toLowerSnakeCase());
    }

    @Test
    void parse_lower_camel() {
        Namer namer = Namer.parse("helloWorld");
        assertEquals("hello_world", namer.toLowerSnakeCase());
    }

    @Test
    void parse_upper_snake() {
        Namer namer = Namer.parse("HELLO_WORLD");
        assertEquals("hello_world", namer.toLowerSnakeCase());
    }

    @Test
    void parse_lower_snake() {
        Namer namer = Namer.parse("hello_world");
        assertEquals("hello_world", namer.toLowerSnakeCase());
    }

    @Test
    void parse_with_trailing_numbers() {
        Namer namer = Namer.parse("hello12_world34");
        assertEquals("hello12_world34", namer.toLowerSnakeCase());
    }
}
