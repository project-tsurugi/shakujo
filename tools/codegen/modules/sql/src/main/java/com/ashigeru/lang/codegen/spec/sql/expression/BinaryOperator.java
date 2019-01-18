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

@Description("binary operator expression")
@Extends(Expression.class)
@SuppressWarnings("javadoc")
public class BinaryOperator extends MetaClass {

    @Description("binary operator kind")
    Property<Kind> operatorKind = property();

    @Description("left term")
    Property<Expression> left = property();

    @Description("right term")
    Property<Expression> right = property();

    @Description("binary operator kind")
    public enum Kind {

        @Description("addition")
        ADD,

        @Description("subtraction")
        SUBTRACT,

        @Description("multiplication")
        MULTIPLY,

        @Description("division")
        DIVIDE,

        @Description("remainder")
        REMAINDER,

        @Description("bit-wise and")
        BITWISE_AND,

        @Description("bit-wise or")
        BITWISE_OR,

        @Description("bit-wise exclusive or")
        BITWISE_XOR,

        @Description("left bit shift")
        SHIFT_LEFT,

        @Description("signed right bit shift")
        SHIFT_RIGHT_ARITHMETIC,

        @Description("unsigned right bit shift")
        SHIFT_RIGHT_LOGICAL,

        @Description("equal to")
        EQUAL,

        @Description("not equal to")
        NOT_EQUAL,

        @Description("less than")
        LESS_THAN,

        @Description("greater than")
        GREATER_THAN,

        @Description("less than or equal to")
        LESS_THAN_OR_EQUAL,

        @Description("greater than or equal to")
        GREATER_THAN_OR_EQUAL,

        @Description("membership")
        IN,

        @Description("logical and")
        LOGICAL_AND,

        @Description("logical or")
        LOGICAL_OR,

        @Description("logical exclusive or")
        LOGICAL_XOR,

        @Description("conditional and")
        CONDITIONAL_AND,

        @Description("conditional or")
        CONDITIONAL_OR,

        @Description("relational membership")
        RELATION_IN,

        @Description("relational set union")
        RELATION_UNION,

        @Description("relational set intersection")
        RELATION_INTERSECTION,

        @Description("relational set difference")
        RELATION_DIFFERENCE,

        @Description("relational bag union")
        RELATION_UNION_ALL,

        @Description("relational bag intersection")
        RELATION_INTERSECTION_ALL,

        @Description("relational bag difference")
        RELATION_DIFFERENCE_ALL,

        @Description("invalid operator")
        @Default
        INVALID,
    }
}
