#ifndef AST_NODES_HPP
#define AST_NODES_HPP

#include <any>
#include <memory>
#include "Syntax/IAstNode.hpp"

namespace GeneralDeriver::Syntax {
    class Constant : public IAstNode {
    private:
        double value;

    public:
        Constant();
        Constant(double value_);

        [[nodiscard]] double getValue() const;

        AstNodeType getType() const override;
        AstOpType getOp() const override;
        std::any acceptVisitor(IAstVisitor<std::any>& visitor) const override;
        Models::Composite acceptVisitor(IAstVisitor<Models::Composite>& visitor) const override;
    };

    class VarStub : public IAstNode {
    public:
        VarStub() = default;

        AstNodeType getType() const override;
        AstOpType getOp() const override;
        std::any acceptVisitor(IAstVisitor<std::any>& visitor) const override;
        Models::Composite acceptVisitor(IAstVisitor<Models::Composite>& visitor) const override;
    };

    class Unary : public IAstNode {
    private:
        std::unique_ptr<IAstNode> inner;
        AstOpType op;

    public:
        Unary() = delete;
        Unary(AstOpType op_, std::unique_ptr<IAstNode>&& x_inner);

        const std::unique_ptr<IAstNode>& getInnerPtr() const;

        AstNodeType getType() const override;
        AstOpType getOp() const override;
        std::any acceptVisitor(IAstVisitor<std::any>& visitor) const override;
        Models::Composite acceptVisitor(IAstVisitor<Models::Composite>& visitor) const override;
    };

    class Binary : public IAstNode {
    private:
        std::unique_ptr<IAstNode> lhs;
        std::unique_ptr<IAstNode> rhs;
        AstOpType op;

    public:
        Binary() = delete;
        Binary(AstOpType op_, std::unique_ptr<IAstNode>&& x_lhs, std::unique_ptr<IAstNode>&& x_rhs);

        const std::unique_ptr<IAstNode>& getLeft() const;
        const std::unique_ptr<IAstNode>& getRight() const;

        AstNodeType getType() const override;
        AstOpType getOp() const override;
        std::any acceptVisitor(IAstVisitor<std::any>& visitor) const override;
        Models::Composite acceptVisitor(IAstVisitor<Models::Composite>& visitor) const override;
    };
}

#endif