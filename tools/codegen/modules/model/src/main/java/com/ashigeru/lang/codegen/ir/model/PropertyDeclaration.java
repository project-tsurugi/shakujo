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
 * Represents a property declaration.
 * @since WIP
 */
public class PropertyDeclaration extends AbstractDeclaration implements TypeMemberDeclaration {

    private final TypeMirror type;

    private final Set<Attribute> attributes = EnumSet.noneOf(Attribute.class);

    /**
     * Creates a new instance.
     * @param name the member name
     * @param type the property type
     */
    public PropertyDeclaration(@Nonnull QualifiedName name, @Nonnull TypeMirror type) {
        super(name);
        this.type = type;
    }

    @Override
    public QualifiedName getName() {
        return (QualifiedName) super.getName();
    }

    /**
     * Returns the type of this property.
     * @return the type
     */
    public TypeMirror getType() {
        return type;
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
    public PropertyDeclaration addAttribute(@Nonnull Attribute attribute) {
        attributes.add(attribute);
        return this;
    }

    /**
     * Represents an attribute of {@link PropertyDeclaration}.
     * @since WIP
     */
    public enum Attribute {

        /**
         * The property is not modifiable.
         */
        CONST,

        /**
         * The property is transient.
         */
        TRANSIENT,

        /**
         * The property is optional.
         */
        OPTIONAL,
    }
}
