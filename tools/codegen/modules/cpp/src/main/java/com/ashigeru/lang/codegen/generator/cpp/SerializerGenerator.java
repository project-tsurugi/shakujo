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
import java.util.Collection;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.stream.Stream;

import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.EnumTypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.ListTypeMirror;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.SimpleName;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.model.TypeMirror;

/**
 * Generates node serializer.
 * @since WIP
 */
public class SerializerGenerator {

    private final TypeDeclarationRepository repository;

    private final CppFiler filer;

    private final Name printerTypeName;

    private final Name utilityFileName;

    /**
     * Creates a new instance.
     * @param repository the current repository
     * @param filer the target filer
     * @param printerName collaborating printer type name
     * @param utilityFileName the utility file name which provides {@code append(ostream, T)}
     */
    public SerializerGenerator(
            TypeDeclarationRepository repository,
            CppFiler filer,
            Name printerName,
            Name utilityFileName) {
        this.repository = repository;
        this.filer = filer;
        this.printerTypeName = printerName;
        this.utilityFileName = utilityFileName;
    }

    /**
     * Generates the node walker.
     * @param typeName the type name
     * @param baseClasses the base classes
     * @throws IOException if I/O error was occurred
     */
    public void generate(Name typeName, Collection<? extends ClassDeclaration> baseClasses) throws IOException {
        CppPrinter header = generateWalkerHeader(typeName);
        header.printTo(filer);

        CppPrinter source = generateWalkerSource(typeName, new HashSet<>(baseClasses));
        source.printTo(filer);
    }

    private CppPrinter generateWalkerHeader(Name typeName) {
        CppPrinter printer = CppPrinter.includeFile(typeName);
        typeName.getParent().ifPresent(printer::namespace);
        printer.put("/**");
        printer.put(" * @brief An abstract implementation of IR node serializer.");
        printer.put(" */");
        printer.put("class %s {", getClassSimpleName(printer));
        printer.put("private:");
        printer.indent(() -> {
            generatePimpls(printer);
        });
        printer.put("public:");
        printer.indent(() -> {
            generateCtorDecl(printer);
            generateDtorDecl(printer);
            generateCopyDecl(printer);
            generateMoveDecl(printer);
            generateConfigDecl(printer);
            targets().forEach(it -> generateSerializeDecl(printer, it));
        });
        printer.put("};");
        printer.put();
        return printer;
    }

    private static void generatePimpls(CppPrinter printer) {
        printer.getIncludes().add(IncludeList.Standard.MEMORY);
        printer.put("class Impl;");
        printer.put("std::unique_ptr<Impl> impl_;");
        printer.put();
    }

    private static void generateCtorDecl(CppPrinter printer) {
        printer.put("/**");
        printer.put(" * @brief constructs a new object.");
        printer.put(" */");
        printer.put("%s();", getClassSimpleName(printer));
        printer.put();
    }

    private static void generateDtorDecl(CppPrinter printer) {
        printer.put("/**");
        printer.put(" * @brief destroys this object.");
        printer.put(" */");
        printer.put("virtual ~%s() noexcept;", getClassSimpleName(printer));
        printer.put();
    }

    private static void generateCopyDecl(CppPrinter printer) {
        printer.put("/**");
        printer.put(" * @brief constructs a new object.");
        printer.put(" * @param other the copy source");
        printer.put(" */");
        printer.put("%1$s(%1$s const& other) = delete;", getClassSimpleName(printer));
        printer.put();
        printer.put("/**");
        printer.put(" * @brief assigns the given object into this.");
        printer.put(" * @param other the copy source");
        printer.put(" * @return this");
        printer.put(" */");
        printer.put("%1$s& operator=(%1$s const& other) = delete;", getClassSimpleName(printer));
        printer.put();
    }

    private static void generateMoveDecl(CppPrinter printer) {
        printer.put("/**");
        printer.put(" * @brief constructs a new object.");
        printer.put(" * @param other the move source");
        printer.put(" */");
        printer.put("%1$s(%1$s&& other) noexcept = default;", getClassSimpleName(printer));
        printer.put();
        printer.put("/**");
        printer.put(" * @brief assigns the given object into this.");
        printer.put(" * @param other the move source");
        printer.put(" * @return this");
        printer.put(" */");
        printer.put("%1$s& operator=(%1$s&& other) noexcept = default;", getClassSimpleName(printer));
        printer.put();
    }

    private static void generateConfigDecl(CppPrinter printer) {
        printer.put("/**");
        printer.put(" * @brief returns whether or not show qualified kind for nested models.");
        printer.put(" * @return true if show qualified kind for nested models");
        printer.put(" * @return false otherwise");
        printer.put(" */");
        printer.put("virtual bool show_qualified_kind() const;", printer.getContextName(printer.getName()));
        printer.put();
        printer.put("/**");
        printer.put(" * @brief sets whether or not show qualified kind for nested models.");
        printer.put(" * @param on whether or not show qualified kind (default: false)");
        printer.put(" * @return this");
        printer.put(" */");
        printer.put("virtual %s& show_qualified_kind(bool on);", printer.getContextName(printer.getName()));
        printer.put();
        printer.put("/**");
        printer.put(" * @brief returns whether or not show kind for enumerations.");
        printer.put(" * @return true if show kind for enumerations");
        printer.put(" * @return false otherwise");
        printer.put(" */");
        printer.put("virtual bool show_enum_kind() const;", printer.getContextName(printer.getName()));
        printer.put();
        printer.put("/**");
        printer.put(" * @brief sets whether or not show kind for enumerations.");
        printer.put(" * @param on whether or not show enum kind (default: false)");
        printer.put(" * @return this");
        printer.put(" */");
        printer.put("virtual %s& show_enum_kind(bool on);", printer.getContextName(printer.getName()));
        printer.put();
        printer.put("/**");
        printer.put(" * @brief returns whether or not show kind for fragment models.");
        printer.put(" * @return true if show kind for fragment models");
        printer.put(" * @return false otherwise");
        printer.put(" */");
        printer.put("virtual bool show_fragment_kind() const;", printer.getContextName(printer.getName()));
        printer.put();
        printer.put("/**");
        printer.put(" * @brief sets whether or not show kind for fragment models.");
        printer.put(" * @param on whether or not show fragment kind (default: false)");
        printer.put(" * @return this");
        printer.put(" */");
        printer.put("virtual %s& show_fragment_kind(bool on);", printer.getContextName(printer.getName()));
        printer.put();
    }

    private void generateSerializeDecl(CppPrinter printer, TypeDeclaration decl) {
        printer.getIncludes().add(printerTypeName);
        printer.put("/**");
        printer.put(" * @brief serializes the value into given printer.");
        printer.put(" * @param printer the destination printer");
        printer.put(" * @param value the target value");
        printer.put(" */");
        printer.put("virtual void serialize(%s& printer, %s value);  // NOLINT",
                printer.getContextName(printerTypeName),
                getParameterType(printer, decl));
        printer.put();
    }

    private CppPrinter generateWalkerSource(Name typeName, Set<ClassDeclaration> baseClasses) {
        CppPrinter printer = CppPrinter.sourceFile(typeName);
        printer.getName().getParent().ifPresent(printer::namespace);

        generateImplDef(printer);
        generateCtorDef(printer);
        generateDtorDef(printer);
        generateConfigDef(printer);
        targets().forEach(decl -> {
            if (baseClasses.contains(decl)) {
                List<ClassDeclaration> children = repository.getSubtypes(decl.asTypeMirror());
                generteSerializeBaseClassDef(printer, (ClassDeclaration) decl, children);
            } else {
                switch (getKind(decl)) {
                case NODE:
                    generteSerializeNodeDef(printer, (ClassDeclaration) decl);
                    break;
                case FRAGMENT:
                    generteSerializeFragmentDef(printer, (ClassDeclaration) decl);
                    break;
                case EXTERN:
                case INTERFACE:
                    generteSerializeExternDef(printer, (ClassDeclaration) decl);
                    break;
                case ENUMERATION:
                    generateSerializeEnumerationDef(printer, (EnumTypeDeclaration) decl);
                    break;
                default:
                    throw new AssertionError(decl);
                }
            }
        });
        return printer;
    }

    private void generateImplDef(CppPrinter printer) {
        printer.put("class %s::Impl {", getClassSimpleName(printer));
        printer.put("private:");
        printer.indent(() -> {
            printer.getIncludes().add(IncludeList.Standard.SSTREAM);
            printer.put("std::ostringstream str_buffer_;");
            printer.put();
        });
        printer.put("public:");
        printer.indent(() -> {
            printer.put("bool qualified_kind_ { false };");
            printer.put("bool enum_kind_ { false };");
            printer.put("bool fragment_kind_ { false };");
            printer.put();
            printer.put("template<typename T>");
            printer.put("inline std::string to_string(const T& value) {");
            printer.indent(() -> {
                printer.put("str_buffer_.str(\"\");");
                printer.put("str_buffer_.clear(std::stringstream::goodbit);");
                printer.getIncludes().add(utilityFileName);
                printer.put("%s(str_buffer_, value);",
                        printer.getContextName(utilityFileName.resolveSibling("append")));
                printer.put("return str_buffer_.str();");

            });
            printer.put("}");
        });
        printer.put("};");
        printer.put();
    }

    private static void generateCtorDef(CppPrinter printer) {
        printer.put("%s::%s() : impl_(new Impl()) {}",
                printer.getContextName(printer.getName()),
                getClassSimpleName(printer));
        printer.put();
    }

    private static void generateDtorDef(CppPrinter printer) {
        printer.put("%s::~%s() noexcept = default;",
                printer.getContextName(printer.getName()),
                getClassSimpleName(printer));
        printer.put();
    }

    private static void generateConfigDef(CppPrinter printer) {
        generateConfigDef(printer, "qualified_kind");
        generateConfigDef(printer, "enum_kind");
        generateConfigDef(printer, "fragment_kind");
    }

    private static void generateConfigDef(CppPrinter printer, String configName) {
        printer.put("bool %s::show_%s() const {",
                printer.getContextName(printer.getName()),
                configName);
        printer.indent(() -> {
            printer.put("return impl_->%s_;", configName);
        });
        printer.put("}");
        printer.put();
        printer.put("%s& %s::show_%s(bool on) {",
                printer.getContextName(printer.getName()),
                printer.getContextName(printer.getName()),
                configName);
        printer.indent(() -> {
            printer.put("impl_->%s_ = on;", configName);
            printer.put("return *this;");
        });
        printer.put("}");
        printer.put();
    }

    private void generteSerializeBaseClassDef(
            CppPrinter printer, ClassDeclaration decl, List<ClassDeclaration> children) {
        printSerializeDef(printer, decl, true, () -> {
            putNullGuard(printer);
            printer.put("switch (value->kind()) {");
            printer.getIncludes().add(KindGenerator.getTypeName(decl));
            for (ClassDeclaration child : children) {
                printer.getIncludes().add(child.getName());
                printer.put("case %s::%s:",
                        printer.getContextName(KindGenerator.getTypeName(decl)),
                        KindGenerator.getConstantName(child));
                printer.indent(() -> {
                    printer.put("serialize(printer, dynamic_cast<%s const*>(value));",
                            printer.getContextName(child));
                    printer.put("return;");
                });
            }
            printer.put("}");
            printer.getIncludes().add(IncludeList.Standard.STDEXCEPT);
            printer.getIncludes().add(IncludeList.Standard.SSTREAM);
            printer.put("std::ostringstream ss;");
            printer.put("ss << \"unknown node kind: \" << value->kind();");
            printer.put("throw std::invalid_argument(ss.str());");
        });
    }

    private void generteSerializeNodeDef(CppPrinter printer, ClassDeclaration decl) {
        boolean present = decl.members(PropertyDeclaration.class).findAny().isPresent();
        printSerializeDef(printer, decl, present, () -> {
            putNullGuard(printer);
            putObject(printer, decl, () -> {
                generateNodeLikeBody(printer, decl);
            });
        });

    }

    private void generteSerializeFragmentDef(CppPrinter printer, ClassDeclaration decl) {
        boolean present = decl.members(PropertyDeclaration.class).findAny().isPresent();
        printSerializeDef(printer, decl, present, () -> {
            putObject(printer, decl, () -> {
                generateNodeLikeBody(printer, decl);
            });
        });
    }

    private void generateNodeLikeBody(CppPrinter printer, ClassDeclaration decl) {
        decl.members(PropertyDeclaration.class).forEach(it -> generateProperty(printer, it));
    }

    private static void generateProperty(CppPrinter printer, PropertyDeclaration decl) {
        printer.put("{");
        printer.indent(() -> {
            putProperty(printer, Util.getSimpleName(decl), () -> {
                switch (getKind(decl)) {
                case VALUE:
                    printer.put("printer.value(value->%s());", Util.getSimpleName(decl));
                    break;
                case ELEMENT:
                    printer.put("serialize(printer, value->%s());", Util.getSimpleName(decl));
                    break;
                case ELEMENT_LIST:
                    printer.put("auto& list = value->%s();", Util.getSimpleName(decl));
                    printer.put("auto size = list.size();", Util.getSimpleName(decl));
                    putArray(printer, "size", () -> {
                        printer.put("for (auto element : list) {");
                        printer.indent(() -> {
                            printer.put("serialize(printer, element);");
                        });
                        printer.put("}");
                    });
                    break;
                default:
                    throw new AssertionError(decl);
                }
            });
        });
        printer.put("}");
    }

    private void generteSerializeExternDef(CppPrinter printer, ClassDeclaration decl) {
        boolean present = decl.members(PropertyDeclaration.class).findAny().isPresent();
        printSerializeDef(printer, decl, present, () -> {
            printer.put("// external object");
            putNullGuard(printer);
            putObject(printer, decl, () -> {
                return;
            });
        });
    }

    private static void putNullGuard(CppPrinter printer) {
        printer.put("if (value == nullptr) {");
        printer.indent(() -> {
            printer.put("printer.value(nullptr);");
            printer.put("return;");
        });
        printer.put("}");
    }

    private void generateSerializeEnumerationDef(CppPrinter printer, EnumTypeDeclaration decl) {
        printSerializeDef(printer, decl, true, () -> {
            printer.put("if (show_enum_kind()) {");
            printer.indent(() -> {
                putObject(printer, decl, () -> {
                    putProperty(printer, "value", () -> {
                        putEnumValue(printer);
                    });
                });
            });
            printer.put("} else {");
            printer.indent(() -> {
                putEnumValue(printer);
            });
            printer.put("}");
        });
    }

    private void putObject(CppPrinter printer, TypeDeclaration decl, Runnable body) {
        putObjectBound(printer, decl, "enter");
        body.run();
        putObjectBound(printer, decl, "exit");
    }

    private void putObjectBound(CppPrinter printer, TypeDeclaration decl, String prefix) {
        Name name = getTopRelatedName(decl);
        if (name instanceof SimpleName) {
            printer.put("printer.%s_object(%s);", prefix, toLiteral(name.join("::")));
        } else {
            if (decl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                printer.put("if (!show_fragment_kind()) {");
                printer.indent(() -> {
                    printer.put("printer.%s_object({});", prefix);
                });
                printer.put("} else if ");
            } else {
                printer.put("if ");
            }
            printer.append("(show_qualified_kind()) {");
            printer.indent(() -> {
                printer.put("printer.%s_object(%s);", prefix, toLiteral(name.getLast().getToken()));
            });
            printer.put("} else {");
            printer.indent(() -> {
                printer.put("printer.%s_object(%s);", prefix, toLiteral(name.join("::")));
            });
            printer.put("}");
        }
    }

    private static void putArray(CppPrinter printer, String sizeExpr, Runnable body) {
        printer.put("printer.enter_array(%s);", sizeExpr);
        body.run();
        printer.put("printer.exit_array(%s);", sizeExpr);
    }

    private static void putProperty(CppPrinter printer, String string, Runnable body) {
        String literal = toLiteral(string);
        printer.put("printer.enter_property(%s);", literal);
        body.run();
        printer.put("printer.exit_property(%s);", literal);
    }

    private static void putEnumValue(CppPrinter printer) {
        printer.put("printer.value(impl_->to_string(value));");
    }

    private static String toLiteral(String string) {
        StringBuilder buf = new StringBuilder();
        buf.append("\"");
        string.codePoints().forEach(it -> {
            switch (it) {
            case '"':
            case '\\':
                buf.append('\\');
                buf.appendCodePoint(it);
                break;

            case '\t':
                buf.append('\\');
                buf.append('t');
                break;

            case '\r':
                buf.append('\\');
                buf.append('r');
                break;

            case '\n':
                buf.append('\\');
                buf.append('n');
                break;

            default:
                buf.appendCodePoint(it);
                break;
            }
        });
        buf.append("\"");
        return buf.toString();
    }

    private void printSerializeDef(CppPrinter printer, TypeDeclaration decl, boolean useValue, Runnable body) {
        printer.getIncludes().add(printerTypeName);
        printer.put("void %s::serialize(%s& printer, %s%s value) {",
                printer.getContextName(printer.getName()),
                printer.getContextName(printerTypeName),
                useValue ? "" : "[[maybe_unused]] ",
                getParameterType(printer, decl));
        printer.indent(body);
        printer.put("}");
        printer.put();
    }

    private Name getTopRelatedName(TypeDeclaration decl) {
        LinkedList<String> tokens = new LinkedList<>();
        TypeDeclaration current = decl;
        while (current != null) {
            tokens.addFirst(current.getName().getLast().getToken());
            current = current.getName().getParent()
                    .flatMap(repository::find)
                    .orElse(null);
        }

        return Name.of(tokens);
    }

    private static String getClassSimpleName(CppPrinter printer) {
        return printer.getName().getLast().getToken();
    }

    private String getParameterType(CppPrinter printer, TypeDeclaration decl) {
        includeTop(printer, decl.asTypeMirror());
        switch (getKind(decl)) {
        case EXTERN:
        case INTERFACE:
        case NODE:
        case FRAGMENT:
            return String.format("%s const*", printer.getContextName(decl));
        case ENUMERATION:
            return printer.getContextName(decl);
        default:
            throw new AssertionError(decl);
        }
    }

    private static TypeKind getKind(TypeDeclaration decl) {
        if (decl instanceof ClassDeclaration) {
            var attrs = decl.getAttributes();
            if (attrs.contains(ClassDeclaration.Attribute.INTERFACE)) {
                return TypeKind.INTERFACE;
            }
            if (attrs.contains(ClassDeclaration.Attribute.EXTERN)) {
                return TypeKind.EXTERN;
            }
            if (attrs.contains(ClassDeclaration.Attribute.FRAGMENT)) {
                return TypeKind.FRAGMENT;
            }
            return TypeKind.NODE;
        } else {
            return TypeKind.ENUMERATION;
        }
    }

    private enum TypeKind {
        INTERFACE,
        NODE,
        FRAGMENT,
        EXTERN,
        ENUMERATION,
    }

    private enum PropertyKind {
        ELEMENT,
        ELEMENT_LIST,
        VALUE,
    }

    private static PropertyKind getKind(PropertyDeclaration decl) {
        return getPropertyKind(decl.getType());
    }

    private static PropertyKind getPropertyKind(TypeMirror type) {
        switch (type.getKind()) {
        case DECLARED_TYPE:
            return PropertyKind.ELEMENT;
        case SIZE_TYPE:
        case STRING_TYPE:
            return PropertyKind.VALUE;
        case LIST_TYPE:
            switch (getPropertyKind(((ListTypeMirror) type).getComponentType())) {
            case ELEMENT:
                return PropertyKind.ELEMENT_LIST;
            default:
                throw new AssertionError(type);
            }
        default:
            throw new AssertionError(type);
        }
    }

    private Stream<TypeDeclaration> targets() {
        return repository.all(TypeDeclaration.class)
                .filter(it -> it instanceof EnumTypeDeclaration
                        || !it.getAttributes().contains(ClassDeclaration.Attribute.FOREIGN));
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
}
