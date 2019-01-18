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
package com.ashigeru.lang.codegen.spec.sql.key;

import com.ashigeru.lang.codegen.ir.reflect.Description;
import com.ashigeru.lang.codegen.ir.reflect.Extern;
import com.ashigeru.lang.codegen.ir.reflect.Foreign;
import com.ashigeru.lang.codegen.ir.reflect.Interface;
import com.ashigeru.lang.codegen.ir.reflect.MetaClass;
import com.ashigeru.lang.codegen.ir.reflect.MoveOnly;
import com.ashigeru.lang.codegen.ir.reflect.Transient;
import com.ashigeru.lang.codegen.ir.reflect.Value;

@Description("variable key")
@Interface
@Value
@Extern
@MoveOnly
@SuppressWarnings("javadoc")
public class VariableKey extends MetaClass {

    @Description("mix-in of variable key provider")
    @Foreign
    @Interface
    public static class Provider extends MetaClass {

        @Description("referring variable key")
        @Transient
        Property<VariableKey> variableKey = property();
    }
}
