file(GLOB TEST_SOURCES
    "*.cpp"
    "core/*.cpp"
    "name/*.cpp"
    "util/*.cpp"
    "type/*.cpp"
)

register_tests(
    TARGET model
    SOURCES ${TEST_SOURCES}
    DEPENDS common
)
