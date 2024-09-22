add_library(Frontend "")
target_include_directories(Frontend PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(Frontend PRIVATE Token.cpp PRIVATE Lexer.cpp)
