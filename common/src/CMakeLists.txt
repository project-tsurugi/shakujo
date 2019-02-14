file(GLOB SOURCES
    "*.cpp"
    "api/*.cpp"
    "core/*.cpp"
    "util/*.cpp"
    "schema/*.cpp"
)

add_library(common OBJECT
    ${SOURCES}
)

target_include_directories(common
    PRIVATE .
)

target_link_libraries(common
    PUBLIC common-interface
)

set_compile_options(common)
