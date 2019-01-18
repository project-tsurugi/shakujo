add_library(analyzer-interface INTERFACE)

target_include_directories(analyzer-interface
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/shakujo-analyzer>
)

install_custom(analyzer-interface ${export_name})

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(examples)
