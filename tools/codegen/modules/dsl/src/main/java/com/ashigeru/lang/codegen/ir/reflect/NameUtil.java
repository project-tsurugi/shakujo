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

import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.function.Function;
import java.util.stream.Collectors;

import javax.annotation.Nonnull;
import javax.annotation.Nullable;

import com.ashigeru.lang.codegen.ir.model.Name;
import com.google.common.base.Preconditions;

/**
 * Utilities of type names.
 * @since WIP
 */
public final class NameUtil {

    private NameUtil() {
        return;
    }

    /**
     * Converts a class into its name.
     * @param aClass the class
     * @return the corresponded name, or {@code null} if it has no valid name
     */
    public static Name toName(@Nonnull Class<?> aClass) {
        if (aClass.isArray() || aClass.isPrimitive() || aClass.isAnonymousClass() || aClass.isLocalClass()) {
            return null;
        }
        return Arrays.stream(aClass.getName().split("[\\.\\$]"))
                .filter(it -> !it.isEmpty())
                .collect(Collectors.collectingAndThen(
                        Collectors.toList(),
                        Name::of));
    }

    /**
     * Returns a function that converts classes into its name.
     * @return the to name
     */
    public static Function<Class<?>, Name> toName() {
        return NameUtil::toName;
    }

    /**
     * Returns a function that replaces name prefix.
     * @param prefix the name prefix
     * @param replacement the prefix replacement
     * @return the replacement function
     */
    public static Function<Name, Name> replacePrefix(@Nullable Name prefix, @Nullable Name replacement) {
        List<String> prefixTokens = Optional.ofNullable(prefix)
                .map(Name::getTokens)
                .orElse(List.of());
        return name -> {
            List<String> tokens = name.getTokens();
            if (prefix != null) {
                Preconditions.checkArgument(prefixTokens.size() < tokens.size(), name);
                for (int i = 0, n = prefixTokens.size(); i < n; i++) {
                    Preconditions.checkArgument(prefixTokens.get(i).equals(tokens.get(i)), name);
                }
                tokens = tokens.subList(prefixTokens.size(), tokens.size());
            }
            if (replacement == null) {
                return Name.of(tokens);
            } else {
                Name current = replacement;
                for (String s : tokens) {
                    current = current.resolve(s);
                }
                return current;
            }
        };
    }
}
