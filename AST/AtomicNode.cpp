#include "AtomicNode.h"

#include "NodeEditor.h"
#include "NodeVisitor.h"

namespace TapeWorm::AST {
    AtomicNode::AtomicNode() {
        nodeType = Atomic;
    }

    AtomicNode::AtomicNode(const InterWorm::Token &token) {
        nodeType = Atomic;
        this->token = token;
    }

    void AtomicNode::Accept(NodeVisitor* visitor) {
        visitor->VisitAtomic(this);
    }

    NodeBase* AtomicNode::AcceptEditor(NodeEditor *editor) {
        return editor->VisitAtomic(this);
    }
}
