file(GLOB SOURCES
    "*.cpp"
)

add_executable(model-visitor
    ${SOURCES}
)

target_include_directories(model-visitor
    PRIVATE .
)

target_link_libraries(model-visitor
    PUBLIC model
)

set_compile_options(model-visitor)

if (INSTALL_EXAMPLES)
    install_custom(model-visitor ${export_name})
endif()

if(BUILD_EXAMPLES)
    add_test(
        NAME model-visitor
        COMMAND model-visitor
    )
endif()

if (NOT BUILD_EXAMPLES AND NOT INSTALL_EXAMPLES)
    set_target_properties(model-visitor
        PROPERTIES EXCLUDE_FROM_ALL ON
    )
endif()
