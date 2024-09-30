#ifndef AST_VALIDATOR_HPP
#define AST_VALIDATOR_HPP

#include <memory>
#include <any>
#include <optional>
#include <stack>
#include "Syntax/IAstNode.hpp"
#include "Syntax/IAstVisitor.hpp"
#include "Syntax/AstNodes.hpp"

namespace GeneralDeriver::Backend {
    /// @brief Denotes the type of sub-AST expression folding result.
    enum class FoldType : uint8_t {
        number,        // for purely numeric folded value
        symbolic,      // for part variable, part numeric folded values
        invalid        // for NaN values
    };

    struct SymbolicOpt {};

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

    /**
     * @brief AST visitor & walker for detecting any constant-folded NaN values.
     */
    class AstValidator : public Syntax::IAstVisitor<std::any> {
    private:
        enum class OpStatus {
            ok,
            failed,
            done,
        };

        std::stack<FoldResult> values;
        std::stack<Syntax::AstOpType> ops;

        [[nodiscard]] OpStatus doStackOp();

    public:
        AstValidator();

        std::any visitConstant(const Syntax::Constant& node) override;
        std::any visitVarStub(const Syntax::VarStub& node) override;
        std::any visitUnary(const Syntax::Unary& node) override;
        std::any visitBinary(const Syntax::Binary& node) override;

        [[nodiscard]] bool validateAst(const std::unique_ptr<Syntax::IAstNode>& root);

        void clearState();
    };
}

#endif