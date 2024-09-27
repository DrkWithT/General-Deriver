add_library(Utils "")

target_include_directories(Utils PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(Utils PRIVATE AstPrinter.cpp)
