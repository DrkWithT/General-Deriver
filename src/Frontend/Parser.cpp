/**
 * @file Parser.cpp
 * @author DrkWithT
 * @brief Implements recursive descent parser for x-exprs.
 * @date 2024-09-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include <array>
#include <string_view>
#include <string>
#include <format>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Frontend/Parser.hpp"
#include "Syntax/AstNodes.hpp"

namespace GeneralDeriver::Frontend {
    /* Local constants */

    static constexpr auto parse_errcode_count = static_cast<std::size_t>(ParseError::max) + 1;
    static constexpr std::array<std::string_view, parse_errcode_count> parse_err_names = {
        "No Error",
        "Unexpected Token",
        "Syntax Issue",
        "Unknown Issue"
    };

    /* Helper Functions */

    std::string formatParseError(ParseError error_code, const Token& token, const std::string& source) {
        int error_index = static_cast<int>(error_code);

        return std::format("{} at pos. {}, token \"{}\" \n", parse_err_names[error_index], token.begin, viewLexeme(token, source));
    }

    /* Parser impl. */

    const Token& Parser::peekCurrent() const { return current; }

    const Token& Parser::peekPrevious() const { return previous; }

    Token Parser::advanceToken() {
        Token temp;

        do {
            temp = lexer.lexNext();

            if (temp.tag == TokenType::spacing)
                continue;

            break;
        } while (true);

        return temp;
    }

    void Parser::consumeToken(std::initializer_list<TokenType> expected) {
        if (expected.size() == 0) {
            previous = current;
            current = advanceToken();
            return;
        }

        auto current_tag = peekCurrent().tag;

        if (current_tag == TokenType::eos) {
            return;
        }

        if (std::find(expected.begin(), expected.end(), current_tag) != expected.end()) {
            previous = current;
            current = advanceToken();
            return;
        }

        throw std::runtime_error {
            formatParseError(ParseError::token_err, peekCurrent(), lexer.getSource())
        };
    }

    std::unique_ptr<Syntax::IAstNode> Parser::parseLiteral() {
        auto peeked_tag = peekCurrent().tag;

        if (peeked_tag == TokenType::number) {
            double n = std::stod(getLexeme(peekCurrent(), lexer.getSource()));
            consumeToken({});

            return std::make_unique<Syntax::Constant>(n);
        } else if (peeked_tag == TokenType::variable) {
            consumeToken({});

            return std::make_unique<Syntax::VarStub>();
        } else if (peeked_tag == TokenType::l_paren) {
            consumeToken({});
            auto temp = parseTerm();
            consumeToken({TokenType::r_paren});

            return temp;
        } else if (peeked_tag == TokenType::op_minus) {
            return parseUnary();
        }

        throw std::runtime_error {
            formatParseError(ParseError::syntax_err, peekCurrent(), lexer.getSource())
        };
    }

    std::unique_ptr<Syntax::IAstNode> Parser::parseUnary() {
        auto peeked_tag = peekCurrent().tag;

        if (peeked_tag == TokenType::op_minus) {
            consumeToken({});
            return std::make_unique<Syntax::Unary>(Syntax::AstOpType::neg, parseLiteral());
        }

        return parseLiteral();
    }

    std::unique_ptr<Syntax::IAstNode> Parser::parsePower() {
        auto power_target = parseUnary();
        
        if (peekCurrent().tag == TokenType::op_power) {
            consumeToken({});
            auto exponent = parseLiteral();

            return std::make_unique<Syntax::Binary>(Syntax::AstOpType::power, std::move(power_target), std::move(exponent));
        }

        return power_target;
    }

    std::unique_ptr<Syntax::IAstNode> Parser::parseTerm() {
        auto lhs = parsePower();

        do {
            auto current_tag = peekCurrent().tag;

            if (current_tag != TokenType::op_plus && current_tag != TokenType::op_minus) {
                break;
            }

            auto op = (current_tag == TokenType::op_plus)
                ? Syntax::AstOpType::add
                : Syntax::AstOpType::sub;

            consumeToken({});

            auto rhs = parsePower();

            lhs = std::make_unique<Syntax::Binary>(op, std::move(lhs), std::move(rhs));

            if (current_tag = peekCurrent().tag; current_tag == TokenType::eos) {
                break;
            }
        } while (true);

        return lhs;
    }

    Parser::Parser()
    : lexer {}, current {0, 1, TokenType::unknown}, previous {0, 1, TokenType::unknown} {}

    Parser::Parser(const std::string& source_)
    : lexer {source_}, current {0, 1, TokenType::unknown}, previous {0, 1, TokenType::unknown} {}

    ParseResult Parser::parseAll(const std::string& source_arg) {
        lexer = Lexer(source_arg);
        consumeToken({});

        try {
            return {parseTerm(), true};
        } catch (const std::runtime_error& parse_err) {
            std::cout << parse_err.what();
        }

        return {nullptr, false};
    }
}
