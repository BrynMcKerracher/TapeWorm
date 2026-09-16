/**
 * @file GlobalNode.cpp
 * @author brynm
 */
#include "GlobalNode.h"
#include "NodeEditor.h"
#include "NodeVisitor.h"

namespace TapeWorm::AST {
    GlobalNode::GlobalNode() {
        nodeType = Global;
    }

    void GlobalNode::Accept(NodeVisitor* visitor) {
        visitor->VisitGlobal(this);
    }

    NodeBase* GlobalNode::AcceptEditor(NodeEditor *editor) {
        return editor->VisitGlobal(this);
    }
} // TapeWorm