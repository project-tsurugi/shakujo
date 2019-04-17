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
import com.ashigeru.lang.codegen.ir.model.SimpleName;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;

/**
 * Generates node visitors.
 * @since WIP
 */
public class VisitorGenerator {

    private static final String CONST_PREFIX = "Const";

    private static final String VISITOR_SUFFIX = "Visitor";

    private static final String TYPE_RETURN = "Return";

    private static final String TYPE_PARAMETERS = "Args";

    private final TypeDeclarationRepository repository;

    /**
     * Creates a new instance.
     * @param repository the current repository
     */
    public VisitorGenerator(TypeDeclarationRepository repository) {
        this.repository = repository;
    }

    /**
     * Generates the node visitor file.
     * @param filer the target filer
     * @param baseClass the base class of visitors
     * @throws IOException if I/O error was occurred
     */
    public void generate(CppFiler filer, ClassDeclaration baseClass) throws IOException {
        generateVisitor(filer, baseClass, false);
        generateVisitor(filer, baseClass, true);
    }

    private void generateVisitor(CppFiler filer, ClassDeclaration baseClass, boolean isConst) throws IOException {
        CppPrinter printer = generateVisitor(baseClass, isConst);
        printer.printTo(filer);
    }

    private static Name getTypeName(TypeDeclaration baseClass, boolean isConst) {
        Name name = baseClass.getName();
        SimpleName last = Name.of(
                (isConst ? CONST_PREFIX : "")
                + name.getLast().getToken()
                + VISITOR_SUFFIX);
        return name.getParent()
                .map(it -> (Name) it.resolve(last))
                .orElse(last);
    }

    private CppPrinter generateVisitor(ClassDeclaration baseClass, boolean isConst) {
        CppPrinter printer = CppPrinter.includeFile(getTypeName(baseClass, isConst));
        List<ClassDeclaration> targets = repository.getSubtypes(baseClass.asTypeMirror());
        baseClass.getName().getParent().ifPresent(printer::namespace);
        generateVisitorBody(printer, baseClass, targets, isConst);
        return printer;
    }

    private static void generateVisitorBody(
            CppPrinter printer, TypeDeclaration baseClass, List<ClassDeclaration> targets,
            boolean isConst) {
        printer.put("/**");
        printer.put(" * @brief A visitor for %s.", Util.getSimpleName(baseClass));
        printer.put(" * The default implementation of visit() will be redirected to visitDefault().");
        printer.put(" * @tparam %s the result type", TYPE_RETURN);
        printer.put(" * @tparam %s the parameter types", TYPE_PARAMETERS);
        printer.put(" * @see %s", printer.getContextName(baseClass));
        printer.put(" */");
        printer.put("template <class %s, class... %s>", TYPE_RETURN, TYPE_PARAMETERS);
        printer.put("class %s {", printer.getName().getLast().getToken());
        printer.put("public:");
        printer.indent(() -> {
            processDefault(printer, baseClass, isConst);
            for (ClassDeclaration target : targets) {
                processNode(printer, target, isConst);
            }
            processBase(printer, baseClass, targets, isConst);
        });
        printer.put("};");
        printer.put();
    }

    private static void processDefault(CppPrinter printer, TypeDeclaration target, boolean isConst) {
        printer.getIncludes().add(IncludeList.Standard.SSTREAM);
        printer.getIncludes().add(IncludeList.Standard.STDEXCEPT);
        printer.put("/**");
        printer.put(" * @brief Processes the default %s.", Util.getSimpleName(target));
        printer.put(" * @param node the processing target");
        printer.put(" * @param args the processing arguments");
        printer.put(" * @return the result");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" */");
        printer.put("virtual %s visitDefault(%s%s* node, [[maybe_unused]] %s... args) {%s",
                TYPE_RETURN,
                printer.getContextName(target),
                isConst ? " const" : "",
                TYPE_PARAMETERS,
                isConst ? "" : "  // NOLINT");
        printer.indent(() -> {
            printer.put("std::ostringstream ss;");
            printer.put("ss << \"visit for \" << node->kind() << \" is not implemented.\";");
            printer.put("throw std::runtime_error(ss.str());");
        });
        printer.put("}");
        printer.put();
    }

    private static void processNode(CppPrinter printer, ClassDeclaration target, boolean isConst) {
        printer.getIncludes().add(IncludeList.Standard.UTILITY);
        printer.put("/**");
        printer.put(" * @brief Processes %s.", Util.getSimpleName(target));
        printer.put(" * @param node the processing target");
        printer.put(" * @param args the processing arguments");
        printer.put(" * @return the result");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" */");
        printer.put("virtual %s visit(%s%s* node, %s... args) {%s", // FIXME
                TYPE_RETURN,
                printer.getContextName(target),
                isConst ? " const" : "",
                TYPE_PARAMETERS,
                isConst ? "" : "  // NOLINT");
        printer.indent(() -> {
            printer.put("return visitDefault(node, std::forward<%s>(args)...);", TYPE_PARAMETERS);
        });
        printer.put("}");
        printer.put();
    }

    private static void processBase(
            CppPrinter printer, TypeDeclaration baseClass, List<ClassDeclaration> targets,
            boolean isConst) {
        printer.getIncludes().add(KindGenerator.getTypeName(baseClass));
        printer.getIncludes().add(IncludeList.Standard.UTILITY);
        printer.put("/**");
        printer.put(" * @brief Dispatches visitor action for individual element types.");
        printer.put(" * @param node the processing target element");
        printer.put(" * @param args the processing arguments");
        printer.put(" * @return the result");
        printer.put(" */");
        printer.put("%s dispatch(%s%s* node, %s... args) {%s",
                TYPE_RETURN,
                printer.getContextName(baseClass),
                isConst ? " const" : "",
                TYPE_PARAMETERS,
                isConst ? "" : "  // NOLINT");
        printer.indent(() -> {
            printer.put("switch (node->kind()) {");
            for (ClassDeclaration target : targets) {
                printer.getIncludes().add(target.getName());
                printer.put("case %s::%s:",
                        printer.getContextName(target),
                        KindGenerator.NON_VIRTUAL_KIND_NAME);
                printer.indent(() -> {
                    printer.put("return visit(static_cast<%s%s*>(node), std::forward<%s>(args)...);  // NOLINT",
                            printer.getContextName(target),
                            isConst ? " const" : "",
                            TYPE_PARAMETERS);
                });
            }
            printer.put("}");
            printer.put("// may not occur");
            printer.put("return visitDefault(node, std::forward<%s>(args)...);",
                    TYPE_PARAMETERS);
        });
        printer.put("}");
        printer.put();
    }
}
