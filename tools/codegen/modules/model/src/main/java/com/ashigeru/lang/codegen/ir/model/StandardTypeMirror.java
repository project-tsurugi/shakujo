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
package com.ashigeru.lang.codegen.ir.model;

/**
 * Represents standard types.
 * @since WIP
 */
public enum StandardTypeMirror implements TypeMirror {

    /**
     * The integer type.
     */
    BOOL(Kind.BOOL_TYPE),

    /**
     * The integer type.
     */
    INT(Kind.INT_TYPE),

    /**
     * The float type.
     */
    FLOAT(Kind.FLOAT_TYPE),

    /**
     * The size type.
     */
    SIZE(Kind.SIZE_TYPE),

    /**
     * The string type.
     */
    STRING(Kind.STRING_TYPE),

    ;
    private Kind kind;

    StandardTypeMirror(Kind kind) {
        this.kind = kind;
    }

    @Override
    public Kind getKind() {
        return kind;
    }
}
