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
package com.ashigeru.lang.codegen.spec.sql;

import java.io.IOException;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import com.ashigeru.lang.codegen.generator.cpp.CppFiler;
import com.ashigeru.lang.codegen.ir.model.StandardTypeMirror;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.model.TypeMirror;
import com.ashigeru.lang.codegen.ir.reflect.MetaClassResolver;
import com.ashigeru.lang.codegen.spec.sql.builtin.Bool;
import com.ashigeru.lang.codegen.spec.sql.builtin.Float64;
import com.ashigeru.lang.codegen.spec.sql.builtin.Int64;
import com.ashigeru.lang.codegen.spec.sql.builtin.Size;
import com.google.common.reflect.ClassPath;

/**
 * Program entry.
 * @since WIP
 */
public class Main {

    private static final Function<Class<?>, TypeMirror> TYPES;
    static {
        Map<Class<?>, TypeMirror> map = new HashMap<>();
        map.put(Bool.class, StandardTypeMirror.BOOL);
        map.put(Int64.class, StandardTypeMirror.INT);
        map.put(Float64.class, StandardTypeMirror.FLOAT);
        map.put(Size.class, StandardTypeMirror.SIZE);
        map.put(String.class, StandardTypeMirror.STRING);
        TYPES = map::get;
    }

    private Main() {
        return;
    }

    /**
     * Program entry.
     * @param args arguments
     * @throws IOException if I/O error was occurred
     */
    public static void main(String[] args) throws IOException {
        TypeDeclarationRepository repository = resolve();
        CppFiler filer = new CppFiler(Paths.get("build", "cpp"));
        filer.clean();

        CppGen cpp = new CppGen(filer, repository);
        cpp.process();
    }

    private static TypeDeclarationRepository resolve() throws IOException {
        MetaClassResolver resolver = new MetaClassResolver(TYPES, CppGen.TYPE_NAMER);
        TypeDeclarationRepository repository = resolver.resolve(collect());
        return repository;
    }

    private static List<Class<?>> collect() throws IOException {
        ClassPath cp = ClassPath.from(Main.class.getClassLoader());
        return cp.getTopLevelClassesRecursive("com.ashigeru.lang.codegen.spec.sql").stream()
                .filter(it -> !it.getPackageName().equals(Main.class.getPackageName())
                        || it.getName().equals(Node.class.getName()))
                .filter(it -> !it.getPackageName().endsWith(".builtin"))
                .peek(System.out::println)
                .flatMap(it -> {
                    try {
                        return Stream.of(Class.forName(it.getName()));
                    } catch (ClassNotFoundException e) {
                        throw new IllegalArgumentException(e);
                    }
                })
                .collect(Collectors.toList());
    }
}
