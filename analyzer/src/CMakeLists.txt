file(GLOB SOURCES
    "*.cpp"
    "binding/*.cpp"
    "analyze/*.cpp"
    "optimize/*.cpp"
)

add_library(analyzer
    ${SOURCES}
)

set_target_properties(analyzer
    PROPERTIES
        LIBRARY_OUTPUT_NAME "shakujo-analyzer"
)

target_include_directories(analyzer
    PRIVATE .
)

target_link_libraries(analyzer
    PUBLIC analyzer-interface
    PUBLIC model
    PRIVATE glog::glog
)

set_compile_options(analyzer)
install_custom(analyzer ${export_name})

# for tests
add_library(analyzer-impl INTERFACE)

target_include_directories(analyzer-impl
    INTERFACE .
)
