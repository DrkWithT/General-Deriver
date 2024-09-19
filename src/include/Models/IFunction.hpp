#pragma once

namespace GeneralDeriver::Models {
    enum class FuncType {
        polynomial,
        summation,
        difference,
        product,
        rational,
        none
    };

    // Forward declaration: type erasure container for any supported function class-type
    class FunctionAny;

    /**
     * @brief Interface for common x-function operations.
     */
    class IFunction {
    public:
        virtual ~IFunction() = delete;

        virtual FuncType getType() const = 0;
        virtual double evalAt(double x) const = 0;
        virtual FunctionAny makeDerivative() const = 0;
    };
}
