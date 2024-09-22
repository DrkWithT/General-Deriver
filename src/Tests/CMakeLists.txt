# unit test for Polynomial
add_executable(TestPolynomial)
target_include_directories(TestPolynomial PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(TestPolynomial PRIVATE TestPolynomial.cpp)
target_link_libraries(TestPolynomial PRIVATE Models)

# unit test for Lexer
add_executable(TestLexer)
target_include_directories(TestLexer PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(TestLexer PRIVATE TestLexer.cpp)
target_link_libraries(TestLexer PRIVATE Frontend)

# setup unit test cmds
add_test(NAME Poly COMMAND "$<TARGET_FILE:TestPolynomial>")
add_test(NAME Lexer COMMAND "$<TARGET_FILE:TestLexer>")
