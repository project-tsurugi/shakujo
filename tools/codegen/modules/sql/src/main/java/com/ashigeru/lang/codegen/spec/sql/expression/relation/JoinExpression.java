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
package com.ashigeru.lang.codegen.spec.sql.expression.relation;

import com.ashigeru.lang.codegen.ir.reflect.Default;
import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.ir.reflect.Opt;
import com.ashigeru.lang.codegen.spec.sql.expression.Expression;
import com.ashigeru.lang.codegen.spec.sql.key.VariableKey;

@Description("joining relations")
@Extends(Expression.class)
@Extends(VariableKey.Provider.class)
@SuppressWarnings("javadoc")
public class JoinExpression extends MetaClass {

    @Description("join operator kind")
    Property<Kind> operatorKind = property();

    @Description("left term")
    Property<Expression> left = property();

    @Description("right term")
    Property<Expression> right = property();

    @Description("join condition")
    @Opt
    Property<Expression> condition = property();

    @Description("join operation kind")
    public enum Kind {

        @Description("cross join")
        CROSS,

        @Description("inner join")
        INNER,

        @Description("left outer join")
        LEFT_OUTER,

        @Description("right outer join")
        RIGHT_OUTER,

        @Description("full outer join")
        FULL_OUTER,

        @Description("natural inner join")
        NATURAL_INNER,

        @Description("natural left outer join")
        NATURAL_LEFT_OUTER,

        @Description("natural right outer join")
        NATURAL_RIGHT_OUTER,

        @Description("natural full outer join")
        NATURAL_FULL_OUTER,

        @Description("union join")
        UNION_OUTER,

        @Description("left semi-join")
        LEFT_SEMI,

        @Description("right semi-join")
        RIGHT_SEMI,

        @Description("invalid operator")
        @Default
        INVALID
    }
}
