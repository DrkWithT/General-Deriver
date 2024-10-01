/**
 * @file Lexer.cpp
 * @author DrkWithT
 * @brief Implements x-expression lexer.
 * @date 2024-09-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include "Frontend/Lexer.hpp"
#include "Frontend/Token.hpp"

namespace GeneralDeriver::Frontend {
    bool isSpacing(char s) {
        return s == ' ' || s == '\t' || s == '\r' || s == '\n';
    }

    bool isNumeric(char s) {
        return (s >= '0' && s <= '9') || s == '.';
    }

    bool Lexer::isAtEOS() const {
        return pos >= limit;
    }

    Token Lexer::lexSingle(TokenType type) {
        std::size_t tbegin = pos++;

        return {tbegin, 1, type};
    }

    Token Lexer::lexSpacing() {
        std::size_t tbegin = pos;
        std::size_t tlen = 0;

        while (pos < limit) {
            char temp = source[pos];

            if (!isSpacing(temp)) {
                break;
            }

            tlen++;
            pos++;
        }

        return {tbegin, tlen, TokenType::spacing};
    }

    Token Lexer::lexNumber() {
        std::size_t tbegin = pos;
        std::size_t tlen = 0;
        int dot_count = 0;

        while (pos < limit) {
            char temp = source[pos];

            if (!isNumeric(temp)) {
                break;
            }

            if (temp == '.') {
                dot_count++;
            }

            tlen++;
            pos++;
        }

        if (dot_count < 2) {
            return {tbegin, tlen, TokenType::number};
        } else {
            return {tbegin, tlen, TokenType::unknown};
        }
    }

    Lexer::Lexer()
    : source {""}, pos {0}, limit {0} {}

    Lexer::Lexer(const std::string& source_)
    : source {source_}, pos {0}, limit {source_.size()} {}

    Lexer::Lexer(Lexer&& x_other) noexcept {
        source = std::move(x_other.source);

        std::size_t temp_pos = 0;
        std::size_t temp_limit = 0;
        std::swap(temp_pos, x_other.pos);
        std::swap(temp_limit, x_other.limit);

        pos = temp_pos;
        limit = temp_limit;
    }

    Lexer& Lexer::operator=(Lexer&& x_other) noexcept {
        source = std::move(x_other.source);

        std::size_t temp_pos = 0;
        std::size_t temp_limit = 0;
        std::swap(temp_pos, x_other.pos);
        std::swap(temp_limit, x_other.limit);

        pos = temp_pos;
        limit = temp_limit;

        return *this;
    }

    const std::string& Lexer::getSource() const {
        return source;
    }

    Token Lexer::lexNext() {
        if (isAtEOS()) {
            return {pos, 1, TokenType::eos};
        }

        char temp = source[pos];

        switch(temp) {
            case '(':
                return lexSingle(TokenType::l_paren);
            case ')':
                return lexSingle(TokenType::r_paren);
            case 'x':
                return lexSingle(TokenType::variable);
            case '+':
                return lexSingle(TokenType::op_plus);
            case '-':
                return lexSingle(TokenType::op_minus);
            // case '*':
            // case '/':
            case '^':
                return lexSingle(TokenType::op_power);
            default:
                break;
        }

        if (isSpacing(temp)) {
            return lexSpacing();
        } else if (isNumeric(temp)) {
            return lexNumber();
        } else {
            return {pos++, 1, TokenType::unknown};
        }
    }
}