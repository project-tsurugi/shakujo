/*
 * Copyright 2018-2019 shakujo project.
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
// DON'T EDIT THIS GENERATED FILE //
#include "shakujo/model/program/GlobalDeclarationKind.h"

#include <iostream>


namespace shakujo::model::program {
std::ostream& operator<<(std::ostream& out, GlobalDeclarationKind value) {
    switch (value) {
    case GlobalDeclarationKind::GLOBAL_FUNCTION_DECLARATION:
        out << "GLOBAL_FUNCTION_DECLARATION";
        break;
    case GlobalDeclarationKind::GLOBAL_VARIABLE_DECLARATION:
        out << "GLOBAL_VARIABLE_DECLARATION";
        break;
    }
    return out;
}

}  // namespace shakujo::model::program

