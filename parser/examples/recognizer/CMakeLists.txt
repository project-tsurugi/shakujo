file(GLOB SOURCES
    "*.cpp"
)

add_executable(parser-recognizer
    ${SOURCES}
)

target_include_directories(parser-recognizer
    PRIVATE .
)

target_link_libraries(parser-recognizer
    PUBLIC parser
)

set_compile_options(parser-recognizer)

if (INSTALL_EXAMPLES)
    install_custom(parser-recognizer ${export_name})
endif()

if(BUILD_EXAMPLES)
    add_test(
        NAME parser-recognizer-simple
        COMMAND
            parser-recognizer
            "SELECT * FROM EXAMPLE"
    )
    add_test(
        NAME parser-recognizer-projection
        COMMAND
            parser-recognizer
            "SELECT C1, C2 + 1 as C3 FROM EXAMPLE"
    )
    add_test(
        NAME parser-recognizer-selection
        COMMAND
            parser-recognizer
            "SELECT C3 FROM EXAMPLE WHERE C1 = 1"
    )
    add_test(
        NAME parser-recognizer-join
        COMMAND
            parser-recognizer
            "SELECT T1.ID, T1.NAME, T2.VAULE FROM T1 JOIN T2 ON T1.ID = T2.ID"
    )
    add_test(
        NAME parser-recognizer-insert
        COMMAND
            parser-recognizer
            "INSERT INTO EXAMPLE (C1, C2) VALUES (1, 2)"
    )
    add_test(
            NAME parser-recognizer-create-table
            COMMAND
            parser-recognizer
            "CREATE TABLE EXAMPLE (C1 INT(64) PRIMARY KEY, C2 CHAR(100) NULL)"
    )
endif()

if (NOT BUILD_EXAMPLES AND NOT INSTALL_EXAMPLES)
    set_target_properties(parser-recognizer
        PROPERTIES EXCLUDE_FROM_ALL ON
    )
endif()
