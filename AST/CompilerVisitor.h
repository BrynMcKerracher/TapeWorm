#ifndef TAPEWORM_COMPILERVISITOR_H
#define TAPEWORM_COMPILERVISITOR_H

#include "NodeReadOnlyVisitor.h"
#include "GlobalNode.h"
#include "../Op.h"

#include <vector>

namespace TapeWorm::AST {
    class CompilerVisitor final : public NodeReadOnlyVisitor {
    public:
        std::vector<InterWorm::Op::Type> Visit(const Global& node);
    private:
        std::vector<InterWorm::Op::Type> ops;

        void VisitTerminal(TerminalNode* node) override;
        void VisitBlock(SyntacticNode* node) override;
        void VisitGlobal(GlobalNode* node) override;
        void VisitLeafStatement(LeafStatementNode *node) override;

        void WriteInt64(int64_t n);
        void WriteOps(std::initializer_list<InterWorm::Op::Type> bytes);
        void WriteOp(InterWorm::Op::Type op);
    };
} // TapeWorm

#endif //TAPEWORM_COMPILERVISITOR_H