function(antlr)
    include(CMakeParseArguments)
    cmake_parse_arguments(
        ANTLR_GEN # prefix
        ""
        "GENERATED_SOURCES;OUTPUT;PACKAGE_NAME"
        "SOURCES;DEPENDS"
        ${ARGN}
    )
    if(NOT ANTLR_GEN_GENERATED_SOURCES)
        message(FATAL_ERROR "GENERATED_SOURCES must be set")
    endif()
    if(NOT ANTLR_GEN_OUTPUT)
        message(FATAL_ERROR "OUTPUT must be set")
    endif()
    if(NOT ANTLR_GEN_PACKAGE_NAME)
        message(FATAL_ERROR "PACKAGE_NAME must be set")
    endif()
    if(NOT ANTLR_GEN_SOURCES)
        message(FATAL_ERROR "SOURCES must be set")
    endif()
    if(NOT ANTLR_GEN_DEPENDS)
        list(APPEND DEPENDS ${SOURCES})
    endif()

    set(ANTLR_TOOL_JAR "${shakujo_SOURCE_DIR}/third_party/antlr-4.7.2-complete.jar")
    if (NOT EXISTS ${ANTLR_TOOL_JAR})
        message(FATAL_ERROR "ANTLR tool not found: ${ANTLR_TOOL_JAR}")
    endif()

    # compute output target
    execute_process(
        COMMAND
            ${Java_JAVA_EXECUTABLE}
            -jar
            "${ANTLR_TOOL_JAR}"
            -depend
            -o ${ANTLR_GEN_OUTPUT}
            -encoding UTF-8
            -package ${ANTLR_GEN_PACKAGE_NAME}
            -Dlanguage=Cpp
            -Werror
            ${ANTLR_GEN_SOURCES}
        OUTPUT_VARIABLE ANTLR4_OUTPUT
    )

    # collect output files
    string(REGEX REPLACE "\r?\n" ";" ANTLR4_OUTPUT_LIST "${ANTLR4_OUTPUT}")
    foreach (line IN LISTS ANTLR4_OUTPUT_LIST)
        string(REGEX REPLACE " : .*$" "" line_path "${line}")
        get_filename_component(line_ext "${line_path}" EXT)
        if (line_ext STREQUAL ".cpp"
                # FIXME: lexers will not generate any extra sources
                AND NOT line_path MATCHES "Lexer.+\\.cpp$")
            list(APPEND generated ${line_path})
        endif()
    endforeach()
    set(${ANTLR_GEN_GENERATED_SOURCES} ${generated} PARENT_SCOPE)

    # adds custom command to generate parser
    add_custom_command(
        OUTPUT ${generated}
        DEPENDS ${ANTLR_GEN_DEPENDS}
        COMMAND
            ${Java_JAVA_EXECUTABLE}
            -jar
            "${ANTLR_TOOL_JAR}"
            -o ${ANTLR_GEN_OUTPUT}
            -encoding UTF-8
            -package ${ANTLR_GEN_PACKAGE_NAME}
            -Dlanguage=Cpp
            -Werror
            ${ANTLR_GEN_SOURCES}
    )
endfunction(antlr)

set(ANTLR_RUNTIME_INCLUDE_DIR "${shakujo_SOURCE_DIR}/third_party/antlr4/runtime/Cpp/runtime/src")
set(ANTLR_RUNTIME_SOURCE_DIR "${shakujo_SOURCE_DIR}/third_party/antlr4/runtime/Cpp/runtime/src")

function(antlr_fix_compile_option target_name)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target_name}
            PRIVATE
                -Wno-attributes
                -Wno-unused-parameter
                -Wno-reorder
                -Wno-unused-but-set-variable
                -Wno-implicit-fallthrough)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
            OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        target_compile_options(${target_name}
            PRIVATE
                -Wno-attributes
                -Wno-unused-parameter
                -Wno-reorder)
    endif()
endfunction(antlr_fix_compile_option)
