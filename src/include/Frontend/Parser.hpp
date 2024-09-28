#ifndef PARSER_HPP
#define PARSER_HPP

#include <initializer_list>
#include <string>
#include <memory>
#include "Frontend/Lexer.hpp"
#include "Syntax/AstNodes.hpp"
#include "Syntax/IAstNode.hpp"

namespace GeneralDeriver::Frontend {
    enum class ParseError : uint8_t {
        none,
        token_err,
        syntax_err,
        general_err,
        max = general_err
    };
    
    [[nodiscard]] std::string formatParseError(ParseError error_code, const Token& token, const std::string& source);

    struct ParseResult {
        std::unique_ptr<Syntax::IAstNode> root;
        bool ok;
    };

    /// TODO: implement parseFactor later on.
    class Parser {
    private:
        Lexer lexer;
        Token current;
        Token previous;

        const Token& peekCurrent() const;
        const Token& peekPrevious() const;
        Token advanceToken();
        void consumeToken(std::initializer_list<TokenType> expected);

        [[nodiscard]] std::unique_ptr<Syntax::IAstNode> parseLiteral();
        [[nodiscard]] std::unique_ptr<Syntax::IAstNode> parseUnary();
        [[nodiscard]] std::unique_ptr<Syntax::IAstNode> parsePower();
        // [[nodiscard]] std::unique_ptr<Syntax::IAstNode> parseFactor();
        [[nodiscard]] std::unique_ptr<Syntax::IAstNode> parseTerm();

    public:
        Parser();

        /// @note Use only for injecting test string to parse.
        Parser(const std::string& source_);

        Parser(const Parser& other) = delete;
        Parser& operator=(const Parser& other) = delete;

        Parser(Parser&& x_other) = delete;
        Parser& operator=(Parser&& x_other) = delete;

        [[nodiscard]] ParseResult parseAll(const std::string& source_arg);
    };
}

#endif