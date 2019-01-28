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
import java.util.List;
import java.util.Optional;
import java.util.function.Function;
import java.util.stream.Collectors;

import com.ashigeru.lang.codegen.generator.cpp.CppFiler;
import com.ashigeru.lang.codegen.generator.cpp.CppPrinter;
import com.ashigeru.lang.codegen.generator.cpp.FactoryGenerator;
import com.ashigeru.lang.codegen.generator.cpp.IncludeList;
import com.ashigeru.lang.codegen.generator.cpp.KindGenerator;
import com.ashigeru.lang.codegen.generator.cpp.SerializerGenerator;
import com.ashigeru.lang.codegen.generator.cpp.TypeDeclarationGenerator;
import com.ashigeru.lang.codegen.generator.cpp.VisitorGenerator;
import com.ashigeru.lang.codegen.generator.cpp.WalkerGenerator;
import com.ashigeru.lang.codegen.ir.model.ClassDeclaration;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.TypeDeclaration;
import com.ashigeru.lang.codegen.ir.model.TypeDeclarationRepository;
import com.ashigeru.lang.codegen.ir.reflect.NameUtil;
import com.ashigeru.lang.codegen.spec.sql.expression.Expression;
import com.ashigeru.lang.codegen.spec.sql.name.Index;
import com.ashigeru.lang.codegen.spec.sql.program.GlobalDeclaration;
import com.ashigeru.lang.codegen.spec.sql.statement.Statement;
import com.ashigeru.lang.codegen.spec.sql.type.Type;

/**
 * Program entry.
 * @since WIP
 */
public class CppGen {

    static final Name NAMESPACE_ROOT = Name.of("shakujo");

    static final Name NAMESPACE_MODEL = NAMESPACE_ROOT.resolve("model");

    static final Name NAMESPACE_COMMON = NAMESPACE_ROOT.resolve("common");

    static final Name ROOT_PACKAGE = Name.of(CppGen.class.getPackageName().split("\\."));

    static final Function<Class<?>, Name> TYPE_NAMER = NameUtil.toName()
            .andThen(NameUtil.replacePrefix(
                    Name.of(CppGen.class.getPackageName().split("\\.")),
                    NAMESPACE_MODEL))
            .andThen(it -> {
                List<String> tokens = it.getTokens();
                int sz = NAMESPACE_MODEL.getTokens().size();
                if (tokens.get(sz).equals("common")) {
                    return NAMESPACE_COMMON.resolve(Name.of(tokens.subList(sz + 1, tokens.size())));
                }
                return Name.of(tokens);
            });

    private static final List<Class<?>> BASE_CLASSES = List.of(
            GlobalDeclaration.class,
            com.ashigeru.lang.codegen.spec.sql.name.Name.class,
            Expression.class,
            Statement.class,
            Type.class);

    private static final Name NAMESPACE_MODEL_UTIL = NAMESPACE_MODEL.resolve("util");

    private static final Name NAMESPACE_COMMON_UTIL = NAMESPACE_COMMON.resolve("util");

    private static final Name UTIL_FILENAME = NAMESPACE_COMMON_UTIL.resolve("utility");

    private static final Name IR_BUILDER_BASE_TYPE = NAMESPACE_MODEL.resolve("IRFactoryBase");

    private static final Name MOVE_INIT_LIST_TYPE = NAMESPACE_COMMON_UTIL.resolve("MoveInitializerList");

    private static final Name MANAGED_PTR_TYPE = NAMESPACE_COMMON_UTIL.resolve("ManagedPtr");

    private static final Name DATA_LIST_TYPE = NAMESPACE_MODEL_UTIL.resolve("DataList");

    private static final Name VALUE_LIST_TYPE = NAMESPACE_MODEL_UTIL.resolve("ValueList");

    private static final Name NODE_LIST_TYPE = NAMESPACE_MODEL_UTIL.resolve("NodeList");

    private static final Name ROOT_NOTE_TYPE = NAMESPACE_MODEL.resolve("Node");

    private static final Name MANAGED_NODE_LIST_TYPE = NAMESPACE_MODEL_UTIL.resolve("ManagedNodeList");

    private static final Name FRAGMENT_LIST_TYPE = NAMESPACE_MODEL_UTIL.resolve("FragmentList");

    private static final Name WALKER_TYPE = NAMESPACE_MODEL_UTIL.resolve("NodeWalker");

    private static final Name SERIALIZER_BASE_TYPE = NAMESPACE_MODEL_UTIL.resolve("NodeSerializerBase");

    private static final Name PRINTER_TYPE = NAMESPACE_COMMON_UTIL.resolve("DataSerializer");

    private static final TypeDeclarationGenerator.Configuration GENERATOR_CONF =
            new TypeDeclarationGenerator.Configuration(
                    UTIL_FILENAME,
                    MANAGED_PTR_TYPE,
                    DATA_LIST_TYPE,
                    VALUE_LIST_TYPE,
                    NODE_LIST_TYPE,
                    MANAGED_NODE_LIST_TYPE,
                    FRAGMENT_LIST_TYPE);

    private final CppFiler filer;

    private final TypeDeclarationRepository repository;

    CppGen(CppFiler filer, TypeDeclarationRepository repository) {
        this.filer = filer;
        this.repository = repository;
    }

    void process() throws IOException {
        generateKinds();
        generateTypeHeaders();
        generateTypeSources();
        generateVisitors();
        generateFactory();
        generateWalker();
        generateSerializer();
    }

    private void generateFactory() throws IOException {
        System.out.printf("factory: %s%n", IR_BUILDER_BASE_TYPE);
        FactoryGenerator generator = new FactoryGenerator(
                repository,
                MOVE_INIT_LIST_TYPE);
        generator.generate(filer, IR_BUILDER_BASE_TYPE);
    }

    private void generateWalker() throws IOException {
        System.out.printf("walker: %s%n", WALKER_TYPE);
        WalkerGenerator generator = new WalkerGenerator(repository, filer);
        generator.generate(
                WALKER_TYPE,
                repository.find(ClassDeclaration.class, ROOT_NOTE_TYPE).get(),
                getBaseClasses());
    }

    private void generateSerializer() throws IOException {
        System.out.printf("serializer: %s%n", SERIALIZER_BASE_TYPE);
        SerializerGenerator generator = new SerializerGenerator(
                repository, filer,
                PRINTER_TYPE, UTIL_FILENAME);
        generator.generate(SERIALIZER_BASE_TYPE, getBaseClasses());
    }

    private void generateKinds() throws IOException {
        KindGenerator generator = new KindGenerator(repository);
        for (var c : BASE_CLASSES) {
            var decl = find(c);
            System.out.printf("kind: %s%n", decl);
            generator.generate(filer, decl);
        }
    }

    private void generateTypeHeaders() throws IOException {
        TypeDeclarationGenerator generator = new TypeDeclarationGenerator(GENERATOR_CONF, repository);
        generator.addCallback(this::injectKindMethod);
        generator.addCallback(this::injectIndexDecl);
        for (TypeDeclaration decl : repository) {
            if (isSkipTarget(decl)) {
                continue;
            }
            System.out.printf("header: %s%n", decl.getName());
            generator.generateHeaderFile(filer, decl);
        }
    }

    private void generateTypeSources() throws IOException {
        TypeDeclarationGenerator generator = new TypeDeclarationGenerator(GENERATOR_CONF, repository);
        for (TypeDeclaration decl : repository) {
            if (isSkipTarget(decl)) {
                continue;
            }
            System.out.printf("source: %s%n", decl.getName());
            if (decl instanceof ClassDeclaration) {
                generator.generateSourceFile(filer, (ClassDeclaration) decl);
            }
        }
    }

    private void injectKindMethod(CppPrinter printer, ClassDeclaration decl) {
        TypeDeclaration base = findBaseClassOf(decl).orElse(null);
        if (base == null) {
            return;
        }
        printer.put("public:");
        printer.indent(() -> {
            Name typeName = KindGenerator.getTypeName(base);
            printer.getIncludes().add(typeName);
            printer.put("/**");
            printer.put(" * @brief Returns the node kind.");
            printer.put(" * @return the node kind");
            printer.put(" */");
            printer.put();
            if (base == decl) {
                printer.append("virtual ");
            }
            printer.append("%1$s %2$s() const",
                    printer.getContextName(typeName),
                    "kind");
            if (base == decl) {
                printer.append(" = 0;");
            } else {
                printer.append(" override {");
                printer.indent(() -> {
                    printer.put("return %s::%s;",
                            printer.getContextName(typeName),
                            KindGenerator.getConstantName(decl));
                });
                printer.put("}");
            }
            printer.put();
        });
    }

    private void injectIndexDecl(CppPrinter printer, ClassDeclaration decl) {
        if (!decl.getName().equals(TYPE_NAMER.apply(Index.class))) {
            return;
        }
        printer.put("public:");
        printer.indent(() -> {
            printer.getIncludes().add(IncludeList.Standard.STDDEF);
            printer.put("/**");
            printer.put(" * @brief represents the invalid position.");
            printer.put(" */");
            printer.put("static constexpr %1$s INVALID_POSITION = static_cast<%1$s>(-1);",
                    printer.getContextName(IncludeList.SIZE_TYPE));
            printer.put();

            printer.getIncludes().add(IncludeList.Standard.OPTIONAL);
            printer.put("/**");
            printer.put(" * @brief returns index position.");
            printer.put(" * @return the optional index position");
            printer.put(" */");
            printer.put("inline std::optional<std::size_t> position_optional() const {");
            printer.indent(() -> {
                printer.put("if (position() == INVALID_POSITION) {");
                printer.indent(() -> printer.put("return {};"));
                printer.put("}");
                printer.put("return std::make_optional<std::size_t>(position());");
            });
            printer.put("}");
            printer.put();

            printer.getIncludes().add(IncludeList.Standard.OPTIONAL);
            printer.put("/**");
            printer.put(" * @brief returns index position.");
            printer.put(" * @param position the optional index position");
            printer.put(" * @return this");
            printer.put(" */");
            printer.put("inline Index& position_optional(std::optional<std::size_t> const& position) {");
            printer.indent(() -> {
                printer.put("this->position(position.value_or(INVALID_POSITION));");
                printer.put("return *this;");
            });
            printer.put("}");
            printer.put();

            printer.put("/**");
            printer.put(" * @brief returns whether or not this index has either valid element name or position.");
            printer.put(" * @return true if this has either valid element name or position");
            printer.put(" * @return false otherwise");
            printer.put(" */");
            printer.put("inline bool is_valid() const {");
            printer.indent(() -> {
                printer.put("return name() != nullptr || position() != INVALID_POSITION;");
            });
            printer.put("}");
            printer.put();
        });
    }

    private Optional<ClassDeclaration> findBaseClassOf(ClassDeclaration decl) {
        return getBaseClasses().stream()
                .filter(it -> repository.isSubtypeOf(decl.asTypeMirror(), it.asTypeMirror()))
                .findFirst();
    }

    private TypeDeclaration find(Class<?> c) {
        return repository.find(TYPE_NAMER.apply(c)).get();
    }

    private void generateVisitors() throws IOException {
        VisitorGenerator generator = new VisitorGenerator(repository);
        for (var decl : getBaseClasses()) {
            System.out.printf("visitor: %s%n", decl);
            generator.generate(filer, decl);
        }
    }

    private List<ClassDeclaration> getBaseClasses() {
        return BASE_CLASSES.stream()
                .map(this::find)
                .map(it -> (ClassDeclaration) it)
                .collect(Collectors.toList());
    }

    private static boolean isSkipTarget(TypeDeclaration decl) {
        return decl.getAttributes().contains(ClassDeclaration.Attribute.EXTERN);
    }
}
