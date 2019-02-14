file(GLOB TEST_SOURCES
    "*.cpp"
)

register_tests(
    TARGET parser
    SOURCES ${TEST_SOURCES}
    DEPENDS common model glog::glog
)
