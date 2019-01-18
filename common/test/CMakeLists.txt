file(GLOB TEST_SOURCES
    "*.cpp"
    "api/*.cpp"
    "core/*.cpp"
    "util/*.cpp"
)

register_tests(
    TARGET common
    SOURCES ${TEST_SOURCES}
)
