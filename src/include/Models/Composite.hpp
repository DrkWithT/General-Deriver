#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include "Models/IFunction.hpp"
#include "Models/FunctionAny.hpp"
#include "Syntax/IAstNode.hpp"

namespace GeneralDeriver::Models {
    enum class CompositeArity {
        unary,   // one child for operation
        binary,  // two children for operation
        invalid  // bad Composite: no children to compute with
    };

    /**
     * @brief Represents a more complex x-function formed by composing two functions together or within each other. Composites contain an operation and at most two nested functions, effectively resembling a tree node. Evaluations thus require pre-order traversal: get op, get children, then evaluate a subset, etc.
     * @example `Composite { Syntax::AstOpType::power, Polynomial {Term {1, 1}, Term {-1, 0}}, Polynomial {Term {2, 0}}}` represents `f(x) = (x - 1)^2`.
     */
    class Composite : public IFunction {
    private:
        FunctionAny lhs_subject;     // "inner left" function
        FunctionAny rhs_subject;     // "inner right" function
        Syntax::AstOpType op; // top operation of composite

    public:
        Composite();

        /**
         * @brief Construct a new Composite object. All params are required!
         * @see Composite brief for full description of type.
         * 
         * @param op_ Math operation, unary or binary
         * @param lhs First child always present for either operation type
         * @param rhs Second child present for binary
         */
        Composite(Syntax::AstOpType op_, const FunctionAny& lhs, const FunctionAny& rhs);

        Syntax::AstOpType getOp() const;
        [[nodiscard]] CompositeArity getArity() const;

        FuncType getType() const override;
        double evalAt(double x) const override;
        FunctionAny makeDerivative() const override;
        std::string toText() const override;
    };

    /// @note This overload is for binary Composites e.g EMDAS arithmetic ops.
    FunctionAny deriveComposite(Syntax::AstOpType top_op, const FunctionAny& first_child, const FunctionAny& second_child);

    /// @note This overload is for unary Composites such as negation
    FunctionAny deriveComposite(Syntax::AstOpType top_op, const FunctionAny& inner_child);
}

#endif