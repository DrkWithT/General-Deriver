#pragma once

#include "Frontend/Token.hpp"
#include <string>

namespace GeneralDeriver::Frontend {
    [[nodiscard]] bool isSpacing(char s);

    [[nodiscard]] bool isNumeric(char s);

    class Lexer {
    private:
        std::string source;
        std::size_t pos;
        std::size_t limit;

        /// @note This should handle simple operators, "x", etc.
        [[nodiscard]] bool isAtEOS() const;
        Token lexSingle(TokenType type);
        Token lexSpacing();
        Token lexNumber();

    public:
        Lexer();
        Lexer(const std::string& source_);

        Lexer(const Lexer& other) = delete;
        Lexer& operator=(const Lexer& other) = delete;

        /// @note See Lexer& operator=(Lexer&& x_other) for details.
        Lexer(Lexer&& x_other) noexcept;

        /// @note for cheaper construction from another temporary lexer... will pilfer entire source string instead of a whole copy.
        Lexer& operator=(Lexer&& x_other) noexcept;

        const std::string& getSource() const;

        [[nodiscard]] Token lexNext();
    };
}