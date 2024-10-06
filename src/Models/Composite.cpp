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
#include <utility>
#include <string>
// #include <sstream>
#include "Models/Composite.hpp"
#include "Models/Polynomial.hpp"
#include "Syntax/IAstNode.hpp"

namespace GeneralDeriver::Models {
    FunctionAny deriveComposite(Syntax::AstOpType top_op, const FunctionAny& first_child, const FunctionAny& second_child) {
        auto first = first_child.unpackFunctionAny<Composite>();
        auto second = second_child.unpackFunctionAny<Composite>();
        auto first_derived = first.makeDerivative();
        auto second_derived = second.makeDerivative();

        if (top_op == Syntax::AstOpType::power) {
            Composite new_exp {Syntax::AstOpType::sub, second, Composite {Syntax::AstOpType::none, {Polynomial {std::vector<PolynomialTerm> {{1, 0}}}}, {}}};

            /// @note composed (f(x))^n functions must follow chain rule: (first)^second becomes second * first ^ (second - 1) * dx(first)! I must std::move only when I'm sure that first or second are not used again, avoiding indeterminate values.
            return Composite {
                Syntax::AstOpType::mul,
                Composite {
                    Syntax::AstOpType::mul,
                    second,
                    Composite {
                        Syntax::AstOpType::power,
                        std::move(first),
                        new_exp
                    }
                },
                std::move(first_derived)
            };
        } else if (top_op != Syntax::AstOpType::mul && top_op != Syntax::AstOpType::div) {
            return {
                Composite {
                    top_op,
                    std::move(first_derived),
                    std::move(second_derived)
                }
            };
        }

        return {};
    }

    FunctionAny deriveComposite(Syntax::AstOpType top_op, const FunctionAny& inner_child) {
        /// @note A none Composite is practically just the inner function, so I just skip the wrapping and derive the inner item.
        if (top_op == Syntax::AstOpType::none) {
            return inner_child.unpackFunctionAny<Composite>().makeDerivative();
        }

        /// @note Do not handle negation op. type since the function emitter will detect negations of an expr. and simplify that away.
        return {};
    }

    Composite::Composite()
    : lhs_subject {}, rhs_subject {}, op {Syntax::AstOpType::none} {}

    Composite::Composite(Syntax::AstOpType op_, FunctionAny&& lhs, FunctionAny&& rhs)
    : lhs_subject(std::move(lhs)), rhs_subject(std::move(rhs)), op {op_} {}

    Syntax::AstOpType Composite::getOp() const { return op; }

    CompositeArity Composite::getArity() const {
        if (lhs_subject.getStoragePtr() != nullptr && rhs_subject.getStoragePtr() != nullptr) {
            return CompositeArity::binary;
        } else if (!rhs_subject.getStoragePtr()) {
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
        double lhs_val = lhs_subject.getStoragePtr()->evalAt(x);
        double rhs_val = rhs_subject.getStoragePtr()->evalAt(x);

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
        
        if (checked_arity == CompositeArity::invalid) {
            return {Composite {Syntax::AstOpType::none, {}, {}}};
        }

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
