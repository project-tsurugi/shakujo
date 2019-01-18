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

import java.util.ArrayList;
import java.util.EnumSet;
import java.util.List;
import java.util.Optional;
import java.util.Set;

import javax.annotation.Nonnull;

import com.google.common.base.Preconditions;

/**
 * Represents a class declaration.
 * @since WIP
 */
public class ClassDeclaration extends AbstractDeclaration implements TypeDeclaration {

    private final List<TypeMirror> superTypes = new ArrayList<>();

    private final List<TypeMemberDeclaration> members = new ArrayList<>();

    private final Set<Attribute> attributes = EnumSet.noneOf(Attribute.class);

    private TypeMirror defaultImplementation;

    /**
     * Creates a new instance.
     * @param name the declaration name
     */
    public ClassDeclaration(@Nonnull Name name) {
        super(name);
    }

    /**
     * Returns the super types.
     * @return the super types
     */
    public List<TypeMirror> getSuperTypes() {
        return superTypes;
    }

    @Override
    public List<TypeMemberDeclaration> getMembers() {
        return members;
    }

    @Override
    public Set<Attribute> getAttributes() {
        return attributes;
    }

    /**
     * Adds a super type of this class.
     * @param type the super type
     * @return this
     */
    public ClassDeclaration addSuperType(@Nonnull TypeMirror type) {
        superTypes.add(type);
        return this;
    }

    /**
     * Adds a member of this class.
     * @param member the member
     * @return this
     */
    public ClassDeclaration addMember(@Nonnull TypeMemberDeclaration member) {
        Preconditions.checkArgument(member.isMemberOf(this));
        members.add(member);
        return this;
    }

    /**
     * Adds an attribute of this class.
     * @param attribute the attribute
     * @return this
     */
    public ClassDeclaration addAttribute(@Nonnull Attribute attribute) {
        attributes.add(attribute);
        return this;
    }

    /**
     * Returns the default implementation type.
     * @return the default implementation type
     */
    public Optional<TypeMirror> getDefaultImplementation() {
        return Optional.ofNullable(defaultImplementation);
    }

    /**
     * Sets the default implementation type.
     * @param type the default implementation type
     */
    public void setDefaultImplementation(TypeMirror type) {
        this.defaultImplementation = type;
    }

    /**
     * Represents an attribute of {@link ClassDeclaration}.
     * @since WIP
     */
    public enum Attribute {

        /**
         * The declaration is just an interface.
         */
        INTERFACE,

        /**
         * The object represents a value, instead of IR tree.
         */
        VALUE,

        /**
         * The object never has sub-types.
         */
        FINAL,

        /**
         * The object is immutable.
         */
        CONST,

        /**
         * The object represents a just a fragment of the owner.
         */
        FRAGMENT,

        /**
         * Managed class.
         */
        MANAGED,

        /**
         * The element is already provided.
         */
        EXTERN,

        /**
         * The element is not a generation target.
         */
        FOREIGN,

        /**
         * The element is copyable.
         */
        COPYABLE,

        /**
         * The element is movable.
         */
        MOVABLE,

        /**
         * The element is clonable.
         */
        CLONABLE,
    }
}