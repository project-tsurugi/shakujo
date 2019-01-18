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

import com.ashigeru.lang.codegen.ir.reflect.Default;
import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;

@Description("unary operator expression")
@Extends(Expression.class)
@SuppressWarnings("javadoc")
public class UnaryOperator extends MetaClass {

    @Description("unary operator kind")
    Property<Kind> operatorKind = property();

    @Description("operand")
    Property<Expression> operand = property();

    @Description("unary operator kind")
    public enum Kind {

        @Description("unary plus")
        PLUS,

        @Description("inverts sign of numeric values")
        SIGN_INVERSION,

        @Description("bit-wise complement")
        BITWISE_COMPLEMENT,

        @Description("logical negate")
        LOGICAL_NOT,

        @Description("conditional negate")
        CONDITIONAL_NOT,

        @Description("test whether is null")
        IS_NULL,

        @Description("test whether is NOT null")
        IS_NOT_NULL,

        @Description("test whether is true")
        IS_TRUE,

        @Description("test whether is not true")
        IS_NOT_TRUE,

        @Description("test whether is false")
        IS_FALSE,

        @Description("test whether is not false")
        IS_NOT_FALSE,

        @Description("raise exception if null")
        CHECK_NULL,

        @Description("test whether the all elements in the relation is just true")
        RELATION_ALL,

        @Description("test whether the any elements in the relation contains true")
        RELATION_ANY,

        @Description("test whether the relation is empty")
        RELATION_EXISTS,

        @Description("test whether the relation is NOT empty")
        RELATION_NOT_EXISTS,

        @Default
        @Description("invalid operator")
        INVALID,
    }
}
