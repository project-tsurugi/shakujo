file(GLOB TEST_SOURCES
    "*.cpp"
    "binding/*.cpp"
    "scope/*.cpp"
    "analyze/*.cpp"
    "optimize/*.cpp"
)

register_tests(
    TARGET analyzer
    SOURCES ${TEST_SOURCES}
    DEPENDS analyzer-impl glog::glog
)
