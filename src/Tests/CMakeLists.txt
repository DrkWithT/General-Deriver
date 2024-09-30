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

# smoke test for Parser & AstPrinter (no crashing)
add_executable(TestParser)
target_include_directories(TestParser PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(TestParser PRIVATE TestParser.cpp)
target_link_libraries(TestParser PRIVATE Frontend PRIVATE Syntax PRIVATE Utils)

# test for AST validator
add_executable(TestValidator)
target_include_directories(TestValidator PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(TestValidator PRIVATE TestValidator.cpp)
target_link_libraries(TestValidator PRIVATE Frontend PRIVATE Syntax PRIVATE Backend)

# setup test cmds
add_test(NAME Poly COMMAND "$<TARGET_FILE:TestPolynomial>")
add_test(NAME Lexer COMMAND "$<TARGET_FILE:TestLexer>")
add_test(NAME Parser COMMAND "$<TARGET_FILE:TestParser>")
add_test(NAME Validator COMMAND "$<TARGET_FILE:TestValidator>")
