add_executable(TestPolynomial)
target_include_directories(TestPolynomial PUBLIC "${SOURCE_HEADER_DIR}")
target_sources(TestPolynomial PRIVATE TestPolynomial.cpp)
target_link_libraries(TestPolynomial PRIVATE Models)

add_test(NAME Poly COMMAND "$<TARGET_FILE:TestPolynomial>")
