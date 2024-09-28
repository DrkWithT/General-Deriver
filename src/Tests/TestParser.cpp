/**
 * @file TestParser.cpp
 * @author DrkWithT
 * @brief Implements smoke test for parse before AST print.
 * @date 2024-09-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <format>
#include "Frontend/Parser.hpp"
#include "Syntax/AstNodes.hpp"
#include "Utils/AstPrinter.hpp"

using MyParser = GeneralDeriver::Frontend::Parser;
using MyParseResult = GeneralDeriver::Frontend::ParseResult;
using MyAstNode = GeneralDeriver::Syntax::IAstNode;
using MyAstPrinter = GeneralDeriver::Utils::AstPrinter;

static constexpr const char* test_source_1 = "x^2 -  1";
static constexpr const char* test_source_2 = "42  - x";

int main() {
    MyParser test_parser_1 {};
    auto test_result_1 = test_parser_1.parseAll(test_source_1);

    MyParser test_parser_2 {};
    auto test_result_2 = test_parser_2.parseAll(test_source_2);

    if (!test_result_1.ok) {
        std::cerr << std::format("Parse 1 failed on source \"{}\"\n", test_source_1);
        return 1;
    }

    if (!test_result_2.ok) {
        std::cerr << std::format("Parse 2 failed on source \"{}\"\n", test_source_2);
        return 1;
    }

    MyAstPrinter printer;
    printer.printAST("tree_source_1", test_result_1.root);
    printer.printAST("tree_source_2", test_result_2.root);
}
