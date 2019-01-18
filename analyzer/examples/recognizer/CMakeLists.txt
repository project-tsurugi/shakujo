file(GLOB SOURCES
    "*.cpp"
)

add_executable(analyzer-recognizer
    ${SOURCES}
)

target_include_directories(analyzer-recognizer
    PRIVATE .
)

target_link_libraries(analyzer-recognizer
    PUBLIC parser
    PUBLIC analyzer
)

set_compile_options(analyzer-recognizer)

if (INSTALL_EXAMPLES)
    install_custom(analyzer-recognizer ${export_name})
endif()

if(BUILD_EXAMPLES)
    add_test(
        NAME analyzer-recognizer-simple
        COMMAND
            analyzer-recognizer
            "SELECT * FROM example"
    )
    add_test(
        NAME analyzer-recognizer-projection
        COMMAND
            analyzer-recognizer
            "SELECT C1 + 1 as D1, C2 + 2 as C2 FROM example"
    )
    add_test(
        NAME analyzer-recognizer-selection
        COMMAND
            analyzer-recognizer
            "SELECT C3 FROM example WHERE C1 = 1"
    )
    add_test(
        NAME analyzer-recognizer-insert
        COMMAND
            analyzer-recognizer
            "INSERT INTO example VALUES (1, 2.5, \"Hello\")"
    )
    add_test(
        NAME analyzer-recognizer-insert-columns
        COMMAND
            analyzer-recognizer
            "INSERT INTO example (C1) VALUES (9999)"
    )
    add_test(
            NAME analyzer-recognizer-create-table
            COMMAND
            analyzer-recognizer
            "CREATE TABLE testing(C1 INT PRIMARY KEY, C2 STRING DEFAULT \"Hello\" NOT NULL)"
    )
endif()

if (NOT BUILD_EXAMPLES AND NOT INSTALL_EXAMPLES)
    set_target_properties(analyzer-recognizer
        PROPERTIES EXCLUDE_FROM_ALL ON
    )
endif()
