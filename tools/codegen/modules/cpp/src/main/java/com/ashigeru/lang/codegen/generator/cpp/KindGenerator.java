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
import java.util.List;

import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;

/**
 * Generates node kinds.
 * @since WIP
 */
public class KindGenerator {

    private static final String KIND_SUFFIX = "Kind";

    private final TypeDeclarationRepository repository;

    /**
     * Creates a new instance.
     * @param repository the current repository
     */
    public KindGenerator(TypeDeclarationRepository repository) {
        this.repository = repository;
    }

    /**
     * Generates the node kind file.
     * @param filer the target filer
     * @param baseClass the base class of kind
     * @throws IOException if I/O error was occurred
     */
    public void generate(CppFiler filer, TypeDeclaration baseClass) throws IOException {
        generateHeader(filer, baseClass);
    }

    private void generateHeader(CppFiler filer, TypeDeclaration baseClass) throws IOException {
        CppPrinter printer = generateHeader(baseClass);
        printer.printTo(filer);
    }

    /**
     * Returns the kind type name of the given type declaration.
     * @param baseClass the base class of kind
     * @return the kind class name
     */
    public static Name getTypeName(TypeDeclaration baseClass) {
        return baseClass.getName().addSuffix(KIND_SUFFIX);
    }

    /**
     * Returns the kind constant name of the given type declaration.
     * @param target the target class
     * @return the simple name of the enum constant
     */
    public static String getConstantName(ClassDeclaration target) {
        return Util.toEnumConstantName(target.getName().getLast().getToken());
    }

    private CppPrinter generateHeader(TypeDeclaration baseClass) {
        CppPrinter printer = CppPrinter.includeFile(getTypeName(baseClass));
        List<ClassDeclaration> targets = repository.getSubtypes(baseClass.asTypeMirror());
        baseClass.getName().getParent().ifPresent(printer::namespace);
        generateBody(printer, baseClass, targets);
        printer.put();
        processPrinting(printer, targets);
        return printer;
    }

    private static void generateBody(CppPrinter printer, TypeDeclaration base, List<ClassDeclaration> targets) {
        printer.put("/**");
        printer.put(" * @brief Represents the catalog of %s.", Util.getSimpleName(base));
        printer.put(" * @see %s::kind()", printer.getContextName(base));
        printer.put(" */");
        printer.put("enum class %s {", printer.getName().getLast().getToken());
        printer.indent(() -> {
            for (ClassDeclaration target : targets) {
                printer.put("/**");
                printer.put(" * @brief tag for %s.", Util.getSimpleName(target));
                printer.put(" * @see %s", printer.getContextName(target));
                printer.put(" */");
                printer.put("%s,", getConstantName(target));
            }
        });
        printer.put("};");
    }

    private static void processPrinting(CppPrinter printer, List<ClassDeclaration> targets) {
        printer.getIncludes().add(IncludeList.Standard.STRING);
        printer.getIncludes().add(IncludeList.Standard.STRING_VIEW);
        printer.put("/**");
        printer.put(" * @brief returns string representation of the given value.");
        printer.put(" * @param value the target enum constant");
        printer.put(" * @return string representation");
        printer.put(" * @see %s", printer.getContextName(printer.getName()));
        printer.put(" */");
        printer.put("inline constexpr std::string_view to_string_view(%s value) {",
                printer.getContextName(printer.getName()));
        printer.indent(() -> {
            printer.put("switch (value) {");
            for (ClassDeclaration c : targets) {
                printer.indent(() -> {
                    printer.put("case %s::%s: return \"%s\";",
                            printer.getContextName(printer.getName()),
                            getConstantName(c),
                            getConstantName(c));
                });
            }
            printer.put("}");
            printer.put("return \"(unknown)\";");
        });
        printer.put("}");
        printer.put();
        printer.getIncludes().add(IncludeList.Standard.IOSTREAM);
        printer.put("/**");
        printer.put(" * @brief Appends short name into the given output stream.");
        printer.put(" * @param out the target output stream");
        printer.put(" * @param value the target enum constant");
        printer.put(" * @return the output stream");
        printer.put(" * @see %s", printer.getContextName(printer.getName()));
        printer.put(" */");
        printer.put("inline std::ostream& operator<<(std::ostream& out, %s value) {",
                printer.getContextName(printer.getName()));
        printer.indent(() -> {
            printer.put("return out << to_string_view(value);");
        });
        printer.put("}");
        printer.put();
    }
}
