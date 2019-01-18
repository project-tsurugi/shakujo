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

import static org.junit.jupiter.api.Assertions.*;

import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.List;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.ashigeru.lang.codegen.ir.model.Name;

class FilerTest {

    private Path path;

    @BeforeEach
    void before() throws IOException {
        path = Files.createTempDirectory("junit-temp-");
    }

    @AfterEach
    void after() throws IOException {
        if (path == null || !Files.exists(path)) {
            return;
        }
        Files.walkFileTree(path, new SimpleFileVisitor<Path>() {
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

    @Test
    void simple() {
        CppFiler filer = new CppFiler(path);

        Path header = filer.getHeader(Name.of("testing"));
        assertEquals(path.resolve("include/testing.h"), header);

        Path source = filer.getSource(Name.of("testing"));
        assertEquals(path.resolve("src/testing.cpp"), source);
    }

    @Test
    void include() throws IOException {
        CppFiler filer = new CppFiler(path);

        Name name = Name.of("a", "b");
        try (PrintWriter w = filer.createHeader(name)) {
            w.println("testing");
        }
        Path file = filer.getHeader(name);
        List<String> lines = Files.readAllLines(file);
        assertEquals(List.of("testing"), lines);
    }

    @Test
    void source() throws IOException {
        CppFiler filer = new CppFiler(path);

        Name name = Name.of("a", "b");
        try (PrintWriter w = filer.createSource(name)) {
            w.println("testing");
        }
        Path file = filer.getSource(name);
        List<String> lines = Files.readAllLines(file);
        assertEquals(List.of("testing"), lines);
    }
}
