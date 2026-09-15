/**
 * @file TreeOptimiser.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORMTESTS_TREEOPTIMISER_H
#define TAPEWORMTESTS_TREEOPTIMISER_H

#include "GlobalNode.h"
#include "NodeEditor.h"
#include "NodeVisitor.h"

namespace TapeWorm::AST{
    class TreeOptimiser final : NodeEditor {
    public:
        Global Optimise(Global& node);
    private:
        NodeBase* VisitAtomic(AtomicNode* node) override;
        NodeBase* VisitBlock(BlockNode* node) override;
        NodeBase* VisitGlobal(GlobalNode* node) override;
        NodeBase* VisitLeafStatement(LeafStatementNode* node) override;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_TREEOPTIMISER_H