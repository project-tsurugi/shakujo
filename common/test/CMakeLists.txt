file(GLOB TEST_SOURCES
    "*.cpp"
    "core/*.cpp"
    "schema/*.cpp"
    "util/*.cpp"
)

register_tests(
    TARGET common
    SOURCES ${TEST_SOURCES}
)
