/**
 * @file BlockNode.cpp
 * @author brynm
 */
#include "BlockNode.h"

#include "NodeEditor.h"
#include "NodeVisitor.h"

namespace TapeWorm::AST {
    BlockNode::BlockNode() {
        nodeType = Block;
    }

    void BlockNode::Accept(NodeVisitor* visitor) {
        visitor->VisitBlock(this);
    }

    NodeBase *BlockNode::AcceptEditor(NodeEditor *editor) {
        return editor->VisitBlock(this);
    }
} // TapeWorm