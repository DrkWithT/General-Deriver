/**
 * @file AstPrinter.cpp
 * @author DrkWithT
 * @brief Implements AST printer for debug purposes.
 * @date 2024-09-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <iostream>
#include "Utils/AstPrinter.hpp"

namespace GeneralDeriver::Utils {
    static constexpr int default_indent_spacing = 2;

    AstPrinter::AstPrinter()
    : indent_size {default_indent_spacing} {}

    AstPrinter::AstPrinter(int indent_size_)
    : indent_size {(indent_size_ > 0) ? indent_size_ : default_indent_spacing} {}

    void AstPrinter::printIndent() {
        const int spaces = indent_size * depth;

        if (spaces == 0) {
            return;
        }

        for (int i = 0; i < spaces; i++) {
            std::cout << ' ';
        }
    }

    std::any AstPrinter::visitConstant(const Syntax::Constant& node) {
        printIndent();
        std::cout << "Constant:\n";

        depth++;

        printIndent();
        std::cout << "type: " << static_cast<int>(node.getType()) << '\n';

        printIndent();
        std::cout << "value: " << node.getValue() << '\n';

        depth--;

        return {};
    }

    std::any AstPrinter::visitVarStub(const Syntax::VarStub& node) {
        printIndent();
        std::cout << "VarStub:\n";

        depth++;

        printIndent();
        std::cout << "type: " << static_cast<int>(node.getType()) << '\n';
        printIndent();
        std::cout << "meta-symbol: x\n";

        depth--;

        return {};
    }

    std::any AstPrinter::visitUnary(const Syntax::Unary& node) {
        printIndent();
        std::cout << "Unary:\n";

        depth++;

        printIndent();
        std::cout << "type: " << static_cast<int>(node.getType()) << '\n';

        printIndent();
        std::cout << "op: " << static_cast<int>(node.getOp()) << '\n';

        if (node.getInnerPtr()) {
            node.getInnerPtr()->acceptVisitor(*this);
        }

        depth--;
        return {};
    }

    std::any AstPrinter::visitBinary(const Syntax::Binary& node) {
        printIndent();
        std::cout << "Binary:\n";

        depth++;

        printIndent();
        std::cout << "type: " << static_cast<int>(node.getType()) << '\n';

        printIndent();
        std::cout << "op: " << static_cast<int>(node.getOp()) << '\n';

        if (node.getLeft()) {
            node.getLeft()->acceptVisitor(*this);
        }

        if (node.getRight()) {
            node.getRight()->acceptVisitor(*this);
        }

        depth--;

        return {};
    }

    void AstPrinter::printAST(std::string_view title, const std::unique_ptr<Syntax::IAstNode>& root) {
        std::cout << "Tree name: \"" << title << "\"\n";

        if (!root) {
            std::cout << "(null root, maybe bad!)\n";
            return;
        }

        root->acceptVisitor(*this);
    }
}
