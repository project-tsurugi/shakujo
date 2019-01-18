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
package com.ashigeru.lang.codegen.spec.sql.statement.ddl;

import java.util.List;

import com.ashigeru.lang.codegen.ir.reflect.Default;
import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.ir.reflect.Opt;
import com.ashigeru.lang.codegen.spec.sql.expression.Expression;
import com.ashigeru.lang.codegen.spec.sql.name.Name;
import com.ashigeru.lang.codegen.spec.sql.name.SimpleName;
import com.ashigeru.lang.codegen.spec.sql.statement.Statement;
import com.ashigeru.lang.codegen.spec.sql.type.Type;

@Description("create table statement")
@Extends(Statement.class)
@SuppressWarnings("javadoc")
public class CreateTableStatement extends MetaClass {

    @Description("table name")
    Property<Name> table = property();

    @Description("columns specification")
    Property<List<Column>> columns = property();

    @Description("create table attributes")
    @Opt
    Property<List<Attribute>> attributes = property();

    @Description("primary key settings")
    @Opt
    Property<List<PrimaryKey>> primaryKeys = property();

    @Description("create table columns")
    public static class Column extends MetaClass {

        @Description("column name")
        Property<SimpleName> name = property();

        @Description("column type")
        Property<Type> type = property();

        @Description("column default value")
        @Opt
        Property<Expression> value = property();

        @Description("column attributes")
        @Opt
        Property<List<Attribute>> attributes = property();

        @Description("column attribute")
        public enum Attribute {

            @Description("column value must be never null")
            NOT_NULL,

            @Description("column is the primary key")
            PRIMARY_KEY,
        }
    }

    @Description("create table attribute")
    public enum Attribute {

        @Description("create table only if it does not exist")
        IF_NOT_EXISTS,
    }

    @Description("primary key setting")
    public static class PrimaryKey extends MetaClass {

        @Description("key column name")
        Property<SimpleName> name = property();

        @Description("column direction")
        @Opt
        Property<Direction> direction = property();

        @Description("primary key direction")
        public enum Direction {

            @Description("don't care")
            @Default
            DONT_CARE,

            @Description("ascendant order")
            ASCENDANT,

            @Description("descendant order")
            DESCENDANT,
        }
    }
}
