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

import java.util.Collection;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import javax.annotation.Nonnull;

/**
 * Represents a repository of {@link TypeDeclaration}s.
 * @since WIP
 */
public class TypeDeclarationRepository implements Iterable<TypeDeclaration> {

    private final List<TypeDeclaration> elements;

    private final List<PackageDeclaration> packages;

    /**
     * Creates a new instance.
     * @param types the type declarations
     * @param packages the package declarations
     */
    public TypeDeclarationRepository(
            @Nonnull Collection<? extends TypeDeclaration> types,
            @Nonnull Collection<? extends PackageDeclaration> packages) {
        this.elements = List.copyOf(types);
        this.packages = List.copyOf(packages);
    }

    @Override
    public Iterator<TypeDeclaration> iterator() {
        return elements.iterator();
    }

    /**
     * Returns the elements.
     * @return the elements
     */
    public List<TypeDeclaration> getElements() {
        return elements;
    }

    /**
     * Returns a type declaration of the given type.
     * @param type the target type
     * @return the type declaration
     */
    public Optional<TypeDeclaration> find(TypeMirror type) {
        if (type instanceof DeclaredTypeMirror) {
            return find(((DeclaredTypeMirror) type).getName());
        } else {
            return Optional.empty();
        }
    }

    /**
     * Returns a type declaration which has the given name.
     * @param name the target type name
     * @return the type declaration
     */
    public Optional<TypeDeclaration> find(Name name) {
        return enumerate()
                .filter(it -> it.getName().equals(name))
                .findFirst();
    }

    /**
     * Returns a type declaration which has the given type.
     * @param declType the declaration type
     * @param <T> the declaration type
     * @param typeMirror the target type
     * @return the type declaration
     */
    public <T extends TypeDeclaration> Optional<T> find(Class<T> declType, TypeMirror typeMirror) {
        return find(typeMirror)
                .filter(declType::isInstance)
                .map(declType::cast);
    }

    /**
     * Returns a type declaration which has the given name.
     * @param type the declaration type
     * @param <T> the declaration type
     * @param name the target type name
     * @return the type declaration
     */
    public <T extends TypeDeclaration> Optional<T> find(Class<T> type, Name name) {
        return find(name)
                .filter(type::isInstance)
                .map(type::cast);
    }

    /**
     * Returns all elements in this repository.
     * @param <T> the target type
     * @param type the target type
     * @return the all elements
     */
    public <T extends TypeDeclaration> Stream<T> all(Class<T> type) {
        return enumerate()
                .filter(type::isInstance)
                .map(type::cast);
    }

    private Stream<TypeDeclaration> enumerate() {
        return enumerate(elements);
    }

    private static Stream<TypeDeclaration> enumerate(Collection<? extends TypeMemberDeclaration> elements) {
        return elements.stream()
                .filter(it -> it instanceof TypeDeclaration)
                .map(it -> (TypeDeclaration) it)
                .flatMap(it -> Stream.concat(Stream.of(it), enumerate(it.getMembers())));
    }

    /**
     * Returns whether or not {@code a} is a sub-type of {@code b}.
     * @param a the first type
     * @param b the second type
     * @return {@code true} if {@code a} is sub-type of {@code b}, otherwise {@code false}
     */
    public boolean isSubtypeOf(TypeMirror a, TypeMirror b) {
        if (a.equals(b)) {
            return true;
        }
        return find(a)
                .stream()
                .filter(it -> it instanceof ClassDeclaration)
                .map(it -> (ClassDeclaration) it)
                .flatMap(it -> it.getSuperTypes().stream())
                .anyMatch(it -> isSubtypeOf(it, b));
    }

    /**
     * Returns the sub-types of the given ancestor type.
     * @param ancestor the target type
     * @return the type declarations which are sub-type of the given type, excluding the given type itself
     */
    public List<ClassDeclaration> getSubtypes(TypeMirror ancestor) {
        return subtypes(ancestor)
                .sorted(Comparator.comparing(ClassDeclaration::getName))
                .collect(Collectors.toList());
    }

    private Stream<ClassDeclaration> subtypes(TypeMirror ancestor) {
        return enumerate()
                .filter(it -> it instanceof ClassDeclaration)
                .map(it -> (ClassDeclaration) it)
                .filter(it -> !it.asTypeMirror().equals(ancestor)) // avoid itself
                .filter(it -> isSubtypeOf(it.asTypeMirror(), ancestor))
                .flatMap(it -> Stream.concat(Stream.of(it), subtypes(it.asTypeMirror())));
    }

    /**
     * Returns the all ancestors of the given descendant type.
     * @param descendant the target type
     * @return the type declarations which are sub-type of the given type, excluding the given type itself
     */
    public List<ClassDeclaration> getAncestors(TypeMirror descendant) {
        return ancestors(descendant)
                .sorted(Comparator.comparing(ClassDeclaration::getName))
                .collect(Collectors.toList());
    }

    private Stream<ClassDeclaration> ancestors(TypeMirror descendant) {
        return find(ClassDeclaration.class, descendant).stream()
                .flatMap(it -> Stream.concat(
                        Stream.of(it),
                        it.getSuperTypes().stream().flatMap(this::ancestors)));
    }

    /**
     * Returns the enclosing class.
     * @param decl the declaration
     * @return the enclosing class, or empty if it is on top
     */
    public Optional<ClassDeclaration> getEnclosingClass(TypeDeclaration decl) {
        return decl.getName().getParent()
                .flatMap(it -> find(it)
                        .filter(p -> p instanceof ClassDeclaration).map(p -> (ClassDeclaration) p));
    }

    /**
     * Returns the package declarations.
     * @return the package declarations
     */
    public List<PackageDeclaration> getPackages() {
        return packages;
    }
}
