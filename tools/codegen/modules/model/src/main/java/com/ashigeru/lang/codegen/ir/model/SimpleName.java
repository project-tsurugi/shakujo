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
import java.util.Objects;
import java.util.Optional;

import javax.annotation.Nonnull;

/**
 * Represents a simple name.
 * @since WIP
 */
public class SimpleName implements Name {

    private final String token;

    /**
     * Creates a new instance.
     * @param token the name token
     */
    public SimpleName(@Nonnull String token) {
        this.token = token;
    }

    /**
     * Returns the token.
     * @return the token
     */
    public String getToken() {
        return token;
    }

    @Override
    public List<String> getTokens() {
        return List.of(token);
    }

    @Override
    public Optional<Name> getParent() {
        return Optional.empty();
    }

    @Override
    public SimpleName getLast() {
        return this;
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(token);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        SimpleName other = (SimpleName) obj;
        return Objects.equals(token, other.token);
    }

    @Override
    public String toString() {
        return token;
    }
}
