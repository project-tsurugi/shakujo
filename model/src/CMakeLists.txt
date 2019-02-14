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

add_library(model OBJECT
    ${SOURCES}
)

target_include_directories(model
    PRIVATE .
    PUBLIC $<TARGET_PROPERTY:model-interface,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(model
    PUBLIC common
)

set_compile_options(model)
