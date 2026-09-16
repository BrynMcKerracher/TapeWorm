/**
 * @file StatementNode.cpp
 * @author brynm
 */
#include "LeafStatementNode.h"
#include "NodeEditor.h"
#include "NodeVisitor.h"

namespace TapeWorm::AST {
    void LeafStatementNode::Accept(NodeVisitor *visitor) {
        visitor->VisitLeafStatement(this);
    }

    NodeBase *LeafStatementNode::AcceptEditor(NodeEditor *editor) {
        return editor->VisitLeafStatement(this);
    }
} // TapeWorm