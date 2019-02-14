file(GLOB SOURCES
    "*.cpp"
    "binding/*.cpp"
    "impl/*.cpp"
)

add_library(analyzer OBJECT
    ${SOURCES}
)

target_include_directories(analyzer
    PRIVATE .
    PUBLIC $<TARGET_PROPERTY:analyzer-interface,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(analyzer
    PUBLIC model
)

set_compile_options(analyzer)

# for tests
add_library(analyzer-impl INTERFACE)

target_include_directories(analyzer-impl
    INTERFACE .
)
