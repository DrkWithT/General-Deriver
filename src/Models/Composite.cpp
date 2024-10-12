/**
 * @file Composite.cpp
 * @author DrkWithT
 * @brief Implements composite / aggregate function type.
 * @date 2024-10-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <cmath>
#include <string>
#include <iostream>
#include "Models/Composite.hpp"
#include "Backend/FuncEmitter.hpp"
#include "Models/IFunction.hpp"
#include "Models/Polynomial.hpp"
#include "Syntax/IAstNode.hpp"

namespace GeneralDeriver::Models {
    FunctionAny deriveComposite(Syntax::AstOpType top_op, const FunctionAny& first_child, const FunctionAny& second_child) {
        auto first = first_child.unpackFunctionAny<Composite>();
        auto second = second_child.unpackFunctionAny<Composite>();
        auto first_derived = first.makeDerivative();
        auto second_derived = second.makeDerivative();

        if (top_op == Syntax::AstOpType::power) {
            std::cout << "Chain rule on power op...\n"; // debug
            Composite new_exp {Syntax::AstOpType::sub, second, Composite {Syntax::AstOpType::none, {Polynomial {std::vector<PolynomialTerm> {{1, 0}}}}, {}}};

            /// @note composed (f(x))^n functions must follow chain rule: (first)^second becomes second * first ^ (second - 1) * dx(first)!
            return Composite {
                Syntax::AstOpType::mul,
                Composite {
                    Syntax::AstOpType::mul,
                    second,
                    Composite {
                        Syntax::AstOpType::power,
                        first,
                        new_exp
                    }
                },
                first_derived
            };
        } else if (top_op == Syntax::AstOpType::add || top_op ==  Syntax::AstOpType::sub) {
            std::cout << "Chain rule on term op...\n"; // debug
            return {
                Composite {
                    top_op,
                    first_derived,
                    second_derived
                }
            };
        }

        std::cout << "Derivation bad!\n"; // debug
        return {};
    }

    FunctionAny deriveComposite(Syntax::AstOpType top_op, const FunctionAny& inner_child) {
        auto inner_derived = (inner_child.getStoragePtr()->getType() == FuncType::polynomial)
            ? inner_child.unpackFunctionAny<Polynomial>().makeDerivative()
            : inner_child.unpackFunctionAny<Composite>().makeDerivative();

        /// @note A none Composite is practically just the inner function, so I just skip the wrapping and derive the inner item.
        if (top_op == Syntax::AstOpType::none) {
            std::cout << "Deriving identity op...\n"; // debug

            return inner_derived;
        } else if (top_op == Syntax::AstOpType::neg) {
            std::cout << "Deriving negation op...\n"; // debug
            return Composite {
                Syntax::AstOpType::mul,
                Backend::convertFoldResult({-1}),
                inner_derived
            };
        }

        /// @note Do not handle negation op. type since the function emitter will detect negations of an expr. and simplify that away.
        std::cout << "Derivation bad!\n"; // debug
        return {};
    }

    Composite::Composite()
    : lhs_subject {}, rhs_subject {}, op {Syntax::AstOpType::none} {}

    Composite::Composite(Syntax::AstOpType op_, const FunctionAny& lhs, const FunctionAny& rhs)
    : lhs_subject(lhs), rhs_subject(rhs), op {op_} {}

    Syntax::AstOpType Composite::getOp() const { return op; }

    CompositeArity Composite::getArity() const {
        if (lhs_subject.getStoragePtr() != nullptr && rhs_subject.getStoragePtr() != nullptr) {
            return CompositeArity::binary;
        } else if (lhs_subject.getStoragePtr() != nullptr) {
            return CompositeArity::unary;
        }

        return CompositeArity::invalid;
    }

    FuncType Composite::getType() const {
        switch (op) {
        case Syntax::AstOpType::sub:
            return FuncType::difference;
        case Syntax::AstOpType::add:
            return FuncType::summation;
        case Syntax::AstOpType::div:
            return FuncType::rational;
        case Syntax::AstOpType::power:
            return FuncType::polynomial;
        case Syntax::AstOpType::neg:
        case Syntax::AstOpType::mul:
            return FuncType::product;
        case Syntax::AstOpType::none:
        default:
            return FuncType::identity;
        }
    }

    double Composite::evalAt(double x) const {
        auto op_arity = getArity();
        double lhs_val = 0.0;
        double rhs_val = 0.0;

        if (op_arity == CompositeArity::unary) {
            lhs_val = lhs_subject.getStoragePtr()->evalAt(x);
        } else if (op_arity == CompositeArity::binary) {
            lhs_val = lhs_subject.getStoragePtr()->evalAt(x);
            rhs_val = rhs_subject.getStoragePtr()->evalAt(x);
        } else {
            /// @todo Add exception throwing for invalid op arity, perhaps from some func. emission failure.
            return 0.0;
        }

        switch (op) {
        case Syntax::AstOpType::sub:
            return lhs_val - rhs_val;
        case Syntax::AstOpType::add:
            return lhs_val + rhs_val;
        case Syntax::AstOpType::mul:
            return lhs_val * rhs_val;
        case Syntax::AstOpType::div:
            /// @note division is not safe checked here since the AST validator should find any NaN expressions.
            return lhs_val / rhs_val;
        case Syntax::AstOpType::power:
            return std::pow(lhs_val, rhs_val);
        case Syntax::AstOpType::neg:
            return -1.0 * lhs_val;
        case Syntax::AstOpType::none:
        default:
            return lhs_val;
        }
    }

    /// @todo Implement derivative member function!
    FunctionAny Composite::makeDerivative() const {
        auto checked_arity = getArity();
        auto self_op = op;

        /// @note I dispatch by arity and op to overloaded helper functions to avoid cramming ALL logic in this member function.
        switch (checked_arity) {
        case CompositeArity::binary:
            return deriveComposite(self_op, lhs_subject, rhs_subject);
        case CompositeArity::unary:
            return deriveComposite(self_op, lhs_subject);
        default:
            return {};
        }
    }

    std::string Composite::toText() const {
        /// @todo Implement full stringification logic with recursion.
        return "Composite {...}";
    }
}
