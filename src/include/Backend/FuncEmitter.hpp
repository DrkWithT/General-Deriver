#ifndef FUNC_EMITTER_HPP
#define FUNC_EMITTER_HPP

#include <memory>
#include "Backend/AnalysisTypes.hpp"
#include "Syntax/IAstVisitor.hpp"
#include "Syntax/IAstNode.hpp"
#include "Models/Composite.hpp"

namespace GeneralDeriver::Backend {
    [[nodiscard]] Models::Composite convertFoldResult(const FoldResult& folded_value);

    class FunctionEmitter : public Syntax::IAstVisitor<Models::Composite> {

    public:
        FunctionEmitter();

        Models::Composite visitConstant(const Syntax::Constant& node) override;
        Models::Composite visitVarStub(const Syntax::VarStub& node) override;
        Models::Composite visitUnary(const Syntax::Unary& node) override;
        Models::Composite visitBinary(const Syntax::Binary& node) override;

        [[nodiscard]] Models::Composite emitFunction(const std::unique_ptr<Syntax::IAstNode>& root);
    };
}

#endif