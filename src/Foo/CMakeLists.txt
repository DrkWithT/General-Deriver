add_library(Foo "")

target_include_directories(Foo PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(Foo PRIVATE Foo.cpp)
