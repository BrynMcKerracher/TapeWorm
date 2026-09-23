/**
 * @file TreeOptimiser.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORMTESTS_TREEOPTIMISER_H
#define TAPEWORMTESTS_TREEOPTIMISER_H

#include "GlobalNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    class TreeOptimiser final : NodeReadWriteVisitor {
    public:
        Global Optimise(Global& node);
    private:
        NodeBase* VisitTerminal(TerminalNode* node) override;
        NodeBase* VisitSyntactic(SyntacticNode* node) override;
        NodeBase* VisitGlobal(GlobalNode* node) override;
        NodeBase* VisitLeafStatement(LeafStatementNode* node) override;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_TREEOPTIMISER_H