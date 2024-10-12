/**
 * @file AstValidator.cpp
 * @author DrkWithT
 * @brief Implements AST checker for possibly NaN values or expressions.
 * @date 2024-09-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <cmath>
#include <utility>
#include "Backend/AstValidator.hpp"
#include "Syntax/IAstNode.hpp"

namespace GeneralDeriver::Backend {
    AstValidator::AstValidator()
    : values {}, ops {} {}

    AstValidator::OpStatus AstValidator::doStackOp() {
        if (ops.empty()) {
            return OpStatus::done;
        }

        Syntax::AstOpType operation = ops.top();
        ops.pop();

        FoldResult top = values.top();
        values.pop();

        if (operation == Syntax::AstOpType::neg) {
            values.push(computeOp(operation, top));
            return OpStatus::ok;
        }

        FoldResult under_top = values.top();
        values.pop();

        values.push(computeOp(operation, top, under_top));

        return (values.top().getFoldType() != FoldType::invalid)
            ? OpStatus::ok
            : OpStatus::failed;
    }

    std::any AstValidator::visitConstant(const Syntax::Constant& node) {
        values.push({node.getValue()});
        return {OpStatus::ok};
    }

    std::any AstValidator::visitVarStub([[maybe_unused]] const Syntax::VarStub& node) {
        values.push({SymbolicOpt {}});
        return {OpStatus::ok};
    }

    std::any AstValidator::visitUnary(const Syntax::Unary& node) {
        ops.push(node.getOp());

        node.getInnerPtr()->acceptVisitor(*this);

        return doStackOp();
    }

    /// @note This visits operands in proper order given the power expression `base^power` must simplify power first & the stack is LIFO. Thus, visiting right then left will correctly give `(op_power, base, power)` in the later `doStackOp` call. However, commutative operations are ok to evaluate either way.
    std::any AstValidator::visitBinary(const Syntax::Binary& node) {
        auto op = node.getOp();
        ops.push(op);

        if (op == Syntax::AstOpType::power) {
            node.getRight()->acceptVisitor(*this);
            node.getLeft()->acceptVisitor(*this);
        } else {
            node.getLeft()->acceptVisitor(*this);
            node.getRight()->acceptVisitor(*this);
        }

        return doStackOp();
    }

    bool AstValidator::validateAst(const std::unique_ptr<Syntax::IAstNode>& root) {
        return std::any_cast<AstValidator::OpStatus>(root->acceptVisitor(*this)) != AstValidator::OpStatus::failed;
    }

    void AstValidator::clearState() {
        std::stack<Syntax::AstOpType> temp;
        ops.swap(temp);
        std::stack<FoldResult> temp_2;
        values.swap(temp_2);
    }
}