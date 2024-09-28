#ifndef I_AST_VISITOR_HPP
#define I_AST_VISITOR_HPP

namespace GeneralDeriver::Syntax {
    class Constant;
    class VarStub;
    class Unary;
    class Binary;

    template <typename Rt>
    class IAstVisitor {
    public:
        virtual ~IAstVisitor() = default;

        virtual Rt visitConstant(const Constant& node) = 0;
        virtual Rt visitVarStub(const VarStub& node) = 0;
        virtual Rt visitUnary(const Unary& node) = 0;
        virtual Rt visitBinary(const Binary& node) = 0;
    };
}

#endif