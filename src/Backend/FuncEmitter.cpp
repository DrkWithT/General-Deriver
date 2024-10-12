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
#include "Models/Polynomial.hpp"
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

    FunctionEmitter::FunctionEmitter() {}

    Models::Composite FunctionEmitter::visitConstant(const Syntax::Constant& node) {
        return {Syntax::AstOpType::none, convertFoldResult(node.getValue()), {}};
    }

    Models::Composite FunctionEmitter::visitVarStub([[maybe_unused]] const Syntax::VarStub& node) {
        return {
            Syntax::AstOpType::none,
            Models::Polynomial {
        {Models::PolynomialTerm {1, 1}}
            },
            {}
        };
    }

    Models::Composite FunctionEmitter::visitUnary(const Syntax::Unary& node) {
        auto root_op = node.getOp();
        Models::Composite inside_fn = node.getInnerPtr()->acceptVisitor(*this);

        if (root_op == Syntax::AstOpType::none) {
            return inside_fn;
        } else if (root_op == Syntax::AstOpType::neg) {
            return {
                Syntax::AstOpType::mul,
                convertFoldResult({-1}),
                inside_fn
            };
        }

        /// @note Hacky fix: treat unexpected binary exprs. here as 0
        return {
            Syntax::AstOpType::none,
            convertFoldResult({0}),
            {}
        };
    }

    Models::Composite FunctionEmitter::visitBinary(const Syntax::Binary& node) {
        auto parent_op = node.getOp();
        Models::Composite lhs_fn = node.getLeft()->acceptVisitor(*this);
        Models::Composite rhs_fn = node.getRight()->acceptVisitor(*this);

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
}
