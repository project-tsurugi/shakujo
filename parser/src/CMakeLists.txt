if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(UUID REQUIRED uuid)
elseif(APPLE)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
endif()

file(GLOB ANTLR_SOURCES
    "grammar/ShakujoParser.g4"
    "grammar/ShakujoLexer.g4"
)

file(GLOB ANTLR_DEPENDS
    "grammar/*.g4"
)

set(ANTLR_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/antlr")

antlr(
    GENERATED_SOURCES ANTLR_GENERATED_SOURCES
    OUTPUT ${ANTLR_OUTPUT_DIR}
    PACKAGE_NAME shakujo_lang
    SOURCES ${ANTLR_SOURCES}
    DEPENDS ${ANTLR_DEPENDS}
)

file(GLOB SOURCES
    "*.cpp"
    "impl/*.cpp"
)

file(GLOB_RECURSE ANTLR_RUNTIME_SOURCES
    "${ANTLR_RUNTIME_INCLUDE_DIR}/*.cpp"
)

add_library(parser
    ${SOURCES}
    ${ANTLR_GENERATED_SOURCES}
    ${ANTLR_RUNTIME_SOURCES}
)

target_include_directories(parser
    PRIVATE .
    PRIVATE ${ANTLR_OUTPUT_DIR}
    PRIVATE ${ANTLR_RUNTIME_INCLUDE_DIR}
)

target_link_libraries(parser
    PUBLIC parser-interface
    PUBLIC model
)

if (APPLE)
    target_include_directories(parser
        PRIVATE ${COREFOUNDATION_INCLUDE_DIRS})
    target_link_libraries(parser
        PUBLIC ${COREFOUNDATION_LIBRARY})
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_include_directories(parser
        PRIVATE ${UUID_INCLUDE_DIRS})
    target_link_libraries(parser
        PUBLIC ${UUID_LIBRARIES})
endif()

set_compile_options(parser)
install_custom(parser ${export_name})
antlr_fix_compile_option(parser)
