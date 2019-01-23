file(GLOB SOURCES
    "*.cpp"
)

add_executable(example-parser
    ${SOURCES}
)

target_include_directories(example-parser
    PRIVATE .
)

target_link_libraries(example-parser
    PUBLIC parser
)

set_compile_options(example-parser)

if (INSTALL_EXAMPLES)
    install_custom(example-parser ${export_name})
endif()

add_test(
    NAME example-parser-simple
    COMMAND
        example-parser
        "SELECT * FROM EXAMPLE"
)
add_test(
    NAME example-parser-projection
    COMMAND
        example-parser
        "SELECT C1, C2 + 1 as C3 FROM EXAMPLE"
)
add_test(
    NAME example-parser-selection
    COMMAND
        example-parser
        "SELECT C3 FROM EXAMPLE WHERE C1 = 1"
)
add_test(
    NAME example-parser-join
    COMMAND
        example-parser
        "SELECT T1.ID, T1.NAME, T2.VAULE FROM T1 JOIN T2 ON T1.ID = T2.ID"
)
add_test(
    NAME example-parser-insert
    COMMAND
        example-parser
        "INSERT INTO EXAMPLE (C1, C2) VALUES (1, 2)"
)
add_test(
        NAME example-parser-create-table
        COMMAND
        example-parser
        "CREATE TABLE EXAMPLE (C1 INT(64) PRIMARY KEY, C2 CHAR(100) NULL)"
)
