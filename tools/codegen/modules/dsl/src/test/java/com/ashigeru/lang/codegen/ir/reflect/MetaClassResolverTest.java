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

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.function.Function;

import org.junit.jupiter.api.Test;

import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.DeclaredTypeMirror;
import com.ashigeru.lang.codegen.ir.model.EnumConstantDeclaration;
import com.ashigeru.lang.codegen.ir.model.EnumTypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.model.TypeMirror;

class MetaClassResolverTest {

    private static final DeclaredTypeMirror STRING_TYPE = new DeclaredTypeMirror(Name.of("std", "string"));

    private static final Function<Class<?>, TypeMirror> TYPES;
    static {
        Map<Class<?>, TypeMirror> map = new HashMap<>();
        map.put(String.class, STRING_TYPE);
        TYPES = map::get;
    }

    private static final Function<Class<?>, Name> TYPE_NAMER = NameUtil.toName()
            .andThen(NameUtil.replacePrefix(
                    NameUtil.toName(MetaClassResolverTest.class),
                    Name.of("testing")));

    private final MetaClassResolver resolver = new MetaClassResolver(TYPES, TYPE_NAMER);

    @Test
    void simple() {
        TypeDeclarationRepository resolved = resolver.resolve(SimpleClass.class);
        assertEquals(1, resolved.getElements().size());
        TypeDeclaration decl = resolved.getElements().get(0);
        assertTrue(decl instanceof ClassDeclaration);
        ClassDeclaration cdecl = (ClassDeclaration) decl;
        assertEquals(Name.of("testing", "SimpleClass"), cdecl.getName());
        assertEquals(Optional.empty(), cdecl.getDescription());
        assertEquals(List.of(), cdecl.getSuperTypes());
        assertEquals(List.of(), cdecl.getMembers());
        assertEquals(Set.of(), cdecl.getAttributes());
    }

    @Test
    void resolve_class() {
        ClassDeclaration decl = resolver.resolve(TestClass.class)
                .find(ClassDeclaration.class, Name.of("testing", "TestClass"))
                .get();
        assertEquals(Optional.of("TEST CLASS"), decl.getDescription());
        assertEquals(List.of(), decl.getSuperTypes());

        assertEquals(2, decl.getMembers().size());
        PropertyDeclaration a = decl.findMember(PropertyDeclaration.class, "a").get();
        PropertyDeclaration b = decl.findMember(PropertyDeclaration.class, "b").get();
        assertAll(
                () -> assertEquals(STRING_TYPE, a.getType()),
                () -> assertEquals(STRING_TYPE.asListType(), b.getType()));
    }

    @Test
    void resolve_enum() {
        EnumTypeDeclaration decl = resolver.resolve(TestEnum.class)
                .find(EnumTypeDeclaration.class, Name.of("testing", "TestEnum"))
                .get();
        assertEquals(Optional.of("TEST ENUM"), decl.getDescription());

        List<EnumConstantDeclaration> members = decl.getMembers();
        assertEquals(3, members.size());
        EnumConstantDeclaration a = members.get(0);
        EnumConstantDeclaration b = members.get(1);
        EnumConstantDeclaration c = members.get(2);

        assertAll(
                () -> assertAll(
                        () -> assertEquals(Name.of("testing", "TestEnum", "A"), a.getName()),
                        () -> assertEquals(Optional.of("AAA"), a.getDescription())),
                () -> assertAll(
                        () -> assertEquals(Name.of("testing", "TestEnum", "B"), b.getName()),
                        () -> assertEquals(Optional.of("BBB"), b.getDescription())),
                () -> assertAll(
                        () -> assertEquals(Name.of("testing", "TestEnum", "C"), c.getName()),
                        () -> assertEquals(Optional.of("CCC"), c.getDescription())));
    }

    @Test
    void attribute_value_class() {
        ClassDeclaration decl = resolve(TestValueClass.class);
        assertEquals(Set.of(ClassDeclaration.Attribute.VALUE), decl.getAttributes());
    }

    @Test
    void attribute_extern() {
        ClassDeclaration decl = resolve(TestExternClass.class);
        assertEquals(Set.of(ClassDeclaration.Attribute.EXTERN), decl.getAttributes());
    }

    @Test
    void subclass() {
        ClassDeclaration decl = resolve(SubClass.class, SimpleClass.class);
        assertEquals(List.of(type(SimpleClass.class)), decl.getSuperTypes());
    }

    @Test
    void refs() {
        ClassDeclaration decl = resolve(Refs.class, SimpleClass.class, SimpleEnum.class);
        PropertyDeclaration a = decl.findMember(PropertyDeclaration.class, "a").get();
        PropertyDeclaration b = decl.findMember(PropertyDeclaration.class, "b").get();
        assertAll(
                () -> assertEquals(type(SimpleClass.class), a.getType()),
                () -> assertEquals(type(SimpleEnum.class), b.getType()));
    }

    @Test
    void inners() {
        ClassDeclaration decl = resolve(Inners.class);
        PropertyDeclaration a = decl.findMember(PropertyDeclaration.class, "a").get();
        PropertyDeclaration b = decl.findMember(PropertyDeclaration.class, "b").get();
        ClassDeclaration c = decl.findMember(
                ClassDeclaration.class, Inners.NestedClass.class.getSimpleName()).get();
        EnumTypeDeclaration e = decl.findMember(
                EnumTypeDeclaration.class, Inners.NestedEnum.class.getSimpleName()).get();
        assertAll(
                () -> assertEquals(c.asTypeMirror(), a.getType()),
                () -> assertTrue(c.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)),
                () -> assertEquals(e.asTypeMirror(), b.getType()));
    }

    private ClassDeclaration resolve(Class<?> target, Class<?>... refs) {
        List<Class<?>> classes = new ArrayList<>();
        classes.add(target);
        Collections.addAll(classes, refs);
        Name name = name(target);
        return resolver.resolve(classes)
                .find(ClassDeclaration.class, name)
                .get();
    }

    private static Name name(Class<?> target) {
        return TYPE_NAMER.apply(target);
    }

    private static TypeMirror type(Class<?> target) {
        return Optional.ofNullable(TYPES.apply(target))
                .orElseGet(() -> new DeclaredTypeMirror(name(target)));
    }

    static class SimpleClass extends MetaClass {
        // no special members
    }

    static class SimpleEnum extends MetaClass {
        // no special members
    }

    @Description("TEST CLASS")
    static class TestClass extends MetaClass {

        @Description("AAA")
        Property<String> a = property();

        @Description("BBB")
        Property<List<String>> b = property();
    }

    @Description("TEST ENUM")
    static enum TestEnum {
        @Description("AAA")
        A,

        @Description("BBB")
        B,

        @Description("CCC")
        C,
    }

    @Value
    static class TestValueClass extends MetaClass {
        // no special members
    }

    @Extern
    static class TestExternClass extends MetaClass {
        // no special members
    }

    @Extends(SimpleClass.class)
    static class SubClass extends MetaClass {
        // no special members
    }

    static class Refs extends MetaClass {
        Property<SimpleClass> a = property();
        Property<SimpleEnum> b = property();
    }

    static class Inners extends MetaClass {
        Property<NestedClass> a = property();
        Property<NestedEnum> b = property();
        static class NestedClass extends MetaClass {
            // no special members
        }
        static enum NestedEnum {
            // no special members
        }
        static class Other {
            // no special members
        }
    }
}
