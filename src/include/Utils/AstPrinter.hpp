#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <any>
#include "Syntax/IAstVisitor.hpp"
#include "Syntax/AstNodes.hpp"

namespace GeneralDeriver::Utils {

    class AstPrinter : public Syntax::IAstVisitor<std::any> {
    private:
        int indent_size;
        int depth;

        void printIndent();

    public:
        explicit AstPrinter();
        explicit AstPrinter(int indent_size_);

        std::any visitConstant(const Syntax::Constant& node) override;
        std::any visitVarStub(const Syntax::VarStub& node) override;
        std::any visitUnary(const Syntax::Unary& node) override;
        std::any visitBinary(const Syntax::Binary& node) override;
    };
}

#endif