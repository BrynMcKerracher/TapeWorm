#include "OptimiserVisitor.h"

#include "GlobalNode.h"
#include "BlockNode.h"
#include "AtomicNode.h"

namespace TapeWorm::AST {
    void OptimiserVisitor::Optimise(Global &globalNode) {
        globalNode->Accept(this);
    }

    std::any OptimiserVisitor::VisitGlobal(GlobalNode *node) {
        for (const Node& subNode : node->subNodes) {
            subNode->Accept(this);
        }
        return 0;
    }

    std::any OptimiserVisitor::VisitBlock(BlockNode *node) {
        for (const Node& subNode : node->subNodes) {
            subNode->Accept(this);
        }
        return 0;
    }

    std::any OptimiserVisitor::VisitAtomic(AtomicNode *node) {
        return 0;
    }
}
