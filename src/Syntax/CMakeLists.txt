add_library(Syntax "")

target_include_directories(Syntax PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(Syntax PRIVATE AstNodes.cpp)
