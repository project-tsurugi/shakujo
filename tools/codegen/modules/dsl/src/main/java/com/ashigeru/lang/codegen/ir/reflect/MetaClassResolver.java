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

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Collection;
import java.util.Comparator;
import java.util.Deque;
import java.util.HashSet;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.function.Function;
import java.util.stream.Collectors;

import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.DeclaredTypeMirror;
import com.ashigeru.lang.codegen.ir.model.EnumConstantDeclaration;
import com.ashigeru.lang.codegen.ir.model.EnumTypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PackageDeclaration;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.SimpleName;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.model.TypeMirror;
import com.google.common.base.Preconditions;

/**
 * Resolves declared IR classes.
 * @since WIP
 */
public class MetaClassResolver {

    final Function<? super Class<?>, ? extends TypeMirror> baseTypeResolver;

    final Function<? super Class<?>, ? extends Name> typeNamer;

    /**
     * Creates a new instance.
     * @param baseTypeResolver resolves built-in types
     * @param typeNamer resolves declared type names
     */
    public MetaClassResolver(
            Function<? super Class<?>, ? extends TypeMirror> baseTypeResolver,
            Function<? super Class<?>, ? extends Name> typeNamer) {
        this.baseTypeResolver = baseTypeResolver;
        this.typeNamer = typeNamer;
    }

    /**
     * Resolves the meta classes.
     * @param metaClasses the meta classes
     * @return the resolved declarations
     */
    public TypeDeclarationRepository resolve(Class<?>... metaClasses) {
        return resolve(Arrays.asList(metaClasses));
    }

    /**
     * Resolves the meta classes.
     * @param metaClasses the meta classes
     * @return the resolved declarations
     */
    public TypeDeclarationRepository resolve(Collection<? extends Class<?>> metaClasses) {
        Engine engine = new Engine(metaClasses);
        return engine.resolve();
    }

    private class Engine {

        private static final String PACKAGE_INFO = "package-info";

        private final Set<Class<?>> classes;

        Engine(Collection<? extends Class<?>> classes) {
            this.classes = Set.copyOf(classes);
        }

        private boolean isSupported(Class<?> c) {
            return MetaClass.class.isAssignableFrom(c) || c.isEnum();
        }

        TypeDeclarationRepository resolve() {
            List<TypeDeclaration> resolvedTypes = classes.stream()
                    .filter(it -> !isPackageMeta(it))
                    .peek(it -> Preconditions.checkState(isSupported(it), it))
                    .map(this::resolve)
                    .sorted(Comparator.comparing(TypeDeclaration::getName))
                    .collect(Collectors.toList());
            List<PackageDeclaration> resolvedPackages = classes.stream()
                    .filter(this::isPackageMeta)
                    .map(this::resolvePackage)
                    .sorted(Comparator.comparing(PackageDeclaration::getName))
                    .collect(Collectors.toList());
            TypeDeclarationRepository repository = new TypeDeclarationRepository(resolvedTypes, resolvedPackages);
            resolveInheritance(repository);
            return repository;
        }

        private boolean isPackageMeta(Class<?> aClass) {
            return aClass.getSimpleName().equals(PACKAGE_INFO);
        }

        private PackageDeclaration resolvePackage(Class<?> c) {
            Name name = typeNamer.apply(c).getParent().get();
            PackageDeclaration result = new PackageDeclaration(name);
            Util.description(c).ifPresent(result::setDescription);
            return result;
        }

        private TypeDeclaration resolve(Class<?> c) {
            if (c.isEnum()) {
                return resolveEnum(c);
            } else {
                ClassDeclaration decl = resolveClass(c.asSubclass(MetaClass.class));
                Arrays.stream(c.getDeclaredClasses())
                        .filter(it -> !it.isSynthetic() && !it.isAnonymousClass() && !it.isLocalClass())
                        .filter(it -> isSupported(it))
                        .sorted(Comparator.comparing(Class::getSimpleName)) // stabilize generation
                        .map(this::resolve)
                        .peek(it -> {
                            if (it instanceof ClassDeclaration
                                    && !it.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE)) {
                                ClassDeclaration inner = (ClassDeclaration) it;
                                inner.addAttribute(ClassDeclaration.Attribute.FRAGMENT);
                            }
                        })
                        .forEach(decl::addMember);
                return decl;
            }
        }

        private ClassDeclaration resolveClass(Class<? extends MetaClass> c) {
            MetaClass meta;
            try {
                Constructor<? extends MetaClass> ctor = c.getDeclaredConstructor();
                ctor.setAccessible(true);
                meta = ctor.newInstance();
            } catch (ReflectiveOperationException e) {
                throw new IllegalStateException(c.getName(), e);
            }
            ClassDeclaration decl = new ClassDeclaration(typeNamer.apply(c));
            Util.description(c).ifPresent(decl::setDescription);
            Arrays.stream(c.getAnnotationsByType(Extends.class))
                    .map(Extends::value)
                    .map(MetaType::of)
                    .map(this::resolveType)
                    .forEach(decl::addSuperType);
            meta.getProperties().stream()
                    .map(it -> {
                        PropertyDeclaration p = new PropertyDeclaration(
                                decl.getName().resolve(it.name),
                                resolveType(it.type));
                        p.setDescription(it.description);
                        if (it.reflect.isAnnotationPresent(Const.class)) {
                            p.addAttribute(PropertyDeclaration.Attribute.CONST);
                        }
                        if (it.reflect.isAnnotationPresent(Transient.class)) {
                            p.addAttribute(PropertyDeclaration.Attribute.TRANSIENT);
                        }
                        if (it.reflect.isAnnotationPresent(Opt.class)) {
                            p.addAttribute(PropertyDeclaration.Attribute.OPTIONAL);
                        }
                        return p;
                    })
                    .forEach(decl::addMember);
            if (Modifier.isFinal(c.getModifiers())) {
                decl.addAttribute(ClassDeclaration.Attribute.FINAL);
            }
            if (c.isAnnotationPresent(Interface.class)) {
                decl.addAttribute(ClassDeclaration.Attribute.INTERFACE);
                Optional.ofNullable(c.getAnnotation(Interface.class))
                        .map(Interface::defaultImplementation)
                        .filter(it -> it != void.class)
                        .flatMap(it -> Optional.ofNullable(typeNamer.apply(it)))
                        .map(DeclaredTypeMirror::new)
                        .ifPresent(decl::setDefaultImplementation);
            }
            if (c.isAnnotationPresent(Value.class)) {
                decl.addAttribute(ClassDeclaration.Attribute.VALUE);
            }
            if (c.isAnnotationPresent(Const.class)) {
                decl.addAttribute(ClassDeclaration.Attribute.CONST);
            }
            if (c.isAnnotationPresent(Extern.class)) {
                decl.addAttribute(ClassDeclaration.Attribute.EXTERN);
            }
            if (c.isAnnotationPresent(Foreign.class)) {
                decl.addAttribute(ClassDeclaration.Attribute.FOREIGN);
            }
            if (c.isAnnotationPresent(Managed.class)) {
                decl.addAttribute(ClassDeclaration.Attribute.MANAGED);
            }
            decl.addAttribute(ClassDeclaration.Attribute.MOVABLE);
            if (!c.isAnnotationPresent(MoveOnly.class)) {
                decl.addAttribute(ClassDeclaration.Attribute.CLONABLE);
            }
            return decl;
        }

        private EnumTypeDeclaration resolveEnum(Class<?> c) {
            EnumTypeDeclaration decl = new EnumTypeDeclaration(typeNamer.apply(c));
            Util.description(c).ifPresent(decl::setDescription);
            Arrays.stream(c.getDeclaredFields())
                    .filter(Field::isEnumConstant)
                    .map(it -> {
                        EnumConstantDeclaration p = new EnumConstantDeclaration(
                                decl.getName().resolve(it.getName()));
                        Util.description(it).ifPresent(p::setDescription);
                        if (it.isAnnotationPresent(Default.class)) {
                            p.addAttribute(EnumConstantDeclaration.Attribute.DEFAULT);
                        }
                        return p;
                    })
                    .forEach(decl::addMember);
            return decl;
        }

        private TypeMirror resolveType(MetaType type) {
            switch (type.getKind()) {
            case SCALAR: {
                Class<?> raw = type.getRaw();
                TypeMirror builtin = baseTypeResolver.apply(raw);
                if (builtin != null) {
                    return builtin;
                }
                if (isTarget(raw)) {
                    return new DeclaredTypeMirror(typeNamer.apply(raw));
                }
                throw new IllegalArgumentException(type.toString());
            }
            case LIST:
                return resolveType(type.getArgument(0)).asListType();
            default:
                throw new IllegalArgumentException(type.toString());
            }
        }

        private void resolveInheritance(TypeDeclarationRepository repository) {
            Set<ClassDeclaration> resolved = new HashSet<>();
            Deque<ClassDeclaration> rest = repository.all(ClassDeclaration.class)
                    .collect(Collectors.toCollection(ArrayDeque::new));
            while (!rest.isEmpty()) {
                ClassDeclaration next = rest.removeFirst();
                List<ClassDeclaration> supers = next.getSuperTypes().stream()
                        .flatMap(it -> repository.find(ClassDeclaration.class, it).stream())
                        .collect(Collectors.toList());
                if (supers.stream().anyMatch(it -> !resolved.contains(it))) {
                    // not yet resolved
                    rest.addLast(next);
                    continue;
                }
                for (ClassDeclaration s : supers) {
                    resolveInherit(next, s);
                }
                resolved.add(next);
            }
        }

        private void resolveInherit(ClassDeclaration decl, ClassDeclaration parent) {
            Set<SimpleName> declared = decl.members(PropertyDeclaration.class)
                    .map(it -> it.getName().getName())
                    .collect(Collectors.toSet());
            parent.members(PropertyDeclaration.class)
                    .filter(it -> !declared.contains(it.getName().getLast()))
                    .forEach(it -> {
                        PropertyDeclaration p = new PropertyDeclaration(
                                decl.getName().resolve(it.getName().getLast()),
                                it.getType());
                        it.getDescription().ifPresent(p::setDescription);
                        it.getAttributes().forEach(p::addAttribute);
                        decl.addMember(p);
                    });
        }

        private boolean isTarget(Class<?> c) {
            for (Class<?> current = c; current != null; current = current.getDeclaringClass()) {
                if (!isSupported(current)) {
                    return false;
                }
                if (classes.contains(current)) {
                    return true;
                }
            }
            return false;
        }
    }
}
