/**
 * @file AstNodes.cpp
 * @author DrkWithT
 * @brief Implements AST nodes for x-expr elements, e.g a Constant for 42, Binary for x^2, x + 1, etc.
 * @date 2024-09-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <utility>
#include "Syntax/AstNodes.hpp"
#include "Syntax/IAstVisitor.hpp"
#include "Syntax/IAstNode.hpp"
#include "Models/Composite.hpp"

namespace GeneralDeriver::Syntax {
    static constexpr double placeholder_z = 0.0;


    Constant::Constant()
    : value {placeholder_z} {}

    Constant::Constant(double value_)
    : value {value_} {}

    double Constant::getValue() const { return value; }

    AstNodeType Constant::getType() const { return AstNodeType::literal; }

    AstOpType Constant::getOp() const { return AstOpType::none; }

    std::any Constant::acceptVisitor(IAstVisitor<std::any>& visitor) const { return visitor.visitConstant(*this); }

    Models::Composite Constant::acceptVisitor(IAstVisitor<Models::Composite>& visitor) const { return visitor.visitConstant(*this); }


    AstNodeType VarStub::getType() const { return AstNodeType::literal; }

    AstOpType VarStub::getOp() const { return AstOpType::none; }

    std::any VarStub::acceptVisitor(IAstVisitor<std::any>& visitor) const { return visitor.visitVarStub(*this); }

    Models::Composite VarStub::acceptVisitor(IAstVisitor<Models::Composite>& visitor) const { return visitor.visitVarStub(*this); }


    Unary::Unary(AstOpType op_, std::unique_ptr<IAstNode>&& x_inner)
    : inner (std::move(x_inner)), op {op_} {}

    const std::unique_ptr<IAstNode>& Unary::getInnerPtr() const { return inner; }

    AstNodeType Unary::getType() const { return AstNodeType::unary; }

    AstOpType Unary::getOp() const { return op; }

    std::any Unary::acceptVisitor(IAstVisitor<std::any>& visitor) const { return visitor.visitUnary(*this); }

    Models::Composite Unary::acceptVisitor(IAstVisitor<Models::Composite>& visitor) const { return visitor.visitUnary(*this); }


    Binary::Binary(AstOpType op_, std::unique_ptr<IAstNode>&& x_lhs, std::unique_ptr<IAstNode>&& x_rhs)
    : lhs (std::move(x_lhs)), rhs (std::move(x_rhs)), op {op_} {}

    const std::unique_ptr<IAstNode>& Binary::getLeft() const { return lhs; }

    const std::unique_ptr<IAstNode>& Binary::getRight() const { return rhs; }

    AstNodeType Binary::getType() const { return AstNodeType::binary; };

    AstOpType Binary::getOp() const { return op; }

    std::any Binary::acceptVisitor(IAstVisitor<std::any>& visitor) const { return visitor.visitBinary(*this); }

    Models::Composite Binary::acceptVisitor(IAstVisitor<Models::Composite>& visitor) const { return visitor.visitBinary(*this); }
}
