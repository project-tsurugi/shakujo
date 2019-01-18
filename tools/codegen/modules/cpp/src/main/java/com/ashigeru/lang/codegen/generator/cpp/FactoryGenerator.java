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
import java.util.ArrayDeque;
import java.util.Comparator;
import java.util.Deque;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import com.ashigeru.lang.codegen.generator.cpp.IncludeList.Standard;
import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.DeclaredTypeMirror;
import com.ashigeru.lang.codegen.ir.model.EnumTypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.ListTypeMirror;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.model.TypeMirror;
import com.google.common.base.Preconditions;

/**
 * Generates factory class.
 * @since WIP
 */
public class FactoryGenerator {

    private static final String TEMP_RET = "ret";

    private final TypeDeclarationRepository repository;

    private final Name moveInitList;

    /**
     * Creates a new instance.
     * @param repository the current repository
     * @param moveInitList move initializer list
     */
    public FactoryGenerator(TypeDeclarationRepository repository, Name moveInitList) {
        this.repository = repository;
        this.moveInitList = moveInitList;
    }

    /**
     * Generates factory class.
     * @param filer target filer
     * @param name factory class name
     * @throws IOException if I/O error was occurred
     */
    public void generate(CppFiler filer, Name name) throws IOException {
        generateHeader(filer, name);
        generateSource(filer, name);
    }

    private void generateHeader(CppFiler filer, Name name) throws IOException {
        CppPrinter printer = CppPrinter.includeFile(name);

        printer.getName().getParent().ifPresent(printer::namespace);
        printer.put("/**");
        printer.put(" * @brief a base class of IR model factory.");
        printer.put(" */");
        printer.put("class %s {", printer.getName().getLast().getToken());
        printer.put("public:");
        printer.indent(() -> {
            getTargets().forEach(it -> generatePrototype(printer, it));
        });
        printer.put("};");
        printer.namespace(null);
        printer.printTo(filer);
    }

    private void generatePrototype(CppPrinter printer, ClassDeclaration decl) {
        if (isDefaultRequired(decl)) {
            generateEmptyPrototype(printer, decl);
        }
        if (isParameterizedRequired(decl)) {
            generateDataPrototype(printer, decl);
        }
        decl.members(ClassDeclaration.class).forEach(it -> generatePrototype(printer, it));
    }

    private static boolean isDefaultRequired(ClassDeclaration decl) {
        return !decl.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE)
                && (!decl.getAttributes().contains(ClassDeclaration.Attribute.VALUE)
                        || parameters(decl).findAny().isPresent() == false);
    }

    private static boolean isParameterizedRequired(ClassDeclaration decl) {
        return !decl.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE)
                && !decl.getAttributes().contains(ClassDeclaration.Attribute.EXTERN)
                && parameters(decl).findAny().isPresent();
    }

    private void generateEmptyPrototype(CppPrinter printer, ClassDeclaration decl) {
        printer.getIncludes().add(IncludeList.Standard.MEMORY);
        includeTop(printer, decl.asTypeMirror());
        printer.put("/**");
        printer.put(" * @brief returns a new empty %s.", printer.getContextName(decl));
        printer.put(" * @return a created empty node");
        printer.put(" * @see %s", printer.getContextName(decl));
        printer.put(" */");
        printer.put("virtual std::unique_ptr<%s> %s();",
                printer.getContextName(decl),
                getMemberName(decl));
        printer.put();
    }

    private void generateDataPrototype(CppPrinter printer, ClassDeclaration decl) {
        printer.getIncludes().add(IncludeList.Standard.MEMORY);
        includeTop(printer, decl.asTypeMirror());
        printer.put("/**");
        printer.put(" * @brief returns a new %s.", printer.getContextName(decl));
        parameters(decl).forEach(it -> generateParameterDoc(printer, it));
        printer.put(" * @return a created node");
        printer.put(" * @see %s", printer.getContextName(decl));
        printer.put(" */");
        printer.put("std::unique_ptr<%s> %s(",
                printer.getContextName(decl),
                getMemberName(decl));
        printer.indent(2, () -> generateParameterDecls(printer, decl, true));
        printer.append(");");
        printer.put();
    }

    private void generateParameterDoc(CppPrinter printer, PropertyDeclaration decl) {
        printer.put(" * @param %s %s",
                Util.getSimpleName(decl),
                decl.getDescription().orElseGet(() -> decl.getType().toString()));
    }

    private void generateSource(CppFiler filer, Name name) throws IOException {
        CppPrinter printer = CppPrinter.sourceFile(name);

        printer.getName().getParent().ifPresent(printer::namespace);
        getTargets().forEach(it -> generateBody(printer, it));
        printer.namespace(null);
        printer.printTo(filer);
    }

    private void generateBody(CppPrinter printer, ClassDeclaration decl) {
        if (isDefaultRequired(decl)) {
            generateEmptyBody(printer, decl);
        }
        if (isParameterizedRequired(decl)) {
            generateDataBody(printer, decl);
        }
        decl.members(ClassDeclaration.class).forEach(it -> generateBody(printer, it));
    }

    private void generateEmptyBody(CppPrinter printer, ClassDeclaration decl) {
        printer.getIncludes().add(IncludeList.Standard.MEMORY);
        printer.put("std::unique_ptr<%s> %s::%s() {",
                printer.getContextName(decl),
                printer.getContextName(printer.getName()),
                getMemberName(decl));
        printer.indent(() -> {
            printer.put("return std::make_unique<%s>();", printer.getContextName(decl));
        });
        printer.put("}");
        printer.put();
    }

    private void generateDataBody(CppPrinter printer, ClassDeclaration decl) {
        printer.getIncludes().add(IncludeList.Standard.MEMORY);
        printer.put("std::unique_ptr<%s> %s::%s(",
                printer.getContextName(decl),
                printer.getContextName(printer.getName()),
                getMemberName(decl));
        printer.indent(2, () -> generateParameterDecls(printer, decl, false));
        printer.append(") {");
        printer.indent(() -> {
            if (isDefaultRequired(decl)) {
                printer.put("auto %s = %s();", TEMP_RET, getMemberName(decl));
            } else {
                printer.put("auto %s = std::make_unique<%s>();", TEMP_RET, printer.getContextName(decl));
            }
            assert parameters(decl).findAny().isPresent();
            parameters(decl).forEach(it -> generateBodyLine(printer, it));
            printer.put("return %s;", TEMP_RET);
        });
        printer.put("}");
        printer.put();
    }

    private void generateBodyLine(CppPrinter printer, PropertyDeclaration decl) {
        TypeMirror type = decl.getType();
        switch (getTypeKind(type)) {
        case PRIMITIVE:
        case ENUM:
        case VALUE_FLAT:
            // just copy
            printer.put("%2$s->%1$s(%1$s);", Util.getSimpleName(decl), TEMP_RET);
            break;
        case DATA:
        case NODE:
        case VALUE_POLY:
            printer.getIncludes().add(Standard.UTILITY);
            printer.put("%2$s->%1$s(std::move(%1$s));", Util.getSimpleName(decl), TEMP_RET);
            break;
        case BASIC_LIST:
        case ENUM_LIST:
            printer.put("%2$s->%1$s() = %1$s;", Util.getSimpleName(decl), TEMP_RET);
            break;
        case NODE_LIST:
        case FRAGMENT_LIST:
            printer.getIncludes().add(Standard.UTILITY);
            printer.put("%2$s->%1$s() = std::move(%1$s).build();", Util.getSimpleName(decl), TEMP_RET);
            break;
        default:
            throw new AssertionError(decl);
        }
    }

    private String getMemberName(ClassDeclaration decl) {
        Deque<String> segments = new ArrayDeque<>();
        TypeDeclaration current = decl;
        while (current != null) {
            segments.addFirst(current.getName().getLast().getToken());
            current = current.getName().getParent()
                    .flatMap(repository::find)
                    .orElse(null);
        }
        return String.join("", segments);
    }

    private void generateParameterDecls(CppPrinter printer, ClassDeclaration decl, boolean prototype) {
        List<PropertyDeclaration> params = parameters(decl).collect(Collectors.toList());
        int index = 0;
        boolean sawDefault = false;
        for (PropertyDeclaration prop: params) {
            if (index != 0) {
                printer.append(",");
            }
            printer.put("%s %s", toNameString(printer, prop.getType()), Util.getSimpleName(prop));
            if (prototype && index != 0) { // must have at least one non-default parameter
                if (prop.getAttributes().contains(PropertyDeclaration.Attribute.OPTIONAL)) {
                    appendParameterDefaultValue(printer, prop);
                    sawDefault = true;
                } else {
                    Preconditions.checkState(!sawDefault);
                }
            }
            index++;
        }
    }

    private void appendParameterDefaultValue(CppPrinter printer, PropertyDeclaration prop) {
        switch (prop.getType().getKind()) {
        case DECLARED_TYPE: {
            TypeDeclaration tdecl = repository.find(prop.getType()).get();
            if (tdecl instanceof ClassDeclaration) {
                printer.append(" = {}");
            } else {
                EnumTypeDeclaration enumDecl = (EnumTypeDeclaration) tdecl;
                printer.append(" = %s", printer.getContextName(enumDecl.getDefaultValue().get()));
            }
            break;
        }
        case STRING_TYPE:
        case LIST_TYPE:
            printer.append(" = {}");
            break;
        case INT_TYPE:
        case FLOAT_TYPE:
        case SIZE_TYPE:
        case BOOL_TYPE:
            break;
        default:
            throw new AssertionError(prop);
        }
    }

    private String toNameString(CppPrinter printer, TypeMirror type) {
        switch (getTypeKind(type)) {
        case PRIMITIVE:
        case ENUM:
            return getBareName(printer, type);
        case DATA:
            return getBareName(printer, type);
        case VALUE_FLAT:
            return String.format("%s const&", getBareName(printer, type));
        case VALUE_POLY:
        case NODE:
            return String.format("std::unique_ptr<%s>", getBareName(printer, type));
        case BASIC_LIST:
        case ENUM_LIST:
            printer.getIncludes().add(IncludeList.Standard.INITIALIZER_LIST);
            return String.format("%s<%s>",
                    printer.getContextName(IncludeList.INITIALIZER_LIST_TYPE),
                    getBareName(printer, component(type)));
        case NODE_LIST:
        case FRAGMENT_LIST:
            printer.getIncludes().add(moveInitList);
            printer.getIncludes().add(IncludeList.Standard.MEMORY);
            return String.format("%s<std::unique_ptr<%s>>",
                    printer.getContextName(moveInitList),
                    getBareName(printer, component(type)));
        default:
            throw new AssertionError(type);
        }
    }

    private String getBareName(CppPrinter printer, TypeMirror type) {
        IncludeList includes = printer.getIncludes();
        switch (type.getKind()) {
        case BOOL_TYPE:
            return printer.getContextName(IncludeList.BOOL_TYPE);
        case INT_TYPE:
            includes.add(IncludeList.Standard.STDINT);
            return printer.getContextName(IncludeList.INT_TYPE);
        case FLOAT_TYPE:
            return printer.getContextName(IncludeList.FLOAT_TYPE);
        case SIZE_TYPE:
            includes.add(IncludeList.Standard.STDDEF);
            return printer.getContextName(IncludeList.SIZE_TYPE);
        case STRING_TYPE:
            includes.add(IncludeList.Standard.STRING);
            return printer.getContextName(IncludeList.STRING_TYPE);
        case DECLARED_TYPE:
            includeTop(printer, type);
            return printer.getContextName(((DeclaredTypeMirror) type).getName());
        default:
            throw new AssertionError(type);
        }
    }

    private void includeTop(CppPrinter printer, TypeMirror type) {
        TypeDeclaration current = repository.find(type).orElse(null);
        while (true) {
            TypeDeclaration next = current.getName().getParent()
                    .flatMap(repository::find)
                    .orElse(null);
            if (next == null) {
                printer.getIncludes().add(current.getName());
                break;
            }
            current = next;
        }
    }

    private static TypeMirror component(TypeMirror list) {
        Preconditions.checkArgument(list.getKind() == TypeMirror.Kind.LIST_TYPE);
        return ((ListTypeMirror) list).getComponentType();
    }

    private static Stream<PropertyDeclaration> parameters(ClassDeclaration decl) {
        return decl.members(PropertyDeclaration.class)
                .filter(it -> !it.getAttributes().contains(PropertyDeclaration.Attribute.TRANSIENT));
    }

    enum TypeKind {
        PRIMITIVE,
        ENUM, // enum constants
        DATA, // non-inheritable, copyable
        VALUE_FLAT, // non-inheritable
        VALUE_POLY, // inheritable
        NODE,
        FRAGMENT,
        BASIC_LIST,
        ENUM_LIST,
        NODE_LIST,
        FRAGMENT_LIST,
    }

    private TypeKind getTypeKind(TypeMirror type) {
        switch (type.getKind()) {
        case BOOL_TYPE:
        case INT_TYPE:
        case FLOAT_TYPE:
        case SIZE_TYPE:
            return TypeKind.PRIMITIVE;
        case STRING_TYPE:
            return TypeKind.DATA;
        case DECLARED_TYPE: {
            TypeDeclaration typeDecl = repository.find(type).get();
            if (typeDecl instanceof ClassDeclaration) {
                if (typeDecl.getAttributes().contains(ClassDeclaration.Attribute.VALUE)) {
                    if (typeDecl.getAttributes().contains(ClassDeclaration.Attribute.FINAL)) {
                        return TypeKind.VALUE_FLAT;
                    } else {
                        return TypeKind.VALUE_POLY;
                    }
                } else if (typeDecl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                    return TypeKind.FRAGMENT;
                }
                return TypeKind.NODE;
            } else if (typeDecl instanceof EnumTypeDeclaration) {
                return TypeKind.ENUM;
            } else {
                throw new AssertionError(type);
            }
        }
        case LIST_TYPE: {
            switch (getTypeKind(((ListTypeMirror) type).getComponentType())) {
            case PRIMITIVE:
            case DATA:
                return TypeKind.BASIC_LIST;
            case ENUM:
                return TypeKind.ENUM_LIST;
            case FRAGMENT:
                return TypeKind.FRAGMENT_LIST;
            case NODE:
                return TypeKind.NODE_LIST;
            default:
                throw new IllegalStateException(type.toString());
            }
        }
        default:
            throw new AssertionError(type);
        }
    }

    private Stream<ClassDeclaration> getTargets() {
        return repository.getElements().stream()
                .filter(it -> it instanceof ClassDeclaration)
                .map(it -> (ClassDeclaration) it)
                .filter(it -> !it.getAttributes().contains(ClassDeclaration.Attribute.VALUE))
                .filter(it -> !it.getAttributes().contains(ClassDeclaration.Attribute.FOREIGN))
                .filter(it -> !it.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE))
                .sorted(Comparator.comparing(ClassDeclaration::getName));
    }
}
