#ifndef TAPEWORM_COMPILERVISITOR_H
#define TAPEWORM_COMPILERVISITOR_H

#include "NodeVisitor.h"
#include "GlobalNode.h"
#include "../Op.h"

#include <vector>

namespace TapeWorm::AST {
    class CompilerVisitor final : public NodeVisitor {
    public:
        std::vector<InterWorm::Op::Type> Visit(const Global& node);
    private:
        std::vector<InterWorm::Op::Type> ops;

        std::any VisitAtomic(AtomicNode* node) override;
        std::any VisitBlock(BlockNode* node) override;
        std::any VisitGlobal(GlobalNode* node) override;

        void WriteOps(std::initializer_list<InterWorm::Op::Type> bytes);
        void WriteOp(InterWorm::Op::Type op);
        void CompileStatement(const BlockNode* block);
    };
} // TapeWorm

#endif //TAPEWORM_COMPILERVISITOR_H