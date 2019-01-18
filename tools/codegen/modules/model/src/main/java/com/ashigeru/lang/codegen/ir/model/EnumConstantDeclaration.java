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

import java.util.EnumSet;
import java.util.Set;

import javax.annotation.Nonnull;

/**
 * Represents an enum constant declaration.
 * @since WIP
 */
public class EnumConstantDeclaration extends AbstractDeclaration implements TypeMemberDeclaration {

    private final Set<Attribute> attributes = EnumSet.noneOf(Attribute.class);

    /**
     * Creates a new instance.
     * @param name the member name
     */
    public EnumConstantDeclaration(QualifiedName name) {
        super(name);
    }

    @Override
    public QualifiedName getName() {
        return (QualifiedName) super.getName();
    }

    @Override
    public Set<Attribute> getAttributes() {
        return attributes;
    }

    /**
     * Adds an attribute of this class.
     * @param attribute the attribute
     * @return this
     */
    public EnumConstantDeclaration addAttribute(@Nonnull Attribute attribute) {
        attributes.add(attribute);
        return this;
    }

    /**
     * Represents attributes of {@link EnumConstantDeclaration}.
     * @since WIP
     */
    public enum Attribute {

        /**
         * Whether the declaration is the default value of the enum kind.
         */
        DEFAULT,
    }
}