#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string_view>
#include <string>

namespace GeneralDeriver::Frontend {
    /// TODO: enable times and slash operator support later.
    enum class TokenType : uint8_t {
        eos,
        spacing,
        number,
        variable,
        op_plus,
        op_minus,
        // op_times,
        // op_slash,
        op_power,
        l_paren,
        r_paren,
        unknown
    };

    struct Token {
        std::size_t begin;
        std::size_t length;
        TokenType tag;

        friend bool operator==(const Token& lhs, const Token& rhs);
    };

    std::string_view viewLexeme(const Token& token, const std::string& source);

    std::string getLexeme(const Token& token, const std::string& source);
}

#endif