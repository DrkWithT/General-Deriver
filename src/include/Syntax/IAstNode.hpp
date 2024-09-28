#ifndef I_AST_NODE_HPP
#define I_AST_NODE_HPP

#include <any>
#include "Syntax/IAstVisitor.hpp"

namespace GeneralDeriver::Syntax {
    /// @note Represents only supported operations binding sub-expressions by x.
    enum class AstOpType : uint8_t {
        neg,
        add,
        sub,
        mul,
        div,
        power,
        none
    };

    /// @note Denotes only types of expressions e.g constants.
    enum class AstNodeType : uint8_t {
        literal,
        unary,
        binary,
        unknown
    };

    /**
     * @brief Represents a syntactic construct of a parsed function body of x.
     * @note Assumes that 
     */
    class IAstNode {
    public:
        virtual ~IAstNode() = default;

        virtual AstNodeType getType() const = 0;
        virtual AstOpType getOp() const = 0;
        virtual std::any acceptVisitor(IAstVisitor<std::any>& visitor) const = 0;
    };
}

#endif