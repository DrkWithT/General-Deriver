/**
 * @file TestLexer.cpp
 * @author DrkWithT
 * @brief Implements lexer unit test.
 * @date 2024-09-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include <string>
#include <format>
#include <vector>
#include "Frontend/Token.hpp"
#include "Frontend/Lexer.hpp"

using MyToken = GeneralDeriver::Frontend::Token;
using MyTokenTag = GeneralDeriver::Frontend::TokenType;
using MyLexer = GeneralDeriver::Frontend::Lexer;

static constexpr const char* test_source_1 = "x^2 -  1";
static constexpr const char* test_source_2 = "a + 1";

[[nodiscard]] bool doLexTest(MyLexer& lexer, const std::vector<MyToken>& expected) {
    for (const auto& temp : expected) {
        auto target = lexer.lexNext();

        if (target == temp) {
            continue;
        }

        return false;
    }

    return true;
}

int main() {
    std::vector<MyToken> expected_tokens_1 = {
        MyToken {0, 1, MyTokenTag::variable},
        {1, 1, MyTokenTag::op_power},
        {2, 1, MyTokenTag::number},
        {3, 1, MyTokenTag::spacing},
        {4, 1, MyTokenTag::op_minus},
        {5, 2, MyTokenTag::spacing},
        {7, 1, MyTokenTag::number},
        {8, 1, MyTokenTag::eos}
    };

    std::vector<MyToken> expected_tokens_2 = {
        MyToken {0, 1, MyTokenTag::unknown},
        {1, 1, MyTokenTag::spacing},
        {2, 1, MyTokenTag::op_plus},
        {3, 1, MyTokenTag::spacing},
        {4, 1, MyTokenTag::number},
        {5, 1, MyTokenTag::eos}
    };

    MyLexer foo {test_source_1};

    if (!doLexTest(foo, expected_tokens_1)) {
        std::cerr << std::format("Unexpected token sequence for \"{}\"\n", test_source_1);
        return 1;
    }

    foo = MyLexer(test_source_2);

    if (!doLexTest(foo, expected_tokens_2)) {
        std::cerr << std::format("Unexpected token sequence for \"{}\"\n", test_source_2);
        return 1;
    }
}