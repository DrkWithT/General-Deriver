#ifndef ANALYSIS_TYPES_HPP
#define ANALYSIS_TYPES_HPP

#include <optional>
#include "Syntax/IAstNode.hpp"

namespace GeneralDeriver::Backend {
    /// @brief Denotes eval. priority of a math operation from lowest to highest
    enum class Precedence : uint8_t {
        add = 1,
        sub,
        mul,
        div,
        neg,
        power,
        identity
    };

    /// @brief Denotes the type of sub-AST expression folding result.
    enum class FoldType : uint8_t {
        number,        // for purely numeric folded value
        symbolic,      // for part variable, part numeric folded values
        invalid        // for NaN values
    };

    struct SymbolicOpt {};

    [[nodiscard]] Precedence getOpPrecedence(Syntax::AstOpType op);

    /**
     * @brief Represents the result of constant folding or simplifying the value of a sub-AST.
     * @note Only meant for validating ASTs of x-expressions where their emitted function is defined.
     */
    class FoldResult {
    private:
        std::optional<double> scalar;
        FoldType tag;

    public:
        FoldResult();
        FoldResult(const SymbolicOpt& symbolic_);
        FoldResult(double num_);

        const std::optional<double>& getScalarOptional() const;
        [[nodiscard]] FoldType getFoldType() const;

        friend FoldResult operator+(const FoldResult& lhs, const FoldResult& rhs);
        friend FoldResult operator-(const FoldResult& lhs, const FoldResult& rhs);
        friend FoldResult doNegate(const FoldResult& target);
        friend FoldResult doPower(const FoldResult& lhs, const FoldResult& rhs);
    };

    /// @brief for unary operation e.g negate
    [[nodiscard]] FoldResult computeOp(Syntax::AstOpType op, const FoldResult& target);

    /// @brief for binary operation e.g M D A S
    [[nodiscard]] FoldResult computeOp(Syntax::AstOpType op, const FoldResult& lhs, const FoldResult& rhs);
}

#endif