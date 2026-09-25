/**
 * @file GlobalNode.cpp
 * @author Bryn McKerracher
 **/
#include "GlobalNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    GlobalNode::GlobalNode() {
        type = Global;
    }

    void GlobalNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) {
        visitor->VisitGlobal(this);
    }

    NodeBase* GlobalNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) {
        return editor->VisitGlobal(this);
    }
} // TapeWorm