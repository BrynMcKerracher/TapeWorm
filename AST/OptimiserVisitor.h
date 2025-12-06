#ifndef TAPEWORM_OPTIMISERVISITOR_H
#define TAPEWORM_OPTIMISERVISITOR_H

#include "NodeVisitor.h"
#include "GlobalNode.h"
#include "BlockNode.h"

namespace TapeWorm::AST {
    class OptimiserVisitor final : public NodeVisitor {
    public:
        void Optimise(Global& globalNode);
    private:
        std::any VisitAtomic(AtomicNode* node) override;
        std::any VisitBlock(BlockNode* node) override;
        std::any VisitGlobal(GlobalNode* node) override;
    };
} // TapeWorm

#endif //TAPEWORM_OPTIMISERVISITOR_H