/**
 * @file GlobalNode.cpp
 * @author brynm
 */
#include "GlobalNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    GlobalNode::GlobalNode() {
        nodeType = Global;
    }

    void GlobalNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) {
        visitor->VisitGlobal(this);
    }

    NodeBase* GlobalNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) {
        return editor->VisitGlobal(this);
    }
} // TapeWorm