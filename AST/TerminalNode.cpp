/**
 * @file TerminalNode.cpp
 * @author Bryn McKerracher
 **/
#include "TerminalNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    TerminalNode::TerminalNode() {
        type = Terminal;
    }

    TerminalNode::TerminalNode(const InterWorm::Token &token) {
        type = Terminal;
        this->token = token;
    }

    void TerminalNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor* visitor) {
        visitor->VisitTerminal(this);
    }

    NodeBase* TerminalNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) {
        return editor->VisitTerminal(this);
    }
}
