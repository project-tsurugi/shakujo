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
package com.ashigeru.lang.codegen.spec.sql.expression;

import java.util.List;

import com.ashigeru.lang.codegen.ir.reflect.Default;
import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.ir.reflect.Opt;
import com.ashigeru.lang.codegen.spec.sql.key.FunctionKey;
import com.ashigeru.lang.codegen.spec.sql.name.Name;

@Description("calling functions")
@Extends(Expression.class)
@Extends(FunctionKey.Provider.class)
@SuppressWarnings("javadoc")
public class FunctionCall extends MetaClass {

    @Description("function name")
    Property<Name> name = property();

    @Description("function arguments")
    @Opt
    Property<List<Expression>> arguments = property();

    @Description("set quantifier")
    @Opt
    Property<Quantifier> quantifier = property();

    @Description("set quantifier")
    public enum Quantifier {

        @Description("no quantifiers")
        @Default
        ABSENT,

        @Description("aggregate on bag")
        ALL,

        @Description("aggregate on set")
        DISTINCT,
        
        @Description("special case for f(*)")
        ASTERISK,
    }
}
