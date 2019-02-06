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
package com.ashigeru.lang.codegen.spec.sql.statement.dml;

import java.util.List;

import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.spec.sql.expression.Expression;
import com.ashigeru.lang.codegen.spec.sql.key.RelationKey;
import com.ashigeru.lang.codegen.spec.sql.key.VariableKey;
import com.ashigeru.lang.codegen.spec.sql.name.Name;
import com.ashigeru.lang.codegen.spec.sql.name.SimpleName;
import com.ashigeru.lang.codegen.spec.sql.statement.Statement;

@Description("update table rows statement")
@Extends(Statement.class)
@Extends(RelationKey.Provider.class)
@SuppressWarnings("javadoc")
public class UpdateStatement extends MetaClass {

    @Description("target relation")
    Property<Expression> source = property();

    @Description("table name")
    Property<Name> table = property();

    @Description("initialization statements")
    Property<List<Statement>> initialize = property();

    @Description("destination column specification")
    Property<List<Column>> columns = property();

    @Description("update column specification")
    @Extends(VariableKey.Provider.class)
    public static class Column extends MetaClass {

        @Description("destination column name")
        Property<SimpleName> name = property();

        @Description("column value")
        Property<Expression> value = property();
    }
}
