#include "AtomicNode.h"
#include "NodeVisitor.h"

namespace TapeWorm::AST {
    AtomicNode::AtomicNode() {
        nodeType = Atomic;
    }

    AtomicNode::AtomicNode(const InterWorm::Token &token) {
        nodeType = Atomic;
        this->token = token;
    }

    std::any AtomicNode::Accept(NodeVisitor* visitor) {
        return visitor->VisitAtomic(this);
    }
}