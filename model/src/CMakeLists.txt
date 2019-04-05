file(GLOB SOURCES
    "*.cpp"
    "name/*.cpp"
    "type/*.cpp"
    "value/*.cpp"
    "expression/*.cpp"
    "expression/relation/*.cpp"
    "statement/*.cpp"
    "statement/ddl/*.cpp"
    "statement/dml/*.cpp"
    "statement/transaction/*.cpp"
    "program/*.cpp"
    "util/*.cpp"
)

add_library(model
    ${SOURCES}
)

set_target_properties(model
    PROPERTIES
        LIBRARY_OUTPUT_NAME "shakujo-model"
)

target_include_directories(model
    PRIVATE .
)

target_link_libraries(model
    PUBLIC model-interface
    PUBLIC common
)

set_compile_options(model)
install_custom(model ${export_name})
