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

import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;

import com.ashigeru.lang.codegen.generator.cpp.CppPrinter;
import com.ashigeru.lang.codegen.generator.cpp.IncludeList;
import com.ashigeru.lang.codegen.ir.model.Name;
import com.ashigeru.lang.codegen.ir.model.QualifiedName;
import com.ashigeru.lang.codegen.util.Namer;

/**
 * util to gen visitor.
 * @since WIP
 */
public class VisitorTool {

    private static final String TYPE_RETURN = "Return";

    private static final String TYPE_PARAMETERS = "Args";


    /**
     * main.
     * @param args args
     */
    public static void main(String[] args) {
        value(true);
    }

    static void type(boolean isConst) {
        QualifiedName base = CppGen.NAMESPACE_MODEL.resolve("binding");
        Name baseClass = base.resolve("TypeBinding");
        Name elementRoot = base.resolve("type");
        String[] elements = {
                "INT",
                "FLOAT",
                "CHAR",
                "STRING",
                "BOOL",
                "NULL_",
                "TUPLE",
                "ARRAY",
                "VECTOR",
                "RELATION",
                "CURSOR",
                "UNDEFINED",
                "ERROR",
        };
        gen(baseClass, elementRoot, elements, isConst);
    }

    static void value(boolean isConst) {
        QualifiedName base = CppGen.NAMESPACE_MODEL.resolve("binding");
        Name baseClass = base.resolve("ValueBinding");
        Name elementRoot = base.resolve("value");
        String[] elements = {
                "BOOL",
                "INT",
                "FLOAT",
                "STRING",
                "TUPLE",
                "NULL_",
                "UNDEFINED",
                "ERROR",
        };
        gen(baseClass, elementRoot, elements, isConst);
    }

    static void gen(Name baseClass, Name elementRoot, String[] elements, boolean isConst) {
        Name target = elementRoot.resolve(isConst ? "ConstVisitor" : "Visitor");
        CppPrinter printer = CppPrinter.includeFile(target);
        printer.getName().getParent().ifPresent(printer::namespace);

        printer.put("/**");
        printer.put(" * @brief A visitor for %s.", baseClass.getLast().getToken());
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
            printer.put();
            for (String element :elements) {
                processNode(printer, elementRoot, element, isConst);
                printer.put();
            }
            processBase(printer, baseClass, elementRoot, elements, isConst);
        });
        printer.put("};");
        printer.put();


        PrintWriter w = new PrintWriter(new OutputStreamWriter(System.out, StandardCharsets.UTF_8));
        printer.printTo(w);
        w.flush();
    }

    private static void processDefault(CppPrinter printer, Name baseClass, boolean isConst) {
        printer.getIncludes().add(IncludeList.Standard.SSTREAM);
        printer.getIncludes().add(IncludeList.Standard.STDEXCEPT);
        printer.put("/**");
        printer.put(" * @brief Processes the default %s.", baseClass.getLast().getToken());
        printer.put(" * @param node the processing target");
        printer.put(" * @param args the processing arguments");
        printer.put(" * @return the result");
        printer.put(" * @see %s", printer.getContextName(baseClass));
        printer.put(" */");
        printer.put("virtual %s visitDefault(%s%s& node, %s... args) {",
                TYPE_RETURN,
                isConst ? "const " : "",
                printer.getContextName(baseClass),
                TYPE_PARAMETERS);
        printer.indent(() -> {
            printer.put("(void) sizeof...(args); // ignore unused");
            printer.put("std::ostringstream ss;");
            printer.put("ss << \"visit for \" << node.kind() << \" is not implemented.\";");
            printer.put("throw std::runtime_error(ss.str());");
        });
        printer.put("}");
    }

    private static void processNode(CppPrinter printer, Name elementRoot, String element, boolean isConst) {
        Name target = toClassName(elementRoot, element);
        printer.getIncludes().add(IncludeList.Standard.UTILITY);
        printer.put("/**");
        printer.put(" * @brief Processes %s.", target.getLast().getToken());
        printer.put(" * @param node the processing target");
        printer.put(" * @param args the processing arguments");
        printer.put(" * @return the result");
        printer.put(" * @see %s", printer.getContextName(target));
        printer.put(" */");
        printer.put("virtual %s visit(%s%s& node, %s... args) {",
                TYPE_RETURN,
                isConst ? "const " : "",
                        printer.getContextName(target),
                TYPE_PARAMETERS);
        printer.indent(() -> {
            printer.put("return visitDefault(node, std::forward<%s>(args)...);",
                    TYPE_PARAMETERS);
        });
        printer.put("}");
    }

    private static void processBase(CppPrinter printer, Name baseClass, Name elementRoot, String[] elements, boolean isConst) {
        Name kindName = baseClass.resolve("Kind");
        printer.getIncludes().add(baseClass);
        printer.getIncludes().add(IncludeList.Standard.UTILITY);
        printer.put("/**");
        printer.put(" * @brief Dispatches visitor action for individual element types.");
        printer.put(" * @param node the processing target element");
        printer.put(" * @param args the processing arguments");
        printer.put(" * @return the result");
        printer.put(" */");
        printer.put("%s dispatch(%s%s& node, %s... args) {",
                TYPE_RETURN,
                isConst ? "const " : "",
                printer.getContextName(baseClass),
                TYPE_PARAMETERS);
        printer.indent(() -> {
            printer.put("switch (node.kind()) {");
            for (String element : elements) {
                Name target = toClassName(elementRoot, element);

                printer.getIncludes().add(target);
                printer.put("case %s::%s:",
                        printer.getContextName(kindName),
                        element);
                printer.indent(() -> {
                    printer.put("return visit(");
                    printer.put("    dynamic_cast<%s%s&>(node),",
                            isConst ? "const " : "",
                            printer.getContextName(target));
                    printer.put("    std::forward<%s>(args)...);",
                            TYPE_PARAMETERS);
                });
                printer.put();
            }
            printer.put("default:");
            printer.indent(() -> {
                printer.put("return visitDefault(node, std::forward<%s>(args)...);",
                        TYPE_PARAMETERS);
            });
            printer.put("}");
        });
        printer.put("}");
    }

    private static Name toClassName(Name elementRoot, String element) {
        return elementRoot.resolve(Namer.parse(element).toUpperCamelCase());
    }
}
