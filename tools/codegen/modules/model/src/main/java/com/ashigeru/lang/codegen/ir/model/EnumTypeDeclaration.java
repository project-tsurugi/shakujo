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
import java.util.List;
import java.util.Optional;

import javax.annotation.Nonnull;

import com.google.common.base.Preconditions;

/**
 * Represents an enum type declaration.
 * @since WIP
 */
public class EnumTypeDeclaration extends AbstractDeclaration implements TypeDeclaration {

    private final List<EnumConstantDeclaration> members = new ArrayList<>();

    /**
     * Creates a new instance.
     * @param name the type name
     */
    public EnumTypeDeclaration(@Nonnull Name name) {
        super(name);
    }

    @Override
    public List<EnumConstantDeclaration> getMembers() {
        return members;
    }

    /**
     * Returns the default value.
     * @return the default value
     */
    public Optional<EnumConstantDeclaration> getDefaultValue() {
        return members.stream()
                .filter(it -> it.getAttributes().contains(EnumConstantDeclaration.Attribute.DEFAULT))
                .findFirst();
    }

    /**
     * Adds a member of this type.
     * @param member the member
     * @return this
     */
    public EnumTypeDeclaration addMember(@Nonnull EnumConstantDeclaration member) {
        Preconditions.checkArgument(member.isMemberOf(this));
        members.add(member);
        return this;
    }
}