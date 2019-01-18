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

import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.StandardOpenOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.List;
import java.util.stream.Collectors;

import javax.annotation.Nonnull;

import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.QualifiedName;
import com.ashigeru.lang.codegen.ir.model.SimpleName;

/**
 * Provides file outputs.
 * @since WIP
 */
public class CppFiler {

    static final String PREFIX_INCLUDE = "include";

    static final String SUFFIX_INCLUDE = ".h";

    static final String PREFIX_SOURCE = "src";

    static final String SUFFIX_SOURCE = ".cpp";

    private final Path outputPath;

    /**
     * Creates a new instance.
     * @param outputPath the output path
     */
    public CppFiler(@Nonnull Path outputPath) {
        this.outputPath = outputPath;
    }

    /**
     * Removes the output directory only if it exists.
     * @throws IOException if I/O error was occurred
     */
    public void clean() throws IOException {
        if (Files.exists(outputPath)) {
            Files.walkFileTree(outputPath, new SimpleFileVisitor<>() {
                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
                    Files.delete(file);
                    return FileVisitResult.CONTINUE;
                }
                @Override
                public FileVisitResult postVisitDirectory(Path dir, IOException exc) throws IOException {
                    Files.delete(dir);
                    return FileVisitResult.CONTINUE;
                }
            });
        }
    }

    /**
     * Creates a new file.
     * @param relative the relative path
     * @return the writer
     * @throws IOException if I/O error was occurred
     */
    public @Nonnull PrintWriter createFile(@Nonnull Path relative) throws IOException {
        return create(outputPath.resolve(relative.toString()));
    }

    /**
     * Creates a header file for the type name.
     * @param typeName the type name
     * @return the writer
     * @throws IOException if I/O error was occurred
     */
    public @Nonnull PrintWriter createHeader(@Nonnull Name typeName) throws IOException {
        Path path = getHeader(typeName);
        return create(path);
    }

    Path getHeader(@Nonnull Name typeName) {
        Path path = toPath(PREFIX_INCLUDE, SUFFIX_INCLUDE, typeName);
        return path;
    }

    /**
     * Creates a header file for the type name.
     * @param typeName the type name
     * @return the writer
     * @throws IOException if I/O error was occurred
     */
    public @Nonnull PrintWriter createSource(@Nonnull Name typeName) throws IOException {
        Path path = getSource(typeName);
        return create(path);
    }

    Path getSource(Name typeName) {
        Path path = toPath(PREFIX_SOURCE, SUFFIX_SOURCE, typeName);
        return path;
    }

    private @Nonnull Path toPath(@Nonnull String prefix, @Nonnull String suffix, @Nonnull Name type) {
        Path result = outputPath;
        result = result.resolve(prefix);
        String name;
        if (type instanceof SimpleName) {
            name = ((SimpleName) type).getToken();
        } else {
            QualifiedName qname = (QualifiedName) type;
            name = qname.getName().getToken();
            for (String s : qname.getQualifier().getTokens()) {
                result = result.resolve(s);
            }
        }
        result = result.resolve(name + suffix);
        return result;
    }

    private static @Nonnull PrintWriter create(@Nonnull Path path) throws IOException {
        Files.createDirectories(path.getParent());
        return new PrintWriter(Files.newBufferedWriter(
                path,
                StandardCharsets.UTF_8,
                StandardOpenOption.CREATE_NEW));
    }

    /**
     * Returns the child header file paths.
     * @param namespace the target namespace
     * @return the header files in the given namespace
     * @throws IOException if I/O errors were occurred
     */
    public List<Name> collectHeaders(Name namespace) throws IOException {
        Path dir = resolve(namespace);
        if (!Files.isDirectory(dir)) {
            return List.of();
        }
        return Files.list(dir)
                .filter(Files::isRegularFile)
                .filter(it -> !it.getFileName().toString().startsWith("."))
                .filter(it -> it.getFileName().toString().endsWith(SUFFIX_INCLUDE))
                .map(it -> {
                    String name = it.getFileName().toString();
                    return namespace.resolve(name.substring(0, name.length() - SUFFIX_INCLUDE.length()));
                })
                .sorted()
                .collect(Collectors.toList());
    }

    private Path resolve(Name namespace) {
        Path dir = outputPath.resolve(PREFIX_INCLUDE);
        for (String token : namespace.getTokens()) {
            dir = dir.resolve(token);
        }
        return dir;
    }
}
