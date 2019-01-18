add_library(common-interface INTERFACE)

target_include_directories(common-interface
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/shakujo-common>
)

install_custom(common-interface ${export_name})

add_subdirectory(src)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
