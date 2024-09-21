/**
 * @file Polynomial.cpp
 * @author DrkWithT
 * @brief Implements mathematical function model for any polynomial.
 * @date 2024-09-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <cmath>
#include <utility>
#include <sstream>
#include "Models/IFunction.hpp"
#include "Models/Polynomial.hpp"

namespace GeneralDeriver::Models {
    static constexpr double zero_coefficient = 0.0;

    Polynomial::Polynomial()
    : terms {0} {}

    Polynomial::Polynomial(std::vector<PolynomialTerm>& terms_)
    : terms(std::move(terms_)) {}

    FuncType Polynomial::getType() const { return FuncType::polynomial; }

    double Polynomial::evalAt(double x) const {
        double result = zero_coefficient;

        for (auto [coeff, power] : terms) {
            if (coeff == zero_coefficient) {
                continue;
            }

            if (power != zero_coefficient) {
                result += pow(x, power) * coeff;
            } else {
                result += coeff;
            }
        }

        return result;
    }

    /// @note Uses power rule of differentiation.
    FunctionAny Polynomial::makeDerivative() const {
        std::vector<PolynomialTerm> new_terms;

        for (auto [coeff, power] : terms) {
            if (coeff == zero_coefficient) {
                continue;
            }

            if (power != zero_coefficient) {
                new_terms.push_back( {coeff * power, power - 1.0});
            } else {
                new_terms.push_back({zero_coefficient, zero_coefficient});
            }
        }

        return {Polynomial {new_terms}};
    }
}