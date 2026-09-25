/**
 * @file MetaTerminalNode.h
 * @author Bryn McKerracher
 **/
#include "MetaTerminalNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    void MetaTerminalNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor *visitor) {
        visitor->VisitLeafStatement(this);
    }

    NodeBase *MetaTerminalNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) {
        return editor->VisitMetaTerminal(this);
    }
} // TapeWorm