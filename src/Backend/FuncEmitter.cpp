/**
 * @file FuncEmitter.cpp
 * @author DrkWithT
 * @brief Implements AST to function converter.
 * @date 2024-10-06
 * @todo Implement the emitting logic for Unary and Binary nodes. If this becomes too messy, just refactor: create AST optimizer and then redo emitter.
 * 
 * @copyright Copyright (c) 2024
 * 
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

    void FunctionEmitter::doStackOp() {
        if (ops.empty()) {
            return;
        }

        Syntax::AstOpType op = ops.top();
        ops.pop();

        if (op == Syntax::AstOpType::none) {
            return;
        }

        FoldResult operand_1 = foldable_values.top();
        foldable_values.pop();

        if (op == Syntax::AstOpType::neg) {
            foldable_values.push(computeOp(op, operand_1));
            return;
        }

        FoldResult operand_2 = foldable_values.top();
        foldable_values.pop();

        foldable_values.push(computeOp(op, operand_1, operand_2));
    }

    FunctionEmitter::FunctionEmitter()
    : foldable_values {}, ops {} {}

    Models::Composite FunctionEmitter::visitConstant(const Syntax::Constant& node) {
        foldable_values.push(FoldResult {node.getValue()});
        ops.push(Syntax::AstOpType::none);
        doStackOp();

        return {};
    }

    Models::Composite FunctionEmitter::visitVarStub(const Syntax::VarStub& node) {
        foldable_values.push(FoldResult {SymbolicOpt {}});
        ops.push(node.getOp());
        doStackOp();

        return {};
    }

    Models::Composite FunctionEmitter::visitUnary(const Syntax::Unary& node) {
        auto root_op = node.getOp();
        Models::Composite inside_fn = node.getInnerPtr()->acceptVisitor(*this);

        if (root_op == Syntax::AstOpType::none) {
            return inside_fn;
        } else if (root_op == Syntax::AstOpType::neg) {
            FoldResult inner_fold = foldable_values.top();
            foldable_values.pop();

            if (inner_fold.getFoldType() == FoldType::number) {
                FoldResult negated_fold {-1.0 * inner_fold.getScalarOptional().value()};

                return {
                    Syntax::AstOpType::none,
                    convertFoldResult(negated_fold),
                    {}
                };
            }

            return {
                Syntax::AstOpType::neg,
                inside_fn,
                {}
            };
        }

        return {};
    }

    Models::Composite FunctionEmitter::visitBinary(const Syntax::Binary& node) {
        auto parent_op = node.getOp();
        Models::Composite lhs_fn = node.getLeft()->acceptVisitor(*this);
        Models::Composite rhs_fn = node.getRight()->acceptVisitor(*this);
        FoldResult folded_pow;

        if (parent_op == Syntax::AstOpType::power) {
            folded_pow = foldable_values.top();
            foldable_values.pop();
        }

        if (folded_pow.getFoldType() == FoldType::number) {
            return {
                parent_op,
                convertFoldResult(folded_pow),
                {}
            };
        }

        return {
            parent_op,
            lhs_fn,
            rhs_fn
        };
    }

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
