add_library(Models "")
target_include_directories(Models PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(Models PRIVATE Polynomial.cpp)
