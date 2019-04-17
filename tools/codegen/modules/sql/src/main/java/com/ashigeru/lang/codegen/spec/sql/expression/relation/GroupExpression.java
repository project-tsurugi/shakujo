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

import java.util.List;

import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.spec.sql.expression.Expression;
import com.ashigeru.lang.codegen.spec.sql.key.RelationKey;

@Description("organize row groups from relation")
@Extends(Expression.class)
@Extends(RelationKey.Provider.class)
@SuppressWarnings("javadoc")
public class GroupExpression extends MetaClass {

    @Description("source relation")
    Property<Expression> operand = property();

    @Description("group keys")
    Property<List<Expression>> keys = property();
}