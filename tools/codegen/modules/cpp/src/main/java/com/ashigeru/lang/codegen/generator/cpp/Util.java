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
package com.ashigeru.lang.codegen.generator.cpp;

import java.util.function.Function;

import com.ashigeru.lang.codegen.ir.model.Declaration;
import com.ashigeru.lang.codegen.ir.model.EnumConstantDeclaration;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.util.Namer;

final class Util {

    private Util() {
        return;
    }

    static String getSimpleName(TypeDeclaration decl) {
        return decl.getName().getLast().getToken();
    }

    static String getSimpleName(PropertyDeclaration decl) {
        String name = decl.getName().getLast().getToken();
        return toPropertyName(name);
    }

    static String toPropertyName(String name) {
        Namer namer = Namer.parse(name);
        return namer.toLowerSnakeCase();
    }

    static String getSimpleName(EnumConstantDeclaration decl) {
        String name = decl.getName().getLast().getToken();
        return toEnumConstantName(name);
    }

    static String toEnumConstantName(String name) {
        Namer namer = Namer.parse(name);
        return namer.toUpperSnakeCase();
    }

    static String getQualifiedName(TypeDeclaration decl) {
        return getQualifiedName(decl, Util::getSimpleName);
    }

    static String getQualifiedName(PropertyDeclaration decl) {
        return getQualifiedName(decl, Util::getSimpleName);
    }

    static String getQualifiedName(EnumConstantDeclaration decl) {
        return getQualifiedName(decl, Util::getSimpleName);
    }

    private static <T extends Declaration> String getQualifiedName(T decl, Function<T, String> namer) {
        Name name = decl.getName();
        String last = namer.apply(decl);
        return name.getParent()
                .map(it -> it.resolve(last).join("::"))
                .orElse(last);
    }
}
