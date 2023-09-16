cmake_minimum_required(VERSION 3.5)

project(shakujo-third_party)

## GoogleTest
if(BUILD_TESTS)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif()
