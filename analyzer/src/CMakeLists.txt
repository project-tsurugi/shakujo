file(GLOB SOURCES
    "*.cpp"
    "binding/*.cpp"
    "impl/*.cpp"
)

add_library(analyzer STATIC
    ${SOURCES}
)

target_include_directories(analyzer
    PRIVATE .
)

target_link_libraries(analyzer
    PUBLIC analyzer-interface
    PUBLIC model
)

set_compile_options(analyzer)
install_custom(analyzer ${export_name})


# for tests
add_library(analyzer-impl INTERFACE)

target_include_directories(analyzer-impl
    INTERFACE .
)
