/**
 * @file BlockNode.cpp
 * @author brynm
 */
#include "SyntacticNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    SyntacticNode::SyntacticNode() {
        nodeType = Block;
    }

    void SyntacticNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) {
        visitor->VisitSyntactic(this);
    }

    NodeBase *SyntacticNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *visitor) {
        return visitor->VisitSyntactic(this);
    }
} // TapeWorm