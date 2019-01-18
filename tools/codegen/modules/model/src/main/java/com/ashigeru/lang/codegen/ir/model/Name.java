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

import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Optional;

import javax.annotation.Nonnull;

import com.google.common.base.Preconditions;

/**
 * An abstract super interface of names.
 * @since WIP
 */
public interface Name extends Comparable<Name> {

    /**
     * Returns the tokens of this name.
     * @return the tokens
     */
    List<String> getTokens();

    /**
     * Returns the last name.
     * @return the last name
     */
    SimpleName getLast();

    /**
     * Returns the parent name.
     * @return the parent name, or {@code null} if this is a simple name
     */
    Optional<Name> getParent();

    /**
     * Returns a simple name of the token.
     * @param token the name token
     * @return the corresponded simple name
     */
    static SimpleName of(@Nonnull String token) {
        return new SimpleName(token);
    }

    /**
     * Returns a name.
     * @param tokens the name tokens, must not be empty
     * @return the corresponded name
     */
    static Name of(@Nonnull List<String> tokens) {
        Iterator<String> iter = tokens.iterator();
        Preconditions.checkState(iter.hasNext());
        Name current = of(iter.next());
        while (iter.hasNext()) {
            current = current.resolve(iter.next());
        }
        return current;
    }

    /**
     * Returns a name.
     * @param tokens the name tokens, must not be empty
     * @return the corresponded name
     */
    static Name of(@Nonnull String... tokens) {
        return of(Arrays.asList(tokens));
    }

    /**
     * Returns a qualified name.
     * @param name the last name of the qualified name
     * @return the qualified name
     */
    default QualifiedName resolve(@Nonnull Name name) {
        return name.getParent()
                .map(it -> (Name) resolve(it))
                .orElse(this)
                .resolve(name.getLast());
    }

    /**
     * Returns a qualified name.
     * @param name the last name of the qualified name
     * @return the qualified name
     */
    default QualifiedName resolve(@Nonnull SimpleName name) {
        return new QualifiedName(this, name);
    }

    /**
     * Returns a qualified name.
     * @param token the last name token of the qualified name
     * @return the qualified name
     */
    default QualifiedName resolve(@Nonnull String token) {
        return resolve(of(token));
    }

    /**
     * Returns a name.
     * @param name the last name
     * @return the name
     */
    default Name resolveSibling(String name) {
        return getParent()
                .map(it -> (Name) it.resolve(name))
                .orElseGet(() -> Name.of(name));
    }

    /**
     * Returns a joined string of this name.
     * @param delimiter the delimiter.
     * @return the joined name string
     */
    default String join(String delimiter) {
        return String.join(delimiter, getTokens());
    }

    /**
     * Returns a joined string of this name.
     * This is equivalent to {@code #join(String) this.join(".")}.
     * @return the joined name string.
     */
    default String join() {
        return join("."); //$NON-NLS-1$
    }

    /**
     * Adds the suffix to this name.
     * @param suffix the suffix
     * @return the new name
     */
    default Name addSuffix(String suffix) {
        SimpleName last = of(getLast().getToken() + suffix);
        return getParent()
                .map(it -> (Name) it.resolve(last))
                .orElse(last);
    }

    @Override
    default int compareTo(Name o) {
        List<String> a = getTokens();
        List<String> b = o.getTokens();
        for (int i = 0, n = Math.min(a.size(), b.size()); i < n; i++) {
            int cmp = a.get(i).compareTo(b.get(i));
            if (cmp != 0) {
                return cmp;
            }
        }
        return Integer.compare(a.size(), b.size());
    }
}
