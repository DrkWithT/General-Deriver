#ifndef AST_VALIDATOR_HPP
#define AST_VALIDATOR_HPP

#include <memory>
#include <any>
#include <optional>
#include <stack>
#include "Syntax/IAstNode.hpp"
#include "Syntax/IAstVisitor.hpp"
#include "Syntax/AstNodes.hpp"
#include "Backend/AnalysisTypes.hpp"

namespace GeneralDeriver::Backend {
    /**
     * @brief AST visitor & walker for detecting any constant-folded NaN values.
     */
    class AstValidator : public Syntax::IAstVisitor<std::any> {
    private:
        enum class OpStatus {
            ok,
            failed,
            done,
        };

        std::stack<FoldResult> values;
        std::stack<Syntax::AstOpType> ops;

        [[nodiscard]] OpStatus doStackOp();

    public:
        AstValidator();

        std::any visitConstant(const Syntax::Constant& node) override;
        std::any visitVarStub(const Syntax::VarStub& node) override;
        std::any visitUnary(const Syntax::Unary& node) override;
        std::any visitBinary(const Syntax::Binary& node) override;

        [[nodiscard]] bool validateAst(const std::unique_ptr<Syntax::IAstNode>& root);

        void clearState();
    };
}

#endif