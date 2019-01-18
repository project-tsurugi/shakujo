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

import java.util.List;
import java.util.Optional;
import java.util.stream.Stream;

/**
 * An abstract interface of type declarations of IR models.
 * @since WIP
 */
public interface TypeDeclaration extends TypeMemberDeclaration {

    /**
     * Returns the members of this type.
     * @return the members
     */
    List<? extends TypeMemberDeclaration> getMembers();

    /**
     * Returns the member of this type.
     * @param <T> the member type
     * @param type the member type
     * @return enumeration of the members
     */
    default <T extends TypeMemberDeclaration> Stream<T> members(Class<T> type) {
        return getMembers().stream().filter(type::isInstance).map(type::cast);
    }

    /**
     * Returns a member.
     * @param <T> the member type
     * @param type the member type
     * @param name the member simple name
     * @return the found member, or {@code empty} if it is absent
     */
    default <T extends TypeMemberDeclaration> Optional<T> findMember(Class<T> type, String name) {
        return getMembers().stream()
                .filter(type::isInstance)
                .map(type::cast)
                .filter(it -> {
                    QualifiedName qname = (QualifiedName) it.getName();
                    return qname.getName().getToken().equals(name);
                })
                .findFirst();
    }

    /**
     * Returns a {@link TypeMirror} that represents this type.
     * @return the mirror of this type
     */
    default DeclaredTypeMirror asTypeMirror() {
        return new DeclaredTypeMirror(getName());
    }
}
