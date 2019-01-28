file(GLOB SOURCES
    "*.cpp"
)

add_executable(example-analyzer
    ${SOURCES}
)

target_include_directories(example-analyzer
    PRIVATE .
)

target_link_libraries(example-analyzer
    PUBLIC parser
    PUBLIC analyzer
)

set_compile_options(example-analyzer)

if (INSTALL_EXAMPLES)
    install_custom(example-analyzer ${export_name})
endif()

add_test(
    NAME example-analyzer-simple
    COMMAND
        example-analyzer
        "SELECT * FROM example"
)
add_test(
    NAME example-analyzer-projection
    COMMAND
        example-analyzer
        "SELECT C1 + 1 as D1, C2 + 2 as C2 FROM example"
)
add_test(
    NAME example-analyzer-selection
    COMMAND
        example-analyzer
        "SELECT C3 FROM example WHERE C1 = 1"
)
add_test(
    NAME example-analyzer-insert
    COMMAND
        example-analyzer
        "INSERT INTO example VALUES (1, 2.5, 'Hello')"
)
add_test(
    NAME example-analyzer-insert-columns
    COMMAND
        example-analyzer
        "INSERT INTO example (C1) VALUES (9999)"
)
add_test(
        NAME example-analyzer-create-table
        COMMAND
        example-analyzer
        "CREATE TABLE testing(C1 INT PRIMARY KEY, C2 STRING DEFAULT 'Hello' NOT NULL)"
)
