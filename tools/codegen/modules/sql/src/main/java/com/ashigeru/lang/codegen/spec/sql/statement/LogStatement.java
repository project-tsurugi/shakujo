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
package com.ashigeru.lang.codegen.spec.sql.statement;

import com.ashigeru.lang.codegen.ir.reflect.Default;
import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extends;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.spec.sql.expression.Expression;

@Description("logging statement")
@Extends(Statement.class)
@SuppressWarnings("javadoc")
public class LogStatement extends MetaClass {

    @Description("log level")
    Property<Level> level = property();

    @Description("log message")
    Property<Expression> value = property();

    @Description("log level")
    public enum Level {

        @Description("trace level")
        TRACE,

        @Description("debug level")
        DEBUG,

        @Description("information level")
        @Default
        INFO,

        @Description("message level")
        MESSAGE,

        @Description("warning level")
        WARNING,

        @Description("error level")
        ERROR,

        @Description("critical level")
        CRITICAL,
    }
}
