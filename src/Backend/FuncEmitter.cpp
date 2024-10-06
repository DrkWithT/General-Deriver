/**
 * @file FuncEmitter.cpp
 * @author DrkWithT
 * @brief Implements AST to function converter.
 * @date 2024-10-06
 * @todo Implement the emitting algorithm for Unary and Binary nodes.
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/**
 * @brief This entire comment explains how the AST is converted to a function instance. Constant folding and function creation are done in one pass for simplicity. However, future changes might require multiple passes by various visitors. I suggest reading the algorithm details below.
 * 
 * @details Emission Strategy
 * As a tree, the AST will be recursively defined by subtrees until leaves. Subtrees contain a root node and some children. Leaves are elementary syntactic elements e.g constants and "x" variables that mark the endings. I take advantage of these properties for the `FuncEmitter` algorithm:
 * Begin: Visit the root and note the operation first.
 * 1. Check node types and op precendence...
 * 2. For any related children that represent numbers and have the same op precedence, fold their data into constants. Relation is either parent-child or sibling under a binary parent.
 * 3. Otherwise, other children will be ignored during constant folding, but their subtrees will still be visited for emission.
 * 4. Emit a Composite function for the subtree and incorporate folded constants...
 * 4a. Recurse into LHS and/or RHS subtrees (if any) and keep the sub-results.
 * 4b. Integrate the LHS and RHS parts into a Composite and return it.
 */

#include <utility>
#include <vector>
#include "Backend/AnalysisTypes.hpp"
#include "Backend/FuncEmitter.hpp"
#include "Syntax/IAstNode.hpp"
#include "Syntax/AstNodes.hpp"

namespace GeneralDeriver::Backend {
    Models::Composite convertFoldResult(const FoldResult& folded_value) {
        if (folded_value.getFoldType() == FoldType::number) {
            std::vector<Models::PolynomialTerm> terms = {
                Models::PolynomialTerm {folded_value.getScalarOptional().value(), 0}
            };

            return {
                Syntax::AstOpType::none,
                Models::Polynomial (std::move(terms)),
                {}
            };
        }

        return {Syntax::AstOpType::none, {}, {}};
    }

    FunctionEmitter::OpStatus FunctionEmitter::doStackOp() {
        if (ops.empty()) {
            return OpStatus::done;
        }

        Syntax::AstOpType op = ops.top();
        ops.pop();

        if (op == Syntax::AstOpType::none) {
            return OpStatus::done;
        }

        FoldResult operand_1 = foldable_values.top();
        foldable_values.pop();

        if (op == Syntax::AstOpType::neg) {
            foldable_values.push(computeOp(op, operand_1));
            return OpStatus::done;
        }

        FoldResult operand_2 = foldable_values.top();
        foldable_values.pop();

        foldable_values.push(computeOp(op, operand_1, operand_2));

        return OpStatus::done;
    }

    FunctionEmitter::FunctionEmitter()
    : foldable_values {}, ops {} {}

    Models::Composite FunctionEmitter::visitConstant(const Syntax::Constant& node) {
        foldable_values.push(FoldResult {node.getValue()});
        ops.push(Syntax::AstOpType::none);

        return {};
    }

    Models::Composite FunctionEmitter::visitVarStub(const Syntax::VarStub& node) {
        foldable_values.push(FoldResult {SymbolicOpt {}});
        ops.push(node.getOp());

        return {};
    }

    // Models::Composite FunctionEmitter::visitUnary(const Syntax::Unary& node); // todo

    // Models::Composite FunctionEmitter::visitBinary(const Syntax::Binary& node); // todo

    Models::Composite FunctionEmitter::emitFunction(const std::unique_ptr<Syntax::IAstNode>& root) {
        if (!root) {
            return {};
        }

        return root->acceptVisitor(*this);
    }

    void FunctionEmitter::resetSelf() {
        std::stack<FoldResult> temp_values;
        std::stack<Syntax::AstOpType> temp_ops;
        ops.swap(temp_ops);
        foldable_values.swap(temp_values);
    }
}
