/**
 * @file NodeReadWriteVisitor.h
 * @author Bryn McKerracher
 */
#ifndef TAPEWORM_NODEREADWRITEVISITOR_H
#define TAPEWORM_NODEREADWRITEVISITOR_H

#include "NodeTypes.h"

namespace TapeWorm::AST {
    /**
     * @struct NodeReadWriteVisitor
     * @brief Visitor-pattern AST visitor with ability to edit or replace nodes.
     *
     * This pattern is primarily for optimisation passes of the AST.
     */
    struct NodeReadWriteVisitor {
        virtual ~NodeReadWriteVisitor() = default;
        virtual NodeBase* VisitTerminal(TerminalNode* node) = 0;
        virtual NodeBase* VisitSyntactic(SyntacticNode* node) = 0;
        virtual NodeBase* VisitGlobal(GlobalNode* node) = 0;
        virtual NodeBase* VisitLeafStatement(LeafStatementNode* node) = 0;
    };
} // TapeWorm

#endif //TAPEWORMTESTS_NODEEDITOR_H