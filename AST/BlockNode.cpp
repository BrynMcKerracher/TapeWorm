/**
 * @file BlockNode.cpp
 * @author brynm
 */
#include "BlockNode.h"
#include "NodeVisitor.h"

namespace TapeWorm::AST {
    BlockNode::BlockNode() {
        nodeType = Block;
    }

    std::any BlockNode::Accept(NodeVisitor* visitor) {
        return visitor->VisitBlock(this);
    }
} // TapeWorm