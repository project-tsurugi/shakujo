add_library(parser-interface INTERFACE)

target_include_directories(parser-interface
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/shakujo-parser>
)

install_custom(parser-interface ${export_name})

add_subdirectory(src)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
