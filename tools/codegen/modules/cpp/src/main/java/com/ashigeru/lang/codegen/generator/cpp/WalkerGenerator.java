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
import java.util.Comparator;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.ListTypeMirror;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.SimpleName;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.model.TypeMirror;

/**
 * Generates node walker.
 * @since WIP
 */
public class WalkerGenerator {

    private static final String CONST_PREFIX = "Const";

    private final TypeDeclarationRepository repository;

    private final CppFiler filer;

    /**
     * Creates a new instance.
     * @param repository the current repository
     * @param filer the target filer
     */
    public WalkerGenerator(TypeDeclarationRepository repository, CppFiler filer) {
        this.repository = repository;
        this.filer = filer;
    }

    /**
     * Generates the node walker.
     * @param typeName the non-const walker type name
     * @param rootClass the root class name
     * @param baseClasses the base classes
     * @throws IOException if I/O error was occurred
     */
    public void generate(Name typeName, ClassDeclaration rootClass, List<ClassDeclaration> baseClasses) throws IOException {
        generateWalker(typeName, rootClass, baseClasses, false);
        generateWalker(typeName, rootClass, baseClasses, true);
    }

    private void generateWalker(
            Name typeName,
            ClassDeclaration rootClass,
            List<ClassDeclaration> baseClasses,
            boolean isConst) throws IOException {
        CppPrinter header = generateWalkerHeader(getTypeName(typeName, isConst), rootClass, baseClasses, isConst);
        header.printTo(filer);

        CppPrinter source = generateWalkerSource(getTypeName(typeName, isConst), rootClass, baseClasses, isConst);
        source.printTo(filer);
    }

    private static Name getTypeName(Name typeName, boolean isConst) {
        if (isConst) {
            SimpleName lastName = Name.of(CONST_PREFIX + typeName.getLast().getToken());
            return typeName.getParent().<Name>map(it -> it.resolve(lastName)).orElse(lastName);
        } else {
            return typeName;
        }
    }

    private CppPrinter generateWalkerHeader(
            Name typeName,
            ClassDeclaration rootClass,
            List<ClassDeclaration> baseClasses,
            boolean isConst) {
        CppPrinter printer = CppPrinter.includeFile(typeName);
        typeName.getParent().ifPresent(printer::namespace);
        Map<ClassDeclaration, List<ClassDeclaration>> subtypes = collectSubtypes(baseClasses);

        printer.put("/**");
        printer.put(" * @brief An IR node walker.");
        printer.put(" */");
        printer.put("class %s {", printer.getName().getLast().getToken());
        printer.put("protected:");
        printer.indent(() -> {
            subtypes.forEach((base, subs) -> {
                subs.forEach(it -> {
                    generateEnterMethod(printer, it, base, isConst);
                    generateExitMethod(printer, it, base, isConst);
                });
            });
            subtypes.keySet().stream().filter(it -> it != null).forEach(it -> {
                generateEnterDefaultMethod(printer, it, isConst);
                generateExitDefaultMethod(printer, it, isConst);
            });
        });
        printer.put("public:");
        printer.indent(() -> {
            generateWalkerMethodDecl(printer, rootClass, isConst);
            subtypes.forEach((base, subs) -> {
                if (base != null) {
                    generateWalkerMethodDecl(printer, base, isConst);
                }
                subs.forEach(it -> generateWalkerMethodDecl(printer, it, isConst));
            });
        });
        printer.put("};");
        printer.put();
        return printer;
    }

    private Map<ClassDeclaration, List<ClassDeclaration>> collectSubtypes(List<ClassDeclaration> baseClasses) {
        Set<ClassDeclaration> saw = new HashSet<>();
        Map<ClassDeclaration, List<ClassDeclaration>> subtypes = new LinkedHashMap<>();
        for (ClassDeclaration baseClass : baseClasses) {
            List<ClassDeclaration> targets = repository.getSubtypes(baseClass.asTypeMirror());
            subtypes.put(baseClass, targets);
            saw.add(baseClass);
            saw.addAll(targets);
        }
        List<ClassDeclaration> solos = repository.getElements().stream()
                .filter(it -> it instanceof ClassDeclaration)
                .map(it -> (ClassDeclaration) it)
                .filter(it -> !saw.contains(it))
                .filter(it -> !it.getAttributes().contains(ClassDeclaration.Attribute.VALUE))
                .filter(it -> !it.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE))
                .collect(Collectors.toList());
        subtypes.put(null, solos);
        return subtypes;
    }

    private static void generateEnterDefaultMethod(
            CppPrinter printer, ClassDeclaration target, boolean isConst) {
        printer.put("/**");
        printer.put(" * @brief callback on enter into %s.", Util.getSimpleName(target));
        printer.put(" * @param node the processing target");
        printer.put(" * @return true if continue to enter into child elements of the given node");
        printer.put(" * @return false don't walk into child elements");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" * @see walk()");
        printer.put(" * @see exitDefault()");
        printer.put(" */");
        printer.put("virtual bool enterDefault([[maybe_unused]] %s%s* node) {",
                printer.getContextName(target),
                isConst ? " const" : "");
        printer.indent(() -> {
            printer.put("return true;");
        });
        printer.put("}");
        printer.put();
    }

    private static void generateExitDefaultMethod(
            CppPrinter printer, ClassDeclaration target, boolean isConst) {
        printer.put("/**");
        printer.put(" * @brief callback on exit from %s.", Util.getSimpleName(target));
        printer.put(" * @param node the processing target");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" * @see walk()");
        printer.put(" * @see enterDefault()");
        printer.put(" */");
        printer.put("virtual void exitDefault([[maybe_unused]] %s%s* node) {}",
                printer.getContextName(target),
                isConst ? " const" : "");
        printer.put();
    }

    private static void generateEnterMethod(
            CppPrinter printer, ClassDeclaration target, ClassDeclaration delegate, boolean isConst) {
        printer.getIncludes().add(target.getName());
        printer.put("/**");
        printer.put(" * @brief callback on enter into %s.", Util.getSimpleName(target));
        printer.put(" * @param node the processing target");
        printer.put(" * @return true if continue to enter into child elements of the given node");
        printer.put(" * @return false don't walk into child elements, also the corresponded exit() will not be called");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" * @see walk()");
        printer.put(" * @see exit()");
        printer.put(" */");
        printer.put("virtual bool enter([[maybe_unused]] %s%s* node) {",
                printer.getContextName(target),
                isConst ? " const" : "");
        printer.indent(() -> {
            if (delegate == null) {
                printer.put("return true;");
            } else {
                printer.put("return enterDefault(node);");
            }
        });
        printer.put("}");
        printer.put();
    }

    private static void generateExitMethod(
            CppPrinter printer, ClassDeclaration target, ClassDeclaration delegate, boolean isConst) {
        printer.getIncludes().add(target.getName());
        printer.put("/**");
        printer.put(" * @brief callback on exit from %s.", Util.getSimpleName(target));
        printer.put(" * @param node the processing target");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" * @see walk()");
        printer.put(" * @see enter()");
        printer.put(" */");
        printer.put("virtual void exit([[maybe_unused]] %s%s* node) {",
                printer.getContextName(target),
                isConst ? " const" : "");
        if (delegate == null) {
            printer.append("}");
        } else {
            printer.indent(() -> {
                printer.put("exitDefault(node);");
            });
            printer.put("}");
        }
        printer.put();
    }

    private static void generateWalkerMethodDecl(
            CppPrinter printer, ClassDeclaration target, boolean isConst) {
        printer.getIncludes().add(target.getName());
        printer.put("/**");
        printer.put(" * @brief Begins to walk %s.", Util.getSimpleName(target));
        printer.put(" * @param node the processing target");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" */");
        printer.put("void walk(%s%s* node);",
                printer.getContextName(target),
                isConst ? " const" : "");
        printer.put();
    }

    private CppPrinter generateWalkerSource(
            Name typeName,
            ClassDeclaration rootClass,
            List<ClassDeclaration> baseClasses,
            boolean isConst) {
        CppPrinter printer = CppPrinter.sourceFile(typeName);
        typeName.getParent().ifPresent(printer::namespace);
        Map<ClassDeclaration, List<ClassDeclaration>> subtypes = collectSubtypes(baseClasses);

        generateRootMethodDef(printer, rootClass, isConst);
        subtypes.forEach((base, subs) -> {
            if (base != null) {
                generateWalkerMethodDef(printer, base, subs, isConst);
            }
            subs.forEach(it -> generateWalkerMethodDef(printer, it, isConst));
        });
        return printer;
    }

    private void generateRootMethodDef(CppPrinter printer, ClassDeclaration rootClass, boolean isConst) {
        printer.put("void %s::walk(%s%s* node) {",
                printer.getContextName(printer.getName()),
                printer.getContextName(rootClass),
                isConst ? " const" : "");
        List<ClassDeclaration> targets = repository.getElements().stream()
            .filter(it -> it instanceof ClassDeclaration)
            .map(it -> (ClassDeclaration) it)
            .filter(it -> it.getSuperTypes().contains(rootClass.asTypeMirror()))
            .sorted(Comparator.comparing(ClassDeclaration::getName))
            .collect(Collectors.toList());
        printer.indent(() -> {
            for (var base : targets) {
                printer.put("if (auto ptr = dynamic_cast<%s%s*>(node)) {",
                        printer.getContextName(base),
                        isConst ? " const" : "");
                printer.indent(() -> {
                    printer.put("walk(ptr);");
                    printer.put("return;");
                });
                printer.put("}");
            }
            printer.getIncludes().add(IncludeList.Standard.STDLIB);
            printer.put("std::abort();");
        });
        printer.put("}");
        printer.put();
    }

    private static void generateWalkerMethodDef(
            CppPrinter printer, ClassDeclaration target, List<ClassDeclaration> children, boolean isConst) {
        printer.put("void %s::walk(%s%s* node) {",
                printer.getContextName(printer.getName()),
                printer.getContextName(target),
                isConst ? " const" : "");
        printer.indent(() -> {
            printer.put("switch (node->kind()) {");
            printer.getIncludes().add(KindGenerator.getTypeName(target));
            for (ClassDeclaration child : children) {
                printer.put("case %s::%s:",
                        printer.getContextName(KindGenerator.getTypeName(target)),
                        KindGenerator.getConstantName(child));
                printer.indent(() -> {
                    printer.put("walk(static_cast<%s%s*>(node));  // NOLINT",
                            printer.getContextName(child),
                            isConst ? " const" : "");
                    printer.put("return;");
                });
            }
            printer.put("}");
            printer.getIncludes().add(IncludeList.Standard.STDEXCEPT);
            printer.getIncludes().add(IncludeList.Standard.SSTREAM);
            printer.put("std::ostringstream ss;");
            printer.put("ss << \"unknown node kind: \" << node->kind();");
            printer.put("throw std::invalid_argument(ss.str());");
        });
        printer.put("}");
        printer.put();
    }

    private void generateWalkerMethodDef(CppPrinter printer, ClassDeclaration target, boolean isConst) {
        printer.put("void %s::walk(%s%s* node) {",
                printer.getContextName(printer.getName()),
                printer.getContextName(target),
                isConst ? " const" : "");
        printer.indent(() -> {
            printer.put("if (!enter(node)) return;");
            processWalkerBody(printer, target, "node");
            printer.put("exit(node);");
        });
        printer.put("}");
        printer.put();
    }

    private void processWalkerBody(CppPrinter printer, ClassDeclaration decl, String var) {
        decl.members(PropertyDeclaration.class)
                .filter(it -> isNodeLike(it.getType()))
                .forEach(it -> generateWalkerBody(printer, it, var));
    }

    private void generateWalkerBody(CppPrinter printer, PropertyDeclaration decl, String var) {
        TypeMirror type = decl.getType();
        String elem;
        switch (var) {
        case "node":
            elem = "child";
            break;
        case "child":
            elem = "child2";
            break;
        default:
            throw new AssertionError(var);
        }
        if (type instanceof ListTypeMirror) {
            ClassDeclaration cdecl = repository
                    .find(ClassDeclaration.class, ((ListTypeMirror) type).getComponentType()).get();
            printer.put("for (auto %s : %s->%s()) {", elem, var, Util.getSimpleName(decl));
            printer.indent(() -> { // FIXME check fragment style
                if (cdecl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                    processWalkerBody(printer, cdecl, elem);
                } else {
                    printer.put("if (%1$s) walk(%1$s);", elem);
                }
            });
            printer.put("}");
        } else {
            ClassDeclaration cdecl = repository.find(ClassDeclaration.class, type).get();
            if (cdecl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                printer.put("{");
                printer.indent(() -> { // FIXME check fragment style
                    printer.put("auto %s = %s->%s();", elem, var, Util.getSimpleName(decl));
                    processWalkerBody(printer, cdecl, elem);
                });
                printer.put("}");
            } else {
                printer.put("if (%1$s->%2$s()) walk(%1$s->%2$s());", var, Util.getSimpleName(decl));
            }
        }
    }

    private boolean isNodeLike(TypeMirror type) {
        switch (type.getKind()) {
        case DECLARED_TYPE:
            return repository.find(ClassDeclaration.class, type)
                    .filter(it -> !it.getAttributes().contains(ClassDeclaration.Attribute.VALUE))
                    .isPresent();
        case LIST_TYPE:
            return isNodeLike(((ListTypeMirror) type).getComponentType());
        default:
            return false;
        }
    }
}
