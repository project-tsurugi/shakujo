add_library(model-interface INTERFACE)

target_include_directories(model-interface
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/shakujo-model>
)

install_custom(model-interface ${export_name})

add_subdirectory(src)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
