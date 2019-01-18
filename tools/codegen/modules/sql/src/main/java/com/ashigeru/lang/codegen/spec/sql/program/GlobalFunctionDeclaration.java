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
package com.ashigeru.lang.codegen.spec.sql.program;

import java.util.List;

import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.ir.reflect.Opt;
import com.ashigeru.lang.codegen.spec.sql.key.FunctionKey;
import com.ashigeru.lang.codegen.spec.sql.key.VariableKey;
import com.ashigeru.lang.codegen.spec.sql.name.Name;
import com.ashigeru.lang.codegen.spec.sql.statement.Statement;
import com.ashigeru.lang.codegen.spec.sql.type.Type;

@Description("declaring program level functions")
@Extends(GlobalDeclaration.class)
@Extends(FunctionKey.Provider.class)
@SuppressWarnings("javadoc")
public class GlobalFunctionDeclaration extends MetaClass {

    @Description("function name")
    Property<Name> name = property();

    @Description("function return type")
    Property<Type> type = property();

    @Description("function parameters specification")
    Property<List<Parameter>> parameters = property();

    @Description("function body")
    Property<Statement> body = property();

    @Description("function attributes")
    @Opt
    Property<List<Attribute>> attributes = property();

    @Description("function parameter declaration")
    @Extends(VariableKey.Provider.class)
    public static class Parameter extends MetaClass {

        @Description("parameter name")
        Property<Name> name = property();

        @Description("parameter type")
        Property<Type> type = property();

        @Description("parameter attributes")
        @Opt
        Property<List<Attribute>> attributes = property();

        @Description("function parameter attribute")
        public enum Attribute {

            @Description("parameter is not modifiable")
            CONST,
        }
    }

    @Description("function declaration attribute")
    public enum Attribute {

        @Description("function should be inlined")
        INLINE,
    }
}
