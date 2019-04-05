file(GLOB SOURCES
    "*.cpp"
    "core/*.cpp"
    "util/*.cpp"
    "schema/*.cpp"
)

add_library(common
    ${SOURCES}
)

set_target_properties(common
    PROPERTIES
        LIBRARY_OUTPUT_NAME "shakujo-common"
)

target_include_directories(common
    PRIVATE .
)

target_link_libraries(common
    PUBLIC common-interface
)

set_compile_options(common)
install_custom(common ${export_name})
