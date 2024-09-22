/**
 * @file Token.cpp
 * @author DrkWithT
 * @brief Implements token structure, interface.
 * @date 2024-09-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Frontend/Token.hpp"

namespace GeneralDeriver::Frontend {
    bool operator==(const Token& lhs, const Token& rhs) {
        return lhs.begin == rhs.begin && lhs.length == rhs.length && lhs.tag == rhs.tag;
    }

    std::string_view viewLexeme(const Token& token, const std::string& source) {
        if (token.length == 0 || token.tag == TokenType::eos) {
            return "";
        }

        return {source.data() + token.begin, token.length};
    }

    std::string getLexeme(const Token& token, const std::string& source) {
        if (token.length == 0 || token.tag == TokenType::eos) {
            return "";
        }

        return source.substr(token.begin, token.length);
    }
}