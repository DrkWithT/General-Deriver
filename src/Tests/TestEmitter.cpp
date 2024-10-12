/**
 * @file TestEmitter.cpp
 * @author DrkWithT
 * @brief Implements FuncEmitter tests.
 * @date 2024-10-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <format>
#include "Models/Composite.hpp"
#include "Frontend/Parser.hpp"
#include "Backend/FuncEmitter.hpp"

using MyCompFunc = GeneralDeriver::Models::Composite;
using MyParser = GeneralDeriver::Frontend::Parser;
using MyParseResult = GeneralDeriver::Frontend::ParseResult;
using MyFuncEmitter = GeneralDeriver::Backend::FunctionEmitter;

static constexpr const char* test_source_1 = "x^2 - 1";
static constexpr double test_x_1 = 2;
static constexpr double test_output_1 = 3;
static constexpr double test_dx_output_1 = 4;

int main() {
    MyParser parser;
    MyParseResult parse_result = parser.parseAll(test_source_1);

    if (!parse_result.ok) {
        std::cerr << std::format("Unexpected parse failure for source \"{}\"\n", test_source_1);
        return 1;
    }

    MyFuncEmitter emitter;
    MyCompFunc poly_1 = emitter.emitFunction(parse_result.root);
    double y_1 = poly_1.evalAt(test_x_1);

    if (y_1 != test_output_1) {
        std::cerr << std::format("Unexpected output of f(x) = {}: {}\n", test_source_1, y_1);
        return 1;
    }
    
    MyCompFunc dx_poly_1 = poly_1.makeDerivative().unpackFunctionAny<MyCompFunc>();
    double dx_y_1 = dx_poly_1.evalAt(test_x_1);

    if (dx_y_1 != test_dx_output_1) {
        std::cerr << std::format("Unexpected output of d/dx({}): {}\n", test_source_1, dx_y_1);
        return 1;
    }
}