# Copyright 2018-2024 Project Tsurugi.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if (NOT Doxygen_FOUND)
    message(FATAL_ERROR "doxygen was not installed, please configure with -DBUILD_DOCUMENTS=OFF to skip doxygen")
endif()

set(TARGET_INCLUDES "")
foreach (target_name
        common-interface
        model-interface
        parser-interface
        analyzer-interface)
    get_target_property(
        _includes
        ${target_name} INTERFACE_INCLUDE_DIRECTORIES
    )
    if (_includes)
        foreach (item IN LISTS _includes)
            # if INTERFACE_INCLUDE_DIRECTORIES contains a generator expression, we only pick $<BUILD_INTERFACE:...>
            if (item MATCHES "^\\$<([A-Z_]+):(.+)>$")
                if (CMAKE_MATCH_1 STREQUAL "BUILD_INTERFACE")
                    set(TARGET_INCLUDES "${TARGET_INCLUDES} ${CMAKE_MATCH_2}")
                endif()
            else()
                set(TARGET_INCLUDES "${TARGET_INCLUDES} ${item}")
            endif()
        endforeach()
    endif()
endforeach()

set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

add_custom_target(doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)
