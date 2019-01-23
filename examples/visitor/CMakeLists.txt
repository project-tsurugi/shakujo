file(GLOB SOURCES
    "*.cpp"
)

add_executable(example-visitor
    ${SOURCES}
)

target_include_directories(example-visitor
    PRIVATE .
)

target_link_libraries(example-visitor
    PUBLIC model
)

set_compile_options(example-visitor)

if (INSTALL_EXAMPLES)
    install_custom(example-visitor ${export_name})
endif()

add_test(
    NAME example-visitor
    COMMAND example-visitor
)
