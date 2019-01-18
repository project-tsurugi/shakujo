cmake_minimum_required(VERSION 3.5)

project(testing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
file(GLOB_RECURSE SRC "src/*.cpp")
message(STATUS ${SRC})

add_executable(testing ${SRC})
