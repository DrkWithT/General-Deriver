/**
 * @file TestPolynomial.cpp
 * @author DrkWithT
 * @brief Implements homemade unit test for Polynomial model.
 * @date 2024-09-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <format>
#include <vector>
#include "Models/Polynomial.hpp"
#include "Models/FunctionAny.hpp"

using MyPoly = GeneralDeriver::Models::Polynomial;
using MyPolyTerm = GeneralDeriver::Models::PolynomialTerm;

static constexpr double eval_input_1 = 3.0; // test x-input value for foo function
static constexpr double expected_output_1 = 16.0; // expected output of foo function
static constexpr double eval_input_2 = 2.0; // test x-input value for deriv(foo)
static constexpr double expected_output_2 = 6.0; // expected output of deriv(foo) function

int main() {
    std::vector<MyPolyTerm> terms = {{1, 2}, {2, 1}, {1, 0}}; // for f(x) = x^2 + 2x + 1

    MyPoly foo {terms};
    double eval_output_1 = foo.evalAt(eval_input_1);

    if (eval_output_1 != expected_output_1) {
        std::cerr << std::format("Invalid output of foo: {} vs. expected {}\n", eval_output_1, expected_output_1) << '\n';
        return 1;
    }

    GeneralDeriver::Models::FunctionAny wrapped_derived_foo = foo.makeDerivative();
    double eval_output_2 = 0.0;

    try {
        MyPoly derived_foo = wrapped_derived_foo.unpackFunctionAny<decltype(derived_foo)>();

        eval_output_2 = derived_foo.evalAt(eval_input_2);
    } catch (const std::runtime_error& access_err) {
        std::cerr << std::format("Unexpected function type for foo derivative, \"{}\"\n", access_err.what());
    }

    if (eval_output_2 != expected_output_2) {
        std::cerr << std::format("Invalid output of foo: {} vs. expected {}\n", eval_output_2, expected_output_2) << '\n';
        return 1;
    }
}