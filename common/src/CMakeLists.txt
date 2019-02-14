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
    PUBLIC $<TARGET_PROPERTY:common-interface,INTERFACE_INCLUDE_DIRECTORIES>
)

set_compile_options(common)
