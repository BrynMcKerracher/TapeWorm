/**
 * @file StatementNode.cpp
 * @author brynm
 */
#include "LeafStatementNode.h"
#include "NodeReadWriteVisitor.h"
#include "NodeReadOnlyVisitor.h"

namespace TapeWorm::AST {
    void LeafStatementNode::AcceptReadOnlyVisitor(NodeReadOnlyVisitor *visitor) {
        visitor->VisitLeafStatement(this);
    }

    NodeBase *LeafStatementNode::AcceptReadWriteVisitor(NodeReadWriteVisitor *editor) {
        return editor->VisitLeafStatement(this);
    }
} // TapeWorm