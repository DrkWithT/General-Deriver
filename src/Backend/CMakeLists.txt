add_library(Backend "")

target_include_directories(Backend PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(Backend PRIVATE AstValidator.cpp)
# TODO: add PRIVATE FuncEmitter.cpp
