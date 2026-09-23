#include "TerminalNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    TerminalNode::TerminalNode() {
        nodeType = Terminal;
    }

    TerminalNode::TerminalNode(const InterWorm::Token &token) {
        nodeType = Terminal;
        this->token = token;
    }

    void TerminalNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) {
        visitor->VisitTerminal(this);
    }

    NodeBase* TerminalNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) {
        return editor->VisitTerminal(this);
    }
}
