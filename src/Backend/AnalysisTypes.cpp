/**
 * @file AnalysisTypes.cpp
 * @author DrkWithT
 * @brief Implements types for AST analysis.
 * @date 2024-10-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Backend/AnalysisTypes.hpp"

namespace GeneralDeriver::Backend {
    Precedence getOpPrecedence(Syntax::AstOpType op) {
        switch (op) {
        case Syntax::AstOpType::add:
            return Precedence::add;
        case Syntax::AstOpType::sub:
            return Precedence::sub;
        case Syntax::AstOpType::mul:
            return Precedence::mul;
        case Syntax::AstOpType::div:
            return Precedence::div;
        case Syntax::AstOpType::neg:
            return Precedence::neg;
        case Syntax::AstOpType::power:
            return Precedence::power;
        case Syntax::AstOpType::none:
        default:
            return Precedence::identity;
        }
    }

    FoldResult::FoldResult()
    : scalar {}, tag {FoldType::invalid} {}

    FoldResult::FoldResult([[maybe_unused]] const SymbolicOpt& symbolic_)
    : scalar {}, tag {FoldType::symbolic} {}

    FoldResult::FoldResult(double num_)
    : scalar {num_}, tag {FoldType::number} {}

    const std::optional<double>& FoldResult::getScalarOptional() const { return scalar; }

    FoldType FoldResult::getFoldType() const { return tag; }

    FoldResult operator+(const FoldResult& lhs, const FoldResult& rhs) {
        auto left_tag = lhs.getFoldType();
        auto right_tag = rhs.getFoldType();

        if (left_tag == FoldType::invalid || right_tag == FoldType::invalid) {
            return {};
        }

        if (left_tag == FoldType::symbolic || right_tag == FoldType::symbolic) {
            return {SymbolicOpt {}};
        }

        return {lhs.getScalarOptional().value() + rhs.getScalarOptional().value()};
    }

    FoldResult operator-(const FoldResult& lhs, const FoldResult& rhs) {
        auto left_tag = lhs.getFoldType();
        auto right_tag = rhs.getFoldType();

        if (left_tag == FoldType::invalid || right_tag == FoldType::invalid) {
            return {};
        }

        if (left_tag == FoldType::symbolic || right_tag == FoldType::symbolic) {
            return {SymbolicOpt {}};
        }

        return {lhs.getScalarOptional().value() - rhs.getScalarOptional().value()};
    }

    FoldResult doNegate(const FoldResult& target) {
        switch (target.getFoldType()) {
            case FoldType::number:
                return {target.getScalarOptional().value() * -1};
            case FoldType::symbolic:
                return {SymbolicOpt {}};
            default:
                return {};
        }
    }

    FoldResult doPower(const FoldResult& lhs, const FoldResult& rhs) {
        auto left_tag = lhs.getFoldType();
        auto right_tag = rhs.getFoldType();

        if (left_tag == FoldType::invalid || right_tag == FoldType::invalid) {
            return {};
        }

        if (left_tag == FoldType::symbolic || right_tag == FoldType::symbolic) {
            return {SymbolicOpt {}};
        }

        auto base = lhs.getScalarOptional().value();
        auto exp = rhs.getScalarOptional().value();

        if (base == 0.0 && exp <= 0.0) {
            return {}; // treat 0 exp -1 or 0 exp 0 as NaN because evaluating them would give a mathematically invalid result!
        }

        return {std::pow(base, exp)};
    }

    FoldResult computeOp(Syntax::AstOpType op, const FoldResult& target) {
        if (op == Syntax::AstOpType::neg) {
            return doNegate(target);
        }

        return {};
    }

    FoldResult computeOp(Syntax::AstOpType op, const FoldResult& lhs, const FoldResult& rhs) {
        switch (op) {
            case Syntax::AstOpType::add:
                return lhs + rhs;
            case Syntax::AstOpType::sub:
                return lhs - rhs;
            case Syntax::AstOpType::power:
                return doPower(lhs, rhs);
            default:
                break;
        }

        return {};
    }
}