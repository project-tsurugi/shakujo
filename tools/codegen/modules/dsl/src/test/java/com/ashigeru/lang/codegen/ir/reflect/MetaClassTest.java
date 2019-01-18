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

import java.util.List;

import org.junit.jupiter.api.Test;

import com.ashigeru.lang.codegen.ir.reflect.MetaClass.Property;

class MetaClassTest {

    @Test
    void simple() {
        MetaClass meta = new T0();
        List<Property<?>> properties = meta.getProperties();
        assertEquals(1, properties.size());
        Property<?> p = properties.get(0);
        assertAll(
                () -> assertEquals("testing", p.name),
                () -> assertEquals(MetaType.of(String.class), p.type),
                () -> assertNull(p.description));
    }

    @Test
    void description() {
        MetaClass meta = new T1();
        List<Property<?>> properties = meta.getProperties();
        assertEquals(1, properties.size());
        Property<?> p = properties.get(0);

        assertEquals("TESTING", p.description);
    }

    @Test
    void multiple() {
        MetaClass meta = new T2();
        List<Property<?>> properties = meta.getProperties();
        assertEquals(3, properties.size());

        // must be ordered
        Property<?> a = properties.get(0);
        Property<?> b = properties.get(1);
        Property<?> c = properties.get(2);

        assertAll(
                () -> assertAll(
                        () -> assertEquals("a", a.name),
                        () -> assertEquals(MetaType.of(Integer.class), a.type)),
                () -> assertAll(
                        () -> assertEquals("b", b.name),
                        () -> assertEquals(MetaType.of(Double.class), b.type)),
                () -> assertAll(
                        () -> assertEquals("c", c.name),
                        () -> assertEquals(MetaType.of(String.class), c.type)));
    }

    static class T0 extends MetaClass {
        Property<String> testing = property();
    }

    static class T1 extends MetaClass {
        @Description("TESTING")
        Property<String> testing = property();
    }

    static class T2 extends MetaClass {
        Property<Integer> a = property();
        Property<Double> b = property();
        Property<String> c = property();
    }
}
