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
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import com.ashigeru.lang.codegen.ir.model.EnumConstantDeclaration;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.util.Namer;
import com.google.common.base.Preconditions;
import com.google.common.base.Strings;

/**
 * Prints C++ style source files.
 * @since WIP
 */
public class CppPrinter {

    private static final String COPYRIGHT_BEGIN = "2018";

    private static final String COPYRIGHT_END = String.valueOf(LocalDate.now().getYear());

    private static final String COPYRIGHT_AUTHOR = "shakujo project";

    private final Name self;

    private final FileKind fileKind;

    private final IncludeList includes = new IncludeList();

    private boolean inFooter = false;

    private final Contents body = new Contents();

    private final Contents footer = new Contents();

    /**
     * Creates a new instance.
     * @param self the context type name
     * @param fileKind the target file kind
     */
    public CppPrinter(Name self, FileKind fileKind) {
        this.self = self;
        this.fileKind = fileKind;
    }

    /**
     * Creates a new instance for printing include files.
     * @param self the context type name
     * @return the created printer
     */
    public static CppPrinter includeFile(Name self) {
        return new CppPrinter(self, FileKind.INCLUDE_FILE);
    }

    /**
     * Creates a new instance for printing source files.
     * @param self the context type name
     * @return the created printer
     */
    public static CppPrinter sourceFile(Name self) {
        return new CppPrinter(self, FileKind.SOURCE_FILE);
    }

    /**
     * Returns the context type name.
     * @return the context type name
     */
    public Name getName() {
        return self;
    }

    private Contents current() {
        return inFooter ? footer : body;
    }

    /**
     * Puts an empty line.
     * @return this
     */
    public CppPrinter put() {
        return put("");
    }

    /**
     * Puts a body line.
     * @param string the line string
     * @return this
     */
    public CppPrinter put(String string) {
        current().put(string);
        return this;
    }

    /**
     * Puts a body line.
     * @param format the line format
     * @param arguments the line arguments
     * @return this
     */
    public CppPrinter put(String format, Object... arguments) {
        if (arguments.length == 0) {
            return put(format);
        } else {
            return put(String.format(format, arguments));
        }
    }

    /**
     * Appends a text into the tail of the last line.
     * @param string the text
     * @return this
     */
    public CppPrinter append(String string) {
        current().append(string);
        return this;
    }

    /**
     * Appends a text into the tail of the last line.
     * @param format the text format
     * @param arguments the text arguments
     * @return this
     */
    public CppPrinter append(String format, Object... arguments) {
        if (arguments.length == 0) {
            return append(format);
        } else {
            return append(String.format(format, arguments));
        }
    }

    /**
     * Returns the includes.
     * @return the includes
     */
    public IncludeList getIncludes() {
        return includes;
    }

    /**
     * Changes the current namespace.
     * @param namespace the namespace (nullable)
     * @return this
     */
    public CppPrinter namespace(Name namespace) {
        current().namespace(namespace);
        return this;
    }

    /**
     * Executes within a new indent block.
     * @param r the runnable
     */
    public void indent(Runnable r) {
        indent(1, r);
    }

    /**
     * Executes within a new indent block.
     * @param level indent level
     * @param r the runnable
     */
    public void indent(int level, Runnable r) {
        for (int i = 0; i < level; i++) {
            enter();
        }
        try {
            r.run();
        } finally {
            for (int i = 0; i < level; i++) {
                exit();
            }
        }
    }

    /**
     * Executes within a new indent block.
     * @param r the runnable
     */
    public void footer(Runnable r) {
        Preconditions.checkState(!inFooter);
        inFooter = true;
        try {
            r.run();
        } finally {
            inFooter = false;
        }
    }

    /**
     * Enters into a new indent block.
     * @return this
     */
    public CppPrinter enter() {
        current().enter();
        return this;
    }

    /**
     * Exits from the current indent block
     * @return this
     */
    public CppPrinter exit() {
        current().exit();
        return this;
    }

    /**
     * Returns the context type name.
     * @param decl the declaration
     * @return the context type name
     */
    public String getContextName(TypeDeclaration decl) {
        return getContextName(decl.getName());
    }

    /**
     * Returns the context type name.
     * @param decl the declaration
     * @return the context type name
     */
    public String getContextName(PropertyDeclaration decl) {
        return String.format("%s::%s",
                getContextName(decl.getName().getQualifier()),
                Util.getSimpleName(decl));
    }

    /**
     * Returns the context type name.
     * @param decl the declaration
     * @return the context type name
     */
    public String getContextName(EnumConstantDeclaration decl) {
        return String.format("%s::%s",
                getContextName(decl.getName().getQualifier()),
                Util.getSimpleName(decl));
    }

    /**
     * Returns the context name.
     * @param namespace the owner namespace
     * @param memberName the member name
     * @return the context name
     */
    public String getContextName(Name namespace, String memberName) {
        List<String> tokens = Optional.ofNullable(namespace)
                .map(Name::getTokens)
                .orElse(List.of());
        int start = Contents.commonPrefixLength(current().currentNamespace, tokens);
        return Stream.concat(
                tokens.subList(start, tokens.size()).stream(),
                Stream.of(memberName))
                .collect(Collectors.joining("::"));
    }

    /**
     * Returns the context name.
     * @param qualifiedMemberName the name
     * @return the context name
     */
    public String getContextName(Name qualifiedMemberName) {
        return getContextName(qualifiedMemberName.getParent().orElse(null), qualifiedMemberName.getLast().getToken());
    }

    /**
     * Puts contents into the given filer.
     * @param filer the destination filer
     * @throws IOException if I/O error was occurred
     */
    public void printTo(CppFiler filer) throws IOException {
        try (PrintWriter writer = open(filer)) {
            printTo(writer);
        }
    }

    private PrintWriter open(CppFiler filer) throws IOException {
        switch (fileKind) {
        case INCLUDE_FILE:
            return filer.createHeader(self);
        case SOURCE_FILE:
            return filer.createSource(self);
        default:
            throw new AssertionError(fileKind);
        }
    }

    /**
     * Prints contents into the given writer.
     * @param writer the target writer
     */
    public void printTo(PrintWriter writer) {
        putLicenseHeader(writer);
        putGeneratedMark(writer);
        if (fileKind == FileKind.INCLUDE_FILE) {
            writer.printf("#ifndef %s%n", getSelfNameDef());
            writer.printf("#define %s%n", getSelfNameDef());
        } else if (fileKind == FileKind.SOURCE_FILE) {
            putIncludeLine(writer, self);
        }
        writer.println();
        printInclude(writer);
        body.printTo(writer);
        footer.printTo(writer);
        if (fileKind == FileKind.INCLUDE_FILE) {
            writer.printf("#endif  // %s%n", getSelfNameDef());
        }
    }

    /**
     * Puts license header into the given writer.
     * @param writer the target writer
     */
    public static void putLicenseHeader(PrintWriter writer) {
        writer.println("/*");
        writer.printf(" * Copyright %s-%s %s.%n",
                COPYRIGHT_BEGIN,
                COPYRIGHT_END,
                COPYRIGHT_AUTHOR);
        writer.println(" *");
        writer.println(" * Licensed under the Apache License, Version 2.0 (the \"License\");");
        writer.println(" * you may not use this file except in compliance with the License.");
        writer.println(" * You may obtain a copy of the License at");
        writer.println(" *");
        writer.println(" *     http://www.apache.org/licenses/LICENSE-2.0");
        writer.println(" *");
        writer.println(" * Unless required by applicable law or agreed to in writing, software");
        writer.println(" * distributed under the License is distributed on an \"AS IS\" BASIS,");
        writer.println(" * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.");
        writer.println(" * See the License for the specific language governing permissions and");
        writer.println(" * limitations under the License.");
        writer.println(" */");
    }

    private static void putGeneratedMark(PrintWriter writer) {
        writer.println("// DON'T EDIT THIS GENERATED FILE //");
    }

    private void printInclude(PrintWriter writer) {
        includes.getStandards().stream()
                .sorted()
                .forEach(it -> writer.printf("#include <%s>%n", it.getName()));
        if (!includes.getStandards().isEmpty()) {
            writer.println();
        }
        includes.getInternals().stream()
                .filter(it -> !it.equals(self))
                .sorted()
                .forEach(it -> putIncludeLine(writer, it));
        writer.println();
    }

    private PrintWriter putIncludeLine(PrintWriter writer, Name name) {
        return writer.printf("#include \"%s%s\"%n", name.join("/"), CppFiler.SUFFIX_INCLUDE);
    }

    private String getSelfNameDef() {
        return self.getTokens().stream()
                .flatMap(it -> Namer.parse(it).getSegments().stream())
                .map(String::toUpperCase)
                .collect(Collectors.joining("_")) + "_H_";
    }

    private static class Contents {

        final List<String> lines = new ArrayList<>();

        final List<String> currentNamespace = new ArrayList<>();

        int indentLevel = 0;

        Contents() {
            return;
        }

        void put(String string) {
            lines.add(indented(string));
        }

        void append(String string) {
            Preconditions.checkState(!lines.isEmpty());
            String last = lines.get(lines.size() - 1);
            if (last.isEmpty()) {
                lines.remove(lines.size() - 1);
                put(string);
            } else {
                lines.set(lines.size() - 1, last + string);
            }
        }

        private String indented(String string) {
            int level = indentLevel;
            if (string.isEmpty()) {
                return string;
            } else {
                return Strings.repeat(" ", level * 4) + string;
            }
        }

        void namespace(Name namespace) {
            List<String> nextNamespace = Optional.ofNullable(namespace)
                    .map(Name::getTokens)
                    .orElse(List.of());
            int start = commonPrefixLength(currentNamespace, nextNamespace);
            if (start < currentNamespace.size()) {
                List<String> range = currentNamespace.subList(start, currentNamespace.size());
                lines.add(String.format("}  // namespace %s", String.join("::", range)));
                for (int i = start, n = currentNamespace.size(); i < n; i++) {
                    currentNamespace.remove(currentNamespace.size() - 1);
                }
            }
            if (start < nextNamespace.size()) {
                List<String> range = nextNamespace.subList(start, nextNamespace.size());
                lines.add(String.format("namespace %s {", String.join("::", range)));
                currentNamespace.addAll(nextNamespace.subList(start, nextNamespace.size()));
            }
        }

        static int commonPrefixLength(List<String> a, List<String> b) {
            int length = Math.min(a.size(), b.size());
            for (int i = 0; i < length; i++) {
                if (!a.get(i).equals(b.get(i))) {
                    return i;
                }
            }
            return length;
        }

        void enter() {
            indentLevel++;
        }

        void exit() {
            Preconditions.checkState(indentLevel > 0);
            indentLevel--;
        }

        void printTo(PrintWriter writer) {
            namespace(null);
            if (lines.isEmpty()) {
                return;
            }
            lines.add("");
            boolean sawEmpty = false;
            for (String s : lines) {
                boolean empty = s.isEmpty();
                if (!empty || !sawEmpty) {
                    writer.println(s);
                }
                sawEmpty = empty;
            }
        }
    }

    /**
     * Represents a file kind.
     * @since WIP
     */
    public enum FileKind {

        /**
         * include files.
         */
        INCLUDE_FILE,

        /**
         * source files.
         */
        SOURCE_FILE,
    }
}
