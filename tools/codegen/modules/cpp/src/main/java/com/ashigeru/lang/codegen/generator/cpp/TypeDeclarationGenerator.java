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
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import com.ashigeru.lang.codegen.generator.cpp.IncludeList.Standard;
import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.Declaration;
import com.ashigeru.lang.codegen.ir.model.DeclaredTypeMirror;
import com.ashigeru.lang.codegen.ir.model.EnumConstantDeclaration;
import com.ashigeru.lang.codegen.ir.model.EnumTypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.ListTypeMirror;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.PropertyDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.model.TypeMirror;
import com.ashigeru.lang.codegen.ir.model.TypeMirror.Kind;

/**
 * Generates C++ type declarations.
 * @since WIP
 */
public class TypeDeclarationGenerator {

    static final String IMPL_FIELD = "impl_";

    static final String REMOVER_PREFIX = "release_";

    static final String CLONE_METHOD_NAME = "clone";

    static final String MAKE_CLONE_METHOD_NAME = "make_clone";

    private final Configuration conf;

    private final TypeDeclarationRepository repository;

    private final List<Callback> scopedCallbacks = new ArrayList<>();

    private final List<Callback> unscopedCallbacks = new ArrayList<>();

    /**
     * Creates a new instance.
     * @param conf the generator configuration
     * @param repository the current repository
     */
    public TypeDeclarationGenerator(
            Configuration conf,
            TypeDeclarationRepository repository) {
        this.conf = conf;
        this.repository = repository;
    }

    /**
     * Adds a callback.
     * @param callback the callback object
     */
    public void addCallback(Callback callback) {
        this.scopedCallbacks.add(callback);
    }

    /**
     * Adds a callback.
     * @param callback the callback object
     */
    public void addUnscopedCallback(Callback callback) {
        this.unscopedCallbacks.add(callback);
    }

    /**
     * Generates an header file.
     * @param filer the target filer
     * @param type the target type
     * @throws IOException if I/O error was occurred
     */
    public void generateHeaderFile(CppFiler filer, TypeDeclaration type) throws IOException {
        HeaderGenerator engine = new HeaderGenerator(conf, repository, type);
        engine.setCallbacks(scopedCallbacks);
        engine.process();
        engine.printer.printTo(filer);
    }

    /**
     * Generates an source file.
     * @param filer the target filer
     * @param type the target type
     * @throws IOException if I/O error was occurred
     */
    public void generateSourceFile(CppFiler filer, ClassDeclaration type) throws IOException {
        SourceGenerator engine = new SourceGenerator(conf, repository, type);
        engine.setCallbacks(scopedCallbacks);
        engine.process();
        engine.printer.printTo(filer);
    }

    enum TypeKind {
        PRIMITIVE,
        ENUM, // enum constants
        DATA, // non-inheritable, copyable
        VALUE_FLAT, // non-inheritable
        VALUE_POLY, // inheritable
        NODE,
        MANAGED,
        FRAGMENT,
        BASIC_LIST,
        ENUM_LIST,
        VALUE_LIST,
        NODE_LIST,
        MANAGED_NODE_LIST,
        FRAGMENT_LIST,
    }

    enum TypeStyle {
        BARE,
        UNIQUE_POINTER,
        MANAGED_POINTER,
        PRIMITIVE_LIST,
        ENUM_LIST,
        DATA_LIST,
        VALUE_LIST,
        NODE_LIST,
        MANAGED_NODE_LIST,
        FRAGMENT_LIST,
    }

    enum TypeModifier {
        BARE,
        CONST,
        POINTER,
        CONST_POINTER,
        REF,
        CONST_REF,
        RVALUE_REF,
    }

    private static class BaseGenerator {

        final Configuration conf;

        final TypeDeclarationRepository repository;

        final CppPrinter printer;

        final Name makeClone;

        final List<Callback> callbacks = new ArrayList<>();

        BaseGenerator(
                Configuration conf,
                TypeDeclarationRepository repository, TypeDeclaration topLevel,
                CppPrinter.FileKind fileKind) {
            this.conf = conf;
            this.repository = repository;
            this.printer = new CppPrinter(topLevel.getName(), fileKind);
            this.makeClone = conf.utilityFileName.resolveSibling(MAKE_CLONE_METHOD_NAME);
        }

        public void setCallbacks(List<Callback> callbacks) {
            this.callbacks.clear();
            this.callbacks.addAll(callbacks);
        }

        void processCallbacks(ClassDeclaration declaration) {
            for (var callback : callbacks) {
                callback.generate(printer, declaration);
            }
        }

        TypeKind getTypeKind(TypeMirror type) {
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
                    } else if (typeDecl.getAttributes().contains(ClassDeclaration.Attribute.MANAGED)) {
                        return TypeKind.MANAGED;
                    } else {
                        if (repository.getAncestors(type).stream()
                                .filter(it -> it != typeDecl)
                                .anyMatch(it -> it.getAttributes().contains(ClassDeclaration.Attribute.MANAGED))) {
                            throw new IllegalStateException();
                        }
                        return TypeKind.NODE;
                    }
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
                case MANAGED:
                    return TypeKind.MANAGED_NODE_LIST;
                default:
                    throw new IllegalStateException(type.toString());
                }
            }
            default:
                throw new AssertionError(type);
            }
        }

        String toNameString(TypeMirror type, TypeStyle style) {
            IncludeList includes = printer.getIncludes();
            if (type instanceof ListTypeMirror) {
                Name name = conf.listTypeNameOf(includes, style);
                return String.format("%s<%s>",
                        printer.getContextName(name),
                        toNameString(((ListTypeMirror) type).getComponentType(), TypeStyle.BARE));
            } else {
                switch (style) {
                case BARE:
                    return printer.getContextName(toName(type));
                case UNIQUE_POINTER:
                    printer.getIncludes().add(IncludeList.Standard.MEMORY);
                    return String.format("std::unique_ptr<%s>", printer.getContextName(toName(type)));
                case MANAGED_POINTER:
                    return String.format("%s<%s>",
                            printer.getContextName(conf.managedPtrTypeName),
                            printer.getContextName(toName(type)));
                default:
                    throw new AssertionError(style);
                }
            }
        }

        Name toName(TypeMirror type) {
            IncludeList includes = printer.getIncludes();
            switch (type.getKind()) {
            case BOOL_TYPE:
                return IncludeList.BOOL_TYPE;
            case INT_TYPE:
                includes.add(IncludeList.Standard.STDINT);
                return IncludeList.INT_TYPE;
            case FLOAT_TYPE:
                return IncludeList.FLOAT_TYPE;
            case SIZE_TYPE:
                includes.add(IncludeList.Standard.STDDEF);
                return IncludeList.SIZE_TYPE;
            case STRING_TYPE:
                includes.add(IncludeList.Standard.STRING);
                return IncludeList.STRING_TYPE;
            case DECLARED_TYPE: {
                Name name = ((DeclaredTypeMirror) type).getName();
                for (Name current = name; current != null; current = current.getParent().orElse(null)) {
                    Optional<Name> top = repository.getElements().stream()
                            .map(Declaration::getName)
                            .filter(Predicate.isEqual(current))
                            .findFirst();
                    if (top.isPresent()) {
                        includes.add(top.get());
                        break;
                    }
                }
                return name;
            }
            default:
                throw new IllegalArgumentException(type.toString());
            }
        }

        static String getSimpleImplName(ClassDeclaration decl) {
            return "Impl";
        }

        String getContextImplName(ClassDeclaration decl) {
            return printer.getContextName(decl.getName().resolve(getSimpleImplName(decl)));
        }

        String getType(
                TypeMirror type,
                TypeStyle style,
                TypeModifier modifier) {
            StringBuilder buf = new StringBuilder();
            buf.append(toNameString(type, style));
            if (modifier == TypeModifier.CONST
                    || modifier == TypeModifier.CONST_POINTER
                    || modifier == TypeModifier.CONST_REF) {
                buf.append(" const");
            }
            if (modifier == TypeModifier.POINTER
                    || modifier == TypeModifier.CONST_POINTER) {
                buf.append("*");
            } else if (modifier == TypeModifier.REF
                        || modifier == TypeModifier.CONST_REF) {
                buf.append("&");
            } else if (modifier == TypeModifier.RVALUE_REF) {
                buf.append("&&");
            }
            return buf.toString();
        }

        String getGetter(
                PropertyDeclaration decl,
                TypeStyle style,
                TypeModifier modifier,
                boolean fqn,
                boolean isConst) {
            StringBuilder buf = new StringBuilder();
            buf.append(getType(decl.getType(), style, modifier));
            buf.append(" ");
            buf.append(fqn ? printer.getContextName(decl) : Util.getSimpleName(decl));
            buf.append("()");
            if (isConst) {
                buf.append(" const");
            }
            if (!fqn && isOverride(decl)) {
                buf.append(" override");
            }
            return buf.toString();
        }

        String getSetter(
                PropertyDeclaration decl,
                TypeStyle style,
                TypeModifier modifier,
                boolean fqn) {
            StringBuilder buf = new StringBuilder();
            buf.append(printer.getContextName(repository.find(decl.getName().getQualifier()).get()));
            buf.append("& ");
            buf.append(fqn ? printer.getContextName(decl) : Util.getSimpleName(decl));
            buf.append("(");
            buf.append(getType(decl.getType(), style, modifier));
            buf.append(" ");
            buf.append(Util.getSimpleName(decl));
            buf.append(")");
            if (!fqn && isOverride(decl)) {
                buf.append(" override");
            }
            return buf.toString();
        }

        String getRemover(
                PropertyDeclaration decl,
                boolean fqn) {
            StringBuilder buf = new StringBuilder();
            buf.append(getType(decl.getType(), TypeStyle.UNIQUE_POINTER, TypeModifier.BARE));
            buf.append(" ");
            if (fqn) {
                buf.append(printer.getContextName(decl.getName().getQualifier()));
                buf.append("::");
            }
            buf.append(REMOVER_PREFIX + Util.getSimpleName(decl));
            buf.append("()");
            if (!fqn && isOverride(decl)) {
                buf.append(" override");
            }
            return buf.toString();
        }

        private boolean isOverride(PropertyDeclaration decl) {
            ClassDeclaration owner = repository
                    .find(ClassDeclaration.class, decl.getName().getQualifier())
                    .get();
            return repository.getAncestors(owner.asTypeMirror()).stream()
                    .filter(it -> it != owner)
                    .flatMap(it -> it.members(PropertyDeclaration.class))
                    .anyMatch(it -> decl.getName().getLast().equals(it.getName().getLast()));
        }

        boolean isConstProperty(PropertyDeclaration decl) {
            return decl.getAttributes().contains(PropertyDeclaration.Attribute.CONST);
        }

        boolean isConst(ClassDeclaration decl) {
            return decl.getAttributes().contains(ClassDeclaration.Attribute.CONST)
                    || decl.getAttributes().contains(ClassDeclaration.Attribute.VALUE);
        }

        boolean isRequireImpl(ClassDeclaration decl) {
            return !decl.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE)
                    && decl.members(PropertyDeclaration.class).findAny().isPresent();
        }

        boolean isRequireEmptyCtor(ClassDeclaration decl) {
            return !decl.getAttributes().contains(ClassDeclaration.Attribute.VALUE)
                    || !isRequireDataCtor(decl);
        }

        boolean isRequireDataCtor(ClassDeclaration decl) {
            return decl.getAttributes().contains(ClassDeclaration.Attribute.VALUE)
                    && decl.members(PropertyDeclaration.class)
                        .filter(this::isDataCtorParameter)
                        .findAny().isPresent();
        }

        boolean isMovable(ClassDeclaration decl) {
            return decl.getAttributes().contains(ClassDeclaration.Attribute.MOVABLE);
        }

        boolean isCopyable(ClassDeclaration decl) {
            return decl.getAttributes().contains(ClassDeclaration.Attribute.COPYABLE);
        }

        boolean isClonable(ClassDeclaration decl) {
            return decl.getAttributes().contains(ClassDeclaration.Attribute.CLONABLE);
        }

        boolean isCopiable(PropertyDeclaration decl) {
            if (isConstProperty(decl)) {
                return false;
            }
            if (decl.getType().getKind() != TypeMirror.Kind.DECLARED_TYPE) {
                return true;
            }
            TypeDeclaration type = repository.find(decl.getType()).get();
            if (type instanceof EnumTypeDeclaration) {
                return true;
            }
            return isCopyable((ClassDeclaration) type);
        }

        boolean isMovable(PropertyDeclaration decl) {
            if (isConstProperty(decl)) {
                return false;
            }
            if (decl.getType().getKind() == Kind.STRING_TYPE) {
                return true;
            }
            if (decl.getType().getKind() != TypeMirror.Kind.DECLARED_TYPE) {
                return false;
            }
            TypeDeclaration type = repository.find(decl.getType()).get();
            if (type instanceof EnumTypeDeclaration) {
                return false;
            }
            return isMovable((ClassDeclaration) type);
        }

        void declareDataCtor(ClassDeclaration decl, boolean fqn, boolean prototype) {
            List<PropertyDeclaration> props = decl.members(PropertyDeclaration.class)
                    .filter(this::isDataCtorParameter)
                    .collect(Collectors.toList());
            assert !props.isEmpty();
            if (prototype) {
                printer.put("/**");
                printer.put(" * @brief Constructs a new object from its members.");
                for (PropertyDeclaration prop : props) {
                    printer.put(" * @param %s %s", Util.getSimpleName(prop), getDescription(prop));
                }
                printer.put(" */");
            }
            String name = Util.getSimpleName(decl);
            if (fqn) {
                name = printer.getContextName(decl.getName(), name);
            }
            printer.put("%s(", name);
            printer.indent(() -> {
                for (int i = 0, n = props.size(); i < n; i++) {
                    printer.put("%s%s",
                            getCtorParameterDecl(props.get(i)),
                            i < n - 1 ? "," : (prototype ? ");" : ")"));
                }
            });
        }

        boolean isDataCtorParameter(PropertyDeclaration prop) {
            return !prop.getAttributes().contains(PropertyDeclaration.Attribute.TRANSIENT);
        }

        private String getDescription(PropertyDeclaration property) {
            if (property.getDescription().isPresent()) {
                return property.getDescription().get();
            }
            TypeMirror type = property.getType();
            switch (type.getKind()) {
            case BOOL_TYPE:
            case INT_TYPE:
            case FLOAT_TYPE:
            case SIZE_TYPE:
            case STRING_TYPE:
            case LIST_TYPE:
                return Util.getSimpleName(property);
            case DECLARED_TYPE:
                return repository.find(type)
                        .map(it -> it.getDescription()
                                .orElseGet(() -> it.getName().getLast().getToken()))
                        .orElseGet(() -> Util.getSimpleName(property));
            default:
                throw new AssertionError(type);
            }
        }

        private String getCtorParameterDecl(PropertyDeclaration decl) {
            TypeMirror type = decl.getType();
            switch (getTypeKind(type)) {
            case PRIMITIVE:
            case ENUM:
                return String.format("%s %s",
                        toNameString(type, TypeStyle.BARE),
                        Util.getSimpleName(decl));

            case DATA:
            case VALUE_FLAT:
            case VALUE_POLY:
            case NODE:
            case MANAGED:
                return String.format("%s const& %s",
                        toNameString(type, TypeStyle.BARE),
                        Util.getSimpleName(decl));

            case BASIC_LIST:
                return String.format("%s const& %s",
                        toNameString(type, TypeStyle.PRIMITIVE_LIST),
                        Util.getSimpleName(decl));

            case ENUM_LIST:
                return String.format("%s const& %s",
                        toNameString(type, TypeStyle.ENUM_LIST),
                        Util.getSimpleName(decl));

            case VALUE_LIST:
                return String.format("%s const& %s",
                        toNameString(type, TypeStyle.VALUE_LIST),
                        Util.getSimpleName(decl));

            case NODE_LIST:
                return String.format("%s const& %s",
                        toNameString(type, TypeStyle.NODE_LIST),
                        Util.getSimpleName(decl));

            case FRAGMENT_LIST:
                return String.format("%s const& %s",
                        toNameString(type, TypeStyle.FRAGMENT_LIST),
                        Util.getSimpleName(decl));

            default:
                throw new AssertionError(type);
            }
        }
    }

    private static class HeaderGenerator extends BaseGenerator {

        private final TypeDeclaration topLevel;

        HeaderGenerator(Configuration conf, TypeDeclarationRepository repository, TypeDeclaration topLevel) {
            super(conf, repository, topLevel, CppPrinter.FileKind.INCLUDE_FILE);
            this.topLevel = topLevel;
        }

        void process() {
            topLevel.getName().getParent().ifPresent(printer::namespace);
            processTypeDecl(topLevel);
            processEnumPrinting(topLevel);
        }

        private void processTypeDecl(TypeDeclaration decl) {
            if (decl instanceof ClassDeclaration) {
                processClassDecl((ClassDeclaration) decl);
            } else if (decl instanceof EnumTypeDeclaration) {
                processEnumTypeDecl((EnumTypeDeclaration) decl);
            }
        }

        private void processClassDecl(ClassDeclaration decl) {
            processClassDoc(decl);
            printer.put("class %s", Util.getSimpleName(decl));
            if (decl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                printer.append(" final");
            }
            if (!decl.getSuperTypes().isEmpty()) {
                printer.indent(2, () -> {
                    boolean first = true;
                    for (TypeMirror t : decl.getSuperTypes()) {
                        if (first) {
                            printer.put(": ");
                            first = false;
                        } else {
                            printer.put(", ");
                        }
                        printer.append("public %s", toNameString(t, TypeStyle.BARE));
                    }
                });
            }
            printer.append(" {");

            // declare Pimpl
            if (isRequireImpl(decl)) {
                printer.getIncludes().add(IncludeList.Standard.MEMORY);
                printer.getIncludes().add(IncludeList.Standard.UTILITY);
                printer.put("private:");
                printer.indent(() -> {
                    printer.put("class %s;", getSimpleImplName(decl));
                    printer.put("std::unique_ptr<%s> %s;", getSimpleImplName(decl), IMPL_FIELD);
                    printer.put("explicit %s(std::unique_ptr<%s>) noexcept;",
                            Util.getSimpleName(decl),
                            getSimpleImplName(decl));
                });
                printer.put();
            }

            // declare member types
            if (decl.members(TypeDeclaration.class).findAny().isPresent()) {
                printer.put("public:");
                printer.indent(() -> {
                    // process enum type first for resolving references
                    decl.members(EnumTypeDeclaration.class).forEach(this::processTypeDecl);
                    decl.members(ClassDeclaration.class).forEach(this::processTypeDecl);
                });
            }

            // declare basics
            printer.put("public:");
            printer.indent(() -> {
                if (isRequireEmptyCtor(decl)) {
                    printer.put("/**");
                    printer.put(" * @brief Constructs a new empty object.");
                    printer.put(" */");
                    printer.put("%s()", Util.getSimpleName(decl));
                    if (!isRequireImpl(decl)) {
                        printer.append(" noexcept");
                    }
                    printer.append(";");
                    printer.put();
                }
                if (isRequireDataCtor(decl)) {
                    declareDataCtor(decl, false, true);
                }
                printer.put("/**");
                printer.put(" * @brief Destroys this object.");
                printer.put(" */");
                boolean override = !decl.getSuperTypes().isEmpty();
                printer.put();
                if(!override && !decl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                    printer.append("virtual ");
                }
                printer.append("~%s() noexcept", Util.getSimpleName(decl));
                if (override) {
                    printer.append(" override");
                }
                printer.append(";");
                printer.put();
                
                printer.put("/**");
                printer.put(" * @brief Copy-constructs a new object.");
                printer.put(" * @param other the source object");
                printer.put(" */");
                printer.put("%1$s(%1$s const& other)%2$s;",
                        Util.getSimpleName(decl),
                        isCopyable(decl) ? "" : " = delete");
                printer.put();
                printer.put("/**");
                printer.put(" * @brief Copy-assigns to this object.");
                printer.put(" * @param other the source object");
                printer.put(" * @return this");
                printer.put(" */");
                printer.put("%1$s& operator=(%1$s const& other)%2$s;",
                        Util.getSimpleName(decl),
                        isCopyable(decl) && !isConst(decl) ? "" : " = delete");
                printer.put();
                printer.put("/**");
                printer.put(" * @brief Move-constructs a new object.");
                printer.put(" * @param other the source object");
                printer.put(" */");
                printer.put("%1$s(%1$s&& other)%2$s;",
                        Util.getSimpleName(decl),
                        isMovable(decl) ? " noexcept" : " = delete");
                printer.put();
                printer.put("/**");
                printer.put(" * @brief Move-assigns to this object.");
                printer.put(" * @param other the source object");
                printer.put(" * @return this");
                printer.put(" */");
                printer.put("%1$s& operator=(%1$s&& other)%2$s;",
                        Util.getSimpleName(decl),
                        isMovable(decl) && !isConst(decl) ? " noexcept" : " = delete");
                printer.put();
            });

            boolean putPublic = false;
            // declare properties
            if (decl.members(PropertyDeclaration.class).findAny().isPresent()) {
                if (!putPublic) {
                    printer.put("public:");
                    putPublic = true;
                }
                printer.indent(() -> {
                    decl.members(PropertyDeclaration.class).forEach(this::processPropertyDecl);
                });
            }

            // declare special members
            if (isClonable(decl)) {
                if (!putPublic) {
                    printer.put("public:");
                    putPublic = true;
                }
                printer.indent(() -> {
                    processCloneMethod(decl);
                });
            }

            // callbacks
            processCallbacks(decl);

            printer.put("};");
        }

        private void processCloneMethod(ClassDeclaration decl) {
            boolean intf = decl.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE);
            boolean override = !decl.getSuperTypes().isEmpty();
            printer.put("/**");
            printer.put(" * @brief Returns a copy of this object.");
            printer.put(" * @return a clone of this");
            printer.put(" */");
            printer.put();
            if(!override && !decl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                printer.append("virtual ");
            }
            printer.append("%s* %s() const &",
                    Util.getSimpleName(decl),
                    CLONE_METHOD_NAME);
            if (override) {
                printer.append(" override");
            }
            if (intf) {
                printer.append(" = 0;");
            } else {
                printer.append(";");
            }
            printer.put();
            printer.put("/**");
            printer.put(" * @brief Returns a copy of this object.");
            printer.put(" * @return a clone of this");
            printer.put(" */");
            printer.put();
            if(!override && !decl.getAttributes().contains(ClassDeclaration.Attribute.FRAGMENT)) {
                printer.append("virtual ");
            }
            printer.append("%s* %s() &&",
                    Util.getSimpleName(decl),
                    CLONE_METHOD_NAME);
            if (override) {
                printer.append(" override");
            }
            if (intf) {
                printer.append(" = 0;");
            } else {
                printer.append(";");
            }
            printer.put();
        }

        private void processPropertyDecl(PropertyDeclaration decl) {
            TypeMirror type = decl.getType();
            switch (getTypeKind(type)) {
            case PRIMITIVE:
            case ENUM:
                processGetterDoc(decl);
                printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.BARE, false, true));
                printer.put();
                if (isCopiable(decl)) {
                    processSetterDoc(decl);
                    printer.put("%s;", getSetter(decl, TypeStyle.BARE, TypeModifier.BARE, false));
                    printer.put();
                }
                break;
            case DATA:
                processGetterDoc(decl);
                printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_REF, false, true));
                if (!isConstProperty(decl)) {
                    processSetterDoc(decl);
                    printer.put("%s;", getSetter(decl, TypeStyle.BARE, TypeModifier.BARE, false));
                    printer.put();
                }
                break;
            case VALUE_FLAT:
                if (isConstProperty(decl)) {
                    processGetterDoc(decl);
                    printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_REF, false, true));
                    printer.put();
                } else {
                    processGetterDoc(decl);
                    printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.REF, false, false));
                    printer.put();

                    processGetterDoc(decl);
                    printer.put("inline %s", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_REF, false, true));
                    processDeConstGetter(decl);
                    printer.put();
                }
                if (isCopiable(decl)) {
                    processSetterDoc(decl);
                    printer.put("%s;", getSetter(decl, TypeStyle.BARE, TypeModifier.CONST_REF, false));
                    printer.put();
                }
                if (isMovable(decl)) {
                    processSetterDoc(decl);
                    printer.put("%s;", getSetter(decl, TypeStyle.BARE, TypeModifier.RVALUE_REF, false));
                    printer.put();
                }
                break;
            case VALUE_POLY:
                if (isConstProperty(decl)) {
                    processGetterDoc(decl);
                    printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_POINTER, false, true));
                    printer.put();
                } else {
                    processGetterDoc(decl);
                    printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.POINTER, false, false));
                    printer.put();

                    processGetterDoc(decl);
                    printer.put("inline %s", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_POINTER, false, true));
                    processDeConstGetter(decl);
                    printer.put();
                }
                if (!isConstProperty(decl)) {
                    processSetterDoc(decl);
                    printer.put("%s;", getSetter(decl, TypeStyle.UNIQUE_POINTER, TypeModifier.BARE, false));
                    printer.put();
                }
                break;
            case NODE:
            case MANAGED:
                if (isConstProperty(decl)) {
                    processGetterDoc(decl);
                    printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_POINTER, false, true));
                } else {
                    processGetterDoc(decl);
                    printer.put("%s;", getGetter(decl, TypeStyle.BARE, TypeModifier.POINTER, false, false));
                    printer.put();

                    processGetterDoc(decl);
                    printer.put("inline %s", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_POINTER, false, true));
                    processDeConstGetter(decl);
                    printer.put();
                }
                if (!isConstProperty(decl)) {
                    processSetterDoc(decl);
                    printer.put("%s;", getSetter(decl, TypeStyle.UNIQUE_POINTER, TypeModifier.BARE, false));
                    printer.put();
                    processRemoverDoc(decl);
                    printer.put("%s;", getRemover(decl, false));
                    printer.put();
                }
                break;
            case BASIC_LIST:
                processListPropertyDecl(decl, TypeStyle.PRIMITIVE_LIST);
                break;
            case ENUM_LIST:
                processListPropertyDecl(decl, TypeStyle.ENUM_LIST);
                break;
            case VALUE_LIST:
                processListPropertyDecl(decl, TypeStyle.VALUE_LIST);
                break;
            case NODE_LIST:
                processListPropertyDecl(decl, TypeStyle.NODE_LIST);
                break;
            case MANAGED_NODE_LIST:
                processListPropertyDecl(decl, TypeStyle.MANAGED_NODE_LIST);
                break;
            case FRAGMENT_LIST:
                processListPropertyDecl(decl, TypeStyle.FRAGMENT_LIST);
                break;
            case FRAGMENT: // never
            default:
                throw new AssertionError(decl);
            }
        }

        private void processListPropertyDecl(PropertyDeclaration decl, TypeStyle style) {
            processGetterDoc(decl);
            printer.put("%s;", getGetter(decl, style, TypeModifier.REF, false, false));
            printer.put();

            processGetterDoc(decl);
            printer.put("inline %s", getGetter(decl, style, TypeModifier.CONST_REF, false, true));
            processDeConstGetter(decl);
        }

        private void processDeConstGetter(PropertyDeclaration decl) {
            printer.append(" {");
            printer.indent(() -> {
                printer.put("return const_cast<%s*>(this)->%s();",
                        printer.getContextName(decl.getName().getParent().get()),
                        Util.getSimpleName(decl));
            });
            printer.put("}");
        }

        private void processEnumPrinting(TypeDeclaration decl) {
            if (decl instanceof EnumTypeDeclaration) {
                processEnumPrinting((EnumTypeDeclaration) decl);
            }
            decl.members(TypeDeclaration.class).forEach(this::processEnumPrinting);
        }

        private void processEnumPrinting(EnumTypeDeclaration decl) {
            printer.getIncludes().add(IncludeList.Standard.STRING);
            printer.getIncludes().add(IncludeList.Standard.STRING_VIEW);
            printer.put("/**");
            printer.put(" * @brief returns string representation of the given value.");
            printer.put(" * @param value the target enum constant");
            printer.put(" * @return string representation");
            printer.put(" * @see %s", printer.getContextName(decl));
            printer.put(" */");
            printer.put("inline constexpr std::string_view to_string_view(%s value) {",
                    printer.getContextName(decl));
            printer.indent(() -> {
                printer.put("switch (value) {");
                printer.indent(() -> {
                    for (EnumConstantDeclaration c : decl.getMembers()) {
                        printer.put("case %s: return \"%s\";", printer.getContextName(c), Util.getSimpleName(c));
                    }
                });
                printer.put("}");
                printer.put("return \"(unknown)\";");
            });
            printer.put("}");
            printer.put();
            printer.getIncludes().add(IncludeList.Standard.IOSTREAM);
            printer.put("/**");
            printer.put(" * @brief appends short name into the given output stream.");
            printer.put(" * @param out the target output stream");
            printer.put(" * @param value the target enum constant");
            printer.put(" * @return the output stream");
            printer.put(" * @see %s", printer.getContextName(decl));
            printer.put(" */");
            printer.put("inline std::ostream& operator<<(std::ostream& out, %s value) {",
                    printer.getContextName(decl));
            printer.indent(() -> {
                printer.put("return out << to_string_view(value);");
            });
            printer.put("}");
            printer.put();
        }

        private void processEnumTypeDecl(EnumTypeDeclaration decl) {
            processEnumTypeDoc(decl);
            printer.put("enum class %s {", Util.getSimpleName(decl));
            printer.indent(() -> decl.getMembers().forEach(this::processEnumConstantDecl));
            printer.put("};");
            printer.put();
        }

        private void processEnumConstantDecl(EnumConstantDeclaration decl) {
            processEnumConstantDoc(decl);
            printer.put("%s,", Util.getSimpleName(decl));
            printer.put();
        }

        private void processClassDoc(ClassDeclaration decl) {
            decl.getDescription().ifPresent(s -> {
                printer.put("/**");
                printer.put(" * @brief Represents %s.", s);
                printer.put(" */");
            });
        }

        private void processGetterDoc(PropertyDeclaration decl) {
            decl.getDescription().ifPresent(s -> {
                printer.put("/**");
                printer.put(" * @brief Returns %s.", s);
                printer.put(" * @return %s.", s);
                printer.put(" */");
            });
        }

        private void processSetterDoc(PropertyDeclaration decl) {
            decl.getDescription().ifPresent(s -> {
                printer.put("/**");
                printer.put(" * @brief Sets %s.", s);
                // FIXME attributes
                printer.put(" * @param %s %s", Util.getSimpleName(decl), s);
                printer.put(" * @return this");
                printer.put(" */");
            });
        }

        private void processRemoverDoc(PropertyDeclaration decl) {
            decl.getDescription().ifPresent(s -> {
                printer.put("/**");
                printer.put(" * @brief Releases %s from this node.", s);
                printer.put(" * @return the released node");
                // FIXME attributes
                printer.put(" */");
            });
        }

        private void processEnumTypeDoc(EnumTypeDeclaration decl) {
            decl.getDescription().ifPresent(s -> {
                printer.put("/**");
                printer.put(" * @brief Represents %s.", s);
                printer.put(" */");
            });
        }

        private void processEnumConstantDoc(EnumConstantDeclaration decl) {
            decl.getDescription().ifPresent(s -> {
                printer.put("/**");
                printer.put(" * @brief %s.", s);
                printer.put(" */");
            });
        }
    }

    private static class SourceGenerator extends BaseGenerator {

        private final ClassDeclaration topLevel;

        SourceGenerator(Configuration conf, TypeDeclarationRepository repository, ClassDeclaration topLevel) {
            super(conf, repository, topLevel, CppPrinter.FileKind.SOURCE_FILE);
            this.topLevel = topLevel;
        }

        void process() {
            topLevel.getName().getParent().ifPresent(printer::namespace);
            printer.put();

            processImplClass(topLevel);
            processWrapperClass(topLevel);
        }

        private void processImplClass(ClassDeclaration decl) {
            if (isRequireImpl(topLevel)) {
                printer.put("class %s {", getContextImplName(decl));
                printer.put("public:");
                printer.indent(() -> {
                    decl.members(PropertyDeclaration.class).forEach(this::processImplProperty);
                    printer.put();
                    processImplXtors(decl);
                    if (isCopyable(decl) || isClonable(decl)) {
                        printer.put();
                        processImplCopier(decl);
                    }
                });
                printer.put("};");
                printer.put();
            }
            decl.members(ClassDeclaration.class).forEach(this::processImplClass);
        }

        private void processImplXtors(ClassDeclaration decl) {
            printer.put("%s() = default;", getSimpleImplName(decl));
            printer.put("~%s() noexcept = default;", getSimpleImplName(decl));

            // impls are not copyable
            printer.put("%1$s(%1$s const&) = delete;", getSimpleImplName(decl));
            printer.put("%1$s& operator=(%1$s const&) = delete;", getSimpleImplName(decl));

            // impls are not movable
            printer.put("%1$s(%1$s&&) = delete;", getSimpleImplName(decl));
            printer.put("%1$s& operator=(%1$s&&) = delete;", getSimpleImplName(decl));
        }

        private void processImplProperty(PropertyDeclaration prop) {
            printer.put("%s %s", getFieldType(prop), getFieldName(prop));
            TypeMirror type = prop.getType();
            switch (type.getKind()) {
            case SIZE_TYPE:
                printer.append(" { static_cast<std::size_t>(-1) }");
                break;
            case BOOL_TYPE:
            case INT_TYPE:
            case FLOAT_TYPE:
            case STRING_TYPE:
            case LIST_TYPE:
                break;
            case DECLARED_TYPE: {
                TypeDeclaration tdecl = repository.find(type).get();
                if (tdecl instanceof EnumTypeDeclaration) {
                    getPropertyDefaultValue((EnumTypeDeclaration) tdecl).ifPresent(it -> printer.append(" { %s }", it));
                }
                break;
            }
            default:
                throw new AssertionError(type);
            }
            printer.append(";");
        }

        private Optional<String> getPropertyDefaultValue(EnumTypeDeclaration decl) {
            return decl.getDefaultValue().map(printer::getContextName);
        }

        private String getFieldType(PropertyDeclaration decl) {
            TypeMirror type = decl.getType();
            switch (getTypeKind(type)) {
            case PRIMITIVE:
            case ENUM:
            case DATA:
            case VALUE_FLAT:
                return toNameString(type, TypeStyle.BARE);
            case VALUE_POLY:
            case NODE:
                return toNameString(type, TypeStyle.UNIQUE_POINTER);
            case MANAGED:
                return toNameString(type, TypeStyle.MANAGED_POINTER);
            case BASIC_LIST:
                return toNameString(type, TypeStyle.PRIMITIVE_LIST);
            case ENUM_LIST:
                return toNameString(type, TypeStyle.ENUM_LIST);
            case VALUE_LIST:
                return toNameString(type, TypeStyle.VALUE_LIST);
            case NODE_LIST:
                return toNameString(type, TypeStyle.NODE_LIST);
            case MANAGED_NODE_LIST:
                return toNameString(type, TypeStyle.MANAGED_NODE_LIST);
            case FRAGMENT_LIST:
                return toNameString(type, TypeStyle.FRAGMENT_LIST);
            default:
                throw new AssertionError(type);
            }
        }

        private void processImplCopier(ClassDeclaration decl) {
            printer.put("std::unique_ptr<%s> %s() const {", getSimpleImplName(decl), CLONE_METHOD_NAME);
            printer.indent(() -> {
                printer.put("auto other = std::make_unique<%s>();", getSimpleImplName(decl));
                decl.members(PropertyDeclaration.class)
                        .filter(it -> !it.getAttributes().contains(PropertyDeclaration.Attribute.TRANSIENT))
                        .forEach(this::processImplFieldCopy);
                printer.put("return other;");
            });
            printer.put("}");
        }

        private void processImplFieldCopy(PropertyDeclaration decl) {
            TypeMirror type = decl.getType();
            switch (getTypeKind(type)) {
            case PRIMITIVE:
            case ENUM:
            case DATA:
            case VALUE_FLAT:
            case BASIC_LIST:
            case ENUM_LIST:
            case VALUE_LIST:
                printer.put("other->%1$s = %1$s;", getFieldName(decl));
                break;
            case NODE_LIST:
            case MANAGED_NODE_LIST:
            case FRAGMENT_LIST: {
                printer.put("if (!%s.empty()) {", getFieldName(decl));
                printer.indent(() -> {
                    printer.put("other->%1$s.reserve(%1$s.size());", getFieldName(decl));
                    printer.put("for (auto e : %s) {", getFieldName(decl));
                    printer.indent(() -> {
                        printer.getIncludes().add(conf.utilityFileName);
                        printer.put("other->%s.push_back(%s(e));",
                                getFieldName(decl),
                                printer.getContextName(makeClone));
                    });
                    printer.put("}");
                });
                printer.put("}");
                break;
            }
            case VALUE_POLY:
            case NODE:
            case MANAGED:
                printer.getIncludes().add(conf.utilityFileName);
                printer.put("other->%1$s = %2$s(%1$s);",
                        getFieldName(decl),
                        printer.getContextName(makeClone));
                break;

            default:
                throw new AssertionError(type);
            }
        }

        private void processWrapperClass(ClassDeclaration decl) {
            processWrapperXtors(decl);
            decl.members(PropertyDeclaration.class).forEach(this::processWrapperProperty);
            if (isClonable(decl)
                    && !decl.getAttributes().contains(ClassDeclaration.Attribute.INTERFACE)) {
                processWrapperCloneMethod(decl);
            }
            processCallbacks(decl);
            decl.members(ClassDeclaration.class).forEach(this::processWrapperClass);
        }

        private void processWrapperXtors(ClassDeclaration decl) {
            if (isRequireImpl(decl)) {
                printer.put("%s::%s(std::unique_ptr<%s> impl) noexcept : %s(std::move(impl)) {}",
                        printer.getContextName(decl),
                        Util.getSimpleName(decl),
                        getContextImplName(decl),
                        IMPL_FIELD);
                printer.put();
            }
            if (isRequireEmptyCtor(decl)) {
                printer.put("%s::%s()", printer.getContextName(decl), Util.getSimpleName(decl));
                if (isRequireImpl(decl)) {
                    printer.append(" : %s(std::make_unique<%s>()) {}", printer.getContextName(decl), getContextImplName(decl));
                } else {
                    printer.append(" noexcept = default;");
                }
                printer.put();
            }
            if (isRequireDataCtor(decl)) {
                assert isRequireImpl(decl);
                declareDataCtor(decl, true, false);
                printer.put("    : %s(new %s()) {", IMPL_FIELD, getContextImplName(decl));
                printer.indent(() -> decl.members(PropertyDeclaration.class)
                        .filter(this::isDataCtorParameter)
                        .forEach(this::processWrapperCtorBody));
                printer.put("}");
                printer.put();
            }
            printer.put("%s::~%s() noexcept = default;", printer.getContextName(decl), Util.getSimpleName(decl));
            printer.put();
            if (isCopyable(decl)) {
                if (isRequireImpl(decl)) {
                    printer.put("%1$s::%2$s(%1$s const& other)",
                            printer.getContextName(decl), Util.getSimpleName(decl));
                    printer.indent(() -> {
                        printer.put(": ");
                        for (TypeMirror t : decl.getSuperTypes()) {
                            printer.append("%s(other), ", toNameString(t, TypeStyle.BARE));
                        }
                        printer.append("%1$s(other.%1$s->%2$s())", IMPL_FIELD, CLONE_METHOD_NAME);
                    });
                    printer.put("{}");
                } else {
                    printer.put("%1$s::%2$s(%1$s const&) = default;",
                            printer.getContextName(decl), Util.getSimpleName(decl));
                }
                printer.put();
            }
            if (isCopyable(decl) && !isConst(decl)) {
                if (isRequireImpl(decl)) {
                    printer.put("%1$s& %1$s::operator=(%1$s const& other)", printer.getContextName(decl));
                    printer.append(" {");
                    printer.indent(() -> {
                        for (TypeMirror superType : decl.getSuperTypes()) {
                            printer.put("%s::operator=(other);", toNameString(superType, TypeStyle.BARE));
                        }
                        printer.put("%1$s = other.%1$s->%2$s();", IMPL_FIELD, CLONE_METHOD_NAME);
                        printer.put("return *this;");
                    });
                    printer.put("}");
                } else {
                    printer.put("%1$s& %1$s::operator=(%1$s const&)", printer.getContextName(decl));
                    printer.append(" = default;");
                }
                printer.put();
            }
            if (isMovable(decl)) {
                printer.put("%1$s::%2$s(%1$s&&) noexcept = default;",
                        printer.getContextName(decl), Util.getSimpleName(decl));
                printer.put();
            }
            if (isMovable(decl) && !isConst(decl)) {
                printer.put("%1$s& %1$s::operator=(%1$s&&) noexcept = default;", printer.getContextName(decl));
                printer.put();
            }
        }

        private void processWrapperCtorBody(PropertyDeclaration decl) {
            TypeMirror type = decl.getType();
            switch (getTypeKind(type)) {
            case PRIMITIVE:
            case ENUM:
            case DATA:
            case VALUE_FLAT:
            case BASIC_LIST:
            case ENUM_LIST:
            case VALUE_LIST:
                printer.put("%s->%s = %s;",
                        IMPL_FIELD, getFieldName(decl),
                        Util.getSimpleName(decl));
                break;

            case VALUE_POLY:
            case NODE:
            case MANAGED:
            case FRAGMENT:
            case NODE_LIST:
            case MANAGED_NODE_LIST:
            case FRAGMENT_LIST:
            default:
                throw new AssertionError(type);
            }
        }

        private void processWrapperProperty(PropertyDeclaration decl) {
            TypeMirror type = decl.getType();
            switch (getTypeKind(type)) {
            case PRIMITIVE:
            case ENUM:
                processPrimitivePropertyDecl(decl);
                break;
            case DATA:
                processDataPropertyDecl(decl);
                break;
            case VALUE_FLAT:
                processValueFlatPropertyDecl(decl);
                break;
            case VALUE_POLY:
                processValuePolyPropertyDecl(decl);
                break;
            case NODE:
                processNodePropertyDecl(decl);
                break;
            case MANAGED:
                processManagedPropertyDecl(decl);
                break;
            case BASIC_LIST:
                processListPropertyDecl(decl, TypeStyle.PRIMITIVE_LIST);
                break;
            case ENUM_LIST:
                processListPropertyDecl(decl, TypeStyle.ENUM_LIST);
                break;
            case VALUE_LIST:
                processListPropertyDecl(decl, TypeStyle.VALUE_LIST);
                break;
            case NODE_LIST:
                processListPropertyDecl(decl, TypeStyle.NODE_LIST);
                break;
            case MANAGED_NODE_LIST:
                processListPropertyDecl(decl, TypeStyle.MANAGED_NODE_LIST);
                break;
            case FRAGMENT_LIST:
                processListPropertyDecl(decl, TypeStyle.FRAGMENT_LIST);
                break;
            case FRAGMENT: // never
            default:
                throw new AssertionError(decl);
            }
        }

        private void processPrimitivePropertyDecl(PropertyDeclaration decl) {
            printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.BARE, true, true));
            putGetterBody(decl, GetterStyle.BARE);

            if (isCopiable(decl)) {
                printer.put("%s {", getSetter(decl, TypeStyle.BARE, TypeModifier.BARE, true));
                putSetterBody(decl, SetterStyle.COPY_ASSIGN);
            }
        }

        private void processDataPropertyDecl(PropertyDeclaration decl) {
            printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_REF, true, true));
            putGetterBody(decl, GetterStyle.BARE);

            if (!isConstProperty(decl)) {
                printer.put("%s {", getSetter(decl, TypeStyle.BARE, TypeModifier.BARE, true));
                putSetterBody(decl, SetterStyle.MOVE_ASSIGN);
            }
        }

        private void processValueFlatPropertyDecl(PropertyDeclaration decl) {
            if (isConstProperty(decl)) {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_REF, true, true));
                putGetterBody(decl, GetterStyle.BARE);
            } else {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.REF, true, false));
                putGetterBody(decl, GetterStyle.BARE);
            }

            if (isCopiable(decl)) {
                printer.put("%s {", getSetter(decl, TypeStyle.BARE, TypeModifier.CONST_REF, true));
                putSetterBody(decl, SetterStyle.COPY_ASSIGN);
            }
            if (isMovable(decl)) {
                printer.put("%s {", getSetter(decl, TypeStyle.BARE, TypeModifier.RVALUE_REF, true));
                putSetterBody(decl, SetterStyle.MOVE_ASSIGN);
            }
        }

        private void processValuePolyPropertyDecl(PropertyDeclaration decl) {
            if (isConstProperty(decl)) {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_POINTER, true, true));
                putGetterBody(decl, GetterStyle.GET);
            } else {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.POINTER, true, false));
                putGetterBody(decl, GetterStyle.GET);
            }
            if (!isConstProperty(decl)) {
                printer.put("%s {", getSetter(decl, TypeStyle.UNIQUE_POINTER, TypeModifier.BARE, true));
                putSetterBody(decl, SetterStyle.MOVE_ASSIGN);
            }
        }

        private void processNodePropertyDecl(PropertyDeclaration decl) {
            if (isConstProperty(decl)) {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_POINTER, true, true));
                putGetterBody(decl, GetterStyle.GET);
            } else {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.POINTER, true, false));
                putGetterBody(decl, GetterStyle.GET);
            }
            if (!isConstProperty(decl)) {
                printer.put("%s {", getSetter(decl, TypeStyle.UNIQUE_POINTER, TypeModifier.BARE, true));
                putSetterBody(decl, SetterStyle.MOVE_ASSIGN);

                printer.put("%s {", getRemover(decl, true));
                putRemoverBody(decl, RemoverStyle.MOVE_RETURN);
            }
        }

        private void processManagedPropertyDecl(PropertyDeclaration decl) {
            if (isConstProperty(decl)) {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.CONST_POINTER, true, true));
                putGetterBody(decl, GetterStyle.GET);
            } else {
                printer.put("%s {", getGetter(decl, TypeStyle.BARE, TypeModifier.POINTER, true, false));
                putGetterBody(decl, GetterStyle.GET);
            }
            if (!isConstProperty(decl)) {
                printer.put("%s {", getSetter(decl, TypeStyle.UNIQUE_POINTER, TypeModifier.BARE, true));
                putSetterBody(decl, SetterStyle.MOVE_ASSIGN);

                printer.put("%s {", getRemover(decl, true));
                putRemoverBody(decl, RemoverStyle.RELEASE_RETURN);
            }
        }

        private void processListPropertyDecl(PropertyDeclaration decl, TypeStyle style) {
            printer.put("%s {", getGetter(decl, style, TypeModifier.REF, true, false));
            putGetterBody(decl, GetterStyle.BARE);
        }

        private static String getFieldName(PropertyDeclaration decl) {
            return Util.getSimpleName(decl) + "_";
        }

        private void putGetterBody(PropertyDeclaration decl, GetterStyle style) {
            printer.indent(() -> {
                switch (style) {
                case BARE:
                    printer.put("return %s->%s;", IMPL_FIELD, getFieldName(decl));
                    break;
                case GET:
                    printer.put("return %s->%s.get();", IMPL_FIELD, getFieldName(decl));
                    break;
                case DEREFERENCE_UNIQUE:
                    printer.put("return *%s->%s;", IMPL_FIELD, getFieldName(decl));
                    break;
                default:
                    throw new AssertionError(style);
                }
            });
            printer.put("}");
            printer.put();
        }

        private void putSetterBody(PropertyDeclaration decl, SetterStyle style) {
            printer.indent(() -> {
                switch (style) {
                case COPY_ASSIGN:
                    printer.put("%s->%s = %s;", IMPL_FIELD, getFieldName(decl), Util.getSimpleName(decl));
                    break;
                case MOVE_ASSIGN:
                    printer.getIncludes().add(Standard.UTILITY);
                    printer.put("%s->%s = std::move(%s);", IMPL_FIELD, getFieldName(decl), Util.getSimpleName(decl));
                    break;
                default:
                    throw new AssertionError(style);
                }
                printer.put("return *this;");
            });
            printer.put("}");
            printer.put();
        }

        private void putRemoverBody(PropertyDeclaration decl, RemoverStyle style) {
            printer.indent(() -> {
                switch (style) {
                case MOVE_RETURN:
                    printer.getIncludes().add(Standard.UTILITY);
                    printer.put("%s ret { std::move(%s->%s) };",
                            toNameString(decl.getType(), TypeStyle.UNIQUE_POINTER),
                            IMPL_FIELD,
                            getFieldName(decl));
                    // explicitly reset
                    printer.put("%s->%s = {};", IMPL_FIELD, getFieldName(decl));
                    printer.put("return ret;");
                    break;
                case RELEASE_RETURN:
                    printer.put("return %s->%s.release();", IMPL_FIELD, getFieldName(decl));
                    break;
                default:
                    throw new AssertionError(style);
                }
            });
            printer.put("}");
            printer.put();
        }

        private enum GetterStyle {
            BARE,
            GET,
            DEREFERENCE_UNIQUE,
        }

        private enum SetterStyle {
            COPY_ASSIGN,
            MOVE_ASSIGN,
        }

        private enum RemoverStyle {
            MOVE_RETURN,
            RELEASE_RETURN,
        }

        private void processWrapperCloneMethod(ClassDeclaration decl) {
            printer.put("%s* %s::%s() const & {",
                    printer.getContextName(decl),
                    printer.getContextName(decl),
                    CLONE_METHOD_NAME);
            printer.indent(() -> {
                if (isRequireImpl(decl) ) {
                    printer.put("return new %s(%s->%s());  // NOLINT", 
                            printer.getContextName(decl), IMPL_FIELD, CLONE_METHOD_NAME);
                } else {
                    printer.put("return new %s();  // NOLINT", 
                            printer.getContextName(decl));
                }
            });
            printer.put("}");
            printer.put();
            
            printer.put("%s* %s::%s() && {",
                    printer.getContextName(decl),
                    printer.getContextName(decl),
                    CLONE_METHOD_NAME);
            printer.indent(() -> {
                if (isRequireImpl(decl) ) {
                    printer.put("return new %s(std::move(%s));  // NOLINT", 
                            printer.getContextName(decl), IMPL_FIELD, CLONE_METHOD_NAME);
                } else {
                    printer.put("return new %s();  // NOLINT", 
                            printer.getContextName(decl));
                }
            });
            printer.put("}");
            printer.put();
        }
    }

    /**
     * A callback of code generator.
     * @since WIP
     */
    @FunctionalInterface
    public interface Callback {

        /**
         * Generates source for the type.
         * @param printer the printer
         * @param declaration the generating declaration
         */
        void generate(CppPrinter printer, ClassDeclaration declaration);
    }

    /**
     * The generator configuration.
     * @since WIP
     */
    public static class Configuration {

        final Name managedPtrTypeName;

        final Name fragmentListTypeName;

        final Name nodeListTypeName;

        final Name valueListTypeName;

        final Name dataListTypeName;

        final Name managedNodeListTypeName;

        final Name utilityFileName;

        /**
         * Creates a new instance.
         * @param utilityNamespace the namespace of internal utilities
         * @param managedPtrTypeName the managed pointer type name
         * @param dataListTypeName the data list type name
         * @param valueListTypeName the value list type name
         * @param nodeListTypeName the node list type name
         * @param managedListTypeName the managed list type name
         * @param fragmentListTypeName the fragment list type name
         */
        public Configuration(
                Name utilityNamespace,
                Name managedPtrTypeName,
                Name dataListTypeName,
                Name valueListTypeName,
                Name nodeListTypeName,
                Name managedListTypeName,
                Name fragmentListTypeName) {
            this.utilityFileName = utilityNamespace;
            this.managedPtrTypeName = managedPtrTypeName;
            this.dataListTypeName = dataListTypeName;
            this.valueListTypeName = valueListTypeName;
            this.nodeListTypeName = nodeListTypeName;
            this.managedNodeListTypeName = managedListTypeName;
            this.fragmentListTypeName = fragmentListTypeName;
        }

        Name listTypeNameOf(IncludeList includes, TypeStyle style) {
            switch (style) {
            case PRIMITIVE_LIST:
                includes.add(IncludeList.Standard.VECTOR);
                return IncludeList.VECTOR_TYPE;
            case ENUM_LIST:
                includes.add(IncludeList.Standard.SET);
                return IncludeList.SET_TYPE;
            case DATA_LIST:
                return bless(includes, dataListTypeName);
            case VALUE_LIST:
                return bless(includes, valueListTypeName);
            case NODE_LIST:
                return bless(includes, nodeListTypeName);
            case MANAGED_NODE_LIST:
                return bless(includes, managedNodeListTypeName);
            case FRAGMENT_LIST:
                return bless(includes, fragmentListTypeName);
            default:
                throw new AssertionError(style);
            }
        }

        private static Name bless(IncludeList includes, Name name) {
            includes.add(name);
            return name;
        }
    }
}
