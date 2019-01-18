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

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.List;
import java.util.Optional;

import javax.annotation.Nonnull;

import com.google.common.base.Objects;

/**
 * Represents a qualified name.
 * @since WIP
 */
public class QualifiedName implements Name {

    private final Name qualifier;

    private final SimpleName name;

    /**
     * Creates a new instance.
     * @param qualifier the name qualifier
     * @param name the last name
     */
    public QualifiedName(@Nonnull Name qualifier, @Nonnull SimpleName name) {
        this.qualifier = qualifier;
        this.name = name;
    }

    /**
     * Returns the qualifier.
     * @return the qualifier
     */
    public Name getQualifier() {
        return qualifier;
    }

    /**
     * Returns the name.
     * @return the name
     */
    public SimpleName getName() {
        return name;
    }

    @Override
    public List<String> getTokens() {
        return List.copyOf(flatten());
    }

    @Override
    public Optional<Name> getParent() {
        return Optional.of(qualifier);
    }

    @Override
    public SimpleName getLast() {
        return name;
    }

    private Deque<String> flatten() {
        Deque<String> work = new ArrayDeque<>();
        Name current = this;
        while (current instanceof QualifiedName) {
            QualifiedName q = (QualifiedName) current;
            work.addFirst(q.name.getToken());
            current = q.qualifier;
        }
        assert current instanceof SimpleName;
        work.addFirst(((SimpleName) current).getToken());
        return work;
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(name, qualifier);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        QualifiedName other = (QualifiedName) obj;
        return Objects.equal(name, other.name) && Objects.equal(qualifier, other.qualifier);
    }

    @Override
    public String toString() {
        return join();
    }
}
