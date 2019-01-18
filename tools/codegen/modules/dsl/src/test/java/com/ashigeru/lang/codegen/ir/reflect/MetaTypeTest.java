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

import static org.junit.jupiter.api.Assertions.*;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.List;

import org.junit.jupiter.api.Test;

class MetaTypeTest {

    @Test
    void simple() {
        MetaType meta = MetaType.of(String.class);
        assertAll(
                () -> assertEquals(MetaType.Kind.SCALAR, meta.getKind()),
                () -> assertEquals(String.class, meta.getRaw()));
    }

    @Test
    void listOf() {
        MetaType meta = MetaType.listOf(String.class);
        assertAll(
                () -> assertEquals(MetaType.Kind.LIST, meta.getKind()),
                () -> assertEquals(List.class, meta.getRaw()),
                () -> assertEquals(MetaType.of(String.class), meta.getArgument(0)));
    }

    @Test
    void of_list() {
        MetaType meta = MetaType.of(new TRef<List<Integer>>() {
            //
        }.resolve());
        assertEquals(MetaType.listOf(Integer.class), meta);
    }

    @Test
    void of_unknown() {
        assertThrows(
                IllegalArgumentException.class,
                () -> MetaType.of(new TRef<List<?>>() {
                    //
                }.resolve()));
    }

    @SuppressWarnings("unused")
    static abstract class TRef<T> {
        Type resolve() {
            Type t = getClass().getGenericSuperclass();
            assert t instanceof ParameterizedType;
            ParameterizedType pt = (ParameterizedType) t;
            assert pt.getRawType() == TRef.class;
            assert pt.getActualTypeArguments().length == 1;
            return pt.getActualTypeArguments()[0];
        }
    }
}
