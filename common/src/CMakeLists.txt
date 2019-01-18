file(GLOB SOURCES
    "*.cpp"
    "api/*.cpp"
    "core/*.cpp"
    "util/*.cpp"
    "schema/*.cpp"
)

add_library(common STATIC
    ${SOURCES}
)

target_include_directories(common
    PRIVATE .
)

target_link_libraries(common
    PUBLIC common-interface
)

set_compile_options(common)
install_custom(common ${export_name})
