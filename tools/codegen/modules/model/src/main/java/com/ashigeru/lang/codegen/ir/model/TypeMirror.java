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
 * An abstract super interface of type mirrors in IR models.
 * @since WIP
 */
public interface TypeMirror extends Model {

    /**
     * Returns the kind of type.
     * @return the kind of type
     */
    Kind getKind();

    /**
     * Returns a list type of this.
     * @return a list type
     */
    default ListTypeMirror asListType() {
        return new ListTypeMirror(this);
    }

    /**
     * Represents a kind of type.
     * @since WIP
     */
    enum Kind {

        /**
         * The bool type.
         */
        BOOL_TYPE,

        /**
         * The integral number type.
         */
        INT_TYPE,

        /**
         * The floating point number type.
         */
        FLOAT_TYPE,

        /**
         * The size type.
         */
        SIZE_TYPE,

        /**
         * The string type.
         */
        STRING_TYPE,

        /**
         * The declared type.
         */
        DECLARED_TYPE,

        /**
         * The list type.
         */
        LIST_TYPE,
    }
}
