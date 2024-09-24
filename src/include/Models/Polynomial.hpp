#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <vector>
#include "Models/IFunction.hpp"
#include "Models/FunctionAny.hpp"

namespace GeneralDeriver::Models {
    struct PolynomialTerm {
        double coeff;
        double power;
    };

    class Polynomial : public IFunction {
    private:
        std::vector<PolynomialTerm> terms;

    public:
        Polynomial();

        Polynomial(std::vector<PolynomialTerm>& terms_);

        FuncType getType() const override;

        [[nodiscard]] double evalAt(double x) const override;

        [[nodiscard]] FunctionAny makeDerivative() const override;

        std::string toText() const override;
    };
}

#endif
