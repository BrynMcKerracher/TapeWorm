/**
 * @file SyntacticNode.cpp
 * @author Bryn McKerracher
 **/
#include "SyntacticNode.h"

#include "NodeAnyVisitor.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    SyntacticNode::SyntacticNode() {
        type = Syntactic;
    }

    void SyntacticNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) {
        visitor->VisitSyntactic(this);
    }

    NodeBase* SyntacticNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *visitor) {
        return visitor->VisitSyntactic(this);
    }

    std::any SyntacticNode::AcceptAnyVisitor(NodeAnyVisitor *visitor) {
        return visitor->VisitSyntactic(this);
    }
} // TapeWorm