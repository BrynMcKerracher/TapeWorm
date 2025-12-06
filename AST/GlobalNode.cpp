/**
 * @file GlobalNode.cpp
 * @author brynm
 */
#include "GlobalNode.h"

namespace TapeWorm::AST {
    GlobalNode::GlobalNode() {
        nodeType = Global;
    }

    std::any GlobalNode::Accept(NodeVisitor* visitor) {
        return visitor->VisitGlobal(this);
    }
} // TapeWorm