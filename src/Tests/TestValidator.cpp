/**
 * @file TestValidator.cpp
 * @author DrkWithT
 * @brief Implements AST validator test. It must detect NaN values such as Infinity or indeterminate values.
 * @date 2024-09-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include "Syntax/AstNodes.hpp"
#include "Frontend/Parser.hpp"
#include "Backend/AstValidator.hpp"

using MyParser = GeneralDeriver::Frontend::Parser;
using MyParseResult = GeneralDeriver::Frontend::ParseResult;
using MyAstNode = GeneralDeriver::Syntax::IAstNode;
using MyValidator = GeneralDeriver::Backend::AstValidator;

static constexpr const char* test_source_1 = "x - 1";
static constexpr const char* test_source_2 = "x + 0^-1";
static constexpr const char* test_source_3 = "x + 0^0";

int main() {
    MyParser parser;
    MyValidator validator;

    auto ast_1 = parser.parseAll(test_source_1);

    if (!validator.validateAst(ast_1.root)) {
        std::cerr << "Unexpected validation failure for source 1.\n";
        return 1;
    }
    validator.clearState();

    auto ast_2 = parser.parseAll(test_source_2);

    if (validator.validateAst(ast_2.root)) {
        std::cerr << "Unexpected validation for source 2.\n";
        return 1;
    }
    validator.clearState();

    auto ast_3 = parser.parseAll(test_source_3);

    if (validator.validateAst(ast_3.root)) {
        std::cerr << "Unexpected validation for source 3.\n";
        return 1;
    }
}